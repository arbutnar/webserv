/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Valid.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 11:48:05 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/03 14:18:15 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Valid.hpp"

Valid::Valid( void )
	: Response ("200 Ok") {
}

Valid::Valid( const Request &request )
	:  Response ("200 Ok"), _request(request) {
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
		std::ifstream		is(_request.getTranslate().c_str());
		std::stringstream	ss;

		ss << is.rdbuf();
		_body = ss.str();
	}
	else if (_request.getMethod() == "PUT")
	{
		struct stat		st;
		std::ofstream	of;

		_status = "204 No Content";
		exit(0);
		if (stat(_request.getTranslate().c_str(), &st) == -1)
			_status = "201 Created";
		of.open(_request.getTranslate().c_str());
		of << _request.getBody();
	}
}
