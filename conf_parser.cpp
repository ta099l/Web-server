/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_parser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 16:34:42 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/12 17:15:28 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	conf_pars(char *file)
{
	std::ifstream inFile(file);
	std::string line;
	if(inFile.is_open())
	{
		while(std::getline(inFile, line))
		{
			if (line == "server")
			{
				
			}
		}
	}
	else
	{
		std::cerr<< "Unable to open file" <<"\n";
		exit(1);
	}
	inFile.close();
}
