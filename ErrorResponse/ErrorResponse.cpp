/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorResponse.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 17:46:34 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/30 18:35:12 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorResponse.hpp"

ErrorResponse::ErrorResponse( void ) {
}

ErrorResponse::ErrorResponse( const std::string &code )
	: Response(code) {
		// generateBody();
}

ErrorResponse::ErrorResponse( const ErrorResponse &src )
	: Response(src) {
		_status = src._status;
		_headers = src._headers;
		_body = src._body;
}

ErrorResponse	&ErrorResponse::operator=( const ErrorResponse &src ) {
	if (this == &src)
		return *this;
	Response::operator=(src);
	return *this;
}

ErrorResponse::~ErrorResponse( ) {
}

// void	ErrorResponse::generateBody( void ) {
// 	int	arr[] = { 200, 201, 204, 308, 400, 403, 404, 405, 411, 505 };
// 	int	i;

// 	for (i = 0; i < 10; i++)
// 		if (_status == arr[i])
// 			break;
// 	switch(i)
// 	{
// 		case OK:
// 			break;
// 		case CREATED:
// 			break;
// 		case NO_CONTENT:
// 			break;
// 		case PERMANENT_REDIRECT:
// 			break;
// 		case BAD_REQUEST:
// 			break;
// 		case FORBIDDEN:
// 			break;
// 		case NOT_FOUND:
// 			break;
// 		case METHOD_NOT_ALLOWED:
// 			break;
// 		case LENGHT_REQUIRED:
// 			break;
// 		case HTTP_NOT_SUPPORTED:
// 			break;
// 		default:
// 			break;
// 	}
	
// }

void	ErrorResponse::sendResponse( void ) const {

}
