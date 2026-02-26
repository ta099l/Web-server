/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_parser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 16:34:42 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/23 14:57:24 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
int	webserv::conf_pars(char *file, int flag)
{
	std::ifstream inFile(file);
	std::string line;
	if(inFile.is_open())
	{
		while(std::getline(inFile, line) && flag == 1)
		{
			line = s_trim(line);
			if (line == "server")
				continue;
			else if(line == "{")
			{
				server new_srv;
				flag = save_info(inFile, new_srv, flag);
				servers.push_back(new_srv);
			}
			count++;
		}
	}
	else
	{
		std::cerr<< "Unable to open file" <<"\n";
		exit(1);
	}
	inFile.close();
	return (flag);
}

int	parse_listen(std::string s_line, std::string line, server& srv, int flag)
{
	flag = check_line(line);
	size_t start = s_line.rfind(" ");
	// if(start == std::string::npos)
	// 	return (-1);
	// std::cout<<start<<"\n";
	int len = s_line.length() - start;
	// std::cout<<len<<"\n";
	std::string substr = s_line.substr(start + 1, len);
	// std::cout<<substr<<"\n";
	ListenConfig listen;
	if(substr.find(":") != std::string::npos)
	{
		int pos = substr.find(":");
		// std::cout<<"pos :"<<pos<<"\n";
		listen.setHost(substr.substr(0 , pos));
		// std::cout<<"host :"<<listen.getHost()<<"\n";
		std::string port = substr.substr(pos + 1, substr.length() - pos);
		listen.setPort(atoi(port.c_str()));
		// std::cout<<"port :"<<listen.getPort()<<"\n";
	}
	else
	{
		if(substr.find(".") != std::string::npos)
		{
			listen.setHost(substr.substr(0 , substr.length()));
			listen.setPort(0);
			// std::cout<<"host :"<<listen.getHost()<<"\n";
		}
		else
		{
			std::string port = substr.substr(0 , substr.length());
			listen.setPort(atoi(port.c_str()));
			// std::cout<<"port :"<<listen.getPort()<<"\n";
		}
	}
	srv.addListen(listen);
	return (flag);
}

int	store_location(std::string line,std::string s_line, server& srv, LocationConfig &loc, int flag)
{
	flag = check_line(line);
	if (flag == 0)
		return (0);
	size_t start = s_line.rfind(" ");
	int len = s_line.length() - start;
	std::string substr = s_line.substr(start + 1, len);
	std::vector<std::string> tokens = split(s_line);

	if(!s_line.compare(0,15, "allowed_methods"))
		parse_allowed_methods(tokens, loc);
	else if (!s_line.compare(0,3, "cgi"))
		parse_cgi(tokens, loc);
	else if (!s_line.compare(0,9, "autoindex"))
	{
		if(!substr.compare(0,2, "on"))
			loc.setAutoindex(true);
		else
			loc.setAutoindex(false);
	}
	else if (!s_line.compare(0,13, "upload_enable"))
	{
		if(!substr.compare(0,2, "on"))
			loc.setUploadEnable(true);
		else
			loc.setUploadEnable(false);
	}
	else if (!line.compare(0,12, "upload_store"))
		loc.setUploadStore(substr);
	else if (!line.compare(0,5, "index"))
		loc.setIndex(substr);
	else if (!line.compare(0,8, "redirect"))
		loc.setRedirect(substr);
	else if (!line.compare(0,4, "root"))
		loc.setRoot(substr);
	else if (!line.compare(0,20, "client_max_body_size"))
		loc.setMaxBodySize(atoll(substr.c_str()));
	else if (!line.compare(0,10, "error_page"))
	{
		if (tokens.size() != 3)
		{
			std::cerr << "Invalid error_page directive\n";
			exit(1);
		}
		char* endptr;
		long code = std::strtol(tokens[1].c_str(), &endptr, 10);
		if (*endptr != '\0' || code < 100 || code > 599)
		{
			std::cerr << "Invalid error code in error_page\n";
			exit(1);
		}
		srv.addErrorPage((int)code, tokens[2]);
	}
	else
	{
		std::cerr << "WHAT EVEN IS THIS CANT U WRITE A CONFIG FILE YOU IDIOT: " << line << "\n";
		exit(1);
	}
	return (flag);
}

