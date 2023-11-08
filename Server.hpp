/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:23:11 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/08 16:41:27 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Directives.hpp"
#include "Location.hpp"

class Server {
	friend void Master::configParse( void );

	private:
		Directives				_directives;
		std::vector<Location>	_locations;
	public:
		Server( void );
		Server( std::string block );
		Server( const Server &src );
		Server& operator=( Server &src ); // connst
		~Server( );
};
