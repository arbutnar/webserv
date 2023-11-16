/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:24:46 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/16 15:35:21 by arbutnar         ###   ########.fr       */
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

void	Server::setSockets( const fd_set &sockets ) {
	this->_sockets = sockets;
}

void	Server::setListener( const int&listener ) {
	this->_listener = listener;
}

const v_locs&	Server::getLocations( void ) const {
    return this->_locations;
}

const fd_set&	Server::getSockets( void ) const {
	return this->_sockets;
}

const int &	Server::getListener( void ) const {
	return this->_listener;
}

void    Server::addLocation( const Location &location ) {
    this->_locations.push_back(location);
}

void	Server::socketInit( void ) {
	this->_listener = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_listener == -1)
		throw std::runtime_error("Cannot create socket");
	struct sockaddr_in	sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(this->_listen_port);
	sockaddr.sin_addr.s_addr = htonl(this->_listen_host);
	if (bind(this->_listener, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) == -1)
		throw std::runtime_error("Cannot bind socket");
	if (listen(this->_listener, 128) == -1)
		throw std::runtime_error("Socket cannot Listen");
	FD_ZERO(&this->_sockets);
	FD_SET(this->_listener, &this->_sockets);
}

int	Server::nfds( void ) {
	int max = this->_sockets.fd_array[0];

	for (int i = 0; i != this->_sockets.fd_count; i++)
	{
		if (max < this->_sockets.fd_array[i])
			max = this->_sockets.fd_array[i];
	}
	return max;
}

void	Server::addSocket( const int&socket ) {
	FD_SET(socket, &this->_sockets);
}

void	Server::displayServer( void ) const {
	std::cout << "Server {" << std::endl;
	this->displayDirectives();
	for (v_locs::const_iterator it = this->_locations.begin(); it != this->_locations.end(); it++)
		it->displayLocation();
	std::cout << "}" << std::endl;
}
