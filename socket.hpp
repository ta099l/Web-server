/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: balhamad <balhamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 13:58:13 by balhamad          #+#    #+#             */
/*   Updated: 2026/02/15 14:33:19 by balhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <fcntl.h>
#include "webserv.hpp"
#include <unistd.h>
struct socketadd_in {
	sa_family_t		sin_family;	// Address family
	in_port_t		sin_port;	// Port
	struct in_addr	sin_addr;
};

int create_listen_socket();




#endif
