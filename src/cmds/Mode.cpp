/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psan <psan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:57:50 by dylan             #+#    #+#             */
/*   Updated: 2024/06/03 15:40:34 by psan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/CommandManager.hpp"

void	CommandManager::mode(Server *server, int fd, const str_vector &tokens)
{
	str_vector	modes;
	str_map		data;
	std::string	message;
	Client		*client = server->getClient(fd);
	Client		*target;
	Channel		*channel;

	// Check whether enough parameters are provided.
	if (tokens.size() < 2)
	{
		data["<command>"] = tokens[0];
		message = _messageManager->getMessage(ERR_NEEDMOREPARAMS, data);
		reply(fd, message);
		return ;
	}

	// Check whether such user exists.
	if (server->getClient(tokens[1]) != NULL)
	{
		if (client->getNickname().compare(tokens[1]) != 0)
		{
			data["<client>"] = client->getNickname();
			message = _messageManager->getMessage(ERR_USERSDONTMATCH, data);
			reply(fd, message);
			return ;
		}

		message = ":" + client->getFullname() + " MODE " + tokens[1] + " " + tokens[2] + "\r\n";
		reply(fd, message);
		return ;
	}

	// Check whether such channel exists.
	channel = server->getChannel(tokens[1]);
	if (channel == NULL)
	{
		data["<client>"] = client->getNickname();
		data["<channel>"] = tokens[1];
		message = _messageManager->getMessage(ERR_NOSUCHCHANNEL, data);
		reply(fd, message);
		return ;
	}

	// Check whether only the modes of the channel are requested.
	if (tokens.size() < 3)
	{
		data["<client>"] = client->getNickname();
		data["<channel>"] = channel->getName();
		data["<modes>"] = channel->getModes();
		data["<params>"] = channel->getMaxClients();
		message = _messageManager->getMessage(RPL_CHANNELMODEIS, data);
		reply(fd, message);
		return ;
	}

	// Check whether the list of bans is requested.
	if (tokens[2][0] == 'b')
	{
		data["<client>"] = client->getNickname();
		data["<channel>"] = channel->getName();
		message = _messageManager->getMessage(RPL_ENDOFBANLIST, data);
		reply(fd, message);
		return ;
	}

	// 
	if (!channel->isOperator(client->getNickname()))
	{
		data["<client>"] = client->getNickname();
		data["<channel>"] = channel->getName();
		message = _messageManager->getMessage(ERR_CHANOPRIVSNEEDED, data);
		reply(fd, message);
		return ;
	}

	bool		isPlus = false;
	size_t		count = 3;

	for (std::string::const_iterator it = tokens[2].begin(); it < tokens[2].end(); ++it)
	{
		if (*it == '+' || *it == '-')
			isPlus = *it == '+';
		else
		{
			switch (*it)
			{
			case (INVITE):
				channel->setInviteOnly(isPlus);
				message = ":" + client->getFullname() + " MODE " + tokens[1] + " " + (isPlus ? "+" : "-") + *it + "\r\n";
				channel->broadcast(message);
				break ;

			case (KEY):
				if (isPlus && count >= tokens.size())
					break ;
				channel->setKey(isPlus ? tokens[count] : "");
				message = ":" + client->getFullname() + " MODE " + tokens[1] + " " + (isPlus ? "+" : "-") + *it + "\r\n";
				channel->broadcast(message);
				count++;
				break ;

			case (LIM):
				if (isPlus && count >= tokens.size())
					break ;
				if (tokens[count].find_first_not_of("0123456789") != std::string::npos)
					break ;
				channel->setMaxClients(isPlus ? ft_stoi(tokens[count]) : 0);
				message = ":" + client->getFullname() + " MODE " + tokens[1] + " " + (isPlus ? "+" : "-") + *it + " " + channel->getMaxClients() + "\r\n";
				channel->broadcast(message);
				count++;
				break ;

			case (OPER):
				if (count >= tokens.size())
					break ;
				target = server->getClient(tokens[count]);
				if (target == NULL)
					break ;
				channel->setClientPrivileges(target, isPlus);
				message = ":" + client->getFullname() + " MODE " + tokens[1] + " " + (isPlus ? "+" : "-") + *it + " " + target->getNickname() + "\r\n";
				channel->broadcast(message);
				count++;
				break ;

			case (TOPIC):
				channel->setModifyTopic(isPlus);
				message = ":" + client->getFullname() + " MODE " + tokens[1] + " " + (isPlus ? "+" : "-") + *it + "\r\n";
				channel->broadcast(message);
				break ;

			default:
				data.clear();
				data["<client>"] = client->getNickname();
				data["<modechar>"] = *it;
				message = _messageManager->getMessage(ERR_UNKNOWNMODE, data);
				reply(fd, message);
			}
		}
	}
}
