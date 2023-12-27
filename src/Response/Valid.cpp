/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Valid.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 11:48:05 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/14 18:54:02 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Valid.hpp"

Valid::Valid( void )
	: Response ("200 Ok") {
}

Valid::Valid( const Request &request )
	:  Response ("200 Ok", request) {
}

Valid::Valid( const Valid &src )
	: Response(src) {
}

Valid	&Valid::operator=( const Valid &src ) {
	if (this == &src)
		return *this;
	Response::operator=(src);
	return *this;
}

Valid::~Valid( ) {
}

const std::fstream	&Valid::getFile( void ) const {
	return _file;
}

void	Valid::cgiOutputParser( std::string &cliBuffer ) {
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
			{
				_status = value;
				continue ;
			}
			_headers.insert(std::make_pair(key, value));
		}
		cliBuffer.erase(0, pos + 2);
	}
	_body = cliBuffer;
}

void	Valid::methodHandler( void ) {
	std::string	methods[5] = {"GET", "HEAD", "POST", "PUT", "DELETE"};
	int	i;
	for (i = 0; i < 5; i++)
		if (methods[i] == _request.getMethod())
			break;
	switch(i) {
		case GET:
		case HEAD:
		case POST:
			getHandler(); break ;
		case PUT:
			putHandler(); break ;
		case DELETE: {
			deleteHandler(); break ;
		}
	}
}

void	Valid::getHandler( void ) {
	_request.finishTranslation();
	if (_request.getMatch().getAutoindex() == true && *_request.getTranslate().rbegin() == '/')
		handleAutoindex();
	else
	{
		_file.open(_request.getTranslate().c_str(), std::fstream::in);
		if (!_file.is_open())
			throw std::runtime_error("500");
		else if (_request.getMethod() == "GET" || _request.getMethod() == "POST")
		{
			std::stringstream ss;
			ss << _file.rdbuf();
			_body = ss.str();
		}
	}
}

void	Valid::putHandler( void ) {
	struct stat st;
	if (stat(_request.getTranslate().c_str(), &st) == 0 && st.st_mode & S_IFDIR)
		throw std::runtime_error("409");
	_status = "201 Created";
	if (stat(_request.getTranslate().c_str(), &st) == 0)
		_status = "204 No Content";
	_file.open(_request.getTranslate().c_str(), std::fstream::out);
	_file << _request.getBody();
}

void	Valid::deleteHandler( void ) {
	struct stat st;
	if (stat(_request.getTranslate().c_str(), &st) == -1)
		throw std::runtime_error("404");
	remove(_request.getTranslate().c_str());
	_status = "204 No Content";
}

void	Valid::handleAutoindex( void ) {
	DIR				*dir;
	struct dirent	*entry;
	std::string		uri = _request.getUri();
	dir = opendir(_request.getTranslate().c_str());
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
