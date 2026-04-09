/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tasnimsamer <tasnimsamer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 15:38:32 by tabuayya          #+#    #+#             */
/*   Updated: 2026/04/09 14:15:08 by tasnimsamer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "httpResponse.hpp"
// HttpResponse::HttpResponse() : generatedResponseHeader(false), fileFd(-1), fileSize(0), Bytes_Sent(0),hasFileBody(false), version("HTTP/1.1"), statusCode(200), reason("OK"), contentLength(0)
// {
// 	_initMimeTypes();
// }
HttpResponse::HttpResponse() :
	fileFd(-1),
	fileSize(0),
	Bytes_Sent(0),
	hasFileBody(false),
	hasMemoryBody(false),
	NeedsAutoindex(false),
	generatedResponseHeader(false),
	version("HTTP/1.1"),
	statusCode(0),
	reason("OK"),
	contentLength(0),
	memoryBody(""),
	fileBody(""),
	contentType(""),
	AutoIndexpath(""),
	resHeaders(),
	_mimeTypes()
{
	_initMimeTypes();
}
HttpResponse::~HttpResponse() {}
int HttpResponse::getFileFd() { return fileFd; }
size_t HttpResponse::getFileSize() { return fileSize; }
size_t HttpResponse::getBytes_Sent() { return Bytes_Sent; }
bool HttpResponse::getHasFileBody() { return hasFileBody; }
bool HttpResponse::getHasMemoryBody() { return hasMemoryBody; }
std::string HttpResponse::getVersion() { return version; }
int HttpResponse::getStatusCode() { return statusCode; }
std::string HttpResponse::getReason() { return reason; }
size_t HttpResponse::getContentLength() { return contentLength; }
std::string HttpResponse::getMemoryBody() { return memoryBody; }
std::string HttpResponse::getFileBody() { return fileBody; }
std::map<std::string, std::string> HttpResponse::getResHeaders() { return resHeaders; }
void HttpResponse::setFileFd(int fd) { fileFd = fd; }
void HttpResponse::setFileSize(size_t size) { fileSize = size; }
void HttpResponse::setBytes_Sent(size_t offset) { Bytes_Sent = offset; }
void HttpResponse::setHasFileBody(bool hasFileBody) { this->hasFileBody = hasFileBody; }
void HttpResponse::setHasMemoryBody(bool memFileBody) { this->hasMemoryBody = memFileBody; }
void HttpResponse::setVersion(const std::string& version) { this->version = version; }
void HttpResponse::setStatusCode(int statusCode) { this->statusCode = statusCode; }
void HttpResponse::setReason(const std::string& reason) { this->reason = reason; }
void HttpResponse::setContentLength(size_t contentLength) { this->contentLength = contentLength; }
void HttpResponse::setMemoryBody(const std::string& memoryBody) { this->memoryBody = memoryBody; }
void HttpResponse::setFileBody(const std::string& fileBody) { this->fileBody = fileBody; }
void HttpResponse::addResHeader(const std::string& key, const std::string& value) { resHeaders[key] = value; }
void HttpResponse::appendFileBody(const std::string &body,ssize_t n)
{
	fileBody.append(body,n);
}
void HttpResponse::setContentTypeString(const std::string& str)
{
	this->contentType = str;
}

void HttpResponse::setContentType(const std::string& filepath)
{
	size_t dotPos = filepath.find_last_of(".");
	std::string type = "application/octet-stream";
	if (dotPos != std::string::npos)
	{
		std::string ext = filepath.substr(dotPos);
		for (size_t i = 0; i < ext.length(); ++i)
		{
			ext[i] = std::tolower(ext[i]);
		}
		if (_mimeTypes.count(ext))
			type = _mimeTypes[ext];
	}
	this->contentType = type;
	// addResHeader("Content-Type", type);
}
std::string HttpResponse::getContentType() { return contentType; }
void HttpResponse::_initMimeTypes()
{
	_mimeTypes[".html"] = "text/html";
	_mimeTypes[".css"]  = "text/css";
	_mimeTypes[".js"]   = "application/javascript";
	_mimeTypes[".png"]  = "image/png";
	_mimeTypes[".jpg"]  = "image/jpeg";
	_mimeTypes[".jpeg"] = "image/jpeg";
	_mimeTypes[".txt"]  = "text/plain";
	_mimeTypes[".json"] = "application/json";
}
// std::string HttpResponse::generateResponse(client &cli, server &srv)
// {
// 	std::string response = version + " " + std::to_string(statusCode) + " " + reason + "\r\n";
// 	for (std::map<std::string, std::string>::iterator it = resHeaders.begin(); it != resHeaders.end(); ++it)
// 	{
// 		response += it->first + ": " + it->second + "\r\n";
// 	}
// 	response += "\r\n";
// 	response += memoryBody;
// 	return response;
// }

void HttpResponse::setAutoindexFsPath(const std::string& path)
{
	AutoIndexpath = path;
}
std::string HttpResponse::getAutoindexFsPath()
{
	return AutoIndexpath;
}
