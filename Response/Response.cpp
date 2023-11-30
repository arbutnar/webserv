/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 13:25:35 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/30 17:54:05 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response( void )
	: _status (0), _body ("") {
}

Response::Response( const std::string &code )
	: _status (std::atoi(code.c_str())), _body("") {
		std::cout << _status << std::endl;
}

Response::Response( const Response &src ) {
	*this = src;
}

Response &Response::operator=( const Response &src ) {
	if (this == &src)
		return *this;
	_status = src._status;
	_headers = src._headers;
	_body = src._body;
	return *this;
}

Response::~Response( void ) {
	_headers.clear();
}

const int	&Response::getStatus( void ) const {
	return _status;
}

const m_strStr	&Response::getHeaders( void ) const {
	return _headers;
}

const std::string	&Response::getBody( void ) const {
	return _body;
}

void	Response::setStatus( const int &status ) {
	_status = status;
}

void	Response::setHeaders( const m_strStr &headers ) {
	_headers = headers;
}

void	Response::setBody( const std::string &body ) {
	_body = body;
}
