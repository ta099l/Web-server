/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: balhamad <balhamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 17:32:41 by tabuayya          #+#    #+#             */
/*   Updated: 2026/03/03 18:26:30 by balhamad         ###   ########.fr       */
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
		if (flags != -1)
			fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);
		epoll_event event;
		event.events = EPOLLIN;
		event.data.fd = client_fd;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1)
		{
			close(client_fd);
			continue;
		}
		client new_client(client_fd);
		srv.addClientFd(client_fd, new_client);
	}
	return 1;
}

void	state_machine(client &cli,server &serv, int fd, uint32_t events)
{
	ClientState state=cli.getState();
	if(state == READING && (events & EPOLLIN))
	{
		if(handleRead(cli,fd) == 1)
		{
			cli.setState(DONE);
			return;
		}
	}
	if(state == ROUTING)
	{
		if(handleRouting(cli,serv) == 1)
		{
			cli.setState(DONE);
			return;
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
						if (fd == servers[j].getServerFd())
						{
							handle_new_connection(fd, servers[j]);
							continue;
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
