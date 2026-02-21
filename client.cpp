/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:25:50 by rabusala          #+#    #+#             */
/*   Updated: 2026/02/21 15:42:06 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"
client::client() : fd(-1), state(""), buffer(""), responseBuffer(""), contentLength(0), headerComplete(false), requestComplete(false) {}
client::client(int fd) : fd(fd), state(""), buffer(""), responseBuffer(""), contentLength(0), headerComplete(false), requestComplete(false) {}
client::client(const client &other) : fd(other.fd), state(other.state), buffer(other.buffer), responseBuffer(other.responseBuffer), contentLength(other.contentLength), headerComplete(other.headerComplete), requestComplete(other.requestComplete), req(other.req) {}
client& client::operator=(const client &other) {
	if (this != &other) {
		fd = other.fd;
		state = other.state;
		buffer = other.buffer;
		responseBuffer = other.responseBuffer;
		contentLength = other.contentLength;
		headerComplete = other.headerComplete;
		requestComplete = other.requestComplete;
		req = other.req;
	}
	return *this;
}
client::~client() {}
int client::getFd()
{
	return fd;
}
std::string client::getBuffer() { return buffer; }
std::string client::getResponseBuffer() { return responseBuffer; }
size_t client::getContentLength() { return contentLength; }
size_t client::getBodyStart(){return bodyStart;}
bool client::isHeaderComplete() { return headerComplete; }
bool client::isRequestComplete() { return requestComplete; }
HttpReq& client::getReq() { return req; }
std::string client::getState() { return state; }
void client::setState(std::string state) { this->state = state;}
void client::setBuffer(const std::string &buffer) { this->buffer = buffer; }
void client::setResponseBuffer(const std::string &responseBuffer) { this->responseBuffer = responseBuffer; }
void client::setContentLength(size_t contentLength) { this->contentLength = contentLength; }
void client::setHeaderComplete(bool headerComplete) { this->headerComplete = headerComplete  ; }
void client::setRequestComplete(bool requestComplete) { this->requestComplete = requestComplete; }
void client::setBodyStart(size_t n){this->bodyStart = n;}
std::string client::getHeader() { return header; }
void client::setHeader(const std::string &header) { this->header = header; }
void client::appendToBuffer(const std::string &data,int n) { buffer.append(data, 0, n); }
size_t client::getFileSize()
{
	return outFileSize;
}
int client::getFileOffset()
{
	return outFileOffset;
}
void client::setFileSize(size_t num)
{
	outFileSize = num;
}
void client::setFileOffset(int num)
{
	outFileOffset = num;
}
bool client::isFileDone()
{
	return fileDone;
}
void client::setFileDone(bool done)
{
	fileDone = done;
}
int client::getFileFd()
{
	return outFileFd;
}
void client::setFileFd(int fd)
{
	outFileFd = fd;
}
HttpResponse& client::getRes()
{
	return res;
}
