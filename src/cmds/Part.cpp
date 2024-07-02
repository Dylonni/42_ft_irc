/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psan <psan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:58:02 by dylan             #+#    #+#             */
/*   Updated: 2024/06/03 15:40:34 by psan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/CommandManager.hpp"

void	CommandManager::part(Server *server, int fd, const str_vector &tokens)
{
	str_map		data;
	std::string	message;
	Client		*client = server->getClient(fd);
	Channel		*channel;

	// Check whether enough parameters are provided
	if (tokens.size() < 2)
	{
		data["<command>"] = tokens[0];
		message = _messageManager->getMessage(ERR_NEEDMOREPARAMS, data);
		reply(fd, message);
		return ;
	}

	std::vector<std::string> channels = splitLine(tokens[1], ',');
	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		// Check whether the channel exists.
		channel = server->getChannel(*it);
		if (channel == NULL)
		{
			data.clear();
			data["<client>"] = client->getNickname();
			data["<channel>"] = *it;
			message = _messageManager->getMessage(ERR_NOSUCHCHANNEL, data);
			reply(fd, message);
			continue ;
		}
		
		// Check whether the user is a member of the channel.
		if (!channel->hasClient(client->getNickname()))
		{
			data.clear();
			data["<client>"] = client->getNickname();
			data["<channel>"] = *it;
			message = _messageManager->getMessage(ERR_NOTONCHANNEL, data);
			reply(fd, message);
			continue ;
		}

		// Remove the user from the channel.
		channel->removeClient(client);
		message = ":" + client->getFullname() + " PART " + channel->getName() + (tokens.size() < 3 ? "" : " :" + tokens[2]) + "\r\n";
		channel->broadcast(message);
		reply(fd, message);

		// Check whether to delete the channel.
		if (channel->getClientCount() < 1)
		{
			server->removeChannel(channel->getName());
			std::cout << "Channel " + channel->getName() + " deleted" << std::endl;
		}
	}
}
