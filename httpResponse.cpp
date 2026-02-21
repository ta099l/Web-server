/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 15:38:32 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/21 15:51:22 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "httpResponse.hpp"
HttpResponse::HttpResponse() : fileFd(-1), fileSize(0), fileOffset(0), hasFileBody(false), version("HTTP/1.1"), statusCode(200), reason("OK"), contentLength(0) {}
HttpResponse::~HttpResponse() {}
int HttpResponse::getFileFd() { return fileFd; }
size_t HttpResponse::getFileSize() { return fileSize; }
size_t HttpResponse::getFileOffset() { return fileOffset; }
bool HttpResponse::getHasFileBody() { return hasFileBody; }
std::string HttpResponse::getVersion() { return version; }
int HttpResponse::getStatusCode() { return statusCode; }
std::string HttpResponse::getReason() { return reason; }
size_t HttpResponse::getContentLength() { return contentLength; }
std::string HttpResponse::getMemoryBody() { return memoryBody; }
std::string HttpResponse::getFileBody() { return fileBody; }
std::map<std::string, std::string> HttpResponse::getResHeaders() { return resHeaders; }
void HttpResponse::setFileFd(int fd) { fileFd = fd; }
void HttpResponse::setFileSize(size_t size) { fileSize = size; }
void HttpResponse::setFileOffset(size_t offset) { fileOffset = offset; }
void HttpResponse::setHasFileBody(bool hasFileBody) { this->hasFileBody = hasFileBody; }
void HttpResponse::setVersion(const std::string& version) { this->version = version; }
void HttpResponse::setStatusCode(int statusCode) { this->statusCode = statusCode; }
void HttpResponse::setReason(const std::string& reason) { this->reason = reason; }
void HttpResponse::setContentLength(size_t contentLength) { this->contentLength = contentLength; }
void HttpResponse::setMemoryBody(const std::string& memoryBody) { this->memoryBody = memoryBody; }
void HttpResponse::setFileBody(const std::string& fileBody) { this->fileBody = fileBody; }
void HttpResponse::addResHeader(const std::string& key, const std::string& value) { resHeaders[key] = value; }
void HttpResponse::setContentType(const std::string& contentType)
{
	this->contentType = contentType;
	// addResHeader("Content-Type", contentType);
}
std::string HttpResponse::getContentType() { return contentType; }
std::string HttpResponse::generateResponse(client &cli, server &srv)
{
	std::string response = version + " " + std::to_string(statusCode) + " " + reason + "\r\n";
	for (std::map<std::string, std::string>::iterator it = resHeaders.begin(); it != resHeaders.end(); ++it)
	{
		response += it->first + ": " + it->second + "\r\n";
	}
	response += "\r\n";
	response += memoryBody;
	return response;
}

