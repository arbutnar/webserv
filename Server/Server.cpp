/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:24:46 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/17 13:05:01 by arbutnar         ###   ########.fr       */
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
		this->_sockets = src._sockets;
}

Server& Server::operator=( const Server &src ) {
    if (this == &src)
        return *this;
    this->_locations = src._locations;
	this->_sockets = src._sockets;
    Directives::operator=(src);
    return *this;
}

Server::~Server() {
}

void    Server::setLocations( const v_locs &locations ) {
    this->_locations = locations;
}

void	Server::setSockets( const s_sock &sockets ) {
	this->_sockets = sockets;
}

const v_locs&	Server::getLocations( void ) const {
    return this->_locations;
}

s_sock	Server::getSockets( void ) const {
	return this->_sockets;
}

void    Server::addLocation( const Location &location ) {
    this->_locations.push_back(location);
}

void	Server::socketInit( void ) {
	int listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener == -1)
		throw std::runtime_error("Cannot create socket");
	struct sockaddr_in	sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(this->_listen_port);
	sockaddr.sin_addr.s_addr = htonl(this->_listen_host);
	if (bind(listener, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) == -1)
		throw std::runtime_error("Cannot bind socket");
	if (listen(listener, 128) == -1)
		throw std::runtime_error("Socket cannot Listen");
	this->_sockets.insert(listener);
}

void	Server::newConnection( void ) {
	struct sockaddr_in sockaddr;
	socklen_t	len = sizeof(sockaddr);
	int client = accept(*this->_sockets.begin(), (struct sockaddr *)&sockaddr, &len);
	if (client == -1)
		throw std::runtime_error("Cannot create Client socket");
	this->_sockets.insert(client);
	std::cout << "ciao " << client << std::endl;
}

void	Server::readRequest( s_sock::iterator &socket, fd_set* const activeSockets ) {
	char	c;
	int	nBytes = recv(*socket, &c, 1, 0);
	if (nBytes == -1)
		throw std::runtime_error("Cannot read from socket");
	else if (nBytes == 0)
	{
		close(*socket);
		FD_CLR(*socket, activeSockets);
		this->_sockets.erase(socket);
		std::cout << *socket << " disconnected" << std::endl;
	}
	else
	{
		std::string	buffer;
		buffer += c;
		while (buffer.find("\r\n\r\n") != std::string::npos)
		{
			nBytes = recv(*socket, &c, 1, 0);
			buffer += c;
		}
		std::cout << buffer << std::endl;
	}
}

void	Server::displayServer( void ) const {
	std::cout << "Server {" << std::endl;
	this->displayDirectives();
	for (v_locs::const_iterator it = this->_locations.begin(); it != this->_locations.end(); it++)
		it->displayLocation();
	std::cout << "}" << std::endl;
}
