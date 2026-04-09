/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bushra <bushra@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:57:41 by tabuayya          #+#    #+#             */
/*   Updated: 2026/04/09 13:36:39 by bushra           ###   ########.fr       */
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
#include <sys/epoll.h>
#include <unistd.h>
// #include "HttpReq.hpp"
#include "client.hpp"
#include "server.hpp"

class client;

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
class webserv
{
	private:
		std::list<server> servers;
		int epoll_fd;
	public:
		webserv();
		~webserv();
		int initialize_epoll();
		int run();
		int count;
		std::list<server>& getServers();
		const	std::list<server>& getServers() const;
		void	setServers(const server& s);
		int	conf_pars(char *file, int flag);
		int		save_info(std::ifstream& inFile, server& s, int flag);
		void	printConfig() const;
		bool is_server_socket(int fd);
		int handle_new_connection(int fd, server& srv);
		void close_client_connection(int fd);
		void setEpoll(int epollFd, int clientFd,int flag);
		void	state_machine(client &cli,server &serv, int fd, uint32_t events);


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
std::string setupRootPath(client &cli, server &srv, const LocationConfig& locConfig, std::string uri);
int get_method(client &cli, server &srv, const LocationConfig& locConfig, std::string uri);
int post_method(client &cli, server &srv, const LocationConfig& locConfig, std::string uri);
void handleUpload(client &cli,server &serv,const ClientState &state);
void handleFileReading(client &cli,server &srv);
bool handleWrite(client &cli,server &serv);
void generateResponseHeader(client &cli,server &srv);
bool generateErrorResponse(client &cli,server &serv);


#endif
