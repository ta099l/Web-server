/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 19:32:26 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/12 18:59:54 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
struct CGIConfig
{
	std::string extension;
	std::string path;
};

struct LocationConfig
{
	std::string path;
	std::vector<std::string> methods;
	bool autoindex;
	bool upload_enable;
	std::string upload_store;
	std::string root;
	std::string index;
	std::string redirect;
	long long max_body_size;
	std::map<std::string, CGIConfig> cgi;
};

struct ListenConfig
{
	std::string host;
	int port;
};

class server
{
	public:
		server();
		server(const server &obj);
		~server();
		std::vector<ListenConfig> listens;
		std::string root;
		std::string index;
		long long max_body_size;
		std::map<int, std::string> error_pages;
		std::map<std::string, LocationConfig> locations;
};

#endif
