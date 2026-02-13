/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_parser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 16:34:42 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/13 21:40:18 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	webserv::conf_pars(char *file)
{
	std::ifstream inFile(file);
	std::string line;
	if(inFile.is_open())
	{
		while(std::getline(inFile, line))
		{
			line = trim(line);
			if (line == "server")
				continue;
			else if(line == "{")
			{
				server new_srv;
				save_info(inFile, new_srv);
				servers.push_back(new_srv);
			}
			count++;
			std::cout << "Number of servers: " << servers.size() << "\n";
		}
	}
	else
	{
		std::cerr<< "Unable to open file" <<"\n";
		exit(1);
	}
	inFile.close();
}

int	parse_listen(std::string line, server& srv)
{
	size_t start = line.rfind(" ");
	// if(start == std::string::npos)
	// 	return (-1);
	// std::cout<<start<<"\n";
	int len = line.length() - start;
	// std::cout<<len<<"\n";
	std::string substr = line.substr(start + 1, len);
	// std::cout<<substr<<"\n";
	ListenConfig listen;
	if(substr.find(":") != std::string::npos)
	{
		int pos = substr.find(":");
		// std::cout<<"pos :"<<pos<<"\n";
		listen.host = substr.substr(0 , pos);
		// std::cout<<"host :"<<listen.host<<"\n";
		std::string port = substr.substr(pos + 1, substr.length() - pos);
		listen.port = atoi(port.c_str());
		// std::cout<<"port :"<<listen.port<<"\n";
	}
	else
	{
		if(substr.find(".") != std::string::npos)
		{
			listen.host = substr.substr(0 , substr.length());
			listen.port = 0;
			// std::cout<<"host :"<<listen.host<<"\n";
		}
		else
		{
			std::string port = substr.substr(0 , substr.length());
			listen.port = atoi(port.c_str());
			// std::cout<<"port :"<<listen.port<<"\n";
		}
	}
	srv.listens.push_back(listen);
	return (0);
}

int	store_location(std::string line, server& srv, LocationConfig &loc)
{
	(void)srv;
	size_t start = line.rfind(" ");
	int len = line.length() - start;
	std::string substr = line.substr(start + 1, len);
	std::vector<std::string> tokens = split(line);

	if(!line.compare(0,15, "allowed_methods"))
		parse_allowed_methods(tokens, loc);
	else if (!line.compare(0,3, "cgi"))
		parse_cgi(tokens, loc);
	else if (!line.compare(0,9, "autoindex"))
	{
		if(!substr.compare(0,2, "on"))
			loc.autoindex = true;
		else
			loc.autoindex = false;
	}
	else if (!line.compare(0,13, "upload_enable"))
	{
		if(!substr.compare(0,2, "on"))
			loc.upload_enable = true;
		else
			loc.upload_enable = false;
	}
	else if (!line.compare(0,12, "upload_store"))
		loc.upload_store = substr;
	else if (!line.compare(0,5, "index"))
		loc.index = substr;
	else if (!line.compare(0,8, "redirect"))
		loc.redirect = substr;
	else if (!line.compare(0,4, "root"))
		loc.root = substr;
	else if (!line.compare(0,20, "client_max_body_size"))
		loc.max_body_size = atoll(substr.c_str());
	return (0);
}

int	parse_location(std::ifstream& inFile, std::string line, server& srv)
{
	LocationConfig loc;

	size_t start = line.rfind(" ");
	int len = line.length() - start;
	std::string substr = line.substr(start + 1, len);
	loc.path = substr;
	while(std::getline(inFile, line))
	{
		line = trim(line);
		if(line == "}")
			break;
		store_location(line, srv, loc);
	}
	srv.locations[loc.path] = loc;
	return (0);
}

int	parse_default(std::string line, server& srv)
{
	size_t start = line.rfind(" ");
	int len = line.length() - start;
	std::string substr = line.substr(start + 1, len);
	std::vector<std::string> tokens = split(line);

	if (!line.compare(0,5, "index"))
		srv.index = substr;
	else if (!line.compare(0,4, "root"))
		srv.root = substr;
	else if (!line.compare(0,20, "client_max_body_size"))
		srv.max_body_size = atoll(substr.c_str());
	else if (!line.compare(0,10, "error_page"))
		parse_error_page(tokens, srv);
	return (0);
}

int	webserv::save_info(std::ifstream& inFile, server& srv)
{
	std::string line;
	(void)srv;
	while(std::getline(inFile, line))
	{
		line = trim(line);
		std::cout<<line<<"\n";
		if(line == "}")
			return (1);
		if(line.empty())
			continue;
		if(!line.compare(0,6, "listen"))
			parse_listen(line, srv);
		else if(!line.compare(0,8, "location"))
			parse_location(inFile, line, srv);
		else
			parse_default(line, srv);
	}
	return (0);
}
