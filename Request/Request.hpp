/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 13:58:17 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/07 11:42:16 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <sys/socket.h>
# include <fstream>
# include "../Location/Location.hpp"

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
		Request( const Request &src );
		Request& operator=( const Request &src );
		~Request( );

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

		void	headersParser( const std::string &buffer );
		void	headersChecker( void ) const;
		void	uriMatcher( const s_locs &locations );
		void	matchChecker( void ) const;
		void	translateUri( void );
		void	readChunk( const int &socket, const size_t &chunkSize );
		void	bodyParser( const int &socket );
		void	displayRequest( void ) const;
};

#endif