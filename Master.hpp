/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Master.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:59:48 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/04 14:33:22 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <fstream>
# include <string>
# include <sstream>
# include <vector>

class Server;

class Master {
	private:
		std::ifstream		configFile;
		std::vector<Server>	cluster;
	public:
		class FileError;

		Master( void );
		Master( const char* path );

		bool	checkSyntax( void );
		void	validateConfig( void );
};
