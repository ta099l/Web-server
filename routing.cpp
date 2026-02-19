/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: balhamad <balhamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 11:17:30 by balhamad          #+#    #+#             */
/*   Updated: 2026/02/19 14:02:01 by balhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "server.hpp"
int routNOW(client &cli, server &srv, const LocationConfig& locConfig)
{
	std::string reqMethod = cli.getReq().getMethod();
	const std::vector<std::string>&allowedMethods= locConfig.getMethods();
	if (std::find(allowedMethods.begin(), allowedMethods.end(), reqMethod) == allowedMethods.end())
		return 1;
	return 0;
}
int get_method(client &cli, server &srv, const LocationConfig& locConfig)
{
	std::ifstream file(locConfig.getRoot() + cli.getReq().getUri());
	if(!file.is_open())
		return -1;
	std::string line;
	while(std::getline(file, line))
	{
		cli.setResponseBuffer(cli.getResponseBuffer() + line + "\n");
	}
	return 0;
}
int post_method(client &cli, server &srv, const LocationConfig& locConfig)
{
	std::ofstream file(locConfig.getRoot() + cli.getReq().getUri());
	if(!file.is_open())
		return -1;
	file << cli.getReq().getBody();
	return 0;
}
int handleRouting(client &cli, server &srv)
{
	std::string uri = cli.getReq().getUri();
	const std::map<std::string, LocationConfig>& locations = srv.getLocations();
	std::string matchedLocation = findLongestMatch(uri, locations);
	if (!matchedLocation.empty())
	{
		const LocationConfig& locConfig = locations.at(matchedLocation);
		if(routNOW(cli, srv, locConfig) == 1)
			return 1;//method not allowed 405
		else if (cli.getReq().getMethod() == "GET")
		{
			if(get_method(cli, srv, locConfig) == -1)
				return 1;
		}
		else if (cli.getReq().getMethod() == "POST")
		{
			if(post_method(cli, srv, locConfig) == -1)
				return 1;
		}
		else if (cli.getReq().getMethod() == "DELETE")
		{
			if(remove((locConfig.getRoot() + cli.getReq().getUri()).c_str()) != 0)
				return 1;
		}
	}
	else
	{
	}
	return 0;
}
std::string findLongestMatch(const std::string& uri, const std::map<std::string, LocationConfig>& locations) //or locationobj
{
	std::string longestMatch = "";
	for (std::map<std::string, LocationConfig>::const_iterator it = locations.begin(); it != locations.end(); ++it)
	{
		const std::string& locationPath = it->first;
		if (uri.compare(0, locationPath.size(), locationPath) == 0)
		{
			if (uri.size() == locationPath.size() || uri[locationPath.size()] == '/')
			{
				if (locationPath.size() > longestMatch.size())
					longestMatch = locationPath;
			}
		}
	}
	return longestMatch;
}

