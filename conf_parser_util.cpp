/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_parser_util.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 21:03:35 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/13 21:22:32 by tabuayya         ###   ########.fr       */
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


