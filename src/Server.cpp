/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daumis <daumis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 15:20:37 by daumis            #+#    #+#             */
/*   Updated: 2024/05/09 17:59:06 by daumis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Server::Server(const std::string &port, const std::string &password)
{
	_commandManager = new CommandManager();
	setServerName();
	if (!checkArgs(port, password))
		throw std::invalid_argument("Invalid Arguments");
	createServer();
}

Server::~Server()
{
	// Clean after server turn off
	for (unsigned long i = 1; i < _fds.size(); i++)
	{
		if (close(_fds[i].fd) < 0)
			throw std::runtime_error("Failed to close during clean up");
	}

	close(_fd);
	delete _commandManager;
}

Client	*Server::getClient(int fd)
{
	std::map<int, Client>::iterator it = _clients.find(fd);
	if (it != _clients.end())
		return (&(it->second));
	return (NULL);
}

Client	*Server::getClient(const std::string &nickname)
{
	std::map<int, Client>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		if (nickname.compare(it->second.getNickname()) == 0)
			return (&(it->second));
	}
	return (NULL);
}

Channel	*Server::getChannel(const std::string &name)
{
	std::map<std::string, Channel>::iterator it = _channels.find(name);
	if (it != _channels.end())
		return (&(it->second));
	return (NULL);
}

const std::string& Server::getPassword() const
{
	return (_password);
}

std::time_t	Server::getCreationTime() const
{
	return (_creationTime);
}

const std::string& Server::getName() const
{
	return (_name);
}

void	Server::addChannel(const std::string &name, const std::string &key)
{
	Channel newChannel(name, key);

	_channels.insert(std::map<std::string, Channel>::value_type(name, newChannel));
}

void	Server::removeChannel(const std::string &name)
{
	_channels.erase(name);
}

void	Server::createServerSocket()
{
	struct protoent	*pe;	// Protocol entry

	// Get the protocol information for TCP
	pe = getprotobyname("tcp");
	if (pe == NULL)
		throw std::runtime_error("Failed to get protocol information");

	// Create a socket using TCP/IP protocol
	_fd = socket(AF_INET, SOCK_STREAM, pe->p_proto);
	if (_fd < 0)
		throw std::runtime_error("Failed to create socket");
}

void	Server::setSocketReuseAddr(int fd)
{
	socklen_t	optionValue = 1;	// Length of socket-related option

	// Set socket option to allow address reuse (boolean option)
	// 
	// Other sockets can bind to the same address and port combination,
	// even if there is still a socket using it, provided that the original socket
	// is in a closed state or in a state where it's bound but not listening or connected.
	// Without it, attempting to bind a socket to an address and port
	// that is already in use will fail with an "Address Already in Use" error.
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optionValue, sizeof(optionValue)) < 0)
		throw std::runtime_error("Failed to set socket option");
}

void	Server::setSocketNonBlocking(int fd)
{
	// Set the socket flag to non-blocking mode
	// 
	// System calls such as read, write, and accept return as soon as they can perform their operation without blocking.
	// They may return with partial data if not all requested data is immediately available,
	// or they may return an error code such as EWOULDBLOCK or EAGAIN to indicate that the operation would block and should be retried later.
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("Failed to set non-blocking mode");
}

void	Server::bindSocket(int fd)
{
	struct sockaddr_in	serverAddress;					// Socket address information

	// Initialize serverAddress to zero
	memset(&serverAddress, 0, sizeof(serverAddress));

	// Set socket address parameters
	serverAddress.sin_addr.s_addr = INADDR_ANY;			// Bind to all available interfaces
	serverAddress.sin_family = AF_INET;					// IPv4
	serverAddress.sin_port = htons(_port);				// Convert port to network byte order

	// Bind socket to the specified address and port
	if (bind(fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
		throw std::runtime_error("Failed to bind socket");
}

void	Server::startListening(int fd)
{
	// Start listening on the server socket
	if (listen(fd, MAX_CONNECTIONS) < 0)
		throw std::runtime_error("Failed to listen on socket");
}

void	Server::createServer()
{
	struct pollfd	server;

	createServerSocket();
	setSocketReuseAddr(_fd);
	setSocketNonBlocking(_fd);
	bindSocket(_fd);
	startListening(_fd);

	// Update the pollfd structure for the server socket
	server.fd = _fd;	// Set file descriptor to server socket
	server.events = POLLIN;	// Set events to poll for (POLLIN: data available to read)
	server.revents = 0;

	// Add the server socket to the list of file descriptors to poll
	_fds.push_back(server);
	_creationTime = std::time(NULL);
	std::cout << GREEN "Server: " PURPLE << _name << RESET << " from " << _address << std::endl;
	std::cout << GREEN "Created at: " RESET << std::asctime(std::localtime(&_creationTime));
	std::cout << GREEN "Running on port: " YELLOW << _port << RESET << "\n" << std::endl;
}

void	Server::waitForEvent()
{
	// Wait for events on file descriptors using poll
	int	res = poll(&_fds[0], _fds.size(), 1000000);
	if (res < 0)
		throw std::runtime_error("Polling for events failed");
	if (res == 0)
		throw std::runtime_error("Polling for events timed out");
}

int	Server::createClientSocketFromAccept()
{
	int 				clientFd;
	struct sockaddr_in	clientAddress;
	socklen_t			clientAddressSize = sizeof(clientAddress);

	// Accept incoming connection
	clientFd = accept(_fd, (struct sockaddr*)&clientAddress, &clientAddressSize);
	if (clientFd < 0)
		throw std::runtime_error("Failed to accept connection");
	return (clientFd);
}

void	Server::waitForConnection()
{
	int 				clientFd = createClientSocketFromAccept();
	struct pollfd		client;

	if (_fds.size() > MAX_CONNECTIONS)
	{
		close(clientFd);
		std::cout << RED "Max connections reached" RESET << std::endl;
		return ;
	}
	// Set to non blocking
	setSocketNonBlocking(clientFd);

	// Configure the pollfd structure for the new client socket
	client.fd = clientFd;
	client.events = POLLIN;
	client.revents = 0;

	// Add the new client to the list of file descriptors to poll
	_fds.push_back(client);

	// Add the new client to the map of clients
	_clients.insert(std::map<int, Client>::value_type(clientFd, Client(clientFd)));
	_fdBuffers.insert(std::map<int, std::string>::value_type(clientFd, ""));
	std::cout << GREEN "Connection accepted\n" RESET;
}

void	Server::handleEvent()
{
	for (unsigned long i = 0; i < _fds.size(); ++i)
	{
		// No event detected on this file descriptor, check the next one
		if (_fds[i].revents == 0)
			continue ;

		// Check if the event indicates data available for reading
		if ((_fds[i].revents & POLLIN) != 0)
		{
			// Handle the event based on the file descriptor type
			if (_fds[i].fd == _fd)
			{
				// Handle server socket event (new connection)
				waitForConnection();
				break ;
			}

			try
			{
				// Handle client socket event (data available for reading)
				readInput(_fds[i].fd);
			}
			catch(const std::invalid_argument& e)
			{
				_fdBuffers.erase(_fds[i].fd);
				break ;
			}
		}
	}
}

void	handleSignal(int sig)
{
	if (sig == SIGINT)
		throw Server::SigIntException();
}

void	Server::startServer()
{
	signal(SIGINT, &handleSignal);
	while (1)
	{
		waitForEvent();
		handleEvent();
	}
}

void	Server::receiveInput(int fd)
{
	ssize_t		res = 0;

	res = recv(fd, _inputBuffer, sizeof(_inputBuffer), 0);
	if (res < 0)
	{
		if (errno != EWOULDBLOCK)
			throw std::runtime_error("Failed to receive input");
		return ;
	}
	if (res == 0)
	{
		_isClosed = true;
		return ;
	}
}

void	Server::handleInput(int fd, const std::string &input)
{
	std::string	command;
	size_t		len;

	_fdBuffers[fd].append(input);
	len = _fdBuffers[fd].find("\r\n");
	while (len != std::string::npos)
	{
		// Extract command from input
		command = _fdBuffers[fd].substr(0, len);

		// Remove command from input
		_fdBuffers[fd] = _fdBuffers[fd].substr(len + 2, _fdBuffers[fd].length() - (len + 2));

		parseInput(fd, command);

		// Find next command in input
		len = _fdBuffers[fd].find("\r\n");
	}
}

void	Server::readInput(int fd)
{
	_isClosed = false;
	memset(_inputBuffer, 0, sizeof(_inputBuffer));

	receiveInput(fd);
	handleInput(fd, _inputBuffer);

	if (_isClosed)
	{
		if (close(fd) < 0)
			throw std::runtime_error("Failed to close socket");
		removeClient(fd);
	}
}

void	Server::parseInput(int fd, std::string &line)
{
	if (line.empty())
		return ;
	std::cout << PURPLE + line + RESET << std::endl;
	std::vector<std::string>	tokens = splitLine(line, ' ');
	std::string					command;
	
	command = tokens[0];
	std::transform(command.begin(), command.end(), command.begin(), ::toupper);
	tokens[0] = command;
	_commandManager->processCommand(this, fd, tokens);
}

void	Server::removeClient(int fd)
{
	if (_clients.find(fd) == _clients.end())
		throw std::out_of_range("Client not found");
	_clients.erase(fd);
	for (std::vector<struct pollfd>::iterator it = _fds.begin(); it != _fds.end(); it++)
	{
		if (it->fd == fd)
		{
			_fds.erase(it);
			close(fd);
			std::cout << YELLOW "Connection closed" RESET << std::endl;
			break ;
		}
	}
}

const char *Server::SigIntException::what() const throw()
{
	return "Server interrupted by SIGINT (CTRL+C)";
}