/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 17:32:41 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/14 19:32:58 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

webserv::webserv()
{
}
webserv::~webserv()
{
}
const std::vector<server>& webserv::getServers() const
{
	return(this->servers);
}
void webserv::setServers(const server& s)
{
	servers.push_back(s);
}
void printLocation(const LocationConfig& loc)
{
	std::cout << "    Location: " << loc.path << std::endl;

	std::cout << "      root: " << loc.root << std::endl;
	std::cout << "      index: " << loc.index << std::endl;
	std::cout << "      autoindex: "
			<< (loc.autoindex ? "on" : "off")
			<< std::endl;

	std::cout << "      methods: ";
	for (size_t i = 0; i < loc.methods.size(); ++i)
		std::cout << loc.methods[i] << " ";
	std::cout << std::endl;

	if (!loc.upload_store.empty())
		std::cout << "      upload_store: "
				<< loc.upload_store
				<< std::endl;

	if (!loc.redirect.empty())
		std::cout << "      redirect: "
				<< loc.redirect
				<< std::endl;
}


void printServer(const server& server)
{
	std::cout << "----------------------------------------" << std::endl;
	std::cout << "Server:" << std::endl;

	for (size_t i = 0; i < server.listens.size(); ++i)
	{
		std::cout << "    "
				<< server.listens[i].host
				<< ":"
				<< server.listens[i].port
				<< std::endl;
	}
	std::cout << "  root: " << server.root << std::endl;
	std::cout << "  index: " << server.index << std::endl;
	std::cout << "  client_max_body_size: "
			<< server.max_body_size << std::endl;
	std::cout << "      methods: ";
	for (size_t i = 0; i < server.methods.size(); ++i)
		std::cout << server.methods[i] << " ";
	std::cout << std::endl;
	std::cout << "  error_pages:" << std::endl;
	for (std::map<int, std::string>::const_iterator it = server.error_pages.begin();
		it != server.error_pages.end(); ++it)
	{
		std::cout << "    " << it->first << " -> " << it->second << std::endl;
	}

	std::cout << "  locations:" << std::endl;

	for (std::map<std::string, LocationConfig>::const_iterator it =
			server.locations.begin();
		it != server.locations.end();
		++it)
	{
		printLocation(it->second);
	}

std::cout << "----------------------------------------" << std::endl;

}

void webserv::printConfig() const
{
	for (size_t i = 0; i < this->servers.size(); ++i)
		printServer(this->servers[i]);
}

