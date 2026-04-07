/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routing_methods.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 13:59:45 by tabuayya          #+#    #+#             */
/*   Updated: 2026/04/07 21:02:40 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <sys/stat.h>
#include <sstream>
#include "webserv.hpp"
#include "server.hpp"

std::string setupRootPath(client &cli, server &srv, const LocationConfig& locConfig, std::string uri)
{
	(void)cli;
	(void)srv;
	std::string Path = locConfig.getPath();
	if(uri.find(Path) == 0)
		uri.erase(0, Path.length());
	std::string root = locConfig.getRoot();
	if (!root.empty() && root[root.size()-1] != '/')
		root += '/';

	if (!uri.empty() && uri[0] == '/')
		uri.erase(0, 1);

	return (root + uri);
}

void generateAutoindexListing(client &cli, const std::string& uri, const std::string& rootPath)
{
	DIR *dir = opendir(rootPath.c_str());
	if (!dir)
	{
		cli.getRes().setStatusCode(500);
		return;
	}

	std::stringstream html;

	// Header and CSS (C++98 compatible)
	html << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n";
	html << "<meta charset=\"UTF-8\">\n";
	html << "<title>✨ Index of " << uri << " ✨</title>\n";
	html << "<style>\n"
		<< "body { margin: 0; font-family: 'Poppins', Arial, sans-serif; height: 100vh; overflow: hidden;\n"
		<< "background: linear-gradient(135deg, #ffd6e8, #e0c3fc, #cdb4db); background-size: 400% 400%;\n"
		<< "animation: gradientFlow 10s ease infinite; }\n"
		<< "@keyframes gradientFlow { 0%{background-position:0% 50%} 50%{background-position:100% 50%} 100%{background-position:0% 50%} }\n"
		<< ".decor span { position: absolute; font-size: 60px; opacity: 0.95; text-shadow: 2px 2px 8px rgba(255,255,255,0.5);\n"
		<< "animation: float 4s linear infinite; z-index: 1; bottom: -100px; }\n"
		<< "@keyframes float { 0%{transform:translateY(0) scale(0.5); opacity:0;} 10%{opacity:1;} 100%{transform:translateY(-120vh) scale(1.2); opacity:0;} }\n"
		<< ".container { position: relative; z-index: 2; max-width: 650px; margin: 80px auto; background: rgba(255,255,255,0.85);\n"
		<< "backdrop-filter: blur(10px); padding: 30px; border-radius: 25px; box-shadow: 0 10px 40px rgba(0,0,0,0.15); }\n"
		<< "h1 { text-align: center; color: #ff4da6; }\n"
		<< "a { display: block; padding: 12px; margin: 8px 0; border-radius: 12px; text-decoration: none; color: #333;\n"
		<< "background: #f9f9f9; transition: 0.3s; }\n"
		<< "a:hover { background: #ffe0f0; color: #ff4da6; transform: scale(1.03); }\n"
		<< ".dir { font-weight: bold; }\n"
		<< "</style>\n</head>\n<body>\n";

	// Fast Floating Emojis
	html << "<div class=\"decor\">\n"
		<< "<span style=\"left:5%; animation-delay:0s;\">💖</span>\n"
		<< "<span style=\"left:20%; animation-delay:0.4s;\">✨</span>\n"
		<< "<span style=\"left:35%; animation-delay:1s;\">💗</span>\n"
		<< "<span style=\"left:50%; animation-delay:0.2s;\">✨</span>\n"
		<< "<span style=\"left:65%; animation-delay:0.8s;\">💞</span>\n"
		<< "<span style=\"left:80%; animation-delay:1.4s;\">✨</span>\n"
		<< "</div>\n";

	html << "<div class=\"container\">\n";
	html << "<h1>📂 Index of " << uri << "</h1>\n";

	// Parent Link
	if (uri != "/")
		html << "<a class=\"dir\" href=\"../\">⬅️ ../</a>\n";

	// Read target directory files and folders
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		std::string name = entry->d_name;
		if (name == "." || name == "..")
			continue;

		std::string fullPath = rootPath + "/" + name;
		struct stat st;
		if (stat(fullPath.c_str(), &st) == -1)
			continue;

		if (S_ISDIR(st.st_mode))
			html << "<a class=\"dir\" href=\"" << name << "/\">📁 " << name << "/</a>\n";
		else
			html << "<a href=\"" << name << "\">📄 " << name << "</a>\n";
	}

	html << "</div>\n</body>\n</html>";
	closedir(dir);

	std::string finalHtml = html.str();
	cli.getRes().appendFileBody(finalHtml, finalHtml.size());
	cli.getRes().setContentLength(finalHtml.size());
}

int get_method(client &cli, server &srv, const LocationConfig& locConfig, std::string uri)
{
	std::string rootPath = setupRootPath(cli, srv, locConfig, uri);
	const char *chr_str = rootPath.c_str();
	struct stat stat_buf;

	if (stat(chr_str, &stat_buf) == -1)
	{
		cli.getRes().setStatusCode(NOT_FOUND);
		cli.setState(ERROR);
		return (-1);
	}
	else if (S_ISDIR(stat_buf.st_mode))
	{
		cli.setIsDir(true);
		if (!uri.empty() && uri[uri.size() - 1] != '/')
		{
			cli.getRes().setStatusCode(301);
			cli.getRes().addResHeader("Location", uri + "/");
			cli.setState(ERROR);
			return (0);
		}
		bool indexUsable = false;
		std::string indexPath;
		struct stat indexStat;
		if (!locConfig.getIndex().empty())
		{
			indexPath = rootPath;
			if (!indexPath.empty() && indexPath[indexPath.size() - 1] != '/')
				indexPath += '/';
			indexPath += locConfig.getIndex();

			if (stat(indexPath.c_str(), &indexStat) == 0 && S_ISREG(indexStat.st_mode) && access(indexPath.c_str(), R_OK) == 0)
				indexUsable = true;
		}
		if (indexUsable)
		{
			cli.setGetFileFd(open(indexPath.c_str(), O_RDONLY));
			if (cli.getGetFileFd() < 0)
			{
				cli.getRes().setStatusCode(500);
				cli.setState(ERROR);
				return (-1);
			}
			if (fstat(cli.getGetFileFd(), &indexStat) == -1)
			{
				close(cli.getGetFileFd());
				cli.setGetFileFd(-1);
				cli.getRes().setStatusCode(500);
				cli.setState(ERROR);
				return (-1);
			}
			if (!S_ISREG(indexStat.st_mode))
			{
				close(cli.getGetFileFd());
				cli.setGetFileFd(-1);
				cli.getRes().setStatusCode(FORBIDDEN);
				cli.setState(ERROR);
				return (-1);
			}
			cli.getRes().setFileSize(indexStat.st_size);
			cli.getRes().setHasFileBody(true);
			cli.getRes().setContentType(indexPath);
			cli.getRes().setStatusCode(OK);
			cli.setState(READINGFILE);
			return (0);
		}
		else if (locConfig.getAutoindex())
		{
			generateAutoindexListing(cli, uri, rootPath);
			cli.getRes().setContentTypeString("text/html");
			cli.getRes().setHasMemoryBody(true);
			cli.getRes().setStatusCode(OK);
			cli.setState(SENDING_RESPONSE);
			return (0);
		}
		else
		{
			cli.getRes().setStatusCode(FORBIDDEN);
			cli.setState(ERROR);
			return (-1);
		}
	}
	else if (S_ISREG(stat_buf.st_mode))
	{
		if (access(chr_str, R_OK) != 0)
		{
			cli.getRes().setStatusCode(FORBIDDEN);
			cli.setState(ERROR);
			return (-1);
		}
		cli.setGetFileFd(open(chr_str, O_RDONLY));
		if (cli.getGetFileFd() < 0)
		{
			cli.getRes().setStatusCode(NOT_FOUND);
			cli.setState(ERROR);
			return (-1);
		}
		if (fstat(cli.getGetFileFd(), &stat_buf) == -1)
		{
			close(cli.getGetFileFd());
			cli.setGetFileFd(-1);
			cli.getRes().setStatusCode(500);
			cli.setState(ERROR);
			return (-1);
		}
		if (!S_ISREG(stat_buf.st_mode))
		{
			close(cli.getGetFileFd());
			cli.setGetFileFd(-1);
			cli.getRes().setStatusCode(FORBIDDEN);
			cli.setState(ERROR);
			return (-1);
		}
		cli.getRes().setContentLength(stat_buf.st_size);
		cli.getRes().setHasFileBody(true);
		cli.getRes().setStatusCode(OK);
		cli.getRes().setContentType(rootPath);
		cli.setState(READINGFILE);
		return (0);
	}
	else
	{
		cli.getRes().setStatusCode(FORBIDDEN);
		cli.setState(ERROR);
		return (-1);
	}
}

std::string setupUploadPath(client &cli, server &srv, const LocationConfig& LocConfig, std::string uri)
{
	(void)cli;
	(void)srv;
	size_t pos = uri.rfind('/');
	if(pos ==std::string::npos)
		return "";
	std::string fileName=uri.substr(pos+1);
	if(fileName.empty())
		return "";
	return(LocConfig.getUploadStore()+"/"+fileName);

}
int post_method(client &cli, server &srv, const LocationConfig& locConfig, std::string uri)
{
	std::string path;
	struct stat st;
	if(!locConfig.getUploadEnable()) //and not CGI
	{
		cli.getRes().setStatusCode(FORBIDDEN);
		cli.setState(ERROR);
		return (-1);
	}
	if (locConfig.getUploadStore().empty())
	{
		cli.getRes().setStatusCode(INTERNAL_SERVER_ERROR);
		cli.setState(ERROR);
		return (-1);
	}
	if (locConfig.getUploadStore() != "")
	{
		if(stat(locConfig.getUploadStore().c_str(),&st) == -1||!S_ISDIR(st.st_mode))
		{
			cli.getRes().setStatusCode(INTERNAL_SERVER_ERROR);
			cli.setState(ERROR);
			return (-1);
		}
		path =  setupUploadPath(cli, srv, locConfig, uri);
		if(path.empty())
		{
			cli.getRes().setStatusCode(BAD_REQUEST);
			cli.setState(ERROR);
			return (-1);
		}
		if(stat(path.c_str(),&st) == 0)
		{
			if(S_ISDIR(st.st_mode))
			{
				cli.getRes().setStatusCode(FORBIDDEN);
				cli.setState(ERROR);
				return (-1);
			}
			cli.setUploadPath(path);
			cli.setState(OVERWRITE);
			return (0);
		}
		else
		{
			cli.setUploadPath(path);
			cli.setState(UPLOADING);
			return (0);
		}
	}
	return 0;
}
