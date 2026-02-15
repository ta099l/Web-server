/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: balhamad <balhamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 17:32:41 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/15 17:03:19 by balhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

webserv::webserv()
{
}
webserv::~webserv()
{
}
std::vector<server>& webserv::getServers()
{
    return servers;
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
    std::cout << "    Location: " << loc.getPath() << std::endl;

    std::cout << "      root: " << loc.getRoot() << std::endl;
    std::cout << "      index: " << loc.getIndex() << std::endl;

    std::cout << "      autoindex: "
              << (loc.getAutoindex() ? "on" : "off")
              << std::endl;

    std::cout << "      upload_enable: "
              << (loc.getUploadEnable() ? "on" : "off")
              << std::endl;

    std::cout << "      max_body_size: "
              << loc.getMaxBodySize()
              << std::endl;

    std::cout << "      methods: ";
    const std::vector<std::string>& methods = loc.getMethods();
    for (size_t i = 0; i < methods.size(); ++i)
        std::cout << methods[i] << " ";
    std::cout << std::endl;

    std::cout << "      upload_store: "
              << (loc.getUploadStore().empty() ? "(none)" : loc.getUploadStore())
              << std::endl;

    std::cout << "      redirect: "
              << (loc.getRedirect().empty() ? "(none)" : loc.getRedirect())
              << std::endl;

    std::cout << "      error_pages:" << std::endl;
    const std::map<int, std::string>& error_pages = loc.getErrorPages();
    for (std::map<int, std::string>::const_iterator it =
         error_pages.begin();
         it != error_pages.end();
         ++it)
    {
        std::cout << "        "
                  << it->first
                  << " -> "
                  << it->second
                  << std::endl;
    }

    std::cout << "      CGI:" << std::endl;
    const std::map<std::string, CGIConfig>& cgi = loc.getCgi();
    for (std::map<std::string, CGIConfig>::const_iterator it =
         cgi.begin();
         it != cgi.end();
         ++it)
    {
        std::cout << "        extension: "
                  << it->first
                  << std::endl;
    }
}



// void printServer(const server& server)
// {
// 	std::cout << "----------------------------------------" << std::endl;
// 	std::cout << "Server:" << std::endl;

// 	for (size_t i = 0; i < server.listens.size(); ++i)
// 	{
// 		std::cout << "    "
// 				<< server.listens[i].host
// 				<< ":"
// 				<< server.listens[i].port
// 				<< std::endl;
// 	}
// 	std::cout << "  root: " << server.root << std::endl;
// 	std::cout << "  index: " << server.index << std::endl;
// 	std::cout << "  client_max_body_size: "
// 			<< server.max_body_size << std::endl;
// 	std::cout << "      methods: ";
// 	for (size_t i = 0; i < server.methods.size(); ++i)
// 		std::cout << server.methods[i] << " ";
// 	std::cout << std::endl;
// 	std::cout << "  error_pages:" << std::endl;
// 	for (std::map<int, std::string>::const_iterator it = server.error_pages.begin();
// 		it != server.error_pages.end(); ++it)
// 	{
// 		std::cout << "    " << it->first << " -> " << it->second << std::endl;
// 	}
// 	std::cout << "  autoindex: "
// 	          << (server.autoindex ? "on" : "off")
// 	          << std::endl;

// 	std::cout << "  upload_enable: "
// 	          << (server.upload_enable ? "on" : "off")
// 	          << std::endl;

// 	std::cout << "  upload_store: "
// 	          << (server.upload_store.empty() ? "(none)" : server.upload_store)
// 	          << std::endl;

// 	std::cout << "  redirect: "
// 	          << (server.redirect.empty() ? "(none)" : server.redirect)
// 	          << std::endl;

// 	std::cout << "  CGI:" << std::endl;
// 	for (std::map<std::string, CGIConfig>::const_iterator it =
// 	     server.cgi.begin();
// 	     it != server.cgi.end();
// 	     ++it)
// 	{
// 	    std::cout << "    extension: "
// 	              << it->first
// 	              << std::endl;
// 	}

// 	std::cout << "  locations:" << std::endl;

// 	for (std::map<std::string, LocationConfig>::const_iterator it =
// 			server.locations.begin();
// 		it != server.locations.end();
// 		++it)
// 	{
// 		printLocation(it->second);
// 	}

// std::cout << "----------------------------------------" << std::endl;

// }

// void webserv::printConfig() const
// {
// 	for (size_t i = 0; i < this->servers.size(); ++i)
// 		printServer(this->servers[i]);
// }

