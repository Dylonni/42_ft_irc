/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psan <psan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:46:43 by dylan             #+#    #+#             */
/*   Updated: 2024/06/03 15:41:27 by psan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/CommandManager.hpp"

CommandManager::CommandManager()
{
	_messageManager = new MessageManager();
	initCommands();
}

CommandManager::~CommandManager()
{
	delete _messageManager;
}

void	CommandManager::initCommands()
{
	_commands["CAP"] = &CommandManager::cap;
	_commands["PASS"] = &CommandManager::pass;
	_commands["NICK"] = &CommandManager::nick;
	_commands["USER"] = &CommandManager::user;

	_commands["JOIN"] = &CommandManager::join;
	_commands["PART"] = &CommandManager::part;
	_commands["MODE"] = &CommandManager::mode;
	_commands["TOPIC"] = &CommandManager::topic;
	_commands["INVITE"] = &CommandManager::invite;
	_commands["KICK"] = &CommandManager::kick;

	_commands["PRIVMSG"] = &CommandManager::privmsg;
	_commands["PING"] = &CommandManager::pong;
	_commands["QUIT"] = &CommandManager::quit;

	_commands["TIME"] = &CommandManager::time;

	_commands["WHO"] = &CommandManager::who;
}

void	CommandManager::processCommand(Server *server, int fd, const str_vector &tokens)
{
	str_map		data;
	std::string	message;
	Client		*client = server->getClient(fd);
	Command		cmd;
	std::string	command = tokens[0];
	std::map<std::string, Command>::iterator it = _commands.find(command);

	// Check whether the command exists.
	if (it == _commands.end())
	{
		data["<command>"] = command;
		message = _messageManager->getMessage(ERR_UNKNOWNCOMMAND, data);
		reply(fd, message);
		return ;
	}

	cmd = it->second;
	if (command.compare("CAP") == 0 || command.compare("PASS") == 0)
		(this->*cmd)(server, fd, tokens);
	else if (client->canRegister() && (command.compare("NICK") == 0 || command.compare("USER") == 0))
		(this->*cmd)(server, fd, tokens);
	else if (client->isRegistered())
		(this->*cmd)(server, fd, tokens);
	else
	{
		message = _messageManager->getMessage(ERR_NOTREGISTERED);
		reply(fd, message);
	}
}

void	CommandManager::sendWelcomeTrain(Server *server, int fd)
{
	str_map		data;
	std::string	message;
	Client		*client = server->getClient(fd);

	data["<client>"] = client->getNickname();
	data["<full>"] = client->getFullname();
	message = _messageManager->getMessage(RPL_WELCOME, data);
	reply(fd, message);

	data.clear();
	data["<client>"] = client->getNickname();
	data["<servername>"] = server->getName();
	data["<version>"] = "4.2.0";
	message = _messageManager->getMessage(RPL_YOURHOST, data);
	reply(fd, message);

	data.clear();
	data["<client>"] = client->getNickname();
	time_t creationTime = server->getCreationTime();
	std::string time = std::asctime(std::localtime(&creationTime));
	time.erase(time.size() - 1 , 1);
	data["<datetime>"] = time;
	message = _messageManager->getMessage(RPL_CREATED, data);
	reply(fd, message);

	data.clear();
	data["<client>"] = client->getNickname();
	data["<servername>"] = server->getName();
	data["<version>"] = "4.2.0";
	message = _messageManager->getMessage(RPL_MYINFO, data);
	reply(fd, message);

	client->setAsRegistered();
}

bool	CommandManager::isNotAlnum(char c)
{
	return !((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'));
}