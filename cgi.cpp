#include <sys/wait.h>
#include <fcntl.h>
#include <csignal>
#include <ctime>
#include <cerrno>
#include <cstring>
#include <sstream>
#include "webserv.hpp"
#include "client.hpp"



void killCgi(client &cli, server &srv)
{
		if (cli.getCgiInputFd() != -1)
		{
				epoll_ctl(srv.getEpollFd(), EPOLL_CTL_DEL, cli.getCgiInputFd(), NULL);
				close(cli.getCgiInputFd());
				cli.setCgiInputFd(-1);
		}
		if (cli.getCgiOutputFd() != -1)
		{
				epoll_ctl(srv.getEpollFd(), EPOLL_CTL_DEL, cli.getCgiOutputFd(), NULL);
				close(cli.getCgiOutputFd());
				cli.setCgiOutputFd(-1);
		}
		if (cli.getCgiPid() > 0)
		{
				kill(cli.getCgiPid(), SIGKILL);
				waitpid(cli.getCgiPid(), NULL, WNOHANG);
				cli.setCgiPid(-1);
		}
		struct epoll_event ev;
		ev.events = EPOLLOUT;
		ev.data.fd = cli.getFd();
		epoll_ctl(srv.getEpollFd(), EPOLL_CTL_ADD, cli.getFd(), &ev);
}



int setupCgi(client &cli, server &srv)
{
		int stdinPipe[2];
		int stdoutPipe[2];

		if (pipe(stdinPipe) < 0 || pipe(stdoutPipe) < 0)
		{
				cli.getRes().setStatusCode(500);
				cli.setState(ERROR);
				return 1;
		}

		fcntl(stdinPipe[1], F_SETFL, fcntl(stdinPipe[1], F_GETFL, 0) | O_NONBLOCK);
		fcntl(stdoutPipe[0], F_SETFL, fcntl(stdoutPipe[0], F_GETFL, 0) | O_NONBLOCK);

		pid_t pid = fork();
		if (pid < 0)
		{
				close(stdinPipe[0]);
				close(stdinPipe[1]);
				close(stdoutPipe[0]);
				close(stdoutPipe[1]);
				cli.getRes().setStatusCode(500);
				cli.setState(ERROR);
				return 1;
		}

		if (pid == 0)
		{
				dup2(stdinPipe[0], STDIN_FILENO);
				dup2(stdoutPipe[1], STDOUT_FILENO);
				for (int i = 3; i < 1024; i++)
						close(i);

				HttpReq &req = cli.getReq();
				std::string scriptPath = cli.getCgiScriptPath();
				std::string interp = cli.getCgiInterpreter();

				std::string qs;
				std::string fullUri = req.getUri();
				size_t qpos = fullUri.find('?');
				if (qpos != std::string::npos)
						qs = fullUri.substr(qpos + 1);

				std::vector<std::string> envStrings;
				envStrings.push_back("REQUEST_METHOD=" + req.getMethod());
				envStrings.push_back("SCRIPT_FILENAME=" + scriptPath);
				envStrings.push_back("SCRIPT_NAME=" + req.getUri());
				envStrings.push_back("PATH_INFO=" + req.getUri());
				envStrings.push_back("QUERY_STRING=" + qs);
				std::ostringstream cls;
				cls << cli.getReq().getBody().size();
				envStrings.push_back("CONTENT_LENGTH=" + cls.str());
				const std::map<std::string, std::string> &hdrs = req.getHeaders();
				std::map<std::string, std::string>::const_iterator ct = hdrs.find("content-type");
				envStrings.push_back("CONTENT_TYPE=" + (ct != hdrs.end() ? ct->second : ""));
				envStrings.push_back("SERVER_PROTOCOL=HTTP/1.1");
				envStrings.push_back("GATEWAY_INTERFACE=CGI/1.1");
				envStrings.push_back("REDIRECT_STATUS=200");

				std::vector<char *> envp;
				for (size_t i = 0; i < envStrings.size(); i++)
						envp.push_back(const_cast<char *>(envStrings[i].c_str()));
				envp.push_back(NULL);

				char *argv[] = {
					const_cast<char *>(interp.c_str()),
					const_cast<char *>(scriptPath.c_str()),
					NULL};

				execve(interp.c_str(), argv, envp.data());
				exit(1);
		}

		close(stdinPipe[0]);
		close(stdoutPipe[1]);

		cli.setCgiPid(pid);
		cli.setCgiInputFd(stdinPipe[1]);
		cli.setCgiOutputFd(stdoutPipe[0]);

		cli.setCgiStartTime(std::time(NULL));

		if (cli.getCgiInput().empty())
		{
				close(cli.getCgiInputFd());
				cli.setCgiInputFd(-1);

				struct epoll_event ev;
				ev.events = EPOLLIN;
				ev.data.fd = cli.getCgiOutputFd();
				epoll_ctl(srv.getEpollFd(), EPOLL_CTL_ADD, cli.getCgiOutputFd(), &ev);

				epoll_ctl(srv.getEpollFd(), EPOLL_CTL_DEL, cli.getFd(), NULL);
				cli.setState(CGI_READING_STDOUT);
		}
		else
		{
				struct epoll_event ev;
				ev.events = EPOLLOUT;
				ev.data.fd = cli.getCgiInputFd();
				epoll_ctl(srv.getEpollFd(), EPOLL_CTL_ADD, cli.getCgiInputFd(), &ev);

				cli.setState(CGI_WRITING_STDIN);
		}
		return 0;
}


