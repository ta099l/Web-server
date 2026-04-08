/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabusala <rabusala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:25:50 by rabusala          #+#    #+#             */
/*   Updated: 2026/04/08 19:02:53 by rabusala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"
#include "server.hpp"
// client::client() :_server(NULL), bodySize(0), getFileFd(-1),BytesSent(0), chunkSize(0), cstate(READCHUNK), chunkedEncoded(false), isDir(false),fd(-1), state(READING), buffer(""), responseBuffer(""), contentLength(0), headerComplete(false), requestComplete(false), code(0), postFileFd(-1), postFileSize(0), fileDone(false),bodyStart(0) {}
client::client() :
	fd(-1),
	_server(NULL),
	location(NULL),
	state(READING),
	cstate(READCHUNK),
	code(0),
	buffer(""),
	responseBuffer(""),
	header(""),
	contentLength(0),
	headerComplete(false),
	requestComplete(false),
	req(),
	res(),
	bodyStart(0),
	postFileSize(0),
	postFileFd(-1),
	getFileFd(-1),
	BytesSent(0),
	fileDone(false),
	isDir(false),
	chunkedEncoded(false),
	uploadPath(""),
	chunkSize(0),
	bodySize(0)
{}
// client::client(int fd,server *srv) :_server(srv),bodySize(0),getFileFd(-1),BytesSent(0),chunkSize(0),cstate(READCHUNK),chunkedEncoded(false),isDir(false), fd(fd), state(READING), buffer(""), responseBuffer(""), contentLength(0), headerComplete(false), requestComplete(false), code(0), postFileFd(-1), postFileSize(0), fileDone(false) {}
client::client(int fd, server *srv) :
	fd(fd),
	_server(srv),
	location(NULL),
	state(READING),
	cstate(READCHUNK),
	code(0),
	buffer(""),
	responseBuffer(""),
	header(""),
	contentLength(0),
	headerComplete(false),
	requestComplete(false),
	req(),
	res(),
	bodyStart(0),
	postFileSize(0),
	postFileFd(-1),
	getFileFd(-1),
	BytesSent(0),
	fileDone(false),
	isDir(false),
	chunkedEncoded(false),
	uploadPath(""),
	chunkSize(0),
	bodySize(0)
{}
// client::client(const client &other) :BytesSent(0),chunkedEncoded(other.chunkedEncoded), isDir(other.isDir), fd(other.fd), state(other.state), buffer(other.buffer), responseBuffer(other.responseBuffer), contentLength(other.contentLength), headerComplete(other.headerComplete), requestComplete(other.requestComplete), req(other.req), bodyStart(other.bodyStart) , fileDone(other.fileDone) ,postFileFd(other.postFileFd){}
client::client(const client &other) :
	fd(other.fd),
	_server(other._server),
	location(other.location),
	state(other.state),
	cstate(other.cstate),
	code(other.code),
	buffer(other.buffer),
	responseBuffer(other.responseBuffer),
	header(other.header),
	contentLength(other.contentLength),
	headerComplete(other.headerComplete),
	requestComplete(other.requestComplete),
	req(other.req),
	res(other.res),
	bodyStart(other.bodyStart),
	postFileSize(other.postFileSize),
	postFileFd(other.postFileFd),
	getFileFd(other.getFileFd),
	BytesSent(other.BytesSent),
	fileDone(other.fileDone),
	isDir(other.isDir),
	chunkedEncoded(other.chunkedEncoded),
	uploadPath(other.uploadPath),
	chunkSize(other.chunkSize),
	bodySize(other.bodySize)
{}
client& client::operator=(const client &other) {
	if (this != &other) {
		fd = other.fd;
		state = other.state;
		// buffer = other.buffer;
		responseBuffer = other.responseBuffer;
		contentLength = other.contentLength;
		headerComplete = other.headerComplete;
		requestComplete = other.requestComplete;
		code = other.code;
		postFileFd = other.postFileFd;
		postFileSize = other.postFileSize;
		BytesSent = other.BytesSent;
		fileDone = other.fileDone;
		req = other.req;
		isDir = other.isDir;
		chunkedEncoded = other.chunkedEncoded;
	}
	return *this;
}
client::~client() {}
int client::getFd()
{
	return fd;
}
size_t client::getChunkSize()
{
	return chunkSize;
}
void client::setChunkSize(size_t size)
{
	chunkSize=size;
}
bool client::isChunkedEncode()
{
	return chunkedEncoded;
}
void client::setIsChunkedEncoded(bool val)
{
	chunkedEncoded=val;
}
std::string client::getBuffer() { return buffer; }
std::string client::getResponseBuffer() { return responseBuffer; }
size_t client::getContentLength() { return contentLength; }
size_t client::getBodyStart(){return bodyStart;}
bool client::isHeaderComplete() { return headerComplete; }
bool client::isRequestComplete() { return requestComplete; }
HttpReq& client::getReq() { return req; }
ClientState client::getState() { return state; }
void client::setState(ClientState state) { this->state = state;}
void client::setBuffer(const std::string &buffer) { this->buffer = buffer; }
void client::setResponseBuffer(const std::string &responseBuffer) { this->responseBuffer = responseBuffer; }
void client::setContentLength(size_t contentLength) { this->contentLength = contentLength; }
void client::setHeaderComplete(bool headerComplete) { this->headerComplete = headerComplete  ; }
void client::setRequestComplete(bool requestComplete) { this->requestComplete = requestComplete; }
void client::setBodyStart(size_t n){this->bodyStart = n;}
std::string client::getHeader() { return header; }
void client::setHeader(const std::string &header) { this->header = header; }
void client::appendToResBuffer(const std::string &data,size_t n)
{
	responseBuffer.append(data, n);
}
chunksStates client::getChunkState(){
    return cstate;
}
void client::setChunkState(chunksStates state) {
    cstate = state;
}
void client::setBytesSent(size_t n)
{
	BytesSent = n;
}
int client::getCode()
{
	return code;
}
std::string client::getUploadPath() const
{
	return uploadPath;

}
void client::setUploadPath(std::string path)
{
	uploadPath=path;
}

void client::setCode(int code)
{
	this->code = code;
}
size_t client::getFileSize()
{
	return postFileSize;
}
size_t client::getBytesSent()
{
	return BytesSent;
}
void client::setFileSize(size_t num)
{
	postFileSize = num;
}
void client::addBytesSent(size_t num)
{
	BytesSent += num;
}
bool client::isFileDone()
{
	return fileDone;
}
bool client::getIsDir()
{
	return isDir;
}
void client::setIsDir(bool val)
{
	isDir=val;
}
void client::setFileDone(bool done)
{
	fileDone = done;
}
int client::getPostFileFd()
{
	return postFileFd;
}
void client::setPostFileFd(int fd)
{
	postFileFd = fd;
}
int client::getGetFileFd()
{
	return getFileFd;
}
void client::setGetFileFd(int fd)
{
	getFileFd = fd;
}
HttpResponse& client::getRes()
{
	return res;
}
size_t client::getBodySize()
{
	return bodySize;
}
void client::setBodySize(size_t s)
{
	bodySize = s;
}
void client::setServer(server* srv)
{
	_server = srv;
}

server* client::getServer()
{
	return _server;
}

const server* client::getServer() const
{
	return _server;
}
const LocationConfig* client::getLocation()
{
	return location;
}
void client::setLocation(const LocationConfig *loc)
{
	location=loc;
}
