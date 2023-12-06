/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 13:25:35 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/04 14:23:52 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response( void )
	: _status(""), _body("") {
}

Response::Response(  const std::string &status )
	: _status(status), _body("") {
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

const std::string	&Response::getStatus( void ) const {
	return _status;
}

const m_strStr	&Response::getHeaders( void ) const {
	return _headers;
}

const std::string	&Response::getBody( void ) const {
	return _body;
}

void	Response::setStatus( const std::string &status ) {
	_status = status;
}

void	Response::setHeaders( const m_strStr &headers ) {
	_headers = headers;
}

void	Response::setBody( const std::string &body ) {
	_body = body;
}

void	Response::addHeader( const p_strStr &header ) {
	_headers.insert(header);
}

void	Response::generateHeaders( void ) {
	std::time_t	now = time(0);
	struct tm	tstruct;
	char		buf[30];

	_headers.insert(std::make_pair("Server", "42webserv"));
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S", &tstruct);
	_headers.insert(std::make_pair("Date", buf));
	if (!_body.empty())
	{
		_headers.insert(std::make_pair("Content-Type", "text/html"));
		std::stringstream ss;
		ss << _body.length();
		_headers.insert(std::make_pair("Content-Length", ss.str()));
	}
}

void	Response::send( const int &socket ) const {
	std::string	response("HTTP/1.1 ");

	response += _status + "\r\n";
	for (m_strStr::const_iterator it = _headers.begin(); it != _headers.end(); it++)
		response += it->first + ": " + it->second + "\r\n";
	response += "\r\n" + _body;
	::send(socket, response.c_str(), response.length(), 0);
	std::cout << response << std::endl;
}
