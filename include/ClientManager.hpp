/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dylan <dylan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 16:31:35 by dylan             #+#    #+#             */
/*   Updated: 2024/05/31 16:34:34 by dylan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_MANAGER_HPP
# define CLIENT_MANAGER_HPP

#include <iostream>
#include <string>
#include <map>
#include "Client.hpp"

class ClientManager
{
private:
	std::map<int, Client>	_clients;
public:
	ClientManager();
	~ClientManager();

	Client	*getClient(int fd);
	Client	*getClient(const std::string &nickname);
};

#endif