/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:24:46 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/08 16:36:58 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server( void ) {
}

Server::Server( std::string block ) {
    std::cout << block << std::endl;
}

Server::Server( const Server &src )
    : Directives(src) {
        this->_locations = src._locations;
}

Server& Server::operator=( const Server &src ) {
    if (this == &src)
        return *this;
    this->_locations = src._locations;
    Directives::operator=(src);
    return *this;
}

Server::~Server() {
}

void    Server::setLocations( const v_locs &locations ) {
    this->_locations = locations;
}

const v_locs&	Server::getLocations( void ) {
    return this->_locations;
}


void    Server::addLocation( Location &location ) {
    this->_locations.push_back(location);
}
