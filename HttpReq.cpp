/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpReq.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabusala <rabusala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:19:50 by rabusala          #+#    #+#             */
/*   Updated: 2026/04/20 17:05:48 by rabusala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpReq.hpp"
HttpReq::HttpReq() : method(""), uri(""), version(""), body("") {}
HttpReq::HttpReq(const HttpReq &other) : method(other.method), uri(other.uri), version(other.version), body(other.body) {}
HttpReq& HttpReq::operator=(const HttpReq &other) {
	if (this != &other) {
		method = other.method;
		uri = other.uri;
		version = other.version;
		body = other.body;
	}
	return *this;
}
HttpReq::~HttpReq() {}
std::string HttpReq::getMethod() { return method; }
std::string HttpReq::getUri() { return uri; }
std::string HttpReq::getVersion() { return version; }
std::string HttpReq::getBody() { return body; }
std::string HttpReq::setMethod(const std::string &method) { this->method = method; return this->method; }
std::string HttpReq::setUri(const std::string &uri) { this->uri = uri; return this->uri; }
std::string HttpReq::setVersion(const std::string &version) { this->version = version; return this->version; }
std::string HttpReq::setBody(const std::string &body) { this->body = body; return this->body; }
void HttpReq::appendBody(std::string str)
{
	// body.append(str,str.size());
	body+=str;
}

const std::map<std::string, std::string>& HttpReq::getHeaders() const
{
	return headers;
}

int HttpReq::setHeader(const std::string &key, const std::string &value)
{
	if(headers.find(key) != headers.end())
		return 1;
	headers[key] = value;
	return 0;
}
