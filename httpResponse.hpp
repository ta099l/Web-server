/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabusala <rabusala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 15:05:44 by tabuayya          #+#    #+#             */
/*   Updated: 2026/03/08 16:32:36 by rabusala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP
#include "client.hpp"
#include <string>
#include <map>
#include <cstddef>
class HttpResponse
{
	private:
		int fileFd;
		size_t fileSize;
		size_t fileOffset;
		bool hasFileBody;
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
		size_t getFileOffset();
		bool getHasFileBody();
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
		void setFileOffset(size_t offset);
		void setHasFileBody(bool hasFileBody);
		void setNeedsAutoindex(bool val);
		void setVersion(const std::string& version);
		void setStatusCode(int statusCode);
		void setReason(const std::string& reason);
		void setContentLength(size_t contentLength);
		void setMemoryBody(const std::string& memoryBody);
		void setFileBody(const std::string& fileBody);
		void appendFileBody(const std::string &fileBody,ssize_t n);
		void addResHeader(const std::string& key, const std::string& value);
		void setContentType(const std::string& contentType);
		void setGeneratedResponseHeader(bool val){generatedResponseHeader = val; }
		//std::string generateResponse(client &cli, server &srv);
		//functions
		//void buildErrorResponse(Codes code,LocationConfig *loc);
};
#endif
