/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psan <psan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:58:58 by dylan             #+#    #+#             */
/*   Updated: 2024/06/03 18:50:01 by psan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/CommandManager.hpp"

void	CommandManager::user(Server *server, int fd, const str_vector &tokens)
{
	str_map		data;
	std::string	message;
	Client		*client = server->getClient(fd);
	
	// Check whether enough parameters are provided.
	if (tokens.size() < 5)
	{
		data["<command>"] = tokens[0];
		message = _messageManager->getMessage(ERR_NEEDMOREPARAMS, data);
		reply(fd, message);
		return ;
	}
	
	// Check whether the client is already registered.
	if (client->isRegistered())
	{
		data["<client>"] = client->getNickname();
		message = _messageManager->getMessage(ERR_ALREADYREGISTRED, data);
		reply(fd, message);
		return ;
	}

	if (!isValidUsername(tokens[1]))
	{
		message = "USER :Invalid username\r\n";
		std::cout << RED + message + RESET;
		server->removeClient(fd);
		throw std::invalid_argument(message);
	}

	client->setUsername(tokens[1]);
	client->setHostname(tokens[2]);
	client->setServname(tokens[3]);
	client->setRealname(tokens[4]);
	message = "USER :Username set\r\n";
	std::cout << message << std::endl;

	if (client->getNickname().length())
		sendWelcomeTrain(server, fd);
}

bool	CommandManager::isValidUsername(const std::string &username)
{
	return (std::find_if(username.begin(), username.end(), isNotAlnum) == username.end());
}