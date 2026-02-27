/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 11:17:30 by balhamad          #+#    #+#             */
/*   Updated: 2026/02/27 15:19:06 by tabuayya         ###   ########.fr       */
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

int get_method(client &cli, server &srv, const LocationConfig& locConfig)
{
	std::string str = locConfig.getRoot() + cli.getReq().getUri();
	const char *chr_str = str.c_str();
	cli.setFileFd(open(chr_str, O_RDONLY));
	//if directory => handle index/autoindex (not implemented yet, but don’t treat as file
	if(cli.getFileFd() < 0)
	{
		cli.getRes().setStatusCode(404);
		return (-1);
		//build error response
	}
	struct stat file_info;
	if (fstat(cli.getFileFd(), &file_info) == 0)
	{
		cli.getRes().setFileSize(file_info.st_size);
		// cli.getRes().setFileModifiedTime(file_info.st_mtime);
		cli.setFileFd(cli.getFileFd());
		cli.getRes().setStatusCode(200);
	}
	else
	{
		perror("Error getting file status");
		cli.getRes().setStatusCode(500);
		return (-1);
	}
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
int handleRouting(client &cli, server &srv)
{
	std::string uri = cli.getReq().getUri();
	const std::map<std::string, LocationConfig>& locations = srv.getLocations();
	const LocationConfig* loc = findLongestMatch(uri, locations);
	if (loc != NULL)
	{
		if(cli.getContentLength() > loc->getMaxBodySize())
		{
			cli.getRes().setStatusCode(PAYLOAD_TOO_LARGE);
			return -1;//403
		}
	}
	else
	{

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

