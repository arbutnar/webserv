/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:23:11 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/05 18:33:42 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Master.hpp"
#include "Location.hpp"

class Server {
	friend void Master::configParse( void );
	private:
		unsigned int			listen;
		std::string				root;
		const char*				index;
		const char*				error_page;
		unsigned int			client_max_body_size;
		std::vector<Location>	_vecLocs;
	public:
		Server( void );
		Server( const Server &src );
		Server& operator=( Server &src ); // connst
		~Server( );
		
		void    	setLocation( std::string block );
		std::string	getMainDirectives( void );
};

std::ostream &operator<<(std::ostream &out, Server &server );
