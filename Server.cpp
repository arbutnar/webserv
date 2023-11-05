/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:24:46 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/05 17:47:05 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server( void )
    : _main ("None") {
}

Server::Server( const Server &src ) {
    (void)src;
}

Server& Server::operator=( Server &src ) { //const
    return src;
}

Server::~Server() {
}

void    Server::setLocation( std::string block ) {
    (void)block;
}

std::string Server::getMainDirectives( void ) {
    return (_main);
    
}

std::ostream &operator<<(std::ostream &out, Server &server ) {
	out << server.getMainDirectives();
	return (out);
}