/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psan <psan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 14:22:06 by daumis            #+#    #+#             */
/*   Updated: 2024/05/14 15:18:16 by psan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client(int fd): _fd(fd), _canRegister(false), _isRegistered(false), _channelCount(0){}

Client::~Client(){}

int	Client::getFd() const { return (_fd); }

bool	Client::canRegister() const { return (_canRegister); }

bool	Client::isRegistered() const { return (_isRegistered); }

int	Client::getChannelCount() const { return (_channelCount); }

const std::string& Client::getNickname() const { return (_nickname); }

const std::string& Client::getUsername() const { return (_username); }

const std::string& Client::getHostname() const { return (_hostname); }

const std::string& Client::getServname() const { return (_servname); }

const std::string& Client::getRealname() const { return (_realname); }

std::string	Client::getFullname() const { return (_nickname + "!" + _username + "@" + _hostname); }

void	Client::allowRegistration() { _canRegister = true; }

void	Client::setAsRegistered() { _isRegistered = true; }

void	Client::modifyChannelCount(int value) { _channelCount += value; }

void Client::setNickname(const std::string &nickname) { _nickname = nickname; }

void Client::setUsername(const std::string &username) { _username = username; }

void Client::setHostname(const std::string &hostname) { _hostname = hostname; }

void Client::setServname(const std::string &servname) { _servname = servname; }

void Client::setRealname(const std::string &realname) { _realname = realname; }
