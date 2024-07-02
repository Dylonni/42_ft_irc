/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psan <psan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:58:15 by dylan             #+#    #+#             */
/*   Updated: 2024/06/03 15:40:34 by psan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/CommandManager.hpp"

void	CommandManager::topic(Server *server, int fd, const str_vector &tokens)
{
	str_map		data;
	std::string	message;
	Client		*client = server->getClient(fd);
	Channel		*channel;

	// Check whether enough parameters are provided.
	if (tokens.size() < 2)
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

	// Check whether the user is a member of the channel.
	if (!channel->hasClient(client->getNickname()))
	{
		data["<client>"] = client->getNickname();
		data["<channel>"] = tokens[1];
		message = _messageManager->getMessage(ERR_NOTONCHANNEL, data);
		reply(fd, message);
		return ;
	}

	// Check whether the user has enough privileges.
	if (!channel->isOperator(client->getNickname())
			|| !channel->canModifyTopic())
	{
		data["<client>"] = client->getNickname();
		data["<channel>"] = tokens[1];
		message = _messageManager->getMessage(ERR_CHANOPRIVSNEEDED, data);
		reply(fd, message);
		return ;
	}

	// Send new channel topic.
	message = ":" + client->getFullname() + " TOPIC " + tokens[1] + " :" + (tokens.size() == 2 ? "" : tokens[2]) + "\r\n";
	channel->setTopic(tokens.size() == 2 ? "" : tokens[2], client);
	channel->broadcast(message);
}