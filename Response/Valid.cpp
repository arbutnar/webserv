/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Valid.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 11:48:05 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/09 18:14:30 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Valid.hpp"

Valid::Valid( void )
	: Response ("200 Ok") {
}

Valid::Valid( const Request &request )
	:  Response ("200 Ok"), _request(request) {
		if (_request.getMatch().getLimitExcept().at(_request.getMethod()) == false)
			throw std::runtime_error("405");
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
		else if (_request.getMethod() == "DELETE")
			if ((remove(_request.getTranslate().c_str())) == 0)
				_status = "204 No Content";
}

Valid::Valid( const Valid &src )
	: Response(src) {
}

Valid	&Valid::operator=( const Valid &src ) {
	if (this == &src)
		return *this;
	Response::operator=(src);
	_request = src._request;
	return *this;
}

Valid::~Valid( ) {
}

const Request	&Valid::getRequest( void ) const {
	return _request;
}

void	Valid::setRequest( const Request &request ) {
	_request = request;
}

void	Valid::createEnv( void ) {
	char	*env[8];

	// GATEWAY_INTERFACE
	// PATH_INFO
	// PATH_TRANSLATED	
	env[0] = strdup("SERVER_SOFTWARE=42webserv/1.0");
	env[1] = strdup("SERVER_NAME=127.0.0.1");
	env[2] = strdup("SERVER_PROTOCOL=HTTP/1.1");
	std::stringstream	ss;
	ss <<  _request.getMatch().getListenPort();
	env[3] = strdup((static_cast<std::string>("SERVER_PORT=") + ss.str()).c_str());
	env[4] = strdup((static_cast<std::string>("REQUEST_METHOD=") + _request.getMethod()).c_str());
	env[5] = strdup((static_cast<std::string>("SCRIPT_NAME=") + _request.getMatch().getCgiPass()).c_str());
	env[6] = strdup((static_cast<std::string>("REMOTE_HOST") + _request.getHeaders().at("Host")).c_str());
	ss << _body.size();
	env[7] = strdup((static_cast<std::string>("CONTENT_LENGTH") + ss.str()).c_str());
	env[8] = NULL;
	(void)env;
}

void	Valid::handleCgi( void ) {
	createEnv();
}

void	Valid::generateBody( void ) {
	if(_request.getMethod() == "GET")
	{
		std::stringstream ss;
		ss << _file.rdbuf();
		_body = ss.str();
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
}
