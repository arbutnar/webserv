/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:51:25 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/02 17:18:48 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request( void )
	: _method (""), _uri ("") {
		_headers.insert(std::make_pair("Connection", "keep-alive"));
}

Request::Request( const Request &src ) {
	*this = src;
}

Request& Request::operator=( const Request &src ) {
	if (this == &src)
		return *this;
	_method = src._method;
	_uri = src._uri;
	_headers = src._headers;
	return *this;
}

Request::~Request( ) {
}

const std::string &Request::getMethod( void ) const {
	return _method;
}

const std::string &Request::getUri( void ) const {
	return _uri;
}

const std::string &Request::getTranslate( void ) const {
	return _translate;
}

const m_strStr	&Request::getHeaders( void ) const {
	return _headers;
}

const Location	&Request::getMatch( void ) const {
	return _match;
}

void	Request::setMethod( const std::string &method ) {
	_method = method;
}

void	Request::setUri( const std::string &uri ) {
	_uri = uri;
}

void	Request::setTranslate( const std::string &translate ) {
	_translate = translate;
}

void	Request::setHeaders( const m_strStr &headers ) {
	_headers = headers;
}

void	Request::setMatch( const Location &match ) {
	_match = match;
}

void	Request::parser( const std::string &buffer ) {
	std::stringstream	ss(buffer);
	std::string			key;
	std::string			value;
	size_t				pos;
	std::string			methods[5] = {"GET", "HEAD", "POST", "PUT", "DELETE"};

	pos = buffer.find_first_of(" \t");
	_method = buffer.substr(0, pos);
	int i;
	for (i = 0; i < 5; i++)
		if (methods[i] == _method)
			break ;
	if (i == 5)
		throw std::runtime_error("400");
	pos = buffer.find_first_not_of(" \t", pos);
	_uri = buffer.substr(pos, buffer.find_first_of(" \t", pos) - pos);
	if (_uri.empty())
		throw std::runtime_error("400");
	pos = buffer.find_first_not_of(" \t", pos + _uri.size());
	std::string	protocol(buffer.substr(pos, buffer.find_first_of("\r\n", pos) - pos));
	if (protocol.empty())
		throw std::runtime_error("400");
	if (protocol != "HTTP/1.1" && protocol != "http/1.1")
		throw std::runtime_error("505");
	bool	isHost = false;
	std::getline(ss, key);
	while (std::getline(std::getline(ss, key, ':') >> std::ws, value))
	{
		_headers.insert(std::make_pair(key, value.substr(0, value.size() - 1)));
		if (key == "Host")
			isHost = true;
	}
	if (!isHost)
		throw std::runtime_error("400");
	if (_uri != "/" && _uri.at(0) == '/')
		_uri.erase(0, 1);
}

void	Request::uriMatcher( const s_locs &locations ) {
	bool		isMatch = false;

	for (s_locs::const_iterator it = locations.begin(); it != locations.end(); it++)
	{
		isMatch = false;
		if ((*it->getLocationName().rbegin() != '/' || it->getModifier() == 1) && _uri == it->getLocationName())
			isMatch = true;
		else if (*it->getLocationName().rbegin() == '/' && _uri.find(it->getLocationName()) == 0)
			isMatch = true;
		if (isMatch && _match.getLocationName().size() < it->getLocationName().size())
			_match = *it;
	}
	if (_match.getLocationName().empty())
		for (s_locs::const_iterator it = locations.begin(); it != locations.end(); it++)
			if (it->getLocationName() == "/")
				_match = *it;
	if (_match.getLimitExcept().at(_method) == false)
		throw std::runtime_error("405");
}

void	Request::translateUri( void ) {
	v_str::const_iterator	it;
	struct stat 			st;

	if (_uri == "/")
		_uri.clear();
	if (_match.getAlias().empty())
		_translate = _match.getRoot() + _uri;
	else if (_match.getRoot().empty())
	{
		if (_uri.find(_match.getLocationName()) != std::string::npos)
			_translate = _match.getAlias() + _uri.substr(_match.getLocationName().size(), std::string::npos);
		else
			_translate = _match.getAlias();
	}
	if (stat(_translate.c_str(), &st) == -1)
		throw std::runtime_error("404");		// testare con nginx e capire quando e' FORBIDDEN
	if (st.st_mode & S_IFDIR)
	{
		_translate = absolutePath + _translate;
		if (*_translate.rbegin() != '/')
			_translate += "/";
		for (it = _match.getIndex().begin(); it != _match.getIndex().end(); it++)
		{
			if (_match.getLimitExcept().at("GET") == true)
				if (access((_translate + *it).c_str(), R_OK) == 0)
					break ;	
		}
		if (it == _match.getIndex().end())
			throw std::runtime_error("404");		// testare con nginx e capire quando e' FORBIDDEN
		_translate += *it;
	}
}

void	Request::displayRequest( void ) const {
	std::cout << "method: " << _method << std::endl;
	std::cout << "uri: " << _uri << std::endl;
	std::cout << "match Loocation name: " << _match.getLocationName() << std::endl;
	std::cout << "uri translated: " << _translate << std::endl;
	for (m_strStr::const_iterator it = _headers.begin(); it != _headers.end(); it++)
		std::cout << it->first << ": " << it->second << std::endl;
}
