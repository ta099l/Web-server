/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 13:36:19 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/14 15:57:02 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"


int	main(int argc, char **argv)
{
	if(argc != 2)
	{
		std::cerr<< "PUT THE CONFIG FILE U IDIOT" <<"\n";
		exit(1);
	}
	webserv spiderweb;
	spiderweb.conf_pars(argv[1]);
	spiderweb.printConfig();
	struct socketadd addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);//hardcoded till we get the config
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int	fd = socket(AF_INET, SOCK_STREAM, 0);

	int opt = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	bind(fd, (struct sockaddr*)&addr,sizeof(addr));
	listen(fd,SOMAXCONN);
}
