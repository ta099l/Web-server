/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: balhamad <balhamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 13:58:13 by balhamad          #+#    #+#             */
/*   Updated: 2026/02/15 18:14:59 by balhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <fcntl.h>
#include "webserv.hpp"
#include <unistd.h>


int create_listen_socket(const std::string& host, int port);
int init_sockets(webserv& spiderweb);



#endif
