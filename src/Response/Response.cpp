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
	: _statusCode (200) {
}

Response::Response( const Response &src )
	: Request() {
		*this = src;
}

Response &Response::operator=( const Response &src ) {
	if (this == &src)
		return *this;
	_statusCode = src._statusCode;
	Request::operator=(src);
	return *this;
}

Response::~Response( void ) {
}

const int	&Response::getStatusCode( void ) const {
	return _statusCode;
}

const std::fstream	&Response::getFile( void ) const {
	return _file;
}

void	Response::setStatusCode( const int &statusCode ) {
	_statusCode = statusCode;
}

void	Response::methodHandler( void ) {
	std::string	methods[5] = {"GET", "HEAD", "POST", "PUT", "DELETE"};
	int	i;
	for (i = 0; i < 5; i++)
		if (methods[i] == _method)
			break;
	switch(i) {
		case GET:
		case HEAD:
		case POST:
			getHandler(); break ;
		case PUT:
			putHandler(); break ;
		case DELETE:
			deleteHandler(); break ;
		default:
			throw std::runtime_error("500");
	}
}

void	Response::handleAutoindex( void ) {
	DIR				*dir;
	struct dirent	*entry;
	std::string		uri = _uri;
	dir = opendir(_translate.c_str());
	_body = "<!DOCTYPE HTML>\n";
	_body += "<html>\n<head>\n\t<title>" + uri + "</title>\n</head>\n<body>\n";
	_body += "\t<h1>Index of " + uri + "</h1>\n\t<hr>\n";
	if (*uri.rbegin() != '/')
		uri += "/";
	if (*uri.begin() != '/')
		uri = "/" + uri;
	while ((entry = readdir(dir)) != NULL)
	{
		_body += "\t<a href=\"";
		_body += uri + entry->d_name;
		_body += "\">";
		_body += entry->d_name;
		_body += "</a><br>\n";
	}
	_body += "\t<hr>\n</body>\n</html>\n";
	closedir(dir);
}

void	Response::getHandler( void ) {
	finishTranslation();
	if (_match.getAutoindex() == true && *_translate.rbegin() == '/')
		handleAutoindex();
	else
	{
		_file.open(_translate.c_str(), std::fstream::in);
		if (!_file.is_open())
			throw std::runtime_error("500");
		else if (_method != "HEAD")
		{
			std::stringstream ss;
			ss << _file.rdbuf();
			_body = ss.str();
		}
	}
}

void	Response::putHandler( void ) {
	struct stat st;
	if (stat(_translate.c_str(), &st) == 0 && st.st_mode & S_IFDIR)
		throw std::runtime_error("409");
	_statusCode = 201;
	if (stat(_translate.c_str(), &st) == 0)
		_statusCode = 204;
	_file.open(_translate.c_str(), std::fstream::out);
	_file << _body;
}

void	Response::deleteHandler( void ) {
	struct stat st;
	if (stat(_translate.c_str(), &st) == -1)
		throw std::runtime_error("404");
	remove(_translate.c_str());
	_statusCode = 204;
}

void	Response::cgiOutputParser( std::string cliBuffer ) {
	size_t		pos;

	for (pos = cliBuffer.find("\r"); pos != std::string::npos; pos = cliBuffer.find("\r"))
		cliBuffer.erase(pos, 1);
	if ((pos = cliBuffer.find("\n\n")) != std::string::npos)
	{
		std::stringstream	ss(cliBuffer.substr(0, pos));
		std::string			key;
		std::string			value;
		while (std::getline(std::getline(ss, key, ':') >> std::ws, value))
		{
			if (value.at(value.size() - 1) == '\r')
				value.erase(value.size() - 1, 1);
			if (_headers.find(key) != _headers.end() || (key.empty() || value.empty()))
				throw std::runtime_error("500");
			if (key.find_first_of(" \t") != std::string::npos)
				throw std::runtime_error("500");
			if (key == "Status")
				_statusCode = std::atoi(value.c_str());
			else
				_headers.insert(std::make_pair(key, value));
		}
		cliBuffer.erase(0, pos + 2);
	}
	_body = cliBuffer;
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
	std::string	connection;
	std::string	cookie;

	if (_headers.find("Cookie") != _headers.end())
		cookie = _headers.at("Cookie");
	if (_headers.find("Connection") == _headers.end())
		connection = "keep-alive";
	else
		connection = _headers.at("Connection");
	_headers.clear();
	if (_statusCode > 300 && _statusCode < 309)
		_headers.insert(std::make_pair("Location", _match.getReturn().second));
	if (!cookie.empty())
	{
		_headers.insert(std::make_pair("Cookie", cookie));
		if (_body.find("<h2>No Cookie!</h2>") != std::string::npos)
			_body.replace(_body.find("<h2>No Cookie!</h2>"), 19, "<img src=\"https://www.freepnglogos.com/uploads/cookie-png/cookie-cliparts-transparent-download-clip-art-22.png\" alt=\"cookie\" width=\"200\" height=\"200\" />");
	}
	else
		_headers.insert(std::make_pair("Set-Cookie", createCookie(5)));
	_headers.insert(std::make_pair("Connection", connection));
	_headers.insert(std::make_pair("Server", "42webserv"));
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S", &tstruct);
	_headers.insert(std::make_pair("Date", buf));
	_headers.insert(std::make_pair("Content-Type", "text/html"));
	_headers.insert(std::make_pair("Content-Length", toString(_body.length())));
}

