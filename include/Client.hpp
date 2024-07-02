/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psan <psan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 14:13:59 by daumis            #+#    #+#             */
/*   Updated: 2024/05/14 15:18:24 by psan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <string>

class Client
{
private:
	int							_fd;
	bool						_canRegister;
	bool						_isRegistered;
	int							_channelCount;

	std::string					_nickname;
	std::string					_username;
	std::string					_hostname;
	std::string					_servname;
	std::string					_realname;

public:
	Client(int fd);
	~Client();

	int					getFd() const;
	bool				canRegister() const;
	bool				isRegistered() const;
	int					getChannelCount() const;

	const std::string	&getNickname() const;
	const std::string	&getUsername() const;
	const std::string	&getHostname() const;
	const std::string	&getServname() const;
	const std::string	&getRealname() const;
	std::string			getFullname() const;

	void				allowRegistration();
	void				setAsRegistered();
	void				modifyChannelCount(int value);

	void				setNickname(const std::string &nickname);
	void				setUsername(const std::string &username);
	void				setHostname(const std::string &hostname);
	void				setServname(const std::string &servname);
	void				setRealname(const std::string &realname);
};


#endif