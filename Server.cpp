/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:24:46 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/03 18:42:16 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server( void )
    : locations () {
}

void    Server::setLocationBlock( std::string block ) {
    (void)block;
}

std::string Server::getMainDirectives( void ) {
    return (main);
    
}

std::ostream &operator<<(std::ostream &out, Server &server ) {
	out << server.getMainDirectives();
	return (out);
}