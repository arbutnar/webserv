/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:23:11 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/07 13:59:00 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Directive.hpp"
#include "Location.hpp"

class Server : protected Directive {
	friend void Master::configParse( void );

	private:
		std::string				_main;
		std::string				_server_name;
		std::vector<Location>	_locations;
	public:
		Server( void );
		Server( const Server &src );
		Server& operator=( Server &src ); // connst
		~Server( );
		
		void    	setLocation( std::string block );
		std::string	getMainDirectives( void );
};

std::ostream &operator<<(std::ostream &out, Server &server );
