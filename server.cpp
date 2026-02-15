/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 17:18:26 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/15 14:43:27 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

server::server() : root(""), index(""), max_body_size(0),
 autoindex(false),  upload_enable(false)
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
	cgi(obj.cgi)
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
	locations[loc.path] = loc;
}
void server::addCgi(const CGIConfig& c)
{
	cgi[c.extension] = c;
}


server::~server()
{

}
LocationConfig::LocationConfig()
{
	autoindex = false;
	upload_enable = false;
	max_body_size = 0;
}

