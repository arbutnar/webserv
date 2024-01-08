/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 16:38:05 by arbutnar          #+#    #+#             */
/*   Updated: 2024/01/07 19:03:21 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <sys/socket.h>
# include <unistd.h>
# include <sys/socket.h>
# include <fstream>
# include "../Location/Location.hpp"

typedef std::set<Location> s_locs;

enum methods {
	GET,
	HEAD,
	POST,
	PUT,
	DELETE
};

class Request {
	protected:
		std::string	_method;
		m_strStr	_headers;
		std::string	_body;
		std::string	_uri;
		std::string	_translate;
		Location	_match;
	public:
		Request( void );
		Request( Request const &src );
		Request	&operator=( Request const &src );
		virtual ~Request();

		const std::string 	&getMethod( void ) const;
		const std::string 	&getUri( void ) const;
		const m_strStr		&getHeaders( void ) const;
		const std::string 	&getBody( void ) const;
		const std::string	&getTranslate( void ) const;
		const Location		&getMatch( void ) const;
		void				setMethod( const std::string &method );
		void				setUri( const std::string &uri );
		void				setHeaders( const m_strStr &headers );
		void				setBody( const std::string &body );
		void				setTranslate( const std::string &uri );
		void				setMatch( const Location &match );

		void	firstLineParser( std::string &line );
		void	headersParser( std::string &line );
		void	bodyParser( std::string &line );
		void	uriMatcher( const s_locs &locations );
		void	translateUri( void );
		void	finishTranslation( void );
		void	displayRequest( void ) const;
};

#endif
