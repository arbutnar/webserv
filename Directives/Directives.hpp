/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directives.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 13:44:09 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/08 18:21:28 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <fstream>
# include <string>
# include <sstream>
# include <vector>
# include <map>
# include <stdlib.h>

typedef std::vector<std::string> v_Str;
typedef std::map<int,std::string> m_IntStr;

enum	commonDirectives {
	LISTEN,
	SERVER_NAME,
	ROOT,
	INDEX,
	AUTOINDEX,
	SCGI_PASS,
	TRY_FILES,
	LIMIT_EXCEPT,
	ERROR_PAGE,
	CLIENT_MAX_BODY_SIZE
};

class Directives {
	protected:
		uint32_t	    _listen_host;	//htonl
		uint16_t 		_listen_port;	//htons
		std::string     _server_name;
		std::string		_root;
		v_Str			_index;
		bool			_autoindex;
		std::string		_scgi_pass;
		v_Str			_try_files;
		v_Str			_limit_except;
		m_IntStr		_error_page;
		unsigned int	_client_max_body_size;
	public:
		Directives( void );
		Directives( std::string block );
		Directives( const Directives &src );
		Directives& operator=( const Directives &src );
		~Directives( );

		void	setListenHost( uint32_t listen_host );
		void	setListenPort( uint16_t listen_port );
		void	setServerName( std::string server_name );
		void	setRoot( std::string root );
		void	setIndex( v_Str index );
		void	setAutoindex( bool autoindex );
		void	setScgiPass( std::string scgi_pass );
		void	setTryFiles( v_Str try_files );
		void	setLimitExcept( v_Str limit_except );
		void	setErrorPage( m_IntStr error_page );
		void	setClientMaxBodySize( unsigned int client_max_body_size );

		uint32_t		getListenHost( void );
		uint16_t		getListenPort( void );
		std::string		getServerName( void );
		std::string		getRoot( void );
		v_Str			getIndex( void );
		bool			getAutoindex( void );
		std::string		getScgiPass( void );
		v_Str			getTryFiles( void );
		v_Str			getLimitExcept( void );
		m_IntStr		getErrorPage( void );
		unsigned int	getClientMaxBodySize( void );

		void	setAttribute( std::string line );
		void	parsAttribute( std::string line );
		void	clearDirectives( void );
		void	showAttributes(void);
};
