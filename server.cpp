/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: balhamad <balhamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 17:18:26 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/15 15:14:11 by balhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

server::server() : root(""), index(""), max_body_size(0),
 autoindex(false),  upload_enable(false), server_fd(-1)
{
}
server::server(const server &obj)
	: listens(obj.listens),
	root(obj.root),
	index(obj.index),
	max_body_size(obj.max_body_size),
	error_pages(obj.error_pages),
	locations(obj.locations),
	methods(obj.methods),
	autoindex(obj.autoindex),
	upload_enable(obj.upload_enable),
	upload_store(obj.upload_store),
	redirect(obj.redirect),
	cgi(obj.cgi),
	server_fd(obj.server_fd)
{
}
server& server::operator=(const server &obj)
{
	if (this != &obj)
	{
		listens = obj.listens;
		root = obj.root;
		index = obj.index;
		max_body_size = obj.max_body_size;
		error_pages = obj.error_pages;
		locations = obj.locations;
		methods = obj.methods;
		autoindex = obj.autoindex;
		upload_enable = obj.upload_enable;
		upload_store = obj.upload_store;
		redirect = obj.redirect;
		cgi = obj.cgi;
		server_fd = obj.server_fd;
	}
	return *this;
}

const std::vector<ListenConfig>& server::getListens() const
{
	return listens;
}
const std::string& server::getRoot() const
{
	return root;
}
const std::string& server::getIndex() const
{
	return index;
}
long long server::getMaxBodySize() const
{
	return max_body_size;
}
const std::map<int, std::string>& server::getErrorPages() const
{
	return error_pages;
}
const std::map<std::string, LocationConfig>& server::getLocations() const
{
	return locations;
}
const std::vector<std::string>& server::getMethods() const
{
	return methods;
}
bool server::getAutoindex() const
{
	return autoindex;
}
bool server::getUploadEnable() const
{
	return upload_enable;
}
const std::string& server::getUploadStore() const
{
	return upload_store;
}
const std::string& server::getRedirect() const
{
	return redirect;
}
const std::map<std::string, CGIConfig>& server::getCgi() const
{
	return cgi;
}
int server::getServerFd() const
{
	return server_fd;
}
void server::setServerFd(int fd)
{
	server_fd = fd;
}
void server::addListen(const ListenConfig& l)
{
	listens.push_back(l);
}
void server::setRoot(const std::string& r)
{
	root = r;
}
void server::setIndex(const std::string& i)
{
	index = i;
}
void server::setMaxBodySize(long long size)
{
	max_body_size = size;
}
void server::setAutoindex(bool value)
{
	autoindex = value;
}
void server::setUploadEnable(bool value)
{
	upload_enable = value;
}
void server::setUploadStore(const std::string& path)
{
	upload_store = path;
}
void server::setRedirect(const std::string& r)
{
	redirect = r;
}
void server::addMethod(const std::string& method)
{
	methods.push_back(method);
}
void server::addErrorPage(int code, const std::string& path)
{
	error_pages[code] = path;
}
void server::addLocation(const LocationConfig& loc)
{
	locations[loc.getPath()] = loc;
}
void server::addCgi(const CGIConfig& c)
{
	cgi[c.extension] = c;
}


LocationConfig::LocationConfig()
{
	autoindex = false;
	upload_enable = false;
	max_body_size = 0;
}

// LocationConfig Getters
const std::string& LocationConfig::getPath() const
{
	return path;
}
const std::vector<std::string>& LocationConfig::getMethods() const
{
	return methods;
}
bool LocationConfig::getAutoindex() const
{
	return autoindex;
}
bool LocationConfig::getUploadEnable() const
{
	return upload_enable;
}
const std::string& LocationConfig::getUploadStore() const
{
	return upload_store;
}
const std::string& LocationConfig::getRoot() const
{
	return root;
}
const std::string& LocationConfig::getIndex() const
{
	return index;
}
const std::string& LocationConfig::getRedirect() const
{
	return redirect;
}
long long LocationConfig::getMaxBodySize() const
{
	return max_body_size;
}
const std::map<std::string, CGIConfig>& LocationConfig::getCgi() const
{
	return cgi;
}
const std::map<int, std::string>& LocationConfig::getErrorPages() const
{
	return error_pages;
}

// LocationConfig Setters
void LocationConfig::setPath(const std::string& p)
{
	path = p;
}
void LocationConfig::addMethod(const std::string& method)
{
	methods.push_back(method);
}
void LocationConfig::setAutoindex(bool value)
{
	autoindex = value;
}
void LocationConfig::setUploadEnable(bool value)
{
	upload_enable = value;
}
void LocationConfig::setUploadStore(const std::string& path)
{
	upload_store = path;
}
void LocationConfig::setRoot(const std::string& r)
{
	root = r;
}
void LocationConfig::setIndex(const std::string& i)
{
	index = i;
}
void LocationConfig::setRedirect(const std::string& r)
{
	redirect = r;
}
void LocationConfig::setMaxBodySize(long long size)
{
	max_body_size = size;
}
void LocationConfig::addCgi(const CGIConfig& c)
{
	cgi[c.extension] = c;
}
void LocationConfig::addErrorPage(int code, const std::string& path)
{
	error_pages[code] = path;
}

// ListenConfig Constructors
ListenConfig::ListenConfig()
	: host(""), port(0)
{
}
ListenConfig::ListenConfig(const std::string& h, int p)
	: host(h), port(p)
{
}

// ListenConfig Getters
const std::string& ListenConfig::getHost() const
{
	return host;
}
int ListenConfig::getPort() const
{
	return port;
}

// ListenConfig Setters
void ListenConfig::setHost(const std::string& h)
{
	host = h;
}
void ListenConfig::setPort(int p)
{
	port = p;
}

