/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 16:38:10 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/04 18:02:07 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( void )
	: _socket (0), _buffer ("") {
}

Client::Client( int const &socket )
	: _socket (socket) {
}

Client::Client( Client const &src ) {
	*this = src;
}

Client	&Client::operator=( Client const &src ) {
	if (this == &src)
		return *this;
	_socket = src._socket;
	_buffer = src._buffer;
	return *this;
}

bool	Client::operator==( Client const &src ) const {
	if (_socket != src._socket)
		return false;
	return true;
}

Client::~Client( void ) {
}

int const	&Client::getSocket( void ) const {
	return _socket;
}

std::string const	&Client::getBuffer( void ) const {
	return _buffer;
}

void	Client::setSocket( int const &socket ) {
	_socket = socket;
}

void	Client::setBuffer( std::string const &buffer ) {
	_buffer = buffer;
}

bool	Client::writeRequest( void ) {
	char	c;
	int		nBytes = recv(_socket, &c, 1, 0);
	if (nBytes <= 0)
		return false;
	_buffer += c;
	return true;
}

void	Client::clearBuffer( void ) {
	_buffer.clear();
}

