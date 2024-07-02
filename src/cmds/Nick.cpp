/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psan <psan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:57:57 by dylan             #+#    #+#             */
/*   Updated: 2024/06/03 17:22:43 by psan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/CommandManager.hpp"

void	CommandManager::nick(Server *server, int fd, const str_vector &tokens)
{
	str_map		data;
	std::string	message;
	Client		*client = server->getClient(fd);

	// Check whether enough parameters are provided.
	if (tokens.size() < 2)
	{
		message = _messageManager->getMessage(ERR_NONICKNAMEGIVEN);
		reply(fd, message);
		return ;
	}

	if (!client->isRegistered())
	{
		if (server->getClient(tokens[1]) != NULL)
		{
			data["<nick>"] = tokens[1];
			message = _messageManager->getMessage(ERR_NICKNAMEINUSE, data);
			std::cout << RED + message + RESET;
			server->removeClient(fd);
			throw std::invalid_argument(message);
		}

		if (!isValidNickname(tokens[1]))
		{
			data["<nick>"] = tokens[1];
			message = _messageManager->getMessage(ERR_ERRONEUSNICKNAME, data);
			std::cout << RED + message + RESET;
			server->removeClient(fd);
			throw std::invalid_argument(message);
		}
	}

	// Check whether the nickname is available.
	if (server->getClient(tokens[1]) != NULL)
	{
		data["<nick>"] = tokens[1];
		message = _messageManager->getMessage(ERR_NICKNAMEINUSE, data);
		reply(fd, message);
		return ;
	}

	// Check whether the nickname doesn't contain any forbidden characters.
	if (!isValidNickname(tokens[1]))
	{
		data["<nick>"] = tokens[1];
		message = _messageManager->getMessage(ERR_ERRONEUSNICKNAME, data);
		reply(fd, message);
		return ;
	}

	// Check whether the user is replacing nickname.
	if (!client->getNickname().empty())
	{
		message = ":" + client->getFullname() + " NICK " + tokens[1] + "\r\n";
		client->setNickname(tokens[1]);
		server->broadcast(message);
		return ;
	}

	// Set the new nickname.
	client->setNickname(tokens[1]);
	message = "NICK :Nickname set\r\n";
	std::cout << message << std::endl;

	if (!client->isRegistered() && client->getUsername().length())
		sendWelcomeTrain(server, fd);
}

bool	CommandManager::isValidNickname(const std::string &nickname)
{
	return (std::find_if(nickname.begin(), nickname.end(), isNotAlnum) == nickname.end());
}
