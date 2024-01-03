/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directives.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 13:44:09 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/09 17:27:35 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVES_HPP
# define DIRECTIVES_HPP

# include <iostream>
# include <fstream>
# include <string>
# include <sstream>
# include <vector>
# include <map>
# include <stdlib.h>
# include <linux/limits.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <algorithm>
# include <set>
# include <sys/stat.h>
# include <unistd.h>
# include <arpa/inet.h>
# include "../Utils.tpp"

# define HOSTS "/etc/hosts"

typedef std::vector<std::string> v_str;
typedef std::map<int,std::string> m_intStr;
typedef std::map<std::string, bool> m_strBool;
typedef std::map<std::string, std::string> m_strStr;
typedef	std::pair<std::string, std::string> p_strStr;
typedef std::pair<int, std::string> p_intStr;

extern std::string	absolutePath;

enum	listDirectives {
	LISTEN,
	SERVER_NAME,
	ROOT,
	ALIAS,
	CGI_ALIAS,
	INDEX,
	AUTOINDEX,
	LIMIT_EXCEPT,
	ERROR_PAGE,
	CLIENT_MAX_BODY_SIZE,
	CLIENT_HEADER_BUFFER_SIZE,
	RETURN
};

class Directives {
	protected:
		u_int32_t	    _listen_host;	//htonl
		u_int16_t 		_listen_port;	//htons
		std::string     _server_name;
		std::string		_root;
		std::string		_alias;
		v_str			_index;
		bool			_autoindex;
		std::string		_cgi_alias;
		m_strBool		_limit_except;
		m_intStr		_error_page;
		unsigned int	_client_max_body_size;
		unsigned int	_client_header_buffer_size;
		p_intStr		_return;
	public:
		struct SyntaxError : public std::exception {
			virtual const char* what() const throw() { return ("Incomplete or wrong configurations Syntax"); }
		};

		Directives( void );
		Directives( const Directives &src );
		Directives& operator=( const Directives &src );
		virtual ~Directives( );

		void	parseListen( const std::string &attribute );
		void	parseListenHost( const std::string &attribute );
		void	parseListenPort( const std::string &attribute );
		void	parseRoot( const std::string &attribute );
		void	parseAlias( const std::string &attribute );
		void	parseIndex( const std::string &attribute );
		void	parseAutoindex( const std::string &attribute );
		void	parseLimitExcept( const std::string &attribute );
		void	parseErrorPage( const std::string &attribute );
		void	parseCgiAlias( const std::string &attribute );
		void	parseClientMaxBodySize( const std::string &attribute );
		void	parseClientHeaderBufferSize( const std::string &attribute );
		void	parseReturn( const std::string &attribute );

		void	setListenHost( const u_int32_t &listen_host );
		void	setListenPort( const u_int16_t &listen_port );
		void	setServerName( const std::string &server_name );
		void	setRoot( const std::string &root );
		void	setAlias( const std::string &root );
		void	setIndex( const v_str &index );
		void	setAutoindex( const bool &autoindex );
		void	setCgiAlias( const std::string &cgi_alias );
		void	setLimitExcept( const m_strBool &limit_except );
		void	setErrorPage( const m_intStr &error_page );
		void	setClientMaxBodySize( const unsigned int &client_max_body_size );
		void	setClientHeaderBufferSize( const unsigned int &client_header_buffer_size );
		void	setReturn( const p_intStr &ret );

		const u_int32_t		&getListenHost( void ) const;
		const u_int16_t		&getListenPort( void ) const;
		const std::string	&getServerName( void ) const;
		const std::string	&getRoot( void ) const;
		const std::string	&getAlias( void ) const;
		const v_str			&getIndex( void ) const;
		const bool			&getAutoindex( void ) const;
		const std::string	&getCgiAlias( void ) const;
		const m_strBool		&getLimitExcept( void ) const;
		const m_intStr		&getErrorPage( void ) const;
		const unsigned int	&getClientMaxBodySize( void ) const;
		const unsigned int	&getClientHeaderBufferSize( void ) const;
		const p_intStr		&getReturn( void ) const;

		void			checkDirective( const std::string &key, const bool &inLocation ) const;
		void			directiveParser( std::string line, const bool &inLocation );
		void			displayDirectives( void ) const;
};

#endif
