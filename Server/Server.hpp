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
#include "../Location/Location.hpp"

class Server : protected Directives {
	private:
		std::vector<Location>	_locations;
		// std::vector<Socket>		_sockets;
	public:
		Server( void );
		Server( std::string block );
		Server( const Server &src );
		Server& operator=( Server &src ); // connst
		~Server( );
};
