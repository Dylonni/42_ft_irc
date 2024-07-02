/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daumis <daumis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:56:54 by dylan             #+#    #+#             */
/*   Updated: 2024/06/05 15:46:18 by daumis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/CommandManager.hpp"

void	CommandManager::quit(Server *server, int fd, const str_vector &tokens)
{
	std::string	message;
	Client		*client = server->getClient(fd);

	message = ":" + client->getFullname() + " QUIT" + (tokens.size() < 2 ? "" : " :" + tokens[1]) + "\r\n";
	server->broadcast(message);
	server->removeClient(fd);
}
