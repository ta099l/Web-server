/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 17:32:41 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/12 18:13:22 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

webserv::webserv()
{
}
webserv::~webserv()
{
}
const std::vector<server>& webserv::getServers() const
{
	return(this->servers);
}
void webserv::setServers(const server& s)
{
	servers.push_back(s);
}
