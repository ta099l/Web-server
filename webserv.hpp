/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: balhamad <balhamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:57:41 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/15 18:36:41 by balhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef WEBSERV_HPP
#define WEBSERV_HPP
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <arpa/inet.h>
#include <fcntl.h>
#include "server.hpp"
#include <unistd.h>

class webserv
{
	private:
		std::vector<server> servers;
	public:
		webserv();
		~webserv();
		int count;
		const	std::vector<server>& getServers() const;
		std::vector<server>& getServers();
		void	setServers(const server& s);
		void	conf_pars(char *file);
		int		save_info(std::ifstream& inFile, server& s);
		void	printConfig() const;


};

int	take_default(std::string line, server& srv, LocationConfig &loc);
void	parse_allowed_methods(const std::vector<std::string>& tokens, LocationConfig& loc);
std::string	trim(std::string &str);
void parse_cgi(const std::vector<std::string>& tokens, LocationConfig& loc);
std::vector<std::string> split(const std::string& line);
void parse_error_page(const std::vector<std::string>& tokens, server& srv);
#endif
