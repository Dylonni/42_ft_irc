/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daumis <daumis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:14:19 by daumis            #+#    #+#             */
/*   Updated: 2024/05/09 15:46:30 by daumis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

int main(int argc, char const *argv[])
{
	if (argc != 3)
        return (std::cerr << RED "Usage: ./Server <port> <password>\n" RESET, 1);
	try
	{
		Server server(argv[1], argv[2]);
		server.startServer();
	}
	catch(const std::exception& e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
	}
	return 0;
}

