/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:57:41 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/12 18:17:02 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef WEBSERV_HPP
#define WEBSERV_HPP
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <arpa/inet.h>
#include <fcntl.h>
#include "server.hpp"

struct socketadd{
	sa_family_t		sin_family;	// Address family
	in_port_t		sin_port;	// Port
	struct in_addr	sin_addr;
};

class webserv
{
	private:
		std::vector<server> servers;
	public:
		webserv();
		~webserv();
		const	std::vector<server>& getServers() const;
		void	setServers(const server& s);
		void	conf_pars(char *file);
};

#endif
