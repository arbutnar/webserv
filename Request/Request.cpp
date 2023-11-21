/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:51:25 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/21 18:17:35 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request( void )
	: _isValid (false), _method (""), _url ("") {
}

Request::Request( const Request &src ) {
	*this = src;
}

Request& Request::operator=( const Request &src ) {
	if (this == &src)
		return *this;
	_isValid = src._isValid;
	_method = src._method;
	_url = src._url;
	_headers = src._headers;
	return *this;
}

Request::~Request( ) {
}

const bool &Request::getIsValid( void ) const {
	return _isValid;
}

const std::string &Request::getMethod( void ) const {
	return _method;
}

const std::string &Request::getUrl( void ) const {
	return _url;
}

const m_hdrs	&Request::getHeaders( void ) const {
	return _headers;
}

void	Request::setIsValid( const bool &isValid ) {
	_isValid = isValid;
}

void	Request::setMethod( const std::string &method ) {
	_method = method;
}

void	Request::setUrl( const std::string &url ) {
	_url = url;
}

void	Request::setHeaders( const m_hdrs &headers ) {
	_headers = headers;
}

void	Request::parser( const std::string &buffer ) {
	std::string			key;
	std::string			value;
	size_t				pos;

	std::cout << buffer << std::endl;
	pos = buffer.find_first_of(" \t");
	_method = buffer.substr(0, pos);
	pos = buffer.find_first_not_of(" \t", pos);
	_url = buffer.substr(pos, buffer.find_first_of(" \t", pos) - pos);
	std::stringstream	ss(buffer);

	std::getline(ss, key);
	while (std::getline(ss, key, ':'))
	{
		std::getline(ss, value);
		value.erase(0, 1);
		if (key.empty() || value.empty() || key == "\r\n")
			break;
		_headers.insert(std::make_pair(key, value));
	}
	std::cout << '$' << _method << '$' << std::endl;
	std::cout << '$' << _url << '$' << std::endl;
	for (m_hdrs::iterator it = _headers.begin(); it != _headers.end(); it++)
		std::cout << '$' << it->first << '$' << it->second << '$' << std::endl;
}