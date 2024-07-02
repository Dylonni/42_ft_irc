/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psan <psan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:58:18 by dylan             #+#    #+#             */
/*   Updated: 2024/06/03 15:40:34 by psan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/CommandManager.hpp"

void	CommandManager::who(Server *server, int fd, const str_vector &tokens)
{
	str_map		data;
	std::string	message;
	Client		*client = server->getClient(fd);
	Channel		*channel;

	// Check if enough parameters are provided
	if (tokens.size() < 2)
	{
		data["<command>"] = tokens[0];
		message = _messageManager->getMessage(ERR_NEEDMOREPARAMS, data);
		reply(fd, message);
		return ;
	}

	channel = server->getChannel(tokens[1]);
	if (channel != NULL)
	{
		std::vector<Client*> members = channel->getClients();

		for (std::vector<Client*>::iterator it = members.begin(); it != members.end(); ++it)
		{
			data.clear();
			data["<client>"] = client->getNickname();
			data["<channel>"] = channel->getName();
			data["<username>"] = (*it)->getUsername();
			data["<host>"] = (*it)->getHostname();
			data["<server>"] = (*it)->getServname();
			data["<nick>"] = (*it)->getNickname();
			data["<flags>"] = "H";
			data["<realname>"] = (*it)->getRealname();
			message = _messageManager->getMessage(RPL_WHOREPLY, data);
			reply(fd, message);
		}
	}

	data.clear();
	data["<client>"] = client->getNickname();
	data["<name>"] = tokens[1];
	message = _messageManager->getMessage(RPL_ENDOFWHO, data);
	reply(fd, message);
}