int	parse_location(std::ifstream& inFile,std::string s_line, std::string line, server& srv, int flag)
{
	LocationConfig loc;

	take_default(s_line, srv, loc);
	size_t start = s_line.rfind(" ");
	int len = s_line.length() - start;
	std::string substr = s_line.substr(start + 1, len);
	loc.setPath(substr);
	while(std::getline(inFile, line) && flag == 1)
	{
		line = n_trim(line);
		if(line == "{")
			continue;
		if(line == "}")
			break;
		if(line.empty() || line[0] == '#')
			continue;
		std::string s_line = s_trim(line);
		flag = store_location(line, s_line, srv, loc, flag);
	}
	srv.addLocation(loc);
	return (flag);
}

int	parse_default(std::string s_line,std::string line, server& srv, int flag)
{
	flag = check_line(line);

	size_t start = s_line.rfind(" ");
	int len = s_line.length() - start;
	std::string substr = s_line.substr(start + 1, len);
	std::vector<std::string> tokens = split(s_line);

	if(!s_line.compare(0,15, "allowed_methods"))
	{
		for (size_t i = 1; i < tokens.size(); ++i)
		{
			if (tokens[i] != "GET" && tokens[i] != "POST" && tokens[i] != "DELETE")
			{
				std::cerr << "Invalid HTTP method: " << tokens[i] << "\n";
				exit(1);
			}
			srv.addMethod(tokens[i]);
		}
	}
	else if (!line.compare(0,3, "cgi"))
	{
		if (tokens.size() != 3)
		{
			std::cerr << "Invalid CGI directive\n";
			exit(1);
		}
		CGIConfig cgi;
		cgi.extension = tokens[1];
		cgi.path = tokens[2];
		srv.addCgi(cgi);
		srv.setIsCgi(true);
	}
	else if (!line.compare(0,9, "autoindex"))
	{
		if(!substr.compare(0,2, "on"))
			srv.setAutoindex(true);
		else
			srv.setAutoindex(false);
	}
	else if (!line.compare(0,13, "upload_enable"))
	{
		if(!substr.compare(0,2, "on"))
			srv.setUploadEnable(true);
		else
			srv.setUploadEnable(false);
	}
	else if (!line.compare(0,12, "upload_store"))
		srv.setUploadStore(substr);
	else if (!line.compare(0,5, "index"))
		srv.setIndex(substr);
	else if (!line.compare(0,8, "redirect"))
		srv.setRedirect(substr);
	else if (!line.compare(0,4, "root"))
		srv.setRoot(substr);
	else if (!line.compare(0,20, "client_max_body_size"))
		srv.setMaxBodySize(atoll(substr.c_str()));
	else if (!line.compare(0,10, "error_page"))
		parse_error_page(tokens, srv);
	else
	{
		std::cerr << "WHAT EVEN IS THIS CANT U WRITE A CONFIG FILE U IDIOT: " << line << "\n";
		exit(1);
	}
	return (flag);
}

int	webserv::save_info(std::ifstream& inFile, server& srv, int flag)
{
	std::string line;
	std::string s_line;
	while(std::getline(inFile, line) && flag == 1)
	{
		line = n_trim(line);
		s_line = s_trim(line);
		if(s_line == "}")
			return (1);
		if(s_line.empty() || s_line[0] == '#')
			continue;
		if(!s_line.compare(0,6, "listen"))
			flag = parse_listen(s_line,line, srv, flag);
		else if(!s_line.compare(0,8, "location"))
			flag = parse_location(inFile,s_line, line, srv, flag);
		else
			flag = parse_default(s_line, line, srv, flag);
	}
	return (flag);
}