void	Response::generateErrorPage( void ) {
	m_intStr::const_iterator it = _match.getErrorPage().find(_statusCode);
	if (it != _match.getErrorPage().end())
	{
		std::ifstream	error_page(it->second.c_str());
		if (error_page.is_open())
		{
			std::stringstream	buffer;
			buffer << error_page.rdbuf();
			_body = buffer.str();
			return ;
		}
		_statusCode = 403;
	}
	_body = "<!DOCTYPE HTML>\n";
	_body += "<html>\n<head>\n\t<title>" + convertStatus() + "</title>\n</head>\n<body>\n\t";
	if ((_statusCode < 301 || _statusCode > 308) && _match.getReturn().first != -1)
		_body += _match.getReturn().second;
	else
		_body += "<h1>" + convertStatus() + "</h1>";
	_body += "\n</body>\n</html>\n";
}

std::string	Response::convertStatus( void ) const {
	switch(_statusCode) {
		case OK:
			return "200 Ok";
		case CREATED:
			return "201 Created";
		case NO_CONTENT:
			return "204 No Content";
		case MOVED_PERMANENTLY:
			return "301 Moved Permanently";
		case MOVED_TEMPORARLY:
			return "302 Moved temporarily";
		case SEE_OTHER:
			return "303 See Other";
		case NOT_MODIFIED:
			return "304 Not Modified";
		case TEMPORARY_REDIRECT:
			return "307 Temporary Redirect";
		case PERMANENT_REDIRECT:
			return "308 Permanent Redirect";
		case BAD_REQUEST:
			return "400 Bad Request";
		case FORBIDDEN:
			return "403 Forbidden";
		case NOT_FOUND:
			return "404 Not Found";
		case METHOD_NOT_ALLOWED:
			return "405 Method Not Allowed";
		case CONFLICT:
			return "409 Conflict";
		case REQUEST_ENTITY_TOO_LARGE:
			return "413 Request Entity Too Large";
		case URI_TOO_LONG:
			return "414 URI Too Long";
		case UNSUPPORTED_MEDIA_TYPE:
			return "415 Unsupported Media Type";
		case REQUEST_HEADER_TOO_LARGE:
			return "494 Request header too large";
		case CLIENT_CLOSED_REQUEST:
			return "499 Request Closed Request";
		case INTERNAL_SERVER:
			return "500 Internal Server Error";
		case NOT_IMPLEMENTED:
			return "501 Not Implemented";
		case HTTP_NOT_SUPPORTED:
			return "505 HTTP Not Supported";
		case INSUFFICIENT_STORAGE:
			return "507 Insufficient Storage";
		default:
			return "500 Internal Server Error";
	}
}

bool	Response::send( const int &socket ) const {
	std::string	response;

	response = "HTTP/1.1 " + convertStatus() + "\r\n";
	for (m_strStr::const_iterator it = _headers.begin(); it != _headers.end(); it++)
		response += it->first + ": " + it->second + "\r\n";
	response += "\r\n" + _body;
	std::cout << response << std::endl;
	if (write(socket, response.c_str(), response.length()) <= 0)
		return true;
	return false;
}
