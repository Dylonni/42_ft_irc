/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daumis <daumis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 15:01:35 by daumis            #+#    #+#             */
/*   Updated: 2024/05/15 15:00:38 by daumis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <ctime>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Server.hpp"

class Client;

class Channel
{
private:
	std::string					_name;
	std::string					_key;
	std::string					_createTime;
	std::string					_topic;
	std::string					_topicTime;
	Client*						_topicLastModifiedBy;

	std::vector<Client*>		_users;
	std::vector<Client*>		_operators;
	std::vector<int>			_invitedFds;
	std::vector<int>			_operatorFds;

	unsigned long				_maxClients;
	bool						_isInviteOnly;
	bool						_canModifyTopic;

public:
	Channel();
	Channel(const std::string& name, const std::string& key);
	~Channel();

	const std::string	&getName() const;
	const std::string	&getKey() const;
	const std::string	&getCreateTime() const;
	const std::string	&getTopic() const;
	const std::string	&getTopicTime() const;
	const std::string	&getWhoModifiedTopic() const;

	std::vector<Client*>getClients() const;
	unsigned long		getClientCount() const;
	std::string			getMaxClients() const;
	std::string			getNicknames() const;
	std::string			getModes() const;

	void				setName(const std::string &name);
	void				setKey(const std::string &key);
	void				setTopic(const std::string &topic, Client *client);
	void				setMaxClients(int limit);
	void				setInviteOnly(bool option);
	void				setModifyTopic(bool option);
	void				setClientPrivileges(Client* client, bool givePrivileges);

	bool				hasClient(const std::string &nickname) const;
	bool				isOperator(const std::string &nickname) const;
	bool				isClientInvited(int fd) const;
	bool				isFull() const;
	bool				isInviteOnly() const;
	bool				canModifyTopic() const;
	
	void				addClient(Client* client);
	void				removeClient(Client* client);
	void				inviteClient(Client* client);

	void				broadcast(const std::string &message);
	void				broadcastExceptFd(const std::string &message, int fd);
};


#endif