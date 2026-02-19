/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: balhamad <balhamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 15:23:44 by tabuayya          #+#    #+#             */
/*   Updated: 2026/02/18 19:52:00 by balhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

std::string	n_trim(std::string &str)
{
	size_t first = str.find_first_not_of(" \a\b\v\f\r\t\n");
	if(std::string::npos != first)
		str.erase(0, first);
	else
		str.clear();
	size_t last = str.find_last_not_of(" \a\b\v\f\r\t\n");
	if(std::string::npos != last)
		str.erase(last + 1);
	else
		str.clear();
	return(str);
}
std::string	s_trim(std::string str)
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

int	check_line(std::string line)
{
	int len = line.length();

	if (line.empty())
		return (1);

	if (line == "{" || line == "}")
		return (1);

	if (!line.compare(0, 6, "server"))
		return (1);

	if (!line.compare(0, 8, "location"))
		return (1);

	if (line[len - 1] != ';')
	{
		std::cerr<<"CHECK THE CONFIG FILE U IDIOT THERE IS NO SEMICOLON WITH: "<<line<<"\n";
		return (0);
	}
	return (1);
}


