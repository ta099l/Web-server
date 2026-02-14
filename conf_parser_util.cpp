/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_parser_util.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 21:03:35 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/14 19:29:28 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	parse_allowed_methods(const std::vector<std::string>& tokens, LocationConfig& loc)
{
	for (size_t i = 1; i < tokens.size(); ++i)
	{
		if (tokens[i] != "GET" && tokens[i] != "POST" && tokens[i] != "DELETE")
		{
			std::cerr << "Invalid HTTP method: " << tokens[i] << "\n";
			exit(1);
		}
		loc.methods.push_back(tokens[i]);
	}
}
void parse_cgi(const std::vector<std::string>& tokens, LocationConfig& loc)
{
	if (tokens.size() != 3)
	{
		std::cerr << "Invalid CGI directive\n";
		exit(1);
	}

	CGIConfig cgi;
	cgi.extension = tokens[1];
	cgi.path = tokens[2];

	loc.cgi[cgi.extension] = cgi;
}
void parse_error_page(const std::vector<std::string>& tokens, server& srv)
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

	srv.error_pages[(int)code] = tokens[2];
}
// int	take_default(std::string line, server& srv, LocationConfig &loc)
// {
// 	std::vector<std::string> tokens = split(line);
// 	if (!line.compare(0,15, "allowed_methods"))
// 		parse_allowed_methods(tokens, loc);
// 	else if (!line.compare(0,9, "autoindex"))
// 	{
// 		if(srv.autoindex ==  true)
// 			loc.autoindex = true;
// 		else
// 			loc.autoindex = false;
// 	}
// 	else if (!line.compare(0,13, "upload_enable"))
// 	{
// 		if(srv.upload_enable == true)
// 			loc.upload_enable = true;
// 		else
// 			loc.upload_enable = false;
// 	}
// 	else if (!line.compare(0,12, "upload_store") && !srv.upload_store.empty())
// 		loc.upload_store = srv.upload_store;
// 	else if (!line.compare(0,5, "index") && !srv.index.empty())
// 		loc.index = srv.index;
// 	else if (!line.compare(0,8, "redirect") && !srv.redirect.empty())
// 		loc.redirect = srv.redirect;
// 	else if (!line.compare(0,4, "root")  && !srv.root.empty())
// 		loc.root = srv.root;
// 	else if (!line.compare(0,20, "client_max_body_size"))
// 		loc.max_body_size = srv.max_body_size;
// 	std::cout<<"loc - autoindex: "<<loc.autoindex<<"\n";
// 	std::cout<<"srv - autoindex: "<<srv.autoindex<<"\n";
// 	std::cout<<"loc - upload_enable: "<<loc.upload_enable<<"\n";
// 	std::cout<<"srv - upload_enable: "<<srv.upload_enable<<"\n";
// 	std::cout<<"loc - upload_store: "<<loc.upload_store<<"\n";
// 	std::cout<<"srv - upload_store: "<<srv.upload_store<<"\n";
// 	std::cout<<"loc - index: "<<loc.index<<"\n";
// 	std::cout<<"srv - index: "<<srv.index<<"\n";
// 	std::cout<<"loc - redirect: "<<loc.redirect<<"\n";
// 	std::cout<<"srv - redirect: "<<srv.redirect<<"\n";
// 	std::cout<<"loc - root: "<<loc.root<<"\n";
// 	std::cout<<"srv - root: "<<srv.root<<"\n";
// 	std::cout<<"loc - client_max_body_size: "<<loc.max_body_size<<"\n";
// 	std::cout<<"srv - client_max_body_size: "<<srv.max_body_size<<"\n";
// 	return (0);
// }

int	take_default(std::string line, server& srv, LocationConfig &loc)
{
	std::cout<<line<<"\n";
	loc.methods = srv.methods;
	loc.autoindex = srv.autoindex;
	loc.upload_enable = srv.upload_enable;
	loc.upload_store = srv.upload_store;
	loc.index = srv.index;
	loc.redirect = srv.redirect;
	loc.root = srv.root;
	loc.max_body_size = srv.max_body_size;
	return (0);
}
