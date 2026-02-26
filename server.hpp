/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 19:32:26 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/23 14:50:54 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
#define SERVER_HPP
#include "webserv.hpp"
#include "client.hpp"
struct CGIConfig //done
{
	std::string extension;
	std::string path;
};

struct LocationConfig //was struct
{
	private:
		std::string path; //done
		std::vector<std::string> methods; //done
		bool autoindex; //done
		bool upload_enable; //done
		std::string upload_store; //done
		std::string root; //done
		std::string index; //done
		std::string redirect; //done
		long long max_body_size; //done
		std::map<std::string, CGIConfig> cgi; //done
		std::map<int, std::string> error_pages; //done

	public:
		LocationConfig();
		~LocationConfig();
		// Getters
		const std::string& getPath() const;
		const std::vector<std::string>& getMethods() const;
		bool getAutoindex() const;
		bool getUploadEnable() const;
		const std::string& getUploadStore() const;
		const std::string& getRoot() const;
		const std::string& getIndex() const;
		const std::string& getRedirect() const;
		long long getMaxBodySize() const;
		const std::map<std::string, CGIConfig>& getCgi() const;
		const std::map<int, std::string>& getErrorPages() const;
		// Setters
		void setPath(const std::string& p);
		void addMethod(const std::string& method);
		void setAutoindex(bool value);
		void setUploadEnable(bool value);
		void setUploadStore(const std::string& path);
		void setRoot(const std::string& r);
		void setIndex(const std::string& i);
		void setRedirect(const std::string& r);
		void setMaxBodySize(long long size);
		void addCgi(const CGIConfig& c);
		void addErrorPage(int code, const std::string& path);

		void clearMethods();
};

struct ListenConfig //done
{
	private:
		std::string host;
		int port;

	public:
		ListenConfig();
		ListenConfig(const std::string& h, int p);
		~ListenConfig();
		// Getters
		const std::string& getHost() const;
		int getPort() const;
		// Setters
		void setHost(const std::string& h);
		void setPort(int p);
};

class server
{
private:
	std::vector<ListenConfig> listens;
	std::string root;
	std::string index;
	long long max_body_size;
	std::map<int, std::string> error_pages;
	std::map<std::string, LocationConfig> locations;
	std::vector<std::string> methods;
	bool autoindex;
	bool upload_enable;
	std::string upload_store;
	std::string redirect;
	std::map<std::string, CGIConfig> cgi;
	int server_fd;
	std::map<int, client> client_fds;
	bool isCgi; //implement
public:
	server();
	server(const server &obj);
	server& operator=(const server &obj);
	~server();
	// getters
	const std::vector<ListenConfig>& getListens() const;
	const std::string& getRoot() const;
	const std::string& getIndex() const;
	long long getMaxBodySize() const;
	const std::map<int, std::string>& getErrorPages() const;
	const std::map<std::string, LocationConfig>& getLocations() const;
	const std::vector<std::string>& getMethods() const;
	bool getAutoindex() const;
	bool getUploadEnable() const;
	const std::string& getUploadStore() const;
	const std::string& getRedirect() const;
	const std::map<std::string, CGIConfig>& getCgi() const;
	int getServerFd() const;
	std::map<int, client>& getClientFds();
	bool getIsCgi();
	// setters
	void addListen(const ListenConfig& l);
	void setRoot(const std::string& r);
	void setIndex(const std::string& i);
	void setMaxBodySize(long long size);
	void setAutoindex(bool value);
	void setUploadEnable(bool value);
	void setUploadStore(const std::string& path);
	void setRedirect(const std::string& r);
	void addMethod(const std::string& method);
	void addErrorPage(int code, const std::string& path);
	void addLocation(const LocationConfig& loc);
	void addCgi(const CGIConfig& cgi);
	void setServerFd(int fd);
	void addClientFd(int fd, const client &c) ;
	void setIsCgi(bool val);
};

#endif
