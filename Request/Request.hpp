/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 13:58:17 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/21 16:57:24 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <string>
# include <sstream>
# include <map>

typedef std::map<std::string, std::string> m_hdrs;

class Request {
	private:
		bool		_isValid;
		std::string	_method;
		std::string	_url;
		m_hdrs		_headers;
	public:
		Request( void );
		Request( const Request &src );
		Request& operator=( const Request &src );
		~Request( );

		const bool			&getIsValid( void ) const;
		const std::string 	&getMethod( void ) const;
		const std::string 	&getUrl( void ) const;
		const m_hdrs		&getHeaders( void ) const;
		void				setIsValid( const bool &isValid );
		void				setMethod( const std::string &method );
		void				setUrl( const std::string &url );
		void				setHeaders( const m_hdrs &headers );

		void				parser( const std::string &buffer );
};

#endif