/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Master.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:59:48 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/08 13:56:03 by arbutnar         ###   ########.fr       */
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
		std::ifstream		_configFile;
		std::vector<Server>	_cluster;
	public:
		class FileError;

		Master( void );
		Master( const char* path );
		Master( const Master &src );
		Master& operator=( Master &src );	// const
		~Master( );

		bool	configSyntax( std::string content );
		void	configParse( void );
};
