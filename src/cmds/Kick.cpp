/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psan <psan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:57:46 by dylan             #+#    #+#             */
/*   Updated: 2024/06/03 15:40:34 by psan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/CommandManager.hpp"

void	CommandManager::kick(Server *server, int fd, const str_vector &tokens)
{
	str_map		data;
	std::string	message;
	Client		*client = server->getClient(fd);
	Client		*target;
	Channel		*channel;

	// Check whether enough parameters are provided.
	if (tokens.size() < 3)
	{
		data["<command>"] = tokens[0];
		message = _messageManager->getMessage(ERR_NEEDMOREPARAMS, data);
		reply(fd, message);
		return ;
	}

	// Check whether the channel exists.
	channel = server->getChannel(tokens[1]);
	if (channel == NULL)
	{
		data["<client>"] = client->getNickname();
		data["<channel>"] = tokens[1];
		message = _messageManager->getMessage(ERR_NOSUCHCHANNEL, data);
		reply(fd, message);
		return ;
	}

	std::vector<std::string> clients = splitLine(tokens[2], ',');
	for (std::vector<std::string>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		// Check whether the target exists.
		target = server->getClient(*it);
		if (target == NULL)
		{
			data.clear();
			data["<client>"] = client->getNickname();
			data["<nickname>"] = *it;
			message = _messageManager->getMessage(ERR_NOSUCHNICK, data);
			reply(fd, message);
			continue ;
		}

		// Check whether the source is a member of the channel.
		if (!channel->hasClient(client->getNickname()))
		{
			data.clear();
			data["<client>"] = client->getNickname();
			data["<channel>"] = tokens[1];
			message = _messageManager->getMessage(ERR_NOTONCHANNEL, data);
			reply(fd, message);
			continue ;
		}

		// Check whether the target is a member of the channel.
		if (!channel->hasClient(*it))
		{
			data.clear();
			data["<client>"] = client->getNickname();
			data["<nick>"] = *it;
			data["<channel>"] = tokens[1];
			message = _messageManager->getMessage(ERR_USERNOTINCHANNEL, data);
			reply(fd, message);
			continue ;
		}

		// Check whether the source has enough privileges.
		if (!channel->isOperator(client->getNickname()))
		{
			data.clear();
			data["<client>"] = client->getNickname();
			data["<channel>"] = tokens[1];
			message = _messageManager->getMessage(ERR_CHANOPRIVSNEEDED, data);
			reply(fd, message);
			continue ;
		}

		// Kick the target.
		channel->removeClient(target);
		message = ":" + target->getFullname() + " PART " + channel->getName() + "\r\n";
		reply(target->getFd(), message);
		message = ":" + client->getFullname() + " KICK " + tokens[1] + " " + *it + (tokens.size() < 4 ? "" : " :" + tokens[3]) + "\r\n";
		channel->broadcast(message);
		reply(target->getFd(), message);

		// Check whether to delete the channel.
		if (channel->getClientCount() < 1)
		{
			server->removeChannel(channel->getName());
			std::cout << "Channel " + channel->getName() + " deleted" << std::endl;
		}
	}
}
