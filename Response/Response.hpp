/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 13:00:17 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/30 16:52:12 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_H
# define RESPONSE_H

# include "../Location/Location.hpp"

enum Success {
	OK = 200,
	CREATED = 201,
	NO_CONTENT = 204,
	PERMANENT_REDIRECT = 308,
	BAD_REQUEST = 400,
	FORBIDDEN = 403,
	NOT_FOUND = 404,
	METHOD_NOT_ALLOWED = 405,
	LENGHT_REQUIRED = 411,
	HTTP_NOT_SUPPORTED = 505
};

class Response {
	protected:
		int			_status;
		m_strStr	_headers;
		std::string	_body;
	public:
		Response( void );
		Response( const std::string &code );
		Response( const Response &src );
		Response &operator=( const Response &src );
		virtual ~Response( );

		const int			&getStatus( void ) const;
		const m_strStr		&getHeaders( void ) const;
		const std::string	&getBody( void ) const;
		void				setStatus( const int &status );
		void				setHeaders( const m_strStr &headers );
		void				setBody( const std::string &body);
		
		virtual	void	sendResponse( void ) const = 0;
};

#endif