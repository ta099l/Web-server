/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 15:05:44 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/22 15:52:49 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP
#include "webserv.hpp"

class HttpResponse
{
	private:
		int fileFd;
		size_t fileSize;
		size_t fileOffset;
		bool hasFileBody;
		std::string version;
		int statusCode;
		std::string reason;
		size_t contentLength;
		std::string memoryBody;
		std::string fileBody;
		std::string contentType;
		std::map<std::string, std::string> resHeaders;
	public:
		HttpResponse();
		~HttpResponse();
		int getFileFd();
		size_t getFileSize();
		size_t getFileOffset();
		bool getHasFileBody();
		std::string getVersion();
		int getStatusCode();
		std::string getReason();
		size_t getContentLength();
		std::string getMemoryBody();
		std::string getFileBody();
		std::string getContentType();
		std::map<std::string, std::string> getResHeaders();
		//setters
		void setFileFd(int fd);
		void setFileSize(size_t size);
		void setFileOffset(size_t offset);
		void setHasFileBody(bool hasFileBody);
		void setVersion(const std::string& version);
		void setStatusCode(int statusCode);
		void setReason(const std::string& reason);
		void setContentLength(size_t contentLength);
		void setMemoryBody(const std::string& memoryBody);
		void setFileBody(const std::string& fileBody);
		void addResHeader(const std::string& key, const std::string& value);
		void setContentType(const std::string& contentType);
		std::string generateResponse(client &cli, server &srv);
};
#endif
