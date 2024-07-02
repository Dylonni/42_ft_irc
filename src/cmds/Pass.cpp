#include "../../include/CommandManager.hpp"

void	CommandManager::pass(Server *server, int fd, const str_vector &tokens)
{
	str_map		data;
	std::string	message;
	Client		*client = server->getClient(fd);

	// Check whether enough parameters are provided.
	if (tokens.size() < 2)
	{
		data["<command>"] = tokens[0];
		message = _messageManager->getMessage(ERR_NEEDMOREPARAMS, data);
		reply(fd, message);
		return ;
	}

	// Check whether the user is registered.
	if (client->canRegister() || client->isRegistered())
	{
		data["<client>"] = client->getNickname();
		message = _messageManager->getMessage(ERR_ALREADYREGISTRED, data);
		reply(fd, message);
		return ;
	}

	// Check whether the user has given the correct password.
	if (server->getPassword().compare(tokens[1]) != 0)
	{
		message = _messageManager->getMessage(ERR_PASSWDMISMATCH);
		std::cout << RED + message + RESET;
		server->removeClient(fd);
		throw std::invalid_argument(message);
	}

	client->allowRegistration();
	message = "PASS :Correct password\r\n";
	std::cout << message << std::endl;
}
