/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:51:25 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/28 16:19:45 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request( void )
	: _isValid (false), _method (""), _uri ("") {
}

Request::Request( const Request &src ) {
	*this = src;
}

Request& Request::operator=( const Request &src ) {
	if (this == &src)
		return *this;
	_isValid = src._isValid;
	_method = src._method;
	_uri = src._uri;
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
	return _uri;
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

void	Request::setUrl( const std::string &uri ) {
	_uri = uri;
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

	pos = buffer.find_first_of(" \t");
	_method = buffer.substr(0, pos);
	for (unsigned int i = 0; i < _method.size(); i++)
		if (!std::isupper(_method[i]))
			throw std::exception();
	pos = buffer.find_first_not_of(" \t", pos);
	_uri = buffer.substr(pos, buffer.find_first_of(" \t", pos) - pos);
	pos = buffer.find_first_not_of(" \t", pos + _uri.size());
	_protocol = buffer.substr(pos, buffer.find_first_of("\r\n", pos) - pos);
	std::transform(_protocol.begin(), _protocol.end(), _protocol.begin(), ::toupper);
	if (_method.empty() || _uri.empty() || _protocol.empty())
		throw std::exception();
	if (_uri != "/" && _uri.at(0) == '/')
		_uri.erase(0, 1);
	std::getline(ss, key);
	while (std::getline(std::getline(ss, key, ':') >> std::ws, value))
		_headers.insert(std::make_pair(key, value.substr(0, value.size() - 1)));
}

const Location	Request::uriMatcher( const s_locs &locations ) {
	s_locs		prefixed;
	s_locs		equalMod;
	s_locs		regex;
	Location	tmp;

	for (s_locs::const_iterator it = locations.begin(); it != locations.end(); it++)
	{
		if (it->getModifier() == 0)
			prefixed.insert(*it);
		else if (it->getModifier() == 1)
			equalMod.insert(*it);
		else
			regex.insert(*it);
	}
	for (s_locs::const_iterator it = equalMod.begin(); it != equalMod.end(); it++)
		if (_uri == it->getLocationName())
			return *it;
	size_t i;
	size_t locSize;
	for (s_locs::const_iterator it = prefixed.begin(); it != prefixed.end(); it++)
	{
		locSize = it->getLocationName().size();
		for (i = 0; i < _uri.size() && i < locSize; i++)
			if (_uri.at(i) != it->getLocationName().at(i))
				break ;
		if ((i == _uri.size() || _uri.at(i) == '/') &&  i == locSize)
			if (locSize > tmp.getLocationName().size())
				tmp = *it;
	}
	for (s_locs::const_iterator it = regex.begin(); it != regex.end(); it++)
	{
		locSize = it->getLocationName().size();
		if (locSize <=  _uri.size() && _uri.substr(_uri.size() - locSize) == it->getLocationName())
			tmp = *it;
	}
	return tmp;
}

const std::string	Request::translateUri( const Location &match ) {
	std::string	translation;

	if (!match.getAlias().empty())
		translation += match.getAlias();
	
	
	return translation;
}

void	Request::displayRequest( void ) const {
	std::cout << "method: " << _method << std::endl;
	std::cout << "uri: " << _uri << std::endl;
	std::cout << "protocol: " << _protocol << std::endl;
	for (m_strStr::const_iterator it = _headers.begin(); it != _headers.end(); it++)
		std::cout << it->first << ": " << it->second << std::endl;
}