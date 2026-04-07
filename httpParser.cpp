/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 14:06:49 by rabusala          #+#    #+#             */
/*   Updated: 2026/04/07 21:08:14 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpReq.hpp"
#include "client.hpp"
#include <algorithm>
#include <ctype.h>

std::string toLower(const std::string& input)
{
	std::string result = input;

	for (size_t i = 0; i < result.size(); ++i)
	{
		result[i] = std::tolower(result[i]);
	}

	return result;
}
std::string ltrim(const std::string& s)
{
	size_t start = s.find_first_not_of(" \t");
	if (start == std::string::npos)
		return "";
	return s.substr(start);
}
std::string rtrim(const std::string& s)
{
	size_t end = s.find_last_not_of(" \t");
	if (end == std::string::npos)
		return "";
	return s.substr(0, end + 1);
}
std::string trim(const std::string& s)
{
	return rtrim(ltrim(s));
}

void iniClient(client &cli)
{
	cli.setContentLength(0);
	cli.setHeaderComplete(false);
	cli.setRequestComplete(false);
}
int checkReqLine(client &cli)
{
	if(cli.getReq().getMethod().empty() ||cli.getReq().getUri().empty()||cli.getReq().getVersion().empty())
	{
		cli.getRes().setStatusCode(400);
		return 1;
	}
	if(cli.getReq().getMethod()!="DELETE" && cli.getReq().getMethod()!="POST" && cli.getReq().getMethod()!="GET")
	{
		cli.getRes().setStatusCode(405);
		return 1;
	}
	if(cli.getReq().getVersion()!="HTTP/1.1"&&cli.getReq().getVersion()!="HTTP/1.0")
	{
		cli.getRes().setStatusCode(505);
		return 1;
	}
	return 0;


}
int checkUri(std::string uri)
{
	if(uri.find("../") != std::string::npos)
		return -1;
	return 1;
}
int parseReqLine(client &cli,std::string &reqline)
{
	std::string trimmedLine=ltrim(reqline);
	size_t pos1=trimmedLine.find(" ");
	if(pos1==std::string::npos)
		return 1;
	size_t pos2=trimmedLine.find(" ",pos1+1);
	if(pos2 == std::string::npos)
		return 1;
	cli.getReq().setMethod(trim(trimmedLine.substr(0,pos1)));
	cli.getReq().setUri(trim(trimmedLine.substr(pos1+1,pos2-pos1-1)));
	if(checkUri(cli.getReq().getUri()) == -1)
	{
		cli.getRes().setStatusCode(403);
		return 1;
	}
	cli.getReq().setVersion(trim(trimmedLine.substr(pos2+1)));
	if(checkReqLine(cli) == 1)
		return 1;
	return 0;



}
int countLines(const std::string &buffer)  // Add const reference
{
	std::string copy = buffer;  // Make a local copy
	int count = 0;
	while(!copy.empty())
	{
		size_t pos = copy.find("\r\n");
		if(pos == std::string::npos)
			break;
		copy.erase(0, pos+2);
		count++;
	}
	return count;
}

int parseHeader(client &cli)
{
	std::string copy = cli.getHeader();
	std::cout<<"Header to parse:\n"<<copy<<std::endl;
	int num = countLines(copy);
	for(int i=0;i<num;i++)
	{
		size_t pos = copy.find("\r\n");
		if(pos == std::string::npos)
		{
			std::cout<<"Error: No \\r\\n found in header."<<std::endl;
			break;
		}
		std::string headerLine = copy.substr(0,pos);
		size_t colonPos = headerLine.find(':');
		if(colonPos == std::string::npos)
			return 1;
		std::string key = trim(headerLine.substr(0,colonPos));
		std::string value = trim(headerLine.substr(colonPos+1));
		std::string realKey=toLower(key);
		if(cli.getReq().setHeader(realKey,value) == 1)
			return 1;
		copy.erase(0,pos+2);
		if(realKey == "content-length")
		{
			std::stringstream ss(value);
			size_t len;
			ss >> len;

			if (ss.fail() || !ss.eof())
				return 1;
			cli.setContentLength(len);

		}
		if(realKey == "Transfer-Encoding")
			cli.setIsChunkedEncoded(true);
	}
	return 0;
}
int parseReq(client &cli)
{
	size_t pos=cli.getHeader().find("\r\n");
	if(pos == std::string::npos)
		return 1;
	std::string requestLine = cli.getHeader().substr(0,pos);
	if(parseReqLine(cli,requestLine) ==1)
		return 1;
	cli.setHeader(cli.getHeader().erase(0,pos+2));
	if(parseHeader(cli) ==1)
		return 1;
	return 0;

}
int  checkHeader(client &cli)
{
	size_t pos = cli.getBuffer().find("\r\n\r\n");
	if(pos!=std::string::npos)
	{
		cli.setHeaderComplete(true);
		cli.setHeader(cli.getBuffer().substr(0,pos+2));
		if(parseReq(cli) == 1)
			return 1;
		if(cli.getContentLength() > 0)
			cli.setBodyStart(pos+4);
	}
	return 0;
}

bool isHexString(const std::string& s)
{
	for (std::string::const_iterator it = s.begin(); it != s.end(); ++it)
	{
		if (!isxdigit(*it))
			return false;
	}
	return true;
}

ssize_t convertHexa(client &cli)
{
    size_t pos = cli.getBuffer().find("\r\n");
    if (pos == std::string::npos)
        return -2;
    std::string hexLine = cli.getBuffer().substr(0, pos);
	size_t semi = hexLine.find(';');
	if (semi != std::string::npos)
	    hexLine = hexLine.substr(0, semi);
	if (hexLine.empty() || !isHexString(hexLine))
    	return -1;
    size_t size = std::strtol(hexLine.c_str(), NULL, 16);
    cli.setBuffer(cli.getBuffer().erase(0, pos + 2));
    return size;
}
int readChunks(client &cli)
{
	ssize_t chunkSize;
	// size_t pos;
	while(true)
	{

		if(cli.getChunkState()==READCHUNK)
		{
			chunkSize = convertHexa(cli);
			if(chunkSize == -2)
				return 0;
			if(chunkSize < 0)
				return -1;
			if(chunkSize == 0)
			{
				cli.setChunkState(DONECHUNKING);
				return 1;
			}
			cli.setChunkSize(chunkSize);
			cli.setChunkState(READDATA);
		}
		if(cli.getChunkState()==READDATA)
		{
			size_t needed=cli.getChunkSize()+2;
			if(cli.getBuffer().size()<needed)
				return 0;
			if (cli.getBuffer()[cli.getChunkSize()] != '\r'||cli.getBuffer()[cli.getChunkSize() + 1] != '\n')
				return -1;
			cli.getReq().appendBody(cli.getBuffer().substr(0,cli.getChunkSize()));
			cli.setBuffer(cli.getBuffer().erase(0,needed));
			if(cli.getReq().getBody().size() > (size_t)cli.getServer()->getMaxBodySize())
			{
				cli.getRes().setStatusCode(403);
				return -1;
			}
			cli.setChunkState(READCHUNK);
		}
	}
	return 0;
}
int	handleRead(client &cli,int fd)
{
	(void) fd;
	char temp[4096];
	ssize_t n=recv(cli.getFd(),temp,4096,0);
	if(n == 0)
		return 1;
	else if(n > 0)
	{
		cli.appendToResBuffer(temp,n);
		if(!cli.isHeaderComplete())
		{
			if(checkHeader(cli) == 1)
			{
				if(cli.getCode() == 0)
					cli.getRes().setStatusCode(400);
				return 1;
			}
		}
		if(cli.isHeaderComplete())
		{
			if(cli.isChunkedEncode())
			{
				int checker = readChunks(cli);
				if(checker == -1)
				{
					if(cli.getCode() == 0)
						cli.getRes().setStatusCode(400);
					cli.setState(ERROR);
					return 1;
				}
				if(checker == 1)
				{
					cli.setRequestComplete(true);
					cli.setBodySize(cli.getReq().getBody().size());
					cli.setState(ROUTING);
				}
			}
			else if(cli.getContentLength() > 0)
			{
				if(cli.getBuffer().size() >= cli.getBodyStart() + cli.getContentLength())
				{
					cli.getReq().setBody(cli.getBuffer().substr(cli.getBodyStart(),cli.getContentLength()));
					cli.setRequestComplete(true);
					cli.setBuffer(cli.getBuffer().erase(0,cli.getBodyStart() + cli.getContentLength()));
					cli.setState(ROUTING);
				}
			}
			else
			{
				cli.setRequestComplete(true);
				cli.setBuffer(cli.getBuffer().erase(0,cli.getBodyStart()));
				cli.setState(ROUTING);
			}
		}
	}
	else
	{
		if(errno == EWOULDBLOCK || errno == EAGAIN)
			return 0;//this is forbidden
		else
		{
			cli.setState(DONE);
			return 1;
		}
	}
	return 0;
}


