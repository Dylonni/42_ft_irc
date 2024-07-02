/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psan <psan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:57:36 by dylan             #+#    #+#             */
/*   Updated: 2024/06/03 15:40:34 by psan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/CommandManager.hpp"

void	CommandManager::invite(Server *server, int fd, const str_vector &tokens)
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

	// Check whether the user exists.
	target = server->getClient(tokens[1]);
	if (target == NULL)
	{
		data["<client>"] = client->getNickname();
		data["<nickname>"] = tokens[1];
		message = _messageManager->getMessage(ERR_NOSUCHNICK, data);
		reply(fd, message);
		return ;
	}

	// Check whether the channel exists.
	channel = server->getChannel(tokens[2]);
	if (channel == NULL)
	{
		data["<client>"] = client->getNickname();
		data["<channel>"] = tokens[2];
		message = _messageManager->getMessage(ERR_NOSUCHCHANNEL, data);
		reply(fd, message);
		return ;
	}

	// Check whether the source is a member of the channel.
	if (!channel->hasClient(client->getNickname()))
	{
		data["<client>"] = client->getNickname();
		data["<channel>"] = tokens[2];
		message = _messageManager->getMessage(ERR_NOTONCHANNEL, data);
		reply(fd, message);
		return ;
	}

	// Check whether the target is a member of the channel.
	if (channel->hasClient(tokens[1]))
	{
		data["<client>"] = client->getNickname();
		data["<nick>"] = tokens[1];
		data["<channel>"] = tokens[2];
		message = _messageManager->getMessage(ERR_USERONCHANNEL, data);
		reply(fd, message);
		return ;
	}

	// Check whether the source is an operator of the channel.
	if (!channel->isOperator(client->getNickname()))
	{
		data["<client>"] = client->getNickname();
		data["<channel>"] = tokens[2];
		message += _messageManager->getMessage(ERR_CHANOPRIVSNEEDED, data);
		reply(fd, message);
		return ;
	}

	// Invite the target.
	channel->inviteClient(target);
	message = ":" + client->getFullname() + " INVITE " + tokens[1] + " " + tokens[2] + "\r\n";
	reply(target->getFd(), message);

	data["<client>"] = client->getNickname();
	data["<nick>"] = tokens[1];
	data["<channel>"] = tokens[2];
	message = _messageManager->getMessage(RPL_INVITING, data);
	reply(fd, message);
}
