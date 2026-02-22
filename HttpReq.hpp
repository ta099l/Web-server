/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpReq.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 10:42:14 by rabusala          #+#    #+#             */
/*   Updated: 2026/02/22 15:54:09 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQ_HPP
#define HTTPREQ_HPP
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include<iostream>
#include <sys/socket.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

class HttpReq
{
	std::string method;
	std::string uri;
	std::string version;
	std::map<std::string, std::string> headers;
	std::string body;
	public:
		HttpReq();
		HttpReq(const HttpReq &other);
		HttpReq& operator=(const HttpReq &other);
		~HttpReq();
		//getters
		std::string getMethod();
		std::string getUri();
		std::string getVersion();
		std::string getBody();
		const std::map<std::string, std::string>& getHeaders() const;
		//setters
		std::string setMethod(const std::string &method);
		std::string setUri(const std::string &uri);
		std::string setVersion(const std::string &version);
		std::string setBody(const std::string &body);
		int setHeader(const std::string &key, const std::string &value);
};
#endif
