/* ************************************************************************** */
/*	*/
/*	:::	  ::::::::   */
/*   routing.cpp	:+:	  :+:	:+:   */
/*	+:+ +:+	 +:+	 */
/*   By: balhamad <balhamad@student.42.fr>	  +#+  +:+	   +#+	*/
/*	+#+#+#+#+#+   +#+	   */
/*   Created: 2026/02/18 11:17:30 by balhamad	  #+#	#+#	 */
/*   Updated: 2026/04/05 20:16:53 by balhamad	 ###   ########.fr	   */
/*	*/
/* ************************************************************************** */

#include <dirent.h>
#include <sys/stat.h>
#include <sstream>
#include "webserv.hpp"
#include "server.hpp"
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
				if (locationPath.size() > (unsigned long)max_size)
				{
					max_size = locationPath.size();
					longestmatch = &(it->second);
				}
			}
		}
	}
	return longestmatch;
}

int checkValidLocConfig(client &cli, server &srv, const LocationConfig& locConfig)
{
	(void)srv;
	std::string reqMethod = cli.getReq().getMethod();
	const std::vector<std::string>&allowedMethods= locConfig.getMethods();
	if (std::find(allowedMethods.begin(), allowedMethods.end(), reqMethod) == allowedMethods.end())
	{
		cli.getRes().setStatusCode(METHOD_NOT_ALLOWED);
		cli.setState(ERROR);
		return 1;
	}
	else if((long long)cli.getContentLength()>locConfig.getMaxBodySize())
	{
		cli.getRes().setStatusCode(PAYLOAD_TOO_LARGE);
		cli.setState(ERROR);
		return 1;
	}
	else if(cli.getRes().getStatusCode()!=0)
	{
		cli.setState(ERROR);
		return 1;
	}
	return 0;
}

std::string getExtension(const std::string& path)
{
	size_t pos = path.find_last_of('.');
	if (pos == std::string::npos)
		return "";
	return path.substr(pos);
}
int handleRouting(client &cli, server &srv)
{
	std::string uri = cli.getReq().getUri();
	const std::map<std::string, LocationConfig>& locations = srv.getLocations();
	const LocationConfig* matchedLocation = findLongestMatch(uri, locations);
	cli.setLocation(matchedLocation);
	if (matchedLocation)
	{
		if(checkValidLocConfig(cli, srv, *matchedLocation) == 1)
		{
			return 1;
		}
		std::string filePath = setupRootPath(cli, srv, *matchedLocation, uri);
		std::string ext = getExtension(filePath);
		const std::map<std::string, CGIConfig>& cgiMap = matchedLocation->getCgi();

		if (cgiMap.find(ext) != cgiMap.end())
		{
			const CGIConfig& cgi = cgiMap.at(ext);
			executeCGI(cli, cgi, filePath);
			return 1;
		}
		if (cli.getReq().getMethod() == "GET")
		{
			get_method(cli, srv, *matchedLocation, uri);
		}
		else if (cli.getReq().getMethod() == "POST")
		{
			std::cerr<<"in post hereeeeeeeeee"<<std::endl;
			post_method(cli, srv, *matchedLocation, uri);
		}
		else if (cli.getReq().getMethod() == "DELETE")
		{
			struct stat st;
			std::string filePath = setupRootPath(cli, srv, *matchedLocation, uri);
			if(stat(filePath.c_str(), &st) == 0 && S_ISDIR(st.st_mode))
			{
				cli.getRes().setStatusCode(FORBIDDEN);
				cli.setState(ERROR);
				return 1;
			}
			if(remove(filePath.c_str()) != 0)
			{
				cli.getRes().setStatusCode(404);
				cli.setState(ERROR);
				return 1;
			}
			else
			{
				cli.getRes().setStatusCode(204);
				cli.setState(ERROR);
				return 1;
			}
		}
	}
	else
	{
		std::cout<<"out matched locatio\n";

	}
	// else
	// {
	// 	if(checkValidLocConfig(cli, srv, LocationConfig()) == 1) //check default location
	// 	{
	// 		cli.getRes().setStatusCode(METHOD_NOT_ALLOWED);
	// 		return -1;//method not allowed 405
	// 	}
	// }
	  //handle default location
	  //similar to get method but with default location config
	  //if no default location => return 404 not found
	// else
	// {
	// 	cli.getRes().setStatusCode(NOT_FOUND);
	// 	cli.setState(ERROR);
	// 	return 1;
	// }
	return 1;

}


