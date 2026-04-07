#include "webserv.hpp"
#include "client.hpp"
void generatePostSuccessPage(client &cli)
{
	std::stringstream html;

	html << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n"
		<< "    <meta charset=\"UTF-8\">\n"
		<< "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
		<< "    <title>✨ Upload Success ✨</title>\n"
		<< "    <style>\n"
		<< "        body {\n"
		<< "            margin: 0; font-family: 'Poppins', Arial, sans-serif;\n"
		<< "            height: 100vh; overflow: hidden; display: flex;\n"
		<< "            justify-content: center; align-items: center;\n"
		<< "            background: linear-gradient(135deg, #ffd6e8, #e0c3fc, #cdb4db);\n"
		<< "            background-size: 400% 400%; animation: gradientFlow 10s ease infinite;\n"
		<< "        }\n"
		<< "        @keyframes gradientFlow {\n"
		<< "            0% { background-position: 0% 50%; } 50% { background-position: 100% 50%; } 100% { background-position: 0% 50%; }\n"
		<< "        }\n"
		<< "        .decor span {\n"
		<< "            position: absolute; font-size: 60px; opacity: 0.95;\n"
		<< "            text-shadow: 2px 2px 8px rgba(255, 255, 255, 0.5);\n"
		<< "            animation: float 4s linear infinite; z-index: 1;\n"
		<< "            pointer-events: none; bottom: -100px;\n"
		<< "        }\n"
		<< "        @keyframes float {\n"
		<< "            0% { transform: translateY(0) scale(0.5); opacity: 0; }\n"
		<< "            10% { opacity: 1; }\n"
		<< "            100% { transform: translateY(-120vh) scale(1.2); opacity: 0; }\n"
		<< "        }\n"
		<< "        .card {\n"
		<< "            position: relative; z-index: 2; width: 450px;\n"
		<< "            background: rgba(255, 255, 255, 0.85); backdrop-filter: blur(10px);\n"
		<< "            padding: 50px; border-radius: 25px; box-shadow: 0 10px 40px rgba(0,0,0,0.15);\n"
		<< "            text-align: center; border: 1px solid rgba(255, 255, 255, 0.3);\n"
		<< "        }\n"
		<< "        h1 { color: #ff4da6; margin-bottom: 10px; font-size: 2.2em; }\n"
		<< "        p { color: #555; font-size: 1.1em; margin-bottom: 20px; }\n"
		<< "        .btn {\n"
		<< "            display: inline-block; padding: 12px 30px;\n"
		<< "            background: linear-gradient(45deg, #ff4da6, #c77dff); color: white;\n"
		<< "            text-decoration: none; border-radius: 25px; transition: 0.3s;\n"
		<< "            font-weight: bold; box-shadow: 0 4px 15px rgba(255, 77, 166, 0.3);\n"
		<< "        }\n"
		<< "        .btn:hover {\n"
		<< "            transform: scale(1.1); box-shadow: 0 6px 20px rgba(255, 77, 166, 0.5);\n"
		<< "        }\n"
		<< "    </style>\n</head>\n<body>\n";

	html << "    <div class=\"decor\">\n"
		<< "        <span style=\"left:10%; animation-delay:0s;\">💖</span>\n"
		<< "        <span style=\"left:25%; animation-delay:0.5s;\">✨</span>\n"
		<< "        <span style=\"left:40%; animation-delay:1.2s;\">💗</span>\n"
		<< "        <span style=\"left:60%; animation-delay:0.2s;\">✨</span>\n"
		<< "        <span style=\"left:75%; animation-delay:0.8s;\">✨</span>\n"
		<< "        <span style=\"left:85%; animation-delay:1.5s;\">💞</span>\n"
		<< "    </div>\n";

	html << "    <div class=\"card\">\n"
		<< "        <h1>💖 Success!</h1>\n"
		<< "        <p>Your file has been uploaded to the server ✨</p>\n"
		<< "        <a href=\"/\" class=\"btn\">Go Back Home</a>\n"
		<< "    </div>\n"
		<< "</body>\n</html>";

	std::string responseBody = html.str();
	cli.getRes().appendFileBody(responseBody, responseBody.size());
	cli.getRes().setHasMemoryBody(true);
	cli.getRes().setContentLength(responseBody.size());
}
void handleUpload(client &cli,server &serv,const ClientState &state)
{
	(void)serv;
	cli.setPostFileFd(open(cli.getUploadPath().c_str(),O_CREAT|O_WRONLY|O_TRUNC,0644));
	if(cli.getPostFileFd()<0)
	{
		cli.setState(ERROR);
		cli.getRes().setStatusCode(500);
		return ;
	}
	write(cli.getPostFileFd(),cli.getReq().getBody().c_str(),cli.getContentLength());
	close(cli.getPostFileFd());
	if(state == UPLOADING)
		cli.getRes().setStatusCode(200);
	else if(state == OVERWRITE)
		cli.getRes().setStatusCode(201);
	generatePostSuccessPage(cli);
	cli.getRes().setContentTypeString("text/html");
	cli.setState(SENDING_RESPONSE);
	return ;
}
void handleFileReading(client &cli,server &srv)
{
	(void)srv;
	char readBuffer[8192];
	ssize_t n=read(cli.getGetFileFd(),readBuffer,sizeof(readBuffer));
	if(n>0)
	{
		cli.getRes().appendFileBody(readBuffer,n);
		cli.setState(SENDING_RESPONSE);
	}
	else if(n==0)
	{
		close(cli.getGetFileFd());
		cli.setFileDone(true);
		cli.getRes().setStatusCode(200);
		cli.setState(SENDING_RESPONSE);
	}
	else
	{
		close(cli.getGetFileFd());
		cli.getRes().setStatusCode(500);
		cli.setState(ERROR);
	}
}
bool handleWrite(client &cli, server &serv)
{
	if (!cli.getRes().getGeneratedResponseHeader()) {
		generateResponseHeader(cli, serv);
	}
	if(cli.getState() == ERROR)
	{
		generateErrorResponse(cli,serv);
	}
	if (cli.getResponseBuffer().empty() && cli.getRes().getHasFileBody() && !cli.isFileDone())
	{
		cli.setState(READINGFILE);
		return false;
	}
	const std::string& buffer = cli.getResponseBuffer();
	size_t remaining = buffer.size() - cli.getBytesSent();
	ssize_t sent = send(cli.getFd(), buffer.c_str() + cli.getBytesSent(), remaining, 0);
	if (sent < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK) return false;
		return true;
	}
	cli.addBytesSent(sent);
	if (cli.getBytesSent() >= buffer.size())
	{
		cli.setResponseBuffer("");
		cli.setBytesSent(0);
		if (cli.getRes().getHasFileBody() && !cli.isFileDone())
		{
			cli.setState(READINGFILE);
			return false;
		}
		return true;
	}
	return false;
}
// bool handleWrite(client &cli,server &serv)
// {
// 	if(!cli.getRes().getGeneratedResponseHeader())
// 	{
// 		generateResponseHeader(cli,serv);
// 	}
// 	if(cli.getState() == ERROR)
// 	{
// 		generateErrorResponse(cli,serv);
// 	}
// 	std::string fileBody = cli.getRes().getFileBody();
// 	cli.appendToResBuffer(fileBody,fileBody.size());
// 	cli.getRes().setFileBody("");
// 	if(cli.getRes().getHasFileBody())
// 	{
// 		if(!cli.isFileDone())
// 			cli.setState(READINGFILE);
// 	}
// 	ssize_t sent = send(cli.getFd(),cli.getResponseBuffer().c_str() + cli.getBytesSent(),cli.getResponseBuffer().size() - cli.getBytesSent(),0);
// 	if (sent < 0)
// 	{
// 		if (errno == EAGAIN || errno == EWOULDBLOCK)
// 			return false;
// 		return true;
// 	}
// 	if (sent == 0)
// 		return true;
// 	cli.addBytesSent(sent);
// 	if ((sent >= cli.getResponseBuffer().size()))
// 	{
// 		cli.setResponseBuffer("");
// 		cli.setBytesSent(0);
// 	}
// 	return false;
// }

void webserv::setEpoll(int epollFd, int clientFd,int flag)
{
	struct epoll_event ev;
	if(flag == 0)
		ev.events = EPOLLIN;
	else if(flag == 1)
		ev.events = EPOLLOUT;
	ev.data.fd = clientFd;
	epoll_ctl(epollFd, EPOLL_CTL_MOD, clientFd, &ev);
}
void	webserv::state_machine(client &cli,server &serv, int fd, uint32_t events)
{
	if(cli.getState()== READING && (events & EPOLLIN))
	{
		if(handleRead(cli,fd) == 1)
		{
			cli.setState(DONE);
			return;
		}
	}
	if(cli.getState() == ROUTING)
	{
		if(handleRouting(cli,serv) == 1)
		{
			setEpoll(epoll_fd,cli.getFd(),1);
			return;
		}
	}
	if(cli.getState() == UPLOADING || cli.getState() == OVERWRITE)
	{
		handleUpload(cli,serv,cli.getState());
	}
	if(events & EPOLLOUT)
	{
		if(cli.getState() == READINGFILE)
		{
			handleFileReading(cli,serv);
		}
		if(cli.getState()==SENDING_RESPONSE || cli.getState()==ERROR)
		{
			if(handleWrite(cli,serv))
			{
				cli.setState(DONE);
			}
		}
	}
	// if(state == SENDING_RESPONSE && (events & EPOLLOUT))
	// {
	// 	if(handleWrite(cli,fd) == 1)
	// 	{
	// 		cli.setState(DONE);
	// 		return;
	// 	}
	// }
}
