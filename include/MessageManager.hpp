/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psan <psan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:47:35 by dylan             #+#    #+#             */
/*   Updated: 2024/06/03 15:43:10 by psan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_MANAGER_HPP
# define MESSAGE_MANAGER_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "Server.hpp"

enum MessageCode
{
	RPL_WELCOME = 001,
	RPL_YOURHOST = 002,
	RPL_CREATED = 003,
	RPL_MYINFO = 004,

	RPL_ENDOFWHO = 315,
	RPL_CHANNELMODEIS = 324,
	RPL_CREATIONTIME = 329,

	RPL_NOTOPIC = 331,
	RPL_TOPIC = 332,
	RPL_TOPICWHOTIME = 333,

	RPL_INVITING = 341,

	RPL_WHOREPLY = 352,
	RPL_NAMREPLY = 353,
	RPL_ENDOFNAMES = 366,
	RPL_ENDOFBANLIST = 368,

	RPL_TIME = 391,

	ERR_NOSUCHNICK = 401,
	ERR_NOSUCHCHANNEL = 403,
	ERR_TOOMANYCHANNELS = 405,

	ERR_NORECIPIENT = 411,
	ERR_NOTEXTTOSEND = 412,

	ERR_UNKNOWNCOMMAND = 421,

	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_NICKCOLLISION = 436,

	ERR_USERNOTINCHANNEL = 441,
	ERR_NOTONCHANNEL = 442,
	ERR_USERONCHANNEL = 443,

	ERR_NOTREGISTERED = 451,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTRED = 462,
	ERR_PASSWDMISMATCH = 464,

	ERR_CHANNELISFULL = 471,
	ERR_UNKNOWNMODE = 472,
	ERR_INVITEONLYCHAN = 473,
	ERR_BADCHANNELKEY = 475,
	ERR_CHANOPRIVSNEEDED = 482,
	ERR_USERSDONTMATCH = 502,
};

typedef std::map<std::string, std::string>	str_map;

class MessageManager
{
private:
	std::map<int, std::string>	_messages;

	void	initMessages();

public:
	MessageManager();
	~MessageManager();

	std::string	getMessage(int messageCode, str_map data = str_map());
};

#endif
