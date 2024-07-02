/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daumis <daumis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 15:29:02 by daumis            #+#    #+#             */
/*   Updated: 2024/05/15 15:00:21 by daumis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"

Channel::Channel(){}

Channel::Channel(const std::string &name, const std::string &key) 
:	_name(name), _key(key),
	_topic(""), _topicTime(""), _topicLastModifiedBy(NULL),
	_maxClients(MAX_CONNECTIONS),
	_isInviteOnly(false), _canModifyTopic(false)
{
	std::time_t currentTime;
	std::time(&currentTime);
	std::stringstream ss;
	ss << currentTime;
	_createTime = ss.str();
	ss.str(std::string());
	ss.clear();
}

Channel::~Channel(){}

const std::string	&Channel::getName() const { return (_name); }

const std::string	&Channel::getKey() const { return (_key); }

const std::string	&Channel::getCreateTime() const { return (_createTime); }

const std::string	&Channel::getTopic() const { return (_topic); }

const std::string	&Channel::getTopicTime() const { return (_topicTime); }

const std::string	&Channel::getWhoModifiedTopic() const { return (_topicLastModifiedBy->getNickname()); }

std::vector<Client*>	Channel::getClients() const
{
	std::vector<Client*> users = _operators;

	users.insert(users.end(), _users.begin(), _users.end());
	return (users);
}

unsigned long	Channel::getClientCount() const { return (_users.size() + _operators.size()); }

std::string	Channel::getMaxClients() const
{
	std::ostringstream ss;

	ss << _maxClients;
	return (ss.str());
}

std::string	Channel::getNicknames() const
{
	std::string	names = "";

	for (std::vector<Client*>::const_iterator it = _operators.begin(); it != _operators.end(); ++it)
	{
		names += "@" + (*it)->getNickname();
		if (it != _operators.end() - 1)
			names += " ";
	}

	if (names.length() > 0)
		names += " ";
	for (std::vector<Client*>::const_iterator it = _users.begin(); it != _users.end(); ++it)
	{
		names += " " + (*it)->getNickname();
		if (it != _users.end() - 1)
			names += " ";
	}

	return (names);
}

std::string	Channel::getModes() const
{
	std::string	modes = "+l";

	if (_isInviteOnly)
		modes += "i";
	if (_key.length() != 0)
		modes += "k";
	if (_canModifyTopic)
		modes += "t";
	return (modes);
}

void	Channel::setName(const std::string &name) { _name = name; }

void	Channel::setKey(const std::string &key) { _key = key; }

void	Channel::setTopic(const std::string &topic, Client *client)
{
	if (!_canModifyTopic)
		return ;

	_topic = topic;
	_topicLastModifiedBy = client;
	std::time_t time = std::time(NULL);
	std::string	t = std::asctime(std::localtime(&time));
	t.erase(t.size() - 1 , 1);
	_topicTime = t;
}

void	Channel::setMaxClients(int maxUsers)
{
	if (maxUsers == 0)
		maxUsers = INT_MAX;
	_maxClients = maxUsers;
}

void	Channel::setInviteOnly(bool option) { _isInviteOnly = option; }

void	Channel::setModifyTopic(bool option) { _canModifyTopic = option; }

void	Channel::setClientPrivileges(Client* client, bool givePrivileges)
{
	if (!client || !hasClient(client->getNickname()))
		return ;
	if (givePrivileges && !isOperator(client->getNickname()))
	{
		_operators.push_back(client);
		_operatorFds.push_back(client->getFd());
		_users.erase(std::find(_users.begin(), _users.end(), client));
	}
	else if (!givePrivileges && isOperator(client->getNickname()))
	{
		_users.push_back(client);
		_operators.erase(std::find(_operators.begin(), _operators.end(), client));
		_operatorFds.erase(std::find(_operatorFds.begin(), _operatorFds.end(), client->getFd()));
	}
}

bool	Channel::hasClient(const std::string &nickname) const
{
	for (std::vector<Client*>::const_iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
			return true;
	}
	return (isOperator(nickname));
}


bool	Channel::isOperator(const std::string &nickname) const
{
	for (std::vector<Client*>::const_iterator it = _operators.begin(); it != _operators.end(); ++it)
	{
		if ((*it)->getNickname().compare(nickname) == 0)
			return true;
	}
	return false;
}

bool	Channel::isClientInvited(int fd) const { return (std::find(_invitedFds.begin(), _invitedFds.end(), fd) != _invitedFds.end()); }

bool	Channel::isFull() const { return (_users.size() + _operators.size() == _maxClients); }

bool	Channel::isInviteOnly() const { return (_isInviteOnly); }

bool	Channel::canModifyTopic() const { return (_canModifyTopic); }

void	Channel::addClient(Client* client)
{
	if (!client)
		return ;
	if (std::find(_operatorFds.begin(), _operatorFds.end(), client->getFd()) != _operatorFds.end())
	{
		_operators.push_back(client);
		client->modifyChannelCount(1);
		return ;
	}
	_users.push_back(client);
	client->modifyChannelCount(1);
}

void	Channel::removeClient(Client* client)
{
	if (!client || !hasClient(client->getNickname()))
		return ;

	std::vector<Client*>::iterator it;
	if ((it = std::find(_users.begin(), _users.end(), client)) != _users.end())
		_users.erase(it);
	else if ((it = std::find(_operators.begin(), _operators.end(), client)) != _operators.end())
		_operators.erase(it);

	std::vector<int>::iterator ite;
	if ((ite = std::find(_invitedFds.begin(), _invitedFds.end(), client->getFd())) != _invitedFds.end())
		_invitedFds.erase(ite);

	client->modifyChannelCount(-1);
}

void	Channel::inviteClient(Client* client)
{
	if (!client)
		return ;
	_invitedFds.push_back(client->getFd());
}

void	Channel::broadcast(const std::string &message)
{
	for (std::vector<Client*>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		reply((*it)->getFd(), message);
	}
	for (std::vector<Client*>::iterator it = _operators.begin(); it != _operators.end(); ++it)
	{
		reply((*it)->getFd(), message);
	}
}

void	Channel::broadcastExceptFd(const std::string &message, int fd)
{
	for (std::vector<Client*>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (fd != (*it)->getFd())
			reply((*it)->getFd(), message);
	}
	for (std::vector<Client*>::iterator it = _operators.begin(); it != _operators.end(); ++it)
	{
		if (fd != (*it)->getFd())
			reply((*it)->getFd(), message);
	}
}

