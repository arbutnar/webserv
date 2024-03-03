/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 13:05:32 by arbutnar          #+#    #+#             */
/*   Updated: 2024/03/01 18:17:09 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/Master/Master.hpp"

Master	*master = NULL;

void	sigintHandler( int signum ) {
	if (signum == SIGINT && master != NULL)
		delete master;
	exit(0);
}

void	sigchldHandler( int signum ) {
	if (signum == SIGCHLD)
		waitpid(-1, NULL, 0);
}

int main(int argc, char *argv[]) {

	signal(SIGINT, sigintHandler);
	signal(SIGCHLD, sigchldHandler);
	master = new Master();
	try {
		if (argc == 2)
			master->configDivider(argv[1]);
		else if (argc > 2)
			throw std::invalid_argument("Invalid Argument");
		else if (argc < 2)
			master->configDivider("conf/default.conf");
		master->start();
	} catch (std::exception &e) {
		if (master != NULL)
			delete master;
		std::cout << e.what() << std::endl;
	}

	return 0;
}
