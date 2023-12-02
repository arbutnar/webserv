/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 13:00:17 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/02 17:44:41 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_H
# define RESPONSE_H

# include <iostream>
# include <sys/socket.h>
# include <fstream>
# include <ctime>
# include "../Request/Request.hpp"

enum Success {
	OK = 200,
	CREATED = 201,
	NO_CONTENT = 204,
	PERMANENT_REDIRECT = 308,
	BAD_REQUEST = 400,
	FORBIDDEN = 403,
	NOT_FOUND = 404,
	METHOD_NOT_ALLOWED = 405,
	LENGTH_REQUIRED = 411,
	INTERNAL_SERVER = 500,
	NOT_IMPLEMENTED = 501,
	HTTP_NOT_SUPPORTED = 505
};

class Response {
	protected:
		std::string	_status;
		m_strStr	_headers;
		std::string	_body;
	public:
		Response( void );
		Response( const Request &request );
		Response( const std::string &code );
		Response( const Response &src );
		Response &operator=( const Response &src );
		virtual ~Response( );

		const std::string	&getStatus( void ) const;
		const m_strStr		&getHeaders( void ) const;
		const std::string	&getBody( void ) const;
		void				setStatus( const std::string &status );
		void				setHeaders( const m_strStr &headers );
		void				setBody( const std::string &body);

		void			addHeader( const p_strStr &header );
		void			generateHeaders( void );
		virtual void	generateBody( void ) = 0;
		void			send( const int &socket ) const;
		// virtual void	generateContent( const bool &isConnected ) = 0;
};

#endif