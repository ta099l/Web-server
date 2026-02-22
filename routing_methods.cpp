/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routing_methods.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 13:59:45 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/22 16:14:06 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	handle_get(int fd, client &cli)
{
	
}
void	handle_post(int fd, client &cli)
{

}
void handle_client_write(int fd,client &cli)
{
	cli.getFileSize();
	if(cli.getReq().getMethod() == "GET")
	{
		handle_get(fd, cli);
	}
	else if (cli.getReq().getMethod() == "POST")
	{
		handle_post(fd, cli);
	}
}
