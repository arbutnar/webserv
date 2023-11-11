/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 13:05:32 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/05 17:57:18 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Master/Master.hpp"

int main(int argc, char *argv[]) {
	try {
		Master master;
		if (argc == 2)
			master.configParse(argv[1])
		else if (argc > 2)
			throw std::invalid_argument("Invalid Argument");
		else if (argc < 2)
			master.configParse("./conf/default.conf");
		
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}
