/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 16:38:10 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/20 16:58:29 by arbutnar         ###   ########.fr       */
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
	if (_socket != src._socket || _buffer != src._buffer)
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

void	Client::buildBuffer( const char &c ) {
	_buffer += c;
}

void	Client::clearBuffer( void ) {
	_buffer.clear();
}
