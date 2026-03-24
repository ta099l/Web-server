/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabusala <rabusala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:24:47 by rabusala          #+#    #+#             */
/*   Updated: 2026/03/17 16:07:46 by rabusala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <cstddef> // size_t

#include "HttpReq.hpp"
#include "server.hpp"
#include "httpResponse.hpp"
enum ClientState
{
	READING,
	ROUTING,
	CGI_WRITING_STDIN,
	CGI_READING_STDOUT,
	UPLOADING,
	OVERWRITE,
	READINGFILE,
	SENDING_RESPONSE,
	ERROR,
	DONE
};
enum chunksStates
{
	READCHUNK,
	READDATA,
	DONECHUNKING
};

class server;

class client
{
	int fd;
	server* _server;
	const LocationConfig *location;
	server srv;
	ClientState state;
	chunksStates cstate;
	int code;
	std::string buffer;
	std::string responseBuffer;
	std::string header;
	size_t contentLength;
	bool headerComplete;
	bool requestComplete;
	HttpReq req;
	HttpResponse res;
	size_t bodyStart;
	//outfile
	size_t postFileSize;
	int postFileFd;
	int postFileOffset;
	int getFileFd;
	int getFileOffset;

	bool fileDone;
	bool isDir;
	bool chunkedEncoded;
	std::string uploadPath;
	size_t chunkSize;
	size_t bodySize;
	public:
		client();
		client(int fd,server *srv);
		client(const client &other);
		client& operator=(const client &other);
		~client();
		//getters
		int getFd();
		chunksStates getChunkState();
		int getGetFileFd();
		int getGetFileOffset();
		int getPostFileFd();
		int getPostFileOffset();

		int getCode();
		bool getIsDir();
		size_t getChunkSize();
		size_t getBodyStart();
		size_t getContentLength();
		HttpReq& getReq();
		ClientState getState();
		std::string getHeader();
		size_t getFileSize();
		HttpResponse& getRes();
		std::string getBuffer();
		std::string getResponseBuffer();
		std::string getUploadPath() const;
		size_t getBodySize();
		//setters
		void setBodySize(size_t s);
		void setFd(int fd);
		void setChunkSize(size_t size);
		void setChunkState(chunksStates cstate);
		void setBodyStart(size_t n);
		void setState(ClientState state);
		void setBuffer(const std::string &buffer);
		void setResponseBuffer(const std::string &responseBuffer);
		void setContentLength(size_t contentLength);
		void setHeaderComplete(bool headerComplete);
		void setRequestComplete(bool requestComplete);
		void setHeader(const std::string &header);
		void appendToBuffer(const std::string &data,int n);
		void setFileSize(size_t num);
		void setPostFileOffset(int num);
		void setGetFileOffset(int num);
		void setFileDone(bool done);
		void setCode(int code);
		void setPostFileFd(int fd);
		void setGetFileFd(int fd);
		void setIsDir(bool val);
		void setIsChunkedEncoded(bool val);
		void setUploadPath(std::string path);
		//checkers
		bool isChunkedEncode();
		bool isHeaderComplete();
		bool isRequestComplete();
		bool isFileDone();
		void setServer(server* srv);
    	server* getServer();
    	const server* getServer() const;
		const LocationConfig* getLocation();
		void setLocation(const LocationConfig *loc);

};

int handleRead(client &cli,int fd);

#endif
