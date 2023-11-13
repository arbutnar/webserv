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

# include "../Server/Server.hpp"

class Master {
	private:
		std::vector<Server>	_cluster;
		void	configCleaner( std::ifstream &configFile, std::string &content );
	public:
		Master( void );
		Master( const char* path );
		Master( const Master &src );
		Master& operator=( const Master &src );
		~Master( );

		void	configDivider( const char* path );
		void	serverParser( std::string &block );
};
