/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Valid.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 11:48:05 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/07 19:12:22 by arbutnar         ###   ########.fr       */
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
		if (!_file.is_open())
			throw std::runtime_error("500");
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

void	Valid::generateBody( void ) {
	if(_request.getMethod() == "GET")
	{
		std::stringstream	ss;

		ss << _file.rdbuf();
		_body = ss.str();
	}
	else if (_request.getMethod() == "PUT")
		_file << _request.getBody();
}
