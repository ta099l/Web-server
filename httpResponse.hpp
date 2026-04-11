/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 15:05:44 by tabuayya          #+#    #+#             */
/*   Updated: 2026/04/11 19:37:32 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP
// #include "client.hpp"
#include <string>
#include <map>
#include <cstddef>
#include <unistd.h>
class client;
class HttpResponse
{
	private:
		int fileFd;
		size_t fileSize;
		size_t Bytes_Sent;
		bool hasFileBody;
		bool hasMemoryBody;
		bool NeedsAutoindex;
		bool generatedResponseHeader;
		std::string version;
		int statusCode;
		std::string reason;
		size_t contentLength;
		std::string memoryBody;
		std::string fileBody;
		std::string contentType;
		std::string AutoIndexpath;
		std::map<std::string, std::string> resHeaders;
		std::map<std::string, std::string> _mimeTypes;
		void _initMimeTypes();
	public:
		HttpResponse();
		~HttpResponse();
		int getFileFd();
		size_t getFileSize();
		size_t getBytes_Sent();
		bool getHasFileBody();
		bool getHasMemoryBody();
		bool getNeedsAutoindex();
		std::string getVersion();
		int getStatusCode();
		std::string getReason();
		size_t getContentLength();
		std::string getMemoryBody();
		std::string getFileBody();
		std::string getContentType();
		std::map<std::string, std::string> getResHeaders();
		bool getGeneratedResponseHeader(){return generatedResponseHeader;}
		//setters
		void setFileFd(int fd);
		void setAutoindexFsPath(const std::string& path);
		std::string getAutoindexFsPath();
		void setFileSize(size_t size);
		void setBytes_Sent(size_t offset);
		void setHasFileBody(bool hasFileBody);
		void setHasMemoryBody(bool hasFileBody);
		void setNeedsAutoindex(bool val);
		void setVersion(const std::string& version);
		void setStatusCode(int statusCode);
		void setReason(const std::string& reason);
		void setContentLength(size_t contentLength);
		void setMemoryBody(const std::string& memoryBody);
		void setFileBody(const std::string& fileBody);
		void appendFileBody(const char *fileBody,ssize_t n);
		void addResHeader(const std::string& key, const std::string& value);
		void setContentType(const std::string& contentType);
		void setGeneratedResponseHeader(bool val){generatedResponseHeader = val; }
	void setContentTypeString(const std::string& str);

		//std::string generateResponse(client &cli, server &srv);
		//functions
		//void buildErrorResponse(Codes code,LocationConfig *loc);
};
#endif
