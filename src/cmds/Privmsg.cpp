/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psan <psan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:58:09 by dylan             #+#    #+#             */
/*   Updated: 2024/06/03 15:40:34 by psan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/CommandManager.hpp"

void	CommandManager::privmsg(Server *server, int fd, const str_vector &tokens)
{
	str_map		data;
	std::string	message;
	Client		*client = server->getClient(fd);
	Client		*target;
	Channel		*channel;

	if (tokens.size() < 2)
	{
		data["<client>"] = client->getNickname();
		data["<command>"] = tokens[0];
		message = _messageManager->getMessage(ERR_NORECIPIENT, data);
		reply(fd, message);
		return ;
	}

	if (tokens.size() < 3)
	{
		data["<client>"] = client->getNickname();
		message = _messageManager->getMessage(ERR_NOTEXTTOSEND, data);
		reply(fd, message);
		return ;
	}

	std::vector<std::string> targetNames = splitLine(tokens[1], ',');
	for (std::vector<std::string>::iterator it = targetNames.begin(); it != targetNames.end(); ++it)
	{
		// Check whether the target exists.
		target = server->getClient(*it);
		channel = server->getChannel(*it);
		if (target == NULL && channel == NULL)
		{
			data["<client>"] = client->getNickname();
			data["<nickname>"] = *it;
			message = _messageManager->getMessage(ERR_NOSUCHNICK, data);
			reply(fd, message);
			continue ;
		}

		if (channel)
		{
			message = ":" + client->getFullname() + " PRIVMSG " + *it + " :" + tokens[2] + "\r\n";
			channel->broadcastExceptFd(message, fd);
			continue ;
		}

		message = ":" + client->getFullname() + " PRIVMSG " + *it + " :" + tokens[2] + "\r\n";
		reply(target->getFd(), message);
	}
}
