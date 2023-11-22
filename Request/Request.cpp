/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:51:25 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/22 18:01:00 by arbutnar         ###   ########.fr       */
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

const std::string 	&Request::getProtocol( void ) const {
	return _protocol;
}

const m_strStr	&Request::getHeaders( void ) const {
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

void	Request::setProtocol( const std::string &protocol ) {
	_protocol = protocol;
}

void	Request::setHeaders( const m_strStr &headers ) {
	_headers = headers;
}

void	Request::parser( const std::string &buffer ) {
	std::stringstream	ss(buffer);
	std::string			key;
	std::string			value;
	size_t				pos;

	std::cout << buffer << std::endl;
	pos = buffer.find_first_of(" \t");
	_method = buffer.substr(0, pos);
	std::transform(_method.begin(), _method.end(), _method.begin(), ::toupper);
	pos = buffer.find_first_not_of(" \t", pos);
	_url = buffer.substr(pos, buffer.find_first_of(" \t", pos) - pos);
	pos = buffer.find_first_not_of(" \t", pos + _url.size());
	_protocol = buffer.substr(pos, buffer.find_first_of("\r\n", pos) - pos);
	std::transform(_protocol.begin(), _protocol.end(), _protocol.begin(), ::toupper);
	if (_method.empty() || _url.empty() || _protocol.empty())
		throw std::exception();
	std::getline(ss, key);
	while (std::getline(std::getline(ss, key, ':') >> std::ws, value))
		_headers.insert(std::make_pair(key, value.substr(0, value.size() - 1)));
}

void	Request::associateUrl( const v_locs &locations ) {
	Location	tmp;
	v_locs		noMod;
	v_locs		equalMod;
	v_locs		caseSensMod;

	for (v_locs::const_iterator it = locations.begin(); it != locations.end(); it++)
	{	
		if (it->getModifier() == 0)
			noMod.push_back(*it);
		else if (it->getModifier() == 1)
			equalMod.push_back(*it);
		else
			caseSensMod.push_back(*it);
	}
	for (v_locs::const_iterator it = noMod.begin(); it != noMod.end(); it++)
		if (_url.find(it->getLocationName()) != std::string::npos)
			tmp = *it;
	for (v_locs::const_iterator it = equalMod.begin(); it != equalMod.end(); it++)
		if (_url == it->getLocationName())
			tmp = *it;
	tmp.displayLocation();
}

void	Request::displayRequest( void ) const {
	std::cout << '$' << _method << '$' << std::endl;
	std::cout << '$' << _url << '$' << std::endl;
	std::cout << '$' << _protocol << '$' << std::endl;
	for (m_strStr::const_iterator it = _headers.begin(); it != _headers.end(); it++)
		std::cout << '$' << it->first << '$' << it->second << '$' << std::endl;
}