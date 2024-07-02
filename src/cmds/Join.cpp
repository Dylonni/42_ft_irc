/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psan <psan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:57:41 by dylan             #+#    #+#             */
/*   Updated: 2024/06/03 17:19:00 by psan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/CommandManager.hpp"

void	CommandManager::join(Server *server, int fd, const str_vector &tokens)
{
	str_map		data;
	std::string	message;
	Client		*client = server->getClient(fd);
	Channel		*channel;
	std::string	channelName;

	// Check whether enough parameters are provided.
	if (tokens.size() < 2)
	{
		data["<command>"] = tokens[0];
		message = _messageManager->getMessage(ERR_NEEDMOREPARAMS, data);
		reply(fd, message);
		return ;
	}

	std::vector<std::string> channelNames = splitLine(tokens[1], ',');
	std::vector<std::string> keys;
	if (tokens.size() == 3)
		keys = splitLine(tokens[2], ',');

	size_t	index = 0;
	for (std::vector<std::string>::iterator it = channelNames.begin(); it != channelNames.end(); ++it, ++index)
	{
		if (!isValidChannelName(*it))
		{
			message = "JOIN :Invalid channel name\r\n";
			std::cout << message << std::endl;
			continue ;
		}

		channelName = "";
		if ((*it)[0] != '#')
			channelName += "#";
		channelName += *it;

		// Check whether the user's channel limit would be exceeded by this user joining.
		if (client->getChannelCount() + 1 > CHANLIMIT)
		{
			data.clear();
			data["<client>"] = client->getNickname();
			data["<channel>"] = channelName;
			message = _messageManager->getMessage(ERR_TOOMANYCHANNELS, data);
			reply(fd, message);
			continue ;
		}

		// Check whether the channel needs to be created.
		channel = server->getChannel(channelName);
		if (channel == NULL)
		{
			server->addChannel(channelName, index < keys.size() ? keys[index] : "");
			channel = server->getChannel(channelName);
		}

		// Check whether the channel key is correct.
		if (channel->getKey().compare(index < keys.size() ? keys[index] : "") != 0)
		{
			data.clear();
			data["<client>"] = client->getNickname();
			data["<channel>"] = channelName;
			message = _messageManager->getMessage(ERR_BADCHANNELKEY, data);
			reply(fd, message);
			continue ;
		}

		// Check whether the invite only mode is set.
		if (channel->isInviteOnly() && !channel->isClientInvited(client->getFd()))
		{
			data.clear();
			data["<client>"] = client->getNickname();
			data["<channel>"] = channelName;
			message = _messageManager->getMessage(ERR_INVITEONLYCHAN, data);
			reply(fd, message);
			continue ;
		}

		// Check whether the user limit would be exceeded by this user joining.
		if (channel->isFull())
		{
			data.clear();
			data["<client>"] = client->getNickname();
			data["<channel>"] = channelName;
			message = _messageManager->getMessage(ERR_CHANNELISFULL, data);
			reply(fd, message);
			continue ;
		}

		// Join the channel.
		channel->addClient(client);
		message = ":" + client->getFullname() + " JOIN " + channelName + "\r\n";
		channel->broadcast(message);

		// Set the creator as a channel operator.
		if (channel->getClientCount() == 1)
			channel->setClientPrivileges(client, true);

		// Show the topic of the channel.
		if (channel->getTopic().size() < 1)
		{
			data.clear();
			data["<channel>"] = channelName;
			message = _messageManager->getMessage(RPL_NOTOPIC, data);
			reply(fd, message);
		}
		else
		{
			data.clear();
			data["<client>"] = client->getNickname();
			data["<channel>"] = channelName;
			data["<topic>"] = channel->getTopic();
			message = _messageManager->getMessage(RPL_TOPIC, data);
			reply(fd, message);

			data.clear();
			data["<channel>"] = channelName;
			data["<nick>"] = channel->getWhoModifiedTopic();
			data["<setat>"] = channel->getTopicTime();
			message = _messageManager->getMessage(RPL_TOPICWHOTIME, data);
			reply(fd, message);
		}

		// Show all members of the channel.
		data.clear();
		data["<client>"] = client->getNickname();
		data["<channel>"] = channelName;
		data["<nicknames>"] = channel->getNicknames();
		message = _messageManager->getMessage(RPL_NAMREPLY, data);
		reply(fd, message);

		data.clear();
		data["<client>"] = client->getNickname();
		data["<channel>"] = channelName;
		message = _messageManager->getMessage(RPL_ENDOFNAMES, data);
		reply(fd, message);

		data.clear();
		data["<client>"] = client->getNickname();
		data["<channel>"] = channelName;
		data["<creationtime>"] = channel->getCreateTime();
		message = _messageManager->getMessage(RPL_CREATIONTIME, data);
		reply(fd, message);
	}
}

bool	CommandManager::isValidChannelName(const std::string& name)
{
	return (std::find_if(name.begin() + (name[0] == '#' ? 1 : 0), name.end(), isNotAlnum) == name.end());
}

bool	CommandManager::isValidChannelKey(const std::string& key)
{
	return (std::find_if(key.begin(), key.end(), isNotAlnum) == key.end());
}
