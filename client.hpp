/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tasnimsamer <tasnimsamer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:24:47 by rabusala          #+#    #+#             */
/*   Updated: 2026/03/01 02:30:00 by tasnimsamer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "HttpReq.hpp"
#include "webserv.hpp"
#include "httpResponse.hpp"

class client
{
	int fd;
	std::string state;
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
	public:
		client();
		client(int fd);
		client(const client &other);
		client& operator=(const client &other);
		~client();
		//getters
		int getFd();
		size_t getBodyStart();
		std::string getBuffer();
		std::string getResponseBuffer();
		size_t getContentLength();
		HttpReq& getReq();
		std::string getState();
		std::string getHeader();
		size_t getFileSize();
		int getFileFd();
		int getFileOffset();
		HttpResponse& getRes();
		//setters
		void setFd(int fd);
		void setBodyStart(size_t n);
		void setState(std::string state);
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
		void setFileFd(int fd);
		//checkers
		bool isHeaderComplete();
		bool isRequestComplete();
		bool isFileDone();
};

int handleRead(client &cli,int fd);

#endif
