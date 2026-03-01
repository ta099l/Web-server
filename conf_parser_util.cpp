/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_parser_util.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 21:03:35 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/21 11:28:39 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	parse_allowed_methods(const std::vector<std::string>& tokens, LocationConfig& loc)
{
	loc.clearMethods();
	for (size_t i = 1; i < tokens.size(); ++i)
	{
		if (tokens[i] != "GET" && tokens[i] != "POST" && tokens[i] != "DELETE")
		{
			std::cerr << "Invalid HTTP method: " << tokens[i] << "\n";
			exit(1);
		}
		loc.addMethod(tokens[i]);
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

	loc.addCgi(cgi);
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

	srv.addErrorPage((int)code, tokens[2]);
}

int	take_default(std::string line, server& srv, LocationConfig &loc)
{
	std::cout<<line<<"\n";
	// Copy methods from server to location
	const std::vector<std::string>& methods = srv.getMethods();
	for (size_t i = 0; i < methods.size(); ++i)
		loc.addMethod(methods[i]);

	// Copy error pages from server to location
	const std::map<int, std::string>& error_pages = srv.getErrorPages();
	for (std::map<int, std::string>::const_iterator it = error_pages.begin(); it != error_pages.end(); ++it)
		loc.addErrorPage(it->first, it->second);

	loc.setAutoindex(srv.getAutoindex());
	loc.setUploadEnable(srv.getUploadEnable());
	loc.setUploadStore(srv.getUploadStore());
	loc.setIndex(srv.getIndex());
	loc.setRedirect(srv.getRedirect());
	loc.setRoot(srv.getRoot());
	loc.setMaxBodySize(srv.getMaxBodySize());

	return (0);
}
