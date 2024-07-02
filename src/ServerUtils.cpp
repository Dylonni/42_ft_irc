/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daumis <daumis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 15:26:29 by psan              #+#    #+#             */
/*   Updated: 2024/05/27 17:41:53 by daumis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

int ft_stoi(const std::string& str)
{
	int num;
	std::stringstream ss(str);

	ss >> num;
	return num;
}

// Split line terminated with \n into vector of strings
std::vector<std::string> splitLine(const std::string &line, char delimiter)
{
    size_t index = 0;
    std::vector<std::string> tokens;
    
    while (index < line.size())
    {
        // Skip successive delimiters
        while (index < line.size() && line[index] == delimiter)
            ++index;
        if (index >= line.size())
            break;
        
        size_t next = line.find(delimiter, index);
        if (next == std::string::npos)
            next = line.size();
        
        std::string token = line.substr(index, line.size() - index);
        if (token[0] == ':')
            token = token.substr(1);
		else
			token = line.substr(index, next - index);

        tokens.push_back(token);
        index = next + 1;
    }
    return tokens;
}

bool	Server::checkArgs(const std::string &port, const std::string &password)
{
	int port_stoi;
	
	if (port.find_first_not_of("0123456789") != std::string::npos)
		return false;
	if (port.empty() || password.empty())
		return false;
	port_stoi = ft_stoi(port);
	if (port_stoi > 65535 || port_stoi < 0)
		return false;
	_port = port_stoi;
	_password = password;
	return true;
}

void	Server::setServerName()
{
	char hostname[1024];
	hostname[1023] = '\0';
	if (gethostname(hostname, 1023) < 0)
		throw std::runtime_error("Failed to get hostname");
	_name = hostname;

	struct hostent* host = gethostbyname(hostname);
	if (host == NULL)
		throw std::runtime_error("Failed to get host by name");

	struct in_addr** addr_list = (struct in_addr**)host->h_addr_list;
	_address = inet_ntoa(*addr_list[0]);
}

void	Server::broadcast(std::string message)
{
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
		reply(it->first, message);
}

void	reply(int fd, const std::string &message)
{
	int	n;
	int	bytesLeft = message.size();
	size_t	total = 0;

	std::cout << message << std::endl;
	while (total < message.size())
	{
		n = send(fd, message.substr(total, bytesLeft).c_str(), bytesLeft, 0);
		if (n < 0)
		{
			std::cout << RED "Error sending message: " + message + RESET<< std::endl;
			return ;
		}
		total += n;
		bytesLeft -= n;
	}
}