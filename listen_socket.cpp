/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listen_socket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: balhamad <balhamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 14:01:48 by balhamad          #+#    #+#             */
/*   Updated: 2026/02/15 15:10:08 by balhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <cstring>
int create_listen_socket()
{
	struct socketadd_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

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

