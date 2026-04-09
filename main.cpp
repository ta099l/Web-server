/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tasnimsamer <tasnimsamer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 13:36:19 by tabuayya          #+#    #+#             */
/*   Updated: 2026/04/09 14:52:37 by tasnimsamer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "socket.hpp"


int	main(int argc, char **argv)
{
	int flag = 1;
	if(argc != 2)
	{
		std::cerr<< "PUT THE CONFIG FILE U IDIOT" <<"\n";
		exit(1);
	}
	webserv spiderweb;
	flag = spiderweb.conf_pars(argv[1], flag);
	if(flag == 0)
		return (0);
	// spiderweb.printConfig();
	if (init_sockets(spiderweb) == -1)
	{
		std::cerr << "Failed to initialize sockets\n";
		return 1;
	}
	if(spiderweb.initialize_epoll() == -1)
	{
		std::cerr << "Failed to initialize epoll instance\n";
		return(1);
	}
	spiderweb.run();

	return 0;
}
