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
		if (_request.getMethod() == "GET" || _request.getMethod() == "HEAD")
			_file.open(request.getTranslate().c_str(), std::fstream::in);
		if (_request.getMethod() == "PUT" || _request.getMethod() == "POST")
		{
			struct stat st;
			if (stat(request.getTranslate().c_str(), &st) == 0 && st.st_mode & S_IFDIR)
				throw std::runtime_error("409");
			_status = "201 Created";
			if (stat(request.getTranslate().c_str(), &st) == 0)
				_status = "204 No Content";
			_file.open(request.getTranslate().c_str(), std::fstream::out);
		}
		if (!_file.is_open() && _request.getMethod() != "DELETE")
			throw std::runtime_error("500");
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

void	Valid::generateBody( void ) {
	if(_request.getMethod() == "GET")
	{
		if (_request.getMatch().getAutoindex() == true && *_request.getTranslate().rbegin() == '/')
			handleAutoindex();
		else
		{
			std::stringstream ss;
			ss << _file.rdbuf();
			_body = ss.str();
		}
	}
	else if (_request.getMethod() == "PUT" || _request.getMethod() == "POST")
	{
		_body = _request.getBody();
		if (!_request.getMatch().getCgiPass().empty())
			handleCgi();
		_file << _body;
		if (_request.getMethod() == "PUT")
			_body.clear();
	}
	else if (_request.getMethod() == "DELETE")
	{
		remove(_request.getTranslate().c_str());
		_status = "204 No Content";
	}
}
