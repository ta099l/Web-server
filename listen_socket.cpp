/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listen_socket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bushra <bushra@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 14:01:48 by balhamad          #+#    #+#             */
/*   Updated: 2026/04/09 13:48:53 by bushra           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "socket.hpp"
#include <cstring>

int init_sockets(webserv& spiderweb)
{
    std::list<server>& servers = spiderweb.getServers(); // get the list

    size_t i = 0;
    for (std::list<server>::iterator sit = servers.begin(); sit != servers.end(); ++sit, ++i)
    {
        const std::vector<ListenConfig>& listens = sit->getListens();

        if (listens.empty())
        {
            std::cerr << "Server " << i << " has no listen directive\n";
            return -1;
        }

        for (size_t j = 0; j < listens.size(); ++j)
        {
            const ListenConfig& l = listens[j];
            int fd = create_listen_socket(l.getHost(), l.getPort());

            if (fd == -1)
            {
                std::cerr << "Failed to create listen socket for server " << i << std::endl;
                return -1;
            }

            sit->addServerFd(fd);

            printf("Server %zu listening on %s:%d (fd: %d)\n",
                   i,
                   l.getHost().c_str(),
                   l.getPort(),
                   fd);
        }
    }

    return 0;
}

int create_listen_socket(const std::string& host, int port)
{
	struct sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	if (host == "0.0.0.0")
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else if (inet_pton(AF_INET, host.c_str(), &addr.sin_addr) <= 0)
	{
		std::cerr << "Invalid host: " << host << std::endl;
		return -1;
	}
	int	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
	{
		std::cerr << "Failed to create socket" << std::endl;
		return -1;
	}
	int opt = 1;
	int setsockopt_result = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (setsockopt_result == -1)
	{
		std::cerr << "errno: " << errno << std::endl;
		close(fd);
		return -1;
	}
	int fcntl_result = fcntl(fd, F_SETFL, O_NONBLOCK);
	if (fcntl_result == -1)
	{
		std::cerr << "Failed to set socket to non-blocking mode" << std::endl;
		close(fd);
		return -1;
	}
	int bind_result = bind(fd, (struct sockaddr*)&addr,sizeof(addr));
	if (bind_result == -1)
	{
		std::cerr << "Failed to bind socket" << std::endl;
		close(fd);
		return -1;
	}
	int listen_result = listen(fd,SOMAXCONN);
	if (listen_result == -1)
	{
		std::cerr << "Failed to listen on socket" << std::endl;
		close(fd);
		return -1;
	}
	return fd;
}

