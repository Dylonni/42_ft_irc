/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dylan <dylan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:59:04 by dylan             #+#    #+#             */
/*   Updated: 2024/05/31 17:34:22 by dylan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/CommandManager.hpp"

void	CommandManager::cap(Server *server, int fd, const str_vector &tokens)
{
	std::string	message;

	(void)fd;
	(void)tokens;
	(void)server;

	message = "CAP :Ignored\r\n";
	std::cout << message << std::endl;
}