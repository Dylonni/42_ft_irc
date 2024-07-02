/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daumis <daumis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:58:05 by dylan             #+#    #+#             */
/*   Updated: 2024/06/05 15:48:52 by daumis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/CommandManager.hpp"

void	CommandManager::pong(Server *server, int fd, const str_vector &tokens)
{
	(void)server;

	std::string	message = "PONG " + tokens[1] + "\r\n";
	reply(fd, message);
}
