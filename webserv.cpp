/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 17:32:41 by tabuayya          #+#    #+#             */
/*   Updated: 2026/04/09 15:46:25 by tabuayya         ###   ########.fr       */
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

        std::vector<int> r_cli; // ✅ only one, correct type

        for (int i = 0; i < num_events; ++i)
        {
            int fd = events[i].data.fd;

            // 🔹 HANDLE NEW CONNECTIONS
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

            // 🔹 HANDLE ERRORS
            if (events[i].events & (EPOLLERR | EPOLLHUP))
            {
                close_client_connection(fd);
                continue;
            }

            // 🔹 HANDLE CLIENT EVENTS
            for (std::list<server>::iterator sit = servers.begin(); sit != servers.end(); ++sit)
            {
                std::map<int, client>& client_fds = sit->getClientFds();
                std::map<int, client>::iterator it = client_fds.find(fd);

                if (it != client_fds.end())
                {
                    state_machine(it->second, *sit, fd, events[i].events);
					std::cerr<<"first"<<it->second.getState()<<std::endl;

                    if (it->second.getState() == ROUTING)
                        r_cli.push_back(fd);

                    if (it->second.getState() == DONE)
                        close_client_connection(fd);

                    break;
                }
            }
        }

        // 🔥 SECOND PASS (VERY IMPORTANT — YOU LOST THIS)
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
					std::cerr<<"second"<<it->second.getState()<<std::endl;
                    if (it->second.getState() == DONE)
					{
                        close_client_connection(fd);
					}

                    break;
                }
            }
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
