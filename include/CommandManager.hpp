/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psan <psan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:47:31 by dylan             #+#    #+#             */
/*   Updated: 2024/06/03 17:22:23 by psan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_MANAGER_HPP
# define COMMAND_MANAGER_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <ctime>
#include "Server.hpp"
#include "MessageManager.hpp"

#define CHANLIMIT 3

class Server;
class MessageManager;

class CommandManager
{
private:
	typedef std::map<std::string, std::string>	str_map;
	typedef std::vector<std::string>			str_vector;
	typedef void (CommandManager::*Command)(Server *server, int fd, const str_vector &tokens);

	MessageManager					*_messageManager;
	std::map<std::string, Command>	_commands;

	void	initCommands();
	void	sendWelcomeTrain(Server *server, int fd);

	//Connection Related Commands

	void	cap(Server *server, int fd, const str_vector &tokens);
	void	pass(Server *server, int fd, const str_vector &tokens);
	void	nick(Server *server, int fd, const str_vector &tokens);
	bool	isValidNickname(const std::string &nickname);
	void	user(Server *server, int fd, const str_vector &tokens);
	bool	isValidUsername(const std::string &username);

	//Channel Related Commmands

	void	join(Server *server, int fd, const str_vector &tokens);
	bool	isValidChannelName(const std::string &name);
	bool	isValidChannelKey(const std::string &key);
	void	part(Server *server, int fd, const str_vector &tokens);
	void	mode(Server *server, int fd, const str_vector &tokens);
	void	topic(Server *server, int fd, const str_vector &tokens);
	void	invite(Server *server, int fd, const str_vector &tokens);
	void	kick(Server *server, int fd, const str_vector &tokens);

	// Server Related Commands

	void	privmsg(Server *server, int fd, const str_vector &tokens);
	void	pong(Server *server, int fd, const str_vector &tokens);
	void	who(Server *server, int fd, const str_vector &tokens);
	void	quit(Server *server, int fd, const str_vector &tokens);

	void	time(Server *server, int fd, const str_vector &tokens);

	static bool	isNotAlnum(char c);

public:
	CommandManager();
	~CommandManager();

	void	processCommand(Server *server, int fd, const str_vector &tokens);
};

#endif
