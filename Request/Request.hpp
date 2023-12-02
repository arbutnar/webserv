/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 13:58:17 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/01 15:45:08 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../Location/Location.hpp"

class Request {
	private:
		std::string	_method;
		std::string	_uri;
		std::string _protocol;
		m_strStr	_headers;
	public:
		Request( void );
		Request( const Request &src );
		Request& operator=( const Request &src );
		~Request( );

		const std::string 	&getMethod( void ) const;
		const std::string 	&getUrl( void ) const;
		const std::string 	&getProtocol( void ) const;
		const m_strStr		&getHeaders( void ) const;
		void				setMethod( const std::string &method );
		void				setUrl( const std::string &uri );
		void				setProtocol( const std::string &protocol );
		void				setHeaders( const m_strStr &headers );

		void				parser( const std::string &buffer );
		const Location		uriMatcher( const s_locs &locations );
		const std::string	translateUri( const Location &match );
		void				displayRequest( void ) const;
};

#endif