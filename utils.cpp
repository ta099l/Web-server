/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabuayya <tabuayya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 15:23:44 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/13 21:27:42 by tabuayya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

std::string	trim(std::string &str)
{
	size_t first = str.find_first_not_of(" \a\b\v\f\r\t\n");
	if(std::string::npos != first)
		str.erase(0, first);
	else
		str.clear();
	size_t last = str.find_last_not_of(" ;\a\b\v\f\r\t\n");
	if(std::string::npos != last)
		str.erase(last + 1);
	else
		str.clear();
	return(str);
}
std::vector<std::string> split(const std::string& line)
{
	std::vector<std::string> tokens;
	std::string current;

	for (size_t i = 0; i < line.size(); ++i)
	{
		if (line[i] == ' ' || line[i] == '\t')
		{
			if (!current.empty())
			{
				tokens.push_back(current);
				current.clear();
			}
		}
		else
			current += line[i];
	}

	if (!current.empty())
		tokens.push_back(current);

	return tokens;
}

