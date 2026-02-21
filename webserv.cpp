/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 17:32:41 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/21 22:15:32 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

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
		int server_fd = getServers()[i].getServerFd();
		struct epoll_event event;
		event.events = EPOLLIN;
		event.data.fd = server_fd;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1)
		{
			std::cerr << "Failed to add server socket to epoll" << std::endl;
			close(epoll_fd);
			return -1;
		}
	}
	return(0);
}
bool webserv::is_server_socket(int fd)
{
    for (size_t i = 0; i < servers.size(); ++i)
    {
        if (fd == servers[i].getServerFd())
            return true;
    }
    return false;
}
void webserv::handle_new_connection(int fd, server& srv)
{
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_fd = accept(fd, (struct sockaddr *)&client_addr, &client_len);
	if (client_fd == -1)
	{
		std::cerr << "Failed to accept new connection" << std::endl;
		return;
	}
	fcntl(client_fd, F_SETFL, O_NONBLOCK);
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = client_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1)
	{
		std::cerr << "Failed to add client socket to epoll" << std::endl;
		close(client_fd);
		return;
	}
	client new_client(client_fd);
	srv.addClientFd(client_fd, new_client);
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
					if (fd == servers[j].getServerFd())
					{
						handle_new_connection(fd, servers[j]);
						break;
					}
				}
			}
			if (events[i].events & EPOLLIN)
			{
				for (size_t j = 0; j < servers.size(); ++j)
				{
					std::map<int, client>& client_fds = servers[j].getClientFds(); // reference!
					std::map<int, client>::iterator it = client_fds.find(fd);
					if (it != client_fds.end())
					{
						handleRead(it->second, fd);
						if(it->second.isRequestComplete())
							handleRouting(it->second, servers[j]);
						break;
					}
				}
			}
			if (events[i].events & EPOLLOUT)
			{
				//handle_client_write(fd);
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
			if (events[i].events & (EPOLLERR | EPOLLHUP))
			{
				std::cerr << "Error on fd " << fd << std::endl;
				//   close_client_connection(fd);
			}
		}
	}
	return 0;
}
