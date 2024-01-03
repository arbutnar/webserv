/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 13:00:17 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/12 17:10:05 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_H
# define RESPONSE_H

# include <iostream>
# include <sys/socket.h>
# include <fstream>
# include <ctime>
# include <signal.h>
# include <dirent.h>
# include "../Request/Request.hpp"

enum Codes {
	OK = 200,
	CREATED = 201,
	NO_CONTENT = 204,
	MOVED_PERMANENTLY = 301,
	MOVED_TEMPORARLY = 302,
	SEE_OTHER = 303,
	NOT_MODIFIED = 304,
	TEMPORARY_REDIRECT = 307,
	PERMANENT_REDIRECT = 308,
	BAD_REQUEST = 400,
	FORBIDDEN = 403,
	NOT_FOUND = 404,
	METHOD_NOT_ALLOWED = 405,
	CONFLICT = 409,
	REQUEST_ENTITY_TOO_LARGE = 413,
	URI_TOO_LONG = 414,
	UNSUPPORTED_MEDIA_TYPE = 415,
	REQUEST_HEADER_TOO_LARGE = 494,
	CLIENT_CLOSED_REQUEST = 499,
	INTERNAL_SERVER = 500,
	NOT_IMPLEMENTED = 501,
	HTTP_NOT_SUPPORTED = 505,
	INSUFFICIENT_STORAGE = 507
};

class Response : public Request {
	private:
		int				_statusCode;
		std::fstream	_file;
	public:
		Response( void );
		Response( const Response &src );
		Response &operator=( const Response &src );
		virtual ~Response( );

		const int			&getStatusCode( void ) const;
		const std::fstream	&getFile( void ) const;
		void				setStatusCode( const int &statusCode );

		void		methodHandler( void );
		void		handleAutoindex( void );
		void		getHandler( void );
		void		putHandler( void );
		void		deleteHandler( void );
		void		cgiOutputParser( std::string cliBuffer );
		std::string	createCookie( const int len );
		void		generateHeaders( void );
		void		generateErrorPage( void );
		std::string	convertStatus( void ) const;
		bool		send( const int &socket ) const;
};

#endif