/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 13:58:17 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/02 16:44:12 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../Location/Location.hpp"

class Request {
	private:
		std::string	_method;
		std::string	_uri;
		std::string	_translate;
		Location	_match;
		m_strStr	_headers;
	public:
		Request( void );
		Request( const Request &src );
		Request& operator=( const Request &src );
		~Request( );

		const std::string 	&getMethod( void ) const;
		const std::string 	&getUri( void ) const;
		const std::string	&getTranslate( void ) const;
		const Location		&getMatch( void ) const;
		const m_strStr		&getHeaders( void ) const;
		void				setMethod( const std::string &method );
		void				setUri( const std::string &uri );
		void				setTranslate( const std::string &uri );
		void				setMatch( const Location &match );
		void				setHeaders( const m_strStr &headers );

		void	parser( const std::string &buffer );
		void	uriMatcher( const s_locs &locations );
		void	translateUri( void );
		void	displayRequest( void ) const;
};

#endif