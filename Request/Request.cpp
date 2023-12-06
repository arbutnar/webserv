/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:51:25 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/06 15:49:28 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request( void )
	: _method (""), _uri (""), _protocol (""), _body (""), _translate ("") {
}

Request::Request( const Request &src ) {
	*this = src;
}

Request& Request::operator=( const Request &src ) {
	if (this == &src)
		return *this;
	_method = src._method;
	_uri = src._uri;
	_protocol = src._protocol;
	_headers = src._headers;
	_body = src._body;
	_translate = src._translate;
	_match = src._match;
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

const std::string &Request::getProtocol( void ) const {
	return _protocol;
}

const m_strStr	&Request::getHeaders( void ) const {
	return _headers;
}

const std::string &Request::getBody( void ) const {
	return _body;
}

const std::string &Request::getTranslate( void ) const {
	return _translate;
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

void	Request::setProtocol( const std::string &protocol ) {
	_protocol = protocol;
}

void	Request::setHeaders( const m_strStr &headers ) {
	_headers = headers;
}

void	Request::setBody( const std::string &body ) {
	_body = body;
}

void	Request::setTranslate( const std::string &translate ) {
	_translate = translate;
}

void	Request::setMatch( const Location &match ) {
	_match = match;
}

void	Request::headersParser( const std::string &buffer ) {
	std::stringstream	ss(buffer);
	std::string			key;
	std::string			value;
	size_t				pos;

	pos = buffer.find_first_of(" ");
	_method = buffer.substr(0, pos);
	if (_method.empty())
		throw std::runtime_error("400");
	pos += 1;
	_uri = buffer.substr(pos, buffer.find_first_of(" ", pos) - pos);
	if (_uri.empty())
		throw std::runtime_error("400");
	pos += _uri.size() + 1;
	_protocol = buffer.substr(pos, buffer.find("\r\n", pos) - pos);
	if (_protocol.empty())
		throw std::runtime_error("400");
	std::getline(ss, key);
	while (std::getline(std::getline(ss, key, ':') >> std::ws, value))
	{
		if (value.at(value.size() - 1) == '\r')
			value.erase(value.size() - 1, 1);
		if (_headers.find(key) != _headers.end())
			throw std::runtime_error("400");
		_headers.insert(std::make_pair(key, value));
	}
	if (_headers.find("Content-Length") != _headers.end() && _headers.find("Transfer-Encoding") != _headers.end())
		_headers.erase("Content-Length");
	else if (_headers.find("Content-Length") == _headers.end())
		_headers.insert(std::make_pair("Transfer-Encoding", "chuncked"));
	if (_uri != "/" && _uri.at(0) == '/')
		_uri.erase(0, 1);
}

void	Request::headersChecker( void ) const {
	std::string	methods[5] = {"GET", "HEAD", "POST", "PUT", "DELETE"};
	int			i;

	for (i = 0; i < 5; i++)
		if (methods[i] == _method)
			break ;
	if (i == 5)
		throw std::runtime_error("501");
	if (_protocol != "HTTP/1.1" && _protocol != "http/1.1")
		throw std::runtime_error("505");
	for (m_strStr::const_iterator it = _headers.begin(); it != _headers.end(); it++)
	{
		if (it->first.find_first_of(" \t") != std::string::npos)
			throw std::runtime_error("400");
		if (it->first.empty() || it->second.empty())
			throw std::runtime_error("400");
	}
	if (_headers.find("Content-Length") != _headers.end())
		if (_headers.at("Content-Length").find_first_not_of("0123456789") != std::string::npos)
			throw std::runtime_error("400");
	if (_headers.find("Host") == _headers.end() || _headers.at("Host").empty())
		throw std::runtime_error("400");
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
}

void	Request::matchChecker( void ) const {
	unsigned int	cl = 0;

	if (_match.getLimitExcept().at(_method) == false)
		throw std::runtime_error("405");
	if (_headers.find("Content-Length") != _headers.end())
		cl = strtoul(_headers.at("Content-Length").c_str(), NULL, 10);
	if (cl > _match.getClientMaxBodySize())
		throw std::runtime_error("400");
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
	if (_method == "PUT")
	{
		if (stat(_translate.c_str(), &st) == -1 || st.st_mode & S_IFREG)
		{
			std::ofstream	of(_translate.c_str());
			if (!of.is_open())
				throw std::runtime_error("500");
		}
		else
			throw std::runtime_error("409");
	}
	else if (_method == "GET" || _method == "HEAD")
	{
		if (stat(_translate.c_str(), &st) == -1)
			throw std::runtime_error("404");
		_translate = absolutePath + _translate;
		if (st.st_mode & S_IFDIR)
		{
			if (*_translate.rbegin() != '/')
				_translate += "/";
			for (it = _match.getIndex().begin(); it != _match.getIndex().end(); it++)
				if (access((_translate + *it).c_str(), R_OK) == 0)
					break ;
			if (it == _match.getIndex().end())
				throw std::runtime_error("404");		// testare con nginx e capire quando e' FORBIDDEN
			_translate += *it;
		}
		if (access((_translate).c_str(), R_OK) == -1)
			throw std::runtime_error("404");
	}
}

void	Request::bodyParser( const int &socket ) {
	if (_method != "POST" && _method != "PUT")
		return ;
	std::string		size;
	//char			c;
	int 			cl;
	//int			pos;
	int 			nBytes;
	char			*buffer;

	if (_headers.find("Content-Length") != _headers.end())
	{
		cl = std::atoi(_headers.at("Content-Length").c_str());
		buffer = (char*)malloc(sizeof(char) * (cl + 1));
		while (cl != 0)
		{
			nBytes = recv(socket, buffer, cl, 0);
			if (nBytes <= 0)
				throw std::runtime_error("499");
			cl -= nBytes;
			buffer += nBytes;
		}
		*buffer = '\0';
		buffer -= std::atoi(_headers.at("Content-Length").c_str());
	}
	_body += buffer;
	free(buffer);
	std::cout << _body << std::endl;
	else if (_headers.find("Transfer-Encoding") != _headers.end())
	{
		cl = 0;
		while (true)
		{
			while (size.find("\r\n\r\n"))
			{
				nBytes = recv(socket, &c, 1, 0);
				if (nBytes <= 0)
					throw std::runtime_error("499");
				size += c;
			}
			std::stringstream ss;
			ss << std::hex << size.substr(0, size.find("\r\n\r\n"));
			ss >> cl;
			if (cl == 0)
				break ;
			while (cl != 0)
			{
				nBytes = recv(socket, &buffer, cl, 0);
				if (nBytes <= 0)
					throw std::runtime_error("499");
				cl -= nBytes;
			}
			_body += buffer;
		}
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
