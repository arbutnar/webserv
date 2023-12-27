/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 16:38:05 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/13 17:57:15 by arbutnar         ###   ########.fr       */
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
# include "../Utils.tpp"

typedef std::set<Location> s_locs;

enum methods {
	GET,
	HEAD,
	POST,
	PUT,
	DELETE
};

class Request {
	private:
		std::string	_method;
		std::string	_uri;
		std::string	_protocol;
		m_strStr	_headers;
		std::string	_body;
		std::string	_translate;
		Location	_match;
	public:
		Request( void );
		Request( Request const &src );
		Request	&operator=( Request const &src );
		bool	operator==( Request const &src ) const ;
		~Request();

		const std::string 	&getMethod( void ) const;
		const std::string 	&getUri( void ) const;
		const std::string 	&getProtocol( void ) const;
		const m_strStr		&getHeaders( void ) const;
		const std::string 	&getBody( void ) const;
		const std::string	&getTranslate( void ) const;
		const Location		&getMatch( void ) const;
		void				setMethod( const std::string &method );
		void				setUri( const std::string &uri );
		void				setProtocol( const std::string &protocol );
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