/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 13:05:32 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/12 15:36:57 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Master/Master.hpp"

int main(int argc, char *argv[]) {
	try {
		Master master;
		if (argc == 2)
			master.configDivider(argv[1]);
		else if (argc > 2)
			throw std::invalid_argument("Invalid Argument");
		else if (argc < 2)
			master.configDivider("./conf/default.conf");
		master.arrangeCluster();
		master.start();
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}
