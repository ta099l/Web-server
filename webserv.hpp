/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:57:41 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/26 15:18:12 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef WEBSERV_HPP
#define WEBSERV_HPP
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <arpa/inet.h>
#include <fcntl.h>
#include "server.hpp"
#include <sys/epoll.h>
#include <unistd.h>
#include "HttpReq.hpp"
#include "client.hpp"

namespace HttpMessages
{
	enum Codes
	{
		OK = 200,
		BAD_REQUEST = 400,
		FORBIDDEN = 403,
		NOT_FOUND = 404,
		METHOD_NOT_ALLOWED = 405,
		PAYLOAD_TOO_LARGE = 413,
		INTERNAL_SERVER_ERROR = 500,
		NOT_IMPLEMENTED = 501
	};
}
class webserv
{
	private:
		std::vector<server> servers;
		int epoll_fd;
	public:
		webserv();
		~webserv();
		int initialize_epoll();
		int epoll_fd;
		int run();
		int count;
		std::vector<server>& getServers();
		const	std::vector<server>& getServers() const;
		void	setServers(const server& s);
		int	conf_pars(char *file, int flag);
		int		save_info(std::ifstream& inFile, server& s, int flag);
		void	printConfig() const;
		bool is_server_socket(int fd);
	void handle_new_connection(int fd, server& srv);
std::vector<std::string> split(const std::string& line);
};
int handleRouting(client &cli, server &srv);
int	check_line(std::string line);
std::string	s_trim(std::string str);
int	take_default(std::string line, server& srv, LocationConfig &loc);
void	parse_allowed_methods(const std::vector<std::string>& tokens, LocationConfig& loc);
std::string	n_trim(std::string &str);
void parse_cgi(const std::vector<std::string>& tokens, LocationConfig& loc);
std::vector<std::string> split(const std::string& line);
void parse_error_page(const std::vector<std::string>& tokens, server& srv);
void handle_client_write(int fd, client &cli);
#endif
