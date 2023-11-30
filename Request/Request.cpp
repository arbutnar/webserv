/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:51:25 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/30 16:56:42 by arbutnar         ###   ########.fr       */
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
			throw std::runtime_error("405");
	pos = buffer.find_first_not_of(" \t", pos);
	_uri = buffer.substr(pos, buffer.find_first_of(" \t", pos) - pos);
	pos = buffer.find_first_not_of(" \t", pos + _uri.size());
	_protocol = buffer.substr(pos, buffer.find_first_of("\r\n", pos) - pos);
	std::getline(ss, key);
	while (std::getline(std::getline(ss, key, ':') >> std::ws, value))
		_headers.insert(std::make_pair(key, value.substr(0, value.size() - 1)));
	if (_uri != "/" && _uri.at(0) == '/')
		_uri.erase(0, 1);
	if (_protocol != "HTTP/1.1" && _protocol != "http/1.1")
		throw std::runtime_error("505");
	if (_method.empty() || _uri.empty() || _protocol.empty())
		throw std::runtime_error("400");
}

const Location	Request::uriMatcher( const s_locs &locations ) {
	Location 	match;
	bool		isMatch;
	
	for (s_locs::const_iterator it = locations.begin(); it != locations.end(); it++)
	{
		isMatch = false;
		if ((*it->getLocationName().rbegin() != '/' || it->getModifier() == 1) && _uri == it->getLocationName())
			isMatch = true;
		else if (*it->getLocationName().rbegin() == '/' && _uri.find(it->getLocationName()) == 0)
			isMatch = true;
		if (isMatch && match.getLocationName().size() < it->getLocationName().size())
			match = *it;
	}
	return match;
}

const std::string	Request::translateUri( const Location &match ) {
	std::string				translation;
	v_str::const_iterator	it;
	struct stat 			st;

	if (match.getAlias().empty())
		translation = match.getRoot() + _uri;
	else if (match.getRoot().empty())
	{
		if (_uri.find(match.getLocationName()) != std::string::npos)
			translation = match.getAlias() + _uri.substr(match.getLocationName().size(), std::string::npos);
		else
			translation = match.getAlias();
	}
	std::cout << translation << std::endl;
	if (stat(translation.c_str(), &st) == -1)
		throw std::runtime_error("404");		// testare con nginx e capire quando e' FORBIDDEN
	if (st.st_mode & S_IFDIR)
	{
		translation = absolutePath + translation;
		if (*translation.rbegin() != '/')
			translation += "/";
		for (it = match.getIndex().begin(); it != match.getIndex().end(); it++)
		{
			if (match.getLimitExcept().at("GET") == true || match.getLimitExcept().at("GET") == true)
				if (access((translation + *it).c_str(), R_OK) == 0)
					break ;	
		}
		if (it == match.getIndex().end())
			throw std::runtime_error("404");		// testare con nginx e capire quando e' FORBIDDEN
		translation += *it;
	}
	return translation;
}

void	Request::displayRequest( void ) const {
	std::cout << "method: " << _method << std::endl;
	std::cout << "uri: " << _uri << std::endl;
	std::cout << "protocol: " << _protocol << std::endl;
	for (m_strStr::const_iterator it = _headers.begin(); it != _headers.end(); it++)
		std::cout << it->first << ": " << it->second << std::endl;
}
