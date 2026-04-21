#include "webserv.hpp"
#include "client.hpp"
std::string buildErrorPath(const server &serv,const LocationConfig& loc, std::string path)
{
	(void)loc;
	std::string root = serv.getRoot();
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
		case 204: return "No Content";
		case 201: return "CREATED";
		case 400: return "Bad Request";
		case 403: return "Forbidden";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";
		case 413: return "Payload Too Large";
		case 500: return "Internal Server Error";
		case 501: return "Not Implemented";
		case 504: return "Gateway Timeout";
		default: return "Unknown Error";
	}
}
#include <string>
#include <sstream>

#include <string>
#include <sstream>

std::string genHtml(int code, std::string reason) {
	std::stringstream ss;

	ss << "<!DOCTYPE html>\n"
	<< "<html lang=\"en\">\n"
	<< "<head>\n"
	<< "    <meta charset=\"UTF-8\">\n"
	<< "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
	<< "    <title>ETERNAL DAMNATION: " << code << "</title>\n"
	<< "    <style>\n"
	<< "        :root {\n"
	<< "            --burgundy: #2e0202;\n"
	<< "            --blood: #8a0303;\n"
	<< "            --fire-gold: #ff9d00;\n"
	<< "            --fire-red: #ff2a00;\n"
	<< "        }\n"
	<< "        body, html { margin: 0; padding: 0; width: 100%; height: 100%; overflow: hidden;\n"
	<< "            background: radial-gradient(circle, var(--burgundy) 0%, #000000 100%);\n"
	<< "            font-family: 'Courier New', Courier, monospace; display: flex;\n"
	<< "            justify-content: center; align-items: center; }\n"
	<< "        body::after { content: \"\"; position: absolute; top: 0; left: 0; width: 100%; height: 100%;\n"
	<< "            background: radial-gradient(transparent 40%, black 150%); pointer-events: none; z-index: 20; }\n"
	<< "        .fire-wrapper { position: absolute; bottom: -20px; left: 0; width: 100%; height: 250px;\n"
	<< "            z-index: 15; pointer-events: none; filter: url(#fire-filter); }\n"
	<< "        .flame { position: absolute; bottom: 0; width: 10%; height: 100%;\n"
	<< "            background: linear-gradient(to top, var(--fire-gold), var(--fire-red), transparent);\n"
	<< "            border-radius: 50% 50% 20% 20%; transform-origin: bottom; opacity: 0.8;\n"
	<< "            animation: flicker-flame 1.5s infinite ease-in-out alternate; }\n"
	<< "        @keyframes flicker-flame {\n"
	<< "            0% { transform: scaleX(1) scaleY(0.8) skewX(-2deg); }\n"
	<< "            50% { transform: scaleX(1.2) scaleY(1.1) skewX(2deg); }\n"
	<< "            100% { transform: scaleX(0.9) scaleY(1.3) skewX(-1deg); } }\n"
	<< "        #canvas { position: absolute; top: 0; left: 0; z-index: 2; }\n"
	<< "        .container { position: relative; z-index: 25; text-align: center;\n"
	<< "            filter: drop-shadow(0 0 10px rgba(255, 0, 0, 0.5)); }\n"
	<< "        h1 { font-size: 8rem; margin: 0; color: var(--blood); text-transform: uppercase; letter-spacing: 20px; }\n"
	<< "        p { font-size: 1.2rem; color: #636363; letter-spacing: 3px; text-transform: uppercase; margin-top: 10px; }\n"
	<< "    </style>\n"
	<< "</head>\n"
	<< "<body>\n"
	<< "    <svg width=\"0\" height=\"0\">\n"
	<< "        <filter id=\"fire-filter\">\n"
	<< "            <feTurbulence type=\"turbulence\" baseFrequency=\"0.05 0.02\" numOctaves=\"3\" seed=\"2\" result=\"turbulence\">\n"
	<< "                <animate attributeName=\"baseFrequency\" dur=\"3s\" values=\"0.05 0.02;0.05 0.04;0.05 0.02\" repeatCount=\"indefinite\" />\n"
	<< "            </feTurbulence>\n"
	<< "            <feDisplacementMap in=\"SourceGraphic\" in2=\"turbulence\" scale=\"35\" />\n"
	<< "            <feGaussianBlur stdDeviation=\"3\" />\n"
	<< "        </filter>\n"
	<< "    </svg>\n"
	<< "    <div class=\"fire-wrapper\" id=\"fire-container\"></div>\n"
	<< "    <canvas id=\"canvas\"></canvas>\n"
	<< "    <div class=\"container\">\n"
	<< "        <h1>" << code << "</h1>\n"
	<< "        <p>" << reason << "</p>\n"
	<< "    </div>\n"
	<< "    <script>\n"
	<< "        const fireContainer = document.getElementById('fire-container');\n"
	<< "        for (let i = 0; i < 15; i++) {\n"
	<< "            const flame = document.createElement('div');\n"
	<< "            flame.className = 'flame';\n"
	<< "            flame.style.left = (i * 7) + '%';\n"
	<< "            flame.style.animationDelay = (Math.random() * 2) + 's';\n"
	<< "            flame.style.width = (Math.random() * 5 + 8) + '%';\n"
	<< "            fireContainer.appendChild(flame);\n"
	<< "        }\n"
	<< "        const canvas = document.getElementById('canvas');\n"
	<< "        const ctx = canvas.getContext('2d');\n"
	<< "        canvas.width = window.innerWidth; canvas.height = window.innerHeight;\n"
	<< "        let particles = [];\n"
	<< "        class Drop {\n"
	<< "            constructor(x, y) {\n"
	<< "                this.x = x; this.y = y; this.size = Math.random() * 10 + 4;\n"
	<< "                this.speedY = Math.random() * 2 + 1; this.opacity = 1;\n"
	<< "            }\n"
	<< "            update() { this.y += this.speedY; this.opacity -= 0.004; }\n"
	<< "            draw() {\n"
	<< "                ctx.fillStyle = `rgba(138, 3, 3, ${this.opacity})`;\n"
	<< "                ctx.beginPath(); ctx.arc(this.x, this.y, this.size, 0, Math.PI * 2); ctx.fill();\n"
	<< "            }\n"
	<< "        }\n"
	<< "        function animate() {\n"
	<< "            ctx.fillStyle = 'rgba(0, 0, 0, 0.08)';\n"
	<< "            ctx.fillRect(0, 0, canvas.width, canvas.height);\n"
	<< "            for (let i = 0; i < particles.length; i++) {\n"
	<< "                particles[i].update();\n"
	<< "                particles[i].draw();\n"
	<< "                if (particles[i].opacity <= 0) { particles.splice(i, 1); i--; }\n"
	<< "            }\n"
	<< "            requestAnimationFrame(animate);\n"
	<< "        }\n"
	<< "        setInterval(() => { particles.push(new Drop(Math.random() * canvas.width, -10)); }, 250);\n"
	<< "        window.addEventListener('resize', () => {\n"
	<< "            canvas.width = window.innerWidth; canvas.height = window.innerHeight;\n"
	<< "        });\n"
	<< "        animate();\n"
	<< "    </script>\n"
	<< "</body>\n"
	<< "</html>";

	return ss.str();
}
bool generateErrorResponse(client &cli,server &serv)
{
	(void)serv;
	const LocationConfig* loc = cli.getLocation();
	const std::map<int, std::string>* errorPages;
	bool genError = false;
	const LocationConfig* base = loc ? loc : NULL;
	int code = cli.getRes().getStatusCode();
	std::string reason = getReasonPhrase(code);
	if (!loc)
	{
		std::cerr<<"NO LOCATION YOU DUMP\n";
		errorPages = &serv.getErrorPages();

	}
	else
	{
		errorPages = &loc->getErrorPages();
	}
	std::map<int, std::string>::const_iterator it = errorPages->begin();
	for (;it != errorPages->end();it++)
	{
		if(code == it->first)
		{
			std::string path = buildErrorPath(serv,*base,it->second);
			std::cerr<<"PATHHTHHTHHT "<<path<<std::endl;
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
			cli.getRes().setContentLength(pathStat.st_size);
			std::cerr<<"IIIIIIIIIIIIIIIIIIIIIIIIIII BELIEVE I CAN FLYYYYYYYYYYYYYYYYYY AND KILL THEMM ALLLLLLLLLLL\n";
			cli.getRes().setHasFileBody(true);
			if(!cli.getReq().getVersion().empty())
				cli.getRes().setVersion(cli.getReq().getVersion());
			else
				cli.getRes().setVersion("HTTP/1.1");
			cli.getRes().setReason(getReasonPhrase(cli.getRes().getStatusCode()));
			cli.getRes().setContentType(path);
			std::cerr<<"content type "<<cli.getRes().getContentType()<<std::endl;
			cli.setState(READINGFILE);
			return false;
		}
	}
	if(it==errorPages->end())
	{
		std::cerr<<"I BELIEVE I CAN FLYYYYYYYYYYYYYYYYYY AND KILL THEMM ALLLLLLLLLLL\n";
		cli.getRes().setHasMemoryBody(true);
		genError = true;
	}
	if(genError)
	{
		std::string html =genHtml(code,reason);
		cli.getRes().setVersion(cli.getReq().getVersion());
		cli.getRes().setReason(getReasonPhrase(cli.getRes().getStatusCode()));
		cli.getRes().setContentTypeString("text/html");
		cli.getRes().setContentLength(html.size());
		cli.getRes().setFileBody(html);
	}
	return true;
}
void generateResponseHeader(client &cli,server &srv)
{
	(void)srv;
	std::stringstream resHeader;
	// if (!cli.getReq().getVersion().empty())
    // 	cli.getRes().setVersion(cli.getReq().getVersion());
	// else
    // 	cli.getRes().setVersion("HTTP/1.1");
	resHeader<< cli.getRes().getVersion()<<" "<<cli.getRes().getStatusCode()<<" "<<getReasonPhrase(cli.getRes().getStatusCode())<<"\r\n";
	if(cli.getRes().getHasFileBody()||cli.getRes().getHasMemoryBody())
		resHeader<<"Content-Type: "<<cli.getRes().getContentType()<<"\r\n";
	resHeader<<"Content-Length: "<<cli.getRes().getContentLength()<<"\r\n"
			<< "Connection: close\r\n"
			<<"\r\n";
	std::string resHeaderStr = resHeader.str();
	cli.getRes().setGeneratedResponseHeader(true);
	cli.appendToResBuffer(resHeaderStr,resHeaderStr.size());
}
