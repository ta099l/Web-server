/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 19:32:26 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/11 20:37:00 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include "conf.hpp"
class	server
{
	std::list<std::string> listen;
	std::string server_name;
	long long max_body_size;
	std::list<std::string> methods;
	std::map<std::string ,std::string> errors;
	std::map<std::string , std::vector<std::string>> server_locations;
};
#endif
