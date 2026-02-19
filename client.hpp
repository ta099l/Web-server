/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: balhamad <balhamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:24:47 by rabusala          #+#    #+#             */
/*   Updated: 2026/02/19 11:43:19 by balhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "HttpReq.hpp"

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
	size_t bodyStart;
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
		//setters
		void setBodyStart(size_t n);
		void setState(std::string state);
		void setBuffer(const std::string &buffer);
		void setResponseBuffer(const std::string &responseBuffer);
		void setContentLength(size_t contentLength);
		void setHeaderComplete(bool headerComplete);
		void setRequestComplete(bool requestComplete);
		void setHeader(const std::string &header);
		void appendToBuffer(const std::string &data,int n);
		//checkers
		bool isHeaderComplete();
		bool isRequestComplete();
};
int handleRead(client &cli,int fd);

#endif
