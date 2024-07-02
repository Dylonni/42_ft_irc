#include "../../include/CommandManager.hpp"

void	CommandManager::time(Server *server, int fd, const str_vector &tokens)
{
	str_map		data;
	std::string	message;
	Client		*client = server->getClient(fd);

	(void)tokens;
	std::time_t currentTime;
	std::time(&currentTime);
	std::string	t = ctime(&currentTime);
	t = t.substr(0, t.size() - 1);

	data["<client>"] = client->getNickname();
	data["<server>"] = server->getName();
	data["<time>"] = t;
	message = _messageManager->getMessage(RPL_TIME, data);
	reply(fd, message);
}