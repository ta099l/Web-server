/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tasnimsamer <tasnimsamer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 11:17:30 by balhamad          #+#    #+#             */
/*   Updated: 2026/03/01 02:59:34 by tasnimsamer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "server.hpp"

int routNOW(client &cli, server &srv, const LocationConfig& locConfig)
{
	std::string reqMethod = cli.getReq().getMethod();
	const std::vector<std::string>&allowedMethods= locConfig.getMethods();
	if (std::find(allowedMethods.begin(), allowedMethods.end(), reqMethod) == allowedMethods.end())
		return 1; // 405
	return 0;
}

int get_method(client &cli, server &srv, const LocationConfig& locConfig, std::string uri)
{
	//I also have uri
	std::string Path = locConfig.getPath(); //location path
	if(uri.find(Path) == 0) //remove location path from uri
		uri.erase(0, Path.length());
	std::string root = locConfig.getRoot(); //root
	if(root[root.length() - 1] != '/' && (uri.empty() || uri[0] != '/'))
		root.insert(0, "/");
	std::string str = root + uri;
	const char *chr_str = str.c_str();

	struct stat stat_buf;
	if(stat(chr_str, &stat_buf) == -1)
	{
		cli.getRes().setStatusCode(NOT_FOUND);
		return (-1);
	}
	else if (S_ISDIR(stat_buf.st_mode))
	{
			return (0); //send to function for dir
		//check to autoindex or check index file
		//if directory => handle index/autoindex (not implemented yet, but don’t treat as file
	}
	else if(S_ISREG(stat_buf.st_mode))
	{
		cli.setFileFd(open(chr_str, O_RDONLY));
		if(cli.getFileFd() < 0)
		{
			cli.getRes().setStatusCode(NOT_FOUND);
			return (-1);
			//build error response
		}
		struct stat file_info;
		if (fstat(cli.getFileFd(), &file_info) == 0)
		{
			cli.getRes().setFileSize(file_info.st_size);
			// cli.getRes().setFileModifiedTime(file_info.st_mtime);
			cli.setFileFd(cli.getFileFd());
			cli.getRes().setStatusCode(OK);
		}
		else
		{
			perror("Error getting file status");
			cli.getRes().setStatusCode(500);
			return (-1);
		}
	}
	else
		cli.getRes().setStatusCode(NOT_FOUND);
	return 0;
}

int post_method(client &cli, server &srv, const LocationConfig& locConfig)
{
	if(cli.getContentLength() > locConfig.getMaxBodySize())
		return(-1); //if client sent more than allowed wrong
	std::ofstream file(locConfig.getRoot() + cli.getReq().getUri());
	if(!file.is_open())
		return -1;
	file << cli.getReq().getBody();
	return 0;
}
int handleRouting(client &cli, server &srv)
{
	std::string uri = cli.getReq().getUri();
	// TODO later: Check Host header here to see if we need to swap 'srv' for a virtual server
	const std::map<std::string, LocationConfig>& locations = srv.getLocations();
	const LocationConfig* loc = findLongestMatch(uri, locations);
	if (loc != NULL)
	{
		if(routNOW(cli, srv, *loc) == 1)
		{
			cli.getRes().setStatusCode(METHOD_NOT_ALLOWED);
			cli.setState("SENDING RESPONSE");
			return -1; //405
		}
		if(cli.getContentLength() > loc->getMaxBodySize())
		{
			cli.getRes().setStatusCode(PAYLOAD_TOO_LARGE);
			return -1;//403
		}
		if (cli.getReq().getMethod() == "GET")
		{
			get_method(cli, srv, *loc, uri);
			cli.setState("SENDING RESPONSE"); // Ready to send the file back!
		}
		else if (cli.getReq().getMethod() == "POST")
		{
			post_method(cli, srv, *loc);
			cli.setState("SENDING RESPONSE");
		}
		else if (cli.getReq().getMethod() == "DELETE")
		{
			//delete_method(cli, srv, *loc);
			cli.setState("SENDING RESPONSE");
		}
	}
	else
	{
		//go to srv defaults
	}
}
const LocationConfig* findLongestMatch(const std::string& uri, const std::map<std::string, LocationConfig>& locations) //or locationobj
{
	const LocationConfig* longestmatch = NULL;
	int max_size  = 0;
	for (std::map<std::string, LocationConfig>::const_iterator it = locations.begin(); it != locations.end(); ++it)
	{
		const std::string& locationPath = it->first;
		if (uri.compare(0, locationPath.size(), locationPath) == 0)
		{
			if (locationPath[locationPath.length() - 1] == '/' || uri.size() == locationPath.size() || uri[locationPath.size()] == '/')
			{
				if (locationPath.size() > max_size)
				{
					max_size = locationPath.size();
					longestmatch = &(it->second);
				}
			}
		}
	}
	return longestmatch;
}


// int handleRouting(client &cli, server &srv)
// {
// 	std::string uri = cli.getReq().getUri();
// 	const std::map<std::string, LocationConfig>& locations = srv.getLocations();
// 	std::string matchedLocation = findLongestMatch(uri, locations);
// 	if (!matchedLocation.empty())
// 	{
// 		const LocationConfig& locConfig = locations.at(matchedLocation);
// 		if(routNOW(cli, srv, locConfig) == 1)
// 		{
// 			cli.getRes().setStatusCode(405);
// 			return -1;//method not allowed 405
// 		}
// 		else if (cli.getReq().getMethod() == "GET")
// 		{
// 			if(get_method(cli, srv, locConfig) == -1)
// 				return 1;
// 			else
// 			{
// 				cli.setState("SENDING RESPONSE");
// 				epoll_ctl(, )
// 			}
// 		}
// 		else if (cli.getReq().getMethod() == "POST")
// 		{
// 			if(post_method(cli, srv, locConfig) == -1)
// 				return 1;
// 			else
// 				cli.setState("SENDING RESPONSE");
// 		}
// 		else if (cli.getReq().getMethod() == "DELETE")
// 		{
// 			if(remove((locConfig.getRoot() + cli.getReq().getUri()).c_str()) != 0)
// 				return 1;
// 			else
// 				cli.setState("SENDING RESPONSE");
// 		}
// 	}
// 	else
// 	{
// 	}
// 	return 0;
// }
