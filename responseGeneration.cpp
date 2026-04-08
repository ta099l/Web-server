#include "webserv.hpp"
#include "client.hpp"
std::string buildErrorPath(const LocationConfig& loc, std::string path)
{
	std::string root = loc.getRoot();
	if (!root.empty() && root[root.size() - 1] != '/')
		root += '/';
	if (!path.empty() && path[0] == '/')
		path.erase(0, 1);
	return root + path;
}
std::string getReasonPhrase(int code)
{
	switch (code)
	{
		case 200: return "OK";
		case 201: return "OK";
		case 400: return "Bad Request";
		case 403: return "Forbidden";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";
		case 413: return "Payload Too Large";
		case 500: return "Internal Server Error";
		case 501: return "Not Implemented";
		default: return "Unknown Error";
	}
}
std::string genHtml(int code,std::string reason)
{
	reason = getReasonPhrase(code);

	std::stringstream ss;
	ss << "<!DOCTYPE html>\n"
		<< "<html lang=\"en\">\n"
		<< "<head>\n"
		<< " <meta charset=\"UTF-8\">\n"
		<< " <title>" << code << " " << reason << "</title>\n"
		<< "</head>\n"
		<< "<body style=\"font-family: Arial; text-align:center; margin-top:50px;\">\n"
		<< " <h1>" << code << "</h1>\n"
		<< " <h2>" << reason << "</h2>\n"
		<< " <hr>\n"
		<< " <p>The server encountered an error and could not complete your request.</p>\n"
		<< "</body>\n"
		<< "</html>";

	return ss.str();
}
bool generateErrorResponse(client &cli,server &serv)
{
	(void)serv;
	const LocationConfig* loc = cli.getLocation();
	bool genError = false;
	const std::map<int, std::string>& errorPages = loc->getErrorPages();
	int code = 0;
	std::string reason;
	for (std::map<int, std::string>::const_iterator it = errorPages.begin();it != errorPages.end();it++)
	{
		if(cli.getRes().getStatusCode() == it->first)
		{
			std::string path = buildErrorPath(*loc,it->second);
			cli.setGetFileFd(open(path.c_str(), O_RDONLY));
			if (cli.getGetFileFd() < 0)
			{
				cli.getRes().setHasMemoryBody(true);
				genError = true;
				break;
			}
			struct stat pathStat;
			if (fstat(cli.getGetFileFd(), &pathStat) == -1)
			{
				close(cli.getGetFileFd());
				cli.setGetFileFd(-1);
				cli.getRes().setHasMemoryBody(true);
				genError = true;
				break;

			}
			if (!S_ISREG(pathStat.st_mode))
			{
				close(cli.getGetFileFd());
				cli.setGetFileFd(-1);
				cli.getRes().setHasMemoryBody(true);
				genError = true;
				break;
			}
			cli.getRes().setFileSize(pathStat.st_size);
			cli.getRes().setHasFileBody(true);
			cli.getRes().setVersion(cli.getReq().getVersion());
			cli.getRes().setReason(getReasonPhrase(cli.getRes().getStatusCode()));
			cli.getRes().setContentType(path);
			cli.setState(READINGFILE);
			return false;
		}
	}
	if(genError)
		cli.getRes().setFileBody(genHtml(code,reason));
	return true;
}
void generateResponseHeader(client &cli,server &srv)
{
	(void)srv;
	std::stringstream resHeader;
	resHeader<< cli.getReq().getVersion()<<" "<<cli.getRes().getStatusCode()<<" "<<getReasonPhrase(cli.getRes().getStatusCode())<<"\r\n";
	if(cli.getRes().getHasFileBody()||cli.getRes().getHasMemoryBody())
		resHeader<<"Content-Type: "<<cli.getRes().getContentType()<<"\r\n";
	resHeader<<"Content-Length: "<<cli.getRes().getContentLength()<<"\r\n"
			 << "Connection: close\r\n"
			 <<"\r\n";
	std::string resHeaderStr = resHeader.str();
	cli.getRes().setGeneratedResponseHeader(true);
	cli.appendToResBuffer(resHeaderStr,resHeaderStr.size());
}
