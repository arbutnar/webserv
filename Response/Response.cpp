/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 13:25:35 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/13 21:54:09 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response( void )
	: _status(""), _body("") {
}

Response::Response( const std::string &status )
	: _status(status), _body("") {
}

Response::Response( const std::string &status, const Request &request )
	: _status(status), _body(""), _request (request) {
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
	_request = src._request;
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

const Request	&Response::getRequest( void ) const {
	return _request;
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

void	Response::setRequest( const Request &request ) {
	_request = request;
}

std::string	Response::createCookie( const int len ) {
	std::string	session = "session_id=";
	std::string	token;
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	token.reserve(len);					// requests that the string should have at least the capacity of len. If the len > max size, an exception is thown. 
	srand(time(NULL));					// it's arg is a seed. Based on the seed it generates random patterns. The value of the seed triggers the pseudo-random number generator algorithm.
	for (int i = 0; i < len; ++i)
		token += alphanum[rand() % (sizeof(alphanum) - 1)];
	return session + token;
}

void	Response::generateHeaders( void ) {
	std::time_t	now = time(0);
	struct tm	tstruct;
	char		buf[30];

	_headers.insert(std::make_pair("Server", "42webserv"));
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S", &tstruct);
	_headers.insert(std::make_pair("Date", buf));
	if (_request.getHeaders().find("Connection") != _request.getHeaders().end())
		_headers.insert(*_request.getHeaders().find("Connection"));
	else
		_headers.insert(std::make_pair("Connection", "keep-alive"));
	int	code = atoi(_status.c_str());
	if (code > 300 && code < 309)
		_headers.insert(std::make_pair("Location", _request.getMatch().getReturn().second));
	if (_request.getHeaders().find("Cookie") == _request.getHeaders().end())
		_headers.insert(std::make_pair("Set-Cookie", createCookie(5)));
	else
	{
		_headers.insert(std::make_pair("Cookie", _request.getHeaders().at("Cookie")));
		if (_body.find("<h2>No Cookie!</h2>") != std::string::npos)
			_body.replace(_body.find("<h2>No Cookie!</h2>"), 19, "<img src=\"https://www.freepnglogos.com/uploads/cookie-png/cookie-cliparts-transparent-download-clip-art-22.png\" alt=\"cookie\" width=\"200\" height=\"200\" />");
	}
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