int handleCgiWrite(client &cli, server &srv)
{
		std::string &input = cli.getCgiInput();

		ssize_t written = write(cli.getCgiInputFd(), input.c_str(), input.size());
		if (written < 0)
		{
				killCgi(cli, srv);
				cli.getRes().setStatusCode(502);
				cli.setState(ERROR);
				return 1;
		}
		input.erase(0, written);

		if (input.empty())
		{
				epoll_ctl(srv.getEpollFd(), EPOLL_CTL_DEL, cli.getCgiInputFd(), NULL);
				close(cli.getCgiInputFd());
				cli.setCgiInputFd(-1);

				struct epoll_event ev;
				ev.events = EPOLLIN;
				ev.data.fd = cli.getCgiOutputFd();
				epoll_ctl(srv.getEpollFd(), EPOLL_CTL_ADD, cli.getCgiOutputFd(), &ev);

				cli.setState(CGI_READING_STDOUT);
		}
		return 0;
}

int handleCgiRead(client &cli, server &srv)
{
		if (std::time(NULL) - cli.getCgiStartTime() > 30)
		{
				killCgi(cli, srv);
				cli.getRes().setStatusCode(504);
				cli.setState(ERROR);
				return 1;
		}

		char buf[4096];
		ssize_t n = read(cli.getCgiOutputFd(), buf, sizeof(buf));

		if (n < 0)
		{
				killCgi(cli, srv);
				cli.getRes().setStatusCode(502);
				cli.setState(ERROR);
				return 1;
		}

		if (n == 0)
		{
				epoll_ctl(srv.getEpollFd(), EPOLL_CTL_DEL, cli.getCgiOutputFd(), NULL);
				close(cli.getCgiOutputFd());
				cli.setCgiOutputFd(-1);

				int status;
				waitpid(cli.getCgiPid(), &status, 0);
				cli.setCgiPid(-1);

				if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
				{
						cli.getRes().setStatusCode(502);
						cli.setState(ERROR);
						struct epoll_event ev;
						ev.events = EPOLLOUT;
						ev.data.fd = cli.getFd();
						epoll_ctl(srv.getEpollFd(), EPOLL_CTL_ADD, cli.getFd(), &ev);
						return 1;
				}

				buildCgiResponse(cli);

				struct epoll_event ev;
				ev.events = EPOLLOUT;
				ev.data.fd = cli.getFd();
				epoll_ctl(srv.getEpollFd(), EPOLL_CTL_ADD, cli.getFd(), &ev);

				cli.setState(SENDING_RESPONSE);
				return 0;
		}

		cli.setCgiOutputBuffer(cli.getCgiOutputBuffer() + std::string(buf, n));
		return 0;
}


void buildCgiResponse(client &cli)
{
		std::string raw = cli.getCgiOutputBuffer();

		std::string sep = "\r\n\r\n";
		size_t pos = raw.find(sep);
		if (pos == std::string::npos)
		{
				sep = "\n\n";
				pos = raw.find(sep);
		}

		int statusCode = 200;
		std::string body;

		if (pos != std::string::npos)
		{
				std::string headers = raw.substr(0, pos);
				body = raw.substr(pos + sep.size());

				std::istringstream iss(headers);
				std::string line;
				while (std::getline(iss, line))
				{
						if (!line.empty() && line[line.size() - 1] == '\r')
								line.erase(line.size() - 1);
						if (line.empty())
								continue;

						if (line.substr(0, 7) == "Status:")
						{
								statusCode = std::atoi(line.substr(8).c_str());
						}
						else if (line.size() >= 13 && line.substr(0, 13) == "Content-Type:")
						{
								cli.getRes().setContentTypeString(line.substr(14));
						}
				}
		}
		else
		{
				body = raw;
		}

		cli.getRes().setStatusCode(statusCode);
		cli.getRes().setMemoryBody(body);
		cli.getRes().setHasMemoryBody(true);
		cli.getRes().setContentLength(body.size());
		if (cli.getRes().getContentType().empty())
				cli.getRes().setContentTypeString("text/html");
		if (!cli.getReq().getVersion().empty())
				cli.getRes().setVersion(cli.getReq().getVersion());
		else
				cli.getRes().setVersion("HTTP/1.1");
}