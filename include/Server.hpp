/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daumis <daumis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 15:20:39 by daumis            #+#    #+#             */
/*   Updated: 2024/05/07 14:04:32 by daumis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# define RESET	"\e[0m"
# define RED	"\e[1;31m"
# define GREEN	"\e[1;32m"
# define YELLOW	"\e[1;93m"
# define BLUE	"\e[1;34m"
# define PURPLE	"\e[1;35m"
# define CYAN	"\e[1;36m"
# define MAX_CONNECTIONS 42
# define LINELEN 512

# define INVITE 105
# define KEY 107
# define LIM 108
# define OPER 111
# define TOPIC 116

#include <inttypes.h>
#include <string>
#include <string.h>
#include <exception>
#include <iostream>
#include <sstream>
#include <netdb.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <map>
#include <errno.h>
#include <arpa/inet.h>
#include <signal.h>
#include <ctime>
#include <limits.h>
#include "Client.hpp"
#include "Channel.hpp"
#include "CommandManager.hpp"

class Client;
class Channel;
class CommandManager;

typedef std::map<std::string, std::string>	str_map;
typedef std::vector<std::string>			str_vector;

class Server
{
private:
	int								_fd;
	std::string						_name;
	std::string						_address;
	uint16_t						_port;
	std::string 					_password;
	std::time_t						_creationTime;

	std::vector<struct pollfd>		_fds;
	std::map<int, Client>			_clients;
	std::map<std::string, Channel>	_channels;

	char							_inputBuffer[LINELEN];
	std::map<int, std::string>		_fdBuffers;
	bool							_isClosed;

	CommandManager					*_commandManager;

public:
	Server(const std::string &port, const std::string &password);
	~Server();

	class SigIntException : public std::exception
	{
	public:
		const char *what() const throw();
	};
	
	Client				*getClient(int fd);
	Client				*getClient(const std::string &nickname);
	Channel				*getChannel(const std::string &name);
	std::time_t			getCreationTime() const;
	const std::string	&getPassword() const;
	const std::string	&getName() const;

	void				addChannel(const std::string &name, const std::string &key);
	void				removeChannel(const std::string &name);

	bool				checkArgs(std::string const &av1, std::string const &av2);
	void				setServerName();

	void				createServerSocket();
	void				setSocketReuseAddr(int fd);
	void				setSocketNonBlocking(int fd);
	void				bindSocket(int fd);
	void				startListening(int fd);
	void				createServer();

	void				waitForEvent();
	int					createClientSocketFromAccept();
	void				waitForConnection();
	void				handleEvent();
	void				startServer();

	void				receiveInput(int fd);
	void				handleInput(int fd, const std::string &input);

	void				removeClient(int fd);
	void				readInput(int fd);
	void				parseInput(int fd, std::string &line);
	void				broadcast(std::string message);
};

void					reply(int fd, const std::string &message);
void					broadcast(std::vector<Client*> clients, std::string message);
void					handleSignal(int sig);
int						ft_stoi(const std::string& str);
str_vector				splitLine(const std::string &line, char delimiter);

#endif