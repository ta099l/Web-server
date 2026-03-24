/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabusala <rabusala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 17:32:41 by tabuayya          #+#    #+#             */
/*   Updated: 2026/03/15 18:05:45 by rabusala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "client.hpp"
webserv::webserv()
{
}
webserv::~webserv()
{
}
std::vector<server>& webserv::getServers()
{
	return servers;
}

const std::vector<server>& webserv::getServers() const
{
	return(this->servers);
}
void webserv::setServers(const server& s)
{
	servers.push_back(s);
}
int webserv::initialize_epoll()
{
	epoll_fd = epoll_create1(0);
	if (epoll_fd == -1)
	{
		std::cerr << "Failed to create epoll file descriptor" << std::endl;
		return -1;
	}
	for(size_t i=0;i<getServers().size();i++)
	{
		const std::vector<int>& listenfds = servers[i].getServerFd();
		for(size_t j=0;j<listenfds.size();j++)
		{
			int fd = listenfds[j];
			struct epoll_event event;
			event.events = EPOLLIN;
			event.data.fd = fd;
			if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1)
			{
				std::cerr << "Failed to add server socket to epoll" << std::endl;
				close(epoll_fd);
				return -1;
			}
		}
	}
	return(0);
}
bool webserv::is_server_socket(int fd)
{
	for (size_t i = 0; i < servers.size(); ++i)
	{
		const std::vector<int>& listenFds = servers[i].getServerFd();
		for (size_t j = 0; j < listenFds.size(); ++j)
		{
			if (fd == listenFds[j])
				return true;
		}
	}
	return false;
}
int webserv::handle_new_connection(int listen_fd, server& srv)
{
	while (true)
	{
		sockaddr_in client_addr;
		socklen_t client_len = sizeof(client_addr);
		int client_fd = accept(listen_fd, (sockaddr*)&client_addr, &client_len);
		if (client_fd == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break;
			std::cerr << "accept() failed" << std::endl;
			break;
		}
		int flags = fcntl(client_fd, F_GETFL, 0);
		if (flags == -1 || fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1)
		{
			close(client_fd);
			continue;
		}
		epoll_event event;
		event.events = EPOLLIN;
		event.data.fd = client_fd;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1)
		{
			close(client_fd);
			continue;
		}
		client new_client(client_fd,&srv);
		srv.addClientFd(client_fd, new_client);
	}
	return 1;
}
void handleUpload(client &cli,server &serv,const ClientState &state)
{
	cli.setPostFileFd(open(cli.getUploadPath().c_str(),O_CREAT|O_WRONLY|O_TRUNC,0644));
	if(cli.getPostFileFd()<0)
	{
		cli.setState(SENDING_RESPONSE);
		cli.getRes().setStatusCode(500);
		return ;
	}
	write(cli.getPostFileFd(),cli.getReq().getBody().c_str(),cli.getContentLength());
	close(cli.getPostFileFd());
	if(state == UPLOADING)
		cli.getRes().setStatusCode(200);
	else if(state == OVERWRITE)
		cli.getRes().setStatusCode(201);
	cli.setState(SENDING_RESPONSE);
	return ;
}
void handleFileReading(client &cli,server &srv)
{
	char readBuffer[8192];
	ssize_t n=read(cli.getGetFileFd(),readBuffer,sizeof(readBuffer));
	if(n>0)
	{
		cli.getRes().appendFileBody(readBuffer,n);
		cli.setState(SENDING_RESPONSE);
	}
	else if(n==0)
	{
		close(cli.getGetFileFd());
		cli.setFileDone(true);
		cli.getRes().setStatusCode(200);
		cli.setState(SENDING_RESPONSE);
	}
	else
	{
		close(cli.getGetFileFd());
		cli.getRes().setStatusCode(500);
		cli.setState(SENDING_RESPONSE);
	}
}
void generateErrorResponse(client &cli,server &serv)
{
	const LocationConfig* loc = cli.getLocation();
	const std::map<int, std::string>& errorPages = loc->getErrorPages();
	for (std::map<int, std::string>::const_iterator it = errorPages.begin();it != errorPages.end();it++)
	{
		if(cli.getRes().getStatusCode() == it->first)

	    int code = it->first;
	    std::string path = it->second;
	}
}
void generateResponseHeader(client &cli,server &srv)
{
	if(cli.getRes().getStatusCode() != 200)
		generateErrorResponse(cli,srv);
}
void handleWrite(client &cli,server &serv)
{
	if(!cli.getRes().getGeneratedResponseHeader())
		generateResponseHeader(cli,serv);
	
}
void webserv::setEpoll(int epollFd, int clientFd,int flag)
{
    struct epoll_event ev;
	if(flag == 0)
		ev.events = EPOLLIN;
	else if(flag == 1)
    	ev.events = EPOLLOUT;
    ev.data.fd = clientFd;
    epoll_ctl(epollFd, EPOLL_CTL_MOD, clientFd, &ev);
}
void	webserv::state_machine(client &cli,server &serv, int fd, uint32_t events)
{
	if(cli.getState()== READING && (events & EPOLLIN))
	{
		if(handleRead(cli,fd) == 1)
		{
			cli.setState(DONE);
			return;
		}
	}
	if(cli.getState() == ROUTING)
	{
		if(handleRouting(cli,serv) == 1)
		{
			setEpoll(epoll_fd,cli.getFd(),1);
			return;
		}
	}
	if(cli.getState() == UPLOADING || cli.getState() == OVERWRITE)
	{
		handleUpload(cli,serv,cli.getState());
	}
	if(events & EPOLLOUT)
	{
		if(cli.getState() == READINGFILE)
		{
			handleFileReading(cli,serv);
		}
		if(cli.getState()==SENDING_RESPONSE)
		{
			handleWrite(cli,serv);
		}
	}
	// if(state == SENDING_RESPONSE && (events & EPOLLOUT))
	// {
	// 	if(handleWrite(cli,fd) == 1)
	// 	{
	// 		cli.setState(DONE);
	// 		return;
	// 	}
	// }
}
void webserv::close_client_connection(int fd)
{
	epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
	close(fd);
	for (size_t j = 0; j < servers.size(); ++j)
	{
	    std::map<int, client>& client_fds = servers[j].getClientFds();
	    std::map<int, client>::iterator it = client_fds.find(fd);
	    if (it != client_fds.end())
	    {
	        client_fds.erase(it);
	        break;
	    }
	}
}
int webserv::run()
{
	struct epoll_event events[100];
	while(true)
	{
		int num_events = epoll_wait(epoll_fd, events, 100, -1);
		if(num_events == -1)
		{
			if (errno == EINTR)
				continue;
			std::cerr << "epoll_wait error" << std::endl;
			break;
		}
		for (int i = 0; i < num_events; ++i)
		{
			int fd = events[i].data.fd;
				if (is_server_socket(fd))
				{
					for (size_t j = 0; j < servers.size(); ++j)
					{
						const std::vector<int>& listenFds = servers[j].getServerFd();
						for (size_t k = 0; k < listenFds.size(); ++k)
						{
							if (fd == listenFds[k])
							{
								handle_new_connection(fd, servers[j]);
								continue;
							}
						}
					}
				}
				if (events[i].events & (EPOLLERR | EPOLLHUP))
				{
					std::cerr << "Error on fd " << fd << std::endl;
					close_client_connection(fd);
				}
				else if (events[i].events & (EPOLLIN | EPOLLOUT))
				{
					for (size_t j = 0; j < servers.size(); ++j)
					{
						std::map<int, client>& client_fds = servers[j].getClientFds(); // reference!
						std::map<int, client>::iterator it = client_fds.find(fd);
						if (it != client_fds.end())
						{
							state_machine(it->second, servers[j], fd,events[i].events);
							break;
						}
					}
				}
				//HANDLE CGI OUTPUT
				//fdin & fdout for CGI processes
				//handle_cgi_output(fd);
				//if CGI process is done, remove fd from epoll and close it
				//epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
				//close(fd);
				//remove CGI process from tracking data structures
				//cgi_processes.erase(fd);
				//remove client associated with this CGI process if needed
			}
	}
	return 0;
}
/**
- state machine implementation
- keep returning to the loop
- get reading in chunks and returning
- post in chunks
-
 */
