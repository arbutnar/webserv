/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 13:00:17 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/07 16:38:52 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_H
# define RESPONSE_H

# include <iostream>
# include <sys/socket.h>
# include <fstream>
# include <ctime>
# include "../Request/Request.hpp"

enum Codes {
	PERMANENT_REDIRECT = 308,
	BAD_REQUEST = 400,
	FORBIDDEN = 403,
	NOT_FOUND = 404,
	METHOD_NOT_ALLOWED = 405,
	CONFLICT = 409,
	URI_TOO_LONG = 414,
	UNSUPPORTED_MEDIA_TYPE = 415,
	REQUEST_HEADER_TOO_LARGE = 494,
	CLIENT_CLOSED_REQUEST = 499,
	INTERNAL_SERVER = 500,
	NOT_IMPLEMENTED = 501,
	HTTP_NOT_SUPPORTED = 505,
	INSUFFICIENT_STORAGE = 507
};

class Response {
	protected:
		std::string	_status;
		m_strStr	_headers;
		std::string	_body;
	public:
		Response( void );
		Response( const std::string &status );
		Response( const Response &src );
		Response &operator=( const Response &src );
		virtual ~Response( );

		const std::string	&getStatus( void ) const;
		const m_strStr		&getHeaders( void ) const;
		const std::string	&getBody( void ) const;
		void				setStatus( const std::string &status );
		void				setHeaders( const m_strStr &headers );
		void				setBody( const std::string &body);

		void			generateHeaders( const Request &request );
		virtual void	generateBody( void ) = 0;
		void			send( const int &socket ) const;
};

#endif