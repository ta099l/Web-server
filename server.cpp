/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 17:18:26 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/12 17:25:50 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

server::server() : root(""), index(""), max_body_size(0)
{
}
server::server(const server &obj)
    : listens(obj.listens),
      root(obj.root),
      index(obj.index),
      max_body_size(obj.max_body_size),
      error_pages(obj.error_pages),
      locations(obj.locations)
{
}

server::~server()
{

}
