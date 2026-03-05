/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabusala <rabusala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 11:17:30 by balhamad          #+#    #+#             */
/*   Updated: 2026/03/05 18:09:58 by rabusala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "server.hpp"

int checkValidLocConfig(client &cli, server &srv, const LocationConfig& locConfig)
{
	std::string reqMethod = cli.getReq().getMethod();
	const std::vector<std::string>&allowedMethods= locConfig.getMethods();
	if (std::find(allowedMethods.begin(), allowedMethods.end(), reqMethod) == allowedMethods.end())
	{
		cli.getRes().setStatusCode(METHOD_NOT_ALLOWED);
		cli.setState(SENDING_RESPONSE);
		return 1;
	}
	else if(cli.getContentLength()>locConfig.getMaxBodySize())
	{
		cli.getRes().setStatusCode(PAYLOAD_TOO_LARGE);
		cli.setState(SENDING_RESPONSE);
		return 1;
	}
	else if(cli.getRes().getStatusCode()!=0)
	{
		cli.setState(SENDING_RESPONSE);
		return 1;
	}
	return 0;
}

std::string setupRootPath(client &cli, server &srv, const LocationConfig& locConfig, std::string uri)
{
	std::string Path = locConfig.getPath(); //location path
	if(uri.find(Path) == 0) //remove location path from uri
		uri.erase(0, Path.length());
	std::string root = locConfig.getRoot();
	if (!root.empty() && root[root.size()-1] != '/')
	    root += '/';

	if (!uri.empty() && uri[0] == '/')
	    uri.erase(0, 1);

	return root + uri;
	// std::string root = locConfig.getRoot(); //root
	// if(root[root.length() - 1] != '/' && (uri.empty() || uri[0] != '/'))
	// 	root.insert(0, "/");
	// std::string str = root + uri;
	// return (str);
}
int get_method(client &cli, server &srv, const LocationConfig& locConfig, std::string uri)
{
	std::string str = setupRootPath(cli, srv, locConfig, uri);
	const char *chr_str = str.c_str();

	struct stat stat_buf;
	if(stat(chr_str, &stat_buf) == -1)
	{
		cli.getRes().setStatusCode(NOT_FOUND);
		cli.setState(SENDING_RESPONSE);
		return (-1);
	}
	else if (S_ISDIR(stat_buf.st_mode))
	{
		cli.setIsDir(true);
		if (!uri.empty() && uri[uri.size()-1] != '/')
		{
			cli.getRes().setStatusCode(301);
			cli.getRes().addResHeader("Location", uri + "/");
			cli.setState(SENDING_RESPONSE);
			return -1;

		}
		if(!locConfig.getAutoindex())
		{
			if(locConfig.getIndex().empty())
			{
				cli.setState(SENDING_RESPONSE);
				cli.getRes().setStatusCode(FORBIDDEN);
				return (-1);
			}
			else
			{
				std::string indexPath = str;
				if (!indexPath.empty() && indexPath[indexPath.size()-1] != '/')
				    indexPath += '/';
				indexPath += locConfig.getIndex();
				cli.setFileFd(open(indexPath.c_str(), O_RDONLY));
				if(cli.getFileFd() < 0)
				{
					cli.getRes().setStatusCode(NOT_FOUND);
					cli.setState(SENDING_RESPONSE);
					return -1;
				}
				if (fstat(cli.getFileFd(), &stat_buf) == -1)
        		{
        		    close(cli.getFileFd());
        		    cli.getRes().setStatusCode(500);
        		    cli.setState(SENDING_RESPONSE);
        		    return -1;
        		}
				cli.getRes().setFileSize(stat_buf.st_size);
				cli.getRes().setHasFileBody(true);
				cli.getRes().setContentType(indexPath);
				cli.getRes().setStatusCode(OK);
				cli.setState(SENDING_RESPONSE);
			}
		}
		else
		{
			cli.setState(SENDING_RESPONSE);
			cli.getRes().setHasMemoryBody(true);
			cli.getRes().setContentLength(cli.getRes().getMemoryBody().size());
			cli.getRes().setStatusCode(OK);
			return(0);
		}
	}
	else if(S_ISREG(stat_buf.st_mode))
	{
		cli.setFileFd(open(chr_str, O_RDONLY));
		if(cli.getFileFd() < 0)
		{
			cli.getRes().setStatusCode(NOT_FOUND);
			cli.setState(SENDING_RESPONSE);
			return (-1);
		}
		if (fstat(cli.getFileFd(), &stat_buf) == 0)
		{
			cli.getRes().setFileSize(stat_buf.st_size);
			cli.getRes().setHasFileBody(true);
			cli.getRes().setStatusCode(OK);
			cli.getRes().setContentType(str);
			cli.setState(SENDING_RESPONSE);
			return 0;
		}
		else
		{
			close(cli.getFileFd());
			perror("Error getting file status");
			cli.setState(SENDING_RESPONSE);
			cli.getRes().setStatusCode(500);
			return (-1);
		}
	}
	else
	{
		cli.setState(SENDING_RESPONSE);
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
int handleRouting(client &cli, server &srv)
{
	std::string uri = cli.getReq().getUri();
	const std::map<std::string, LocationConfig>& locations = srv.getLocations();
	const LocationConfig* matchedLocation = findLongestMatch(uri, locations);
	if (matchedLocation)
	{
		if(checkValidLocConfig(cli, srv, *matchedLocation) == 1)
			return -1;

		//if(cgi)
		else if (cli.getReq().getMethod() == "GET")
		{
			get_method(cli, srv, *matchedLocation, uri);
		}
		// else if (cli.getReq().getMethod() == "POST")
		// {
		// 	if(post_method(cli, srv, *matchedLocation) == -1)
		// 		return 1;
		// 	else
		// 	{
		// 		cli.setState(SENDING_RESPONSE);
		// 		return 0;
		// 	}
		// }
		// else if (cli.getReq().getMethod() == "DELETE")
		// {
		// 	std::string filePath = matchedLocation->getRoot() + uri;
		// 	if(remove(filePath.c_str()) != 0)
		// 		return 1;
		// 	else
		// 	{
		// 		cli.setState(SENDING_RESPONSE);
		// 		return 0;
		// 	}
		// }
		craftResponse(cli,srv,*matchedLocation);
	}
	else
	{
		if(checkValidLocConfig(cli, srv, LocationConfig()) == 1) //check default location
		{
			cli.getRes().setStatusCode(METHOD_NOT_ALLOWED);
			return -1;//method not allowed 405
		}
	}
	  //handle default location
	  //similar to get method but with default location config
	  //if no default location => return 404 not found
	  cli.getRes().setStatusCode(NOT_FOUND);
	  return -1;

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
