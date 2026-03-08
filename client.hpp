/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabusala <rabusala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:24:47 by rabusala          #+#    #+#             */
/*   Updated: 2026/03/08 20:40:23 by rabusala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <cstddef> // size_t

#include "HttpReq.hpp"
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
	DONE
};

class client
{
	int fd;
	ClientState state;
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
	size_t outFileSize;
	int outFileFd;
	int outFileOffset;
	bool fileDone;
	bool isDir;
	std::string uploadPath;
	public:
		client();
		client(int fd);
		client(const client &other);
		client& operator=(const client &other);
		~client();
		//getters
		int getFd();
		int getFileFd();
		int getFileOffset();
		int getCode();
		bool getIsDir();
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
		//setters
		void setFd(int fd);
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
		void setFileOffset(int num);
		void setFileDone(bool done);
		void setCode(int code);
		void setFileFd(int fd);
		void setIsDir(bool val);
		void setUploadPath(std::string path);
		//checkers
		bool isHeaderComplete();
		bool isRequestComplete();
		bool isFileDone();

};

int handleRead(client &cli,int fd);

#endif
