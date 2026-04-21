/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabusala <rabusala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 17:32:41 by tabuayya          #+#    #+#             */
/*   Updated: 2026/04/21 20:13:55 by rabusala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int g_siganl = 1;
#include "webserv.hpp"
#include "client.hpp"
#include <ctime>
#include <csignal>
#include <sys/wait.h>
#include <fcntl.h>
#include <csignal>
#include <ctime>
#include <cerrno>
#include <cstring>
#include <sstream>

webserv::webserv()
{
}
webserv::~webserv()
{
}
std::list<server>& webserv::getServers()
{
	return servers;
}

const std::list<server>& webserv::getServers() const
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
	for (std::list<server>::iterator it = servers.begin(); it != servers.end(); ++it)
		it->setEpollFd(epoll_fd);
	for (std::list<server>::iterator it = servers.begin(); it != servers.end(); ++it)
	{
	const std::vector<int>& listenfds = it->getServerFd();
	for (size_t j = 0; j < listenfds.size(); j++)
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
	for (std::list<server>::iterator it = servers.begin(); it != servers.end(); ++it)
{
	const std::vector<int>& listenFds = it->getServerFd();

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
void webserv::close_client_connection(int fd)
{
	for (std::list<server>::iterator sit = servers.begin(); sit != servers.end(); ++sit)
	{
		std::map<int, client>& client_fds = sit->getClientFds();
		std::map<int, client>::iterator it = client_fds.find(fd);

		if (it != client_fds.end())
		{
			client_fds.erase(it);
			break;
		}
	}

	epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
	close(fd);
}

void handle_sigint(int sig)
{
	(void)sig;
	g_siganl = 0;
}
int webserv::run()
{
	struct epoll_event events[100];
	signal(SIGINT, handle_sigint);
	while(g_siganl)
	{
		int num_events = epoll_wait(epoll_fd, events, 100, -1);
		if(num_events == -1)
		{
			if (errno == EINTR)
				continue;
			std::cerr << "epoll_wait error" << std::endl;
			break;
		}

		std::vector<int> r_cli;

		for (int i = 0; i < num_events; ++i)
		{
			int fd = events[i].data.fd;

			if (is_server_socket(fd))
			{
				for (std::list<server>::iterator sit = servers.begin(); sit != servers.end(); ++sit)
				{
					const std::vector<int>& listenFds = sit->getServerFd();

					for (size_t k = 0; k < listenFds.size(); ++k)
					{
						if (fd == listenFds[k])
						{
							handle_new_connection(fd, *sit);
							break;
						}
					}
				}
				continue;
			}

			bool handled = false;
			for (std::list<server>::iterator sit = servers.begin(); sit != servers.end() && !handled; ++sit)
			{
				std::map<int, client> &client_fds = sit->getClientFds();

				// First: check if fd is a known client socket fd
				std::map<int, client>::iterator it = client_fds.find(fd);
				if (it != client_fds.end())
				{
					if (events[i].events & (EPOLLERR | EPOLLHUP))
					{
						close_client_connection(fd);
					}
					else
					{
						state_machine(it->second, *sit, fd, events[i].events);
						if (it->second.getState() == ROUTING || it->second.getState() == ERROR)
							r_cli.push_back(fd);
						if (it->second.getState() == DONE)
							close_client_connection(fd);
					}
					handled = true;
					break;
				}

				// Second: check if fd is a CGI pipe fd belonging to any client
				for (std::map<int, client>::iterator cit = client_fds.begin(); cit != client_fds.end(); ++cit)
				{
					client &cli = cit->second;
					if (cli.getCgiInputFd() == fd || cli.getCgiOutputFd() == fd)
					{
						state_machine(cli, *sit, fd, events[i].events);
						if (cli.getState() == DONE)
							close_client_connection(cit->first);
						handled = true;
						break;
					}
				}
			}
		}
		// CGI timeout check
		for (std::list<server>::iterator sit = servers.begin(); sit != servers.end(); ++sit)
		{
			std::map<int, client> &client_fds = sit->getClientFds();
			for (std::map<int, client>::iterator it = client_fds.begin(); it != client_fds.end(); ++it)
			{
				client &cli = it->second;
				if (cli.getState() == CGI_READING_STDOUT || cli.getState() == CGI_WRITING_STDIN)
				{
					if (std::time(NULL) - cli.getCgiStartTime() > 10)
					{
						killCgi(cli, *sit);
						cli.getRes().setStatusCode(504);
						cli.setState(ERROR);
						struct epoll_event ev;
						ev.events = EPOLLOUT;
						ev.data.fd = cli.getFd();
						epoll_ctl(epoll_fd, EPOLL_CTL_ADD, cli.getFd(), &ev);
					}
				}
			}
		}
		for (size_t i = 0; i < r_cli.size(); i++)
		{
			int fd = r_cli[i];

			for (std::list<server>::iterator sit = servers.begin(); sit != servers.end(); ++sit)
			{
				std::map<int, client>& client_fds = sit->getClientFds();
				std::map<int, client>::iterator it = client_fds.find(fd);

				if (it != client_fds.end())
				{
					state_machine(it->second, *sit, fd, 0);
					if (it->second.getState() == DONE)
					{
						close_client_connection(fd);
					}

					break;
				}
			}
		}
	}
// 1. Close all client connections
for (std::list<server>::iterator sit = servers.begin(); sit != servers.end(); ++sit)
{
	std::map<int, client>& client_fds = sit->getClientFds();
	// for (std::map<int, client>::iterator it = client_fds.begin(); it != client_fds.end(); ++it)
	// {
	// 	close(it->first);
	// }
	for (std::map<int, client>::iterator it = client_fds.begin(); it != client_fds.end(); ++it)
{
    client &cli = it->second;

    // 🔴 1. Kill CGI if running
    if (cli.getCgiPid() > 0)
    {
        kill(cli.getCgiPid(), SIGKILL);
        waitpid(cli.getCgiPid(), NULL, 0);
    }

    // 🔴 2. Close CGI pipes
    if (cli.getCgiInputFd() != -1)
    {
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, cli.getCgiInputFd(), NULL);
        close(cli.getCgiInputFd());
    }

    if (cli.getCgiOutputFd() != -1)
    {
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, cli.getCgiOutputFd(), NULL);
        close(cli.getCgiOutputFd());
    }

    // 🔴 3. Remove client fd from epoll
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, cli.getFd(), NULL);

    // 🔴 4. Close client socket
    close(cli.getFd());
}
}

// 2. Close listening sockets
for (std::list<server>::iterator sit = servers.begin(); sit != servers.end(); ++sit)
{
	const std::vector<int>& fds = sit->getServerFd();
	for (size_t i = 0; i < fds.size(); ++i)
	{
		close(fds[i]);
	}
}

// 3. Close epoll fd
close(epoll_fd);
	return 0;
}
/**
 - state machine implementation
- keep returning to the loop
- get reading in chunks and returning
- post in chunks
-
*/
