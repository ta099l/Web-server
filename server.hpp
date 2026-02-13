/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 19:32:26 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/13 21:25:18 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
#define SERVER_HPP
#include "webserv.hpp"
struct CGIConfig //done
{
	std::string extension;
	std::string path;
};

struct LocationConfig
{
	std::string path; //done
	std::vector<std::string> methods; //done
	bool autoindex; //done
	bool upload_enable; //done
	std::string upload_store; //done
	std::string root; //done
	std::string index; //done
	std::string redirect; //done
	long long max_body_size; //done
	std::map<std::string, CGIConfig> cgi; //done
};

struct ListenConfig //done
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
		std::vector<ListenConfig> listens; //done
		std::string root; //done
		std::string index; //done
		long long max_body_size; //done
		std::map<int, std::string> error_pages; //done
		std::map<std::string, LocationConfig> locations; //done
};

#endif
