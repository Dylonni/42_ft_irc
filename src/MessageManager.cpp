/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psan <psan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 17:32:05 by dylan             #+#    #+#             */
/*   Updated: 2024/06/03 16:53:23 by psan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/MessageManager.hpp"

MessageManager::MessageManager()
{
	initMessages();
}

MessageManager::~MessageManager(){}

void	MessageManager::initMessages()
{
	_messages[RPL_WELCOME] = "001 <client> :Welcome to the Internet Relay Network, <full>\r\n";
	_messages[RPL_YOURHOST] = "002 <client> :Your host is <servername>, running version <version>\r\n";
	_messages[RPL_CREATED] = "003 <client> :This server was created <datetime>\r\n";
	_messages[RPL_MYINFO] = "004 <client> :<servername> <version>  i,k,l,o,t\r\n";

	_messages[RPL_ENDOFWHO] = "315 <client> <name> :End of /WHO list\r\n";
	_messages[RPL_CHANNELMODEIS] = "324 <client> <channel> <modes> <params>\r\n";
	_messages[RPL_CREATIONTIME] = "329 <client> <channel> <creationtime>\r\n";

	_messages[RPL_NOTOPIC] = "331 <channel> :No topic is set\r\n";
	_messages[RPL_TOPIC] = "332 <client> <channel> :<topic>\r\n";
	_messages[RPL_TOPICWHOTIME] = "333 <channel> <nick> <setat>\r\n";

	_messages[RPL_INVITING] = "341 <client> <nick> <channel>\r\n";

	_messages[RPL_WHOREPLY] = "352 <client> <channel> <username> <host> <server> <nick> <flags> :0 <realname>\r\n";
	_messages[RPL_NAMREPLY] = "353 <client> = <channel> :<nicknames>\r\n";
	_messages[RPL_ENDOFNAMES] = "366 <client> <channel> :End of /NAMES list\r\n";
	_messages[RPL_ENDOFBANLIST] = "368 <client> <channel> :End of channel ban list\r\n";

	_messages[RPL_TIME] = "391 <client> <server> :<time>\r\n";

	_messages[ERR_NOSUCHNICK] = "401 <client> <nickname> :No such nick/channel\r\n";
	_messages[ERR_NOSUCHCHANNEL] = "403 <client> <channel> :No such channel\r\n";
	_messages[ERR_TOOMANYCHANNELS] = "405 <client> <channel> :You have joined too many channels\r\n";

	_messages[ERR_NORECIPIENT] = "411 <client> :No recipient given (<command>)\r\n";
	_messages[ERR_NOTEXTTOSEND] = "412 <client> :No text to send\r\n";

	_messages[ERR_UNKNOWNCOMMAND] = "421 <command> :Unknown command\r\n";

	_messages[ERR_NONICKNAMEGIVEN] = "431 :No nickname given\r\n";
	_messages[ERR_ERRONEUSNICKNAME] = "432 <nick> :Erroneous nickname\r\n";
	_messages[ERR_NICKNAMEINUSE] = "433 <nick> :Nickname is already in use\r\n";
	_messages[ERR_NICKCOLLISION] = "436 <nick> :Nickname collision KILL from <user>@<host>\r\n";

	_messages[ERR_USERNOTINCHANNEL] = "441 <client> <nick> <channel> :They aren't on that channel\r\n";
	_messages[ERR_NOTONCHANNEL] = "442 <client> <channel> :You're not on that channel\r\n";
	_messages[ERR_USERONCHANNEL] = "443 <channel> <nick> :is already on channel\r\n";

	_messages[ERR_NOTREGISTERED] = "451 :You have not registered\r\n";
	_messages[ERR_NEEDMOREPARAMS] = "461 <command> :Not enough parameters\r\n";
	_messages[ERR_ALREADYREGISTRED] = "462 <client> :Unauthorized command (already registered)\r\n";
	_messages[ERR_PASSWDMISMATCH] = "464 :Password incorrect\r\n";

	_messages[ERR_CHANNELISFULL] = "471 <client> <channel> :Cannot join channel (+l)\r\n";
	_messages[ERR_UNKNOWNMODE] = "472 <client> <modechar> :Is unknown mode char to me\r\n";
	_messages[ERR_INVITEONLYCHAN] = "473 <client> <channel> :Cannot join channel (+i)\r\n";
	_messages[ERR_BADCHANNELKEY] = "475 <client> <channel> :Cannot join channel (+k)\r\n";
	_messages[ERR_CHANOPRIVSNEEDED] = "482 <client> <channel> :You're not channel operator\r\n";
	_messages[ERR_USERSDONTMATCH] = "502 <client> :Cant change mode for other users\r\n";
}

std::string	MessageManager::getMessage(int messageCode, str_map data)
{
	std::string								message;
	str_vector								keys;
	std::map<int, std::string>::iterator	it = _messages.find(messageCode);

	// Check if the  code exists in the _replies map
	if (it == _messages.end())
		return ("");

	// Get the template message associated with the reply code
	message = it->second;

	// Collect all the keys from the data map into the keys vector
	for (str_map::iterator dataIt = data.begin(); dataIt != data.end(); ++dataIt)
		keys.push_back(dataIt->first);

	// Loop through each key and replace its occurrences in the message
	for (str_vector::iterator keyIt = keys.begin(); keyIt != keys.end(); ++keyIt)
	{
		std::string	key = *keyIt;
		size_t		pos = 0;

		// Find each occurrence of the key in the message and replace it with the corresponding value from the data map
		while ((pos = message.find(key, pos)) != std::string::npos)
		{
			message.replace(pos, key.length(), data[key]); // Replace the key with its value
			pos += data[key].length(); // Move past the last replaced position to continue searching
		}
	}

	return (message);
}
