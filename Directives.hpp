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

#include <stdlib.h>
#include "Master.hpp"

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
	private:
		class FileError;

		unsigned int    			_listen;
		std::string     			_server_name;
		std::string					_root;
		std::string					_index;
		std::string					_autoindex;
		std::string					_scgi_pass;
		std::vector<std::string>	_try_files;
		std::vector<std::string>	_limit_except;
		std::vector<std::string>	_error_page;
		unsigned int				_client_max_body_size;
	public:
		Directives( void );
		Directives( std::string block );
		Directives( const Directives &src );
		Directives& operator=( const Directives &src );
		~Directives( );

		void	setListen( std::string configLine );
		void	setServerName( std::string configLine );
		void	setRoot( std::string configLine );
		void	setIndex( std::string configLine );
		void	setAutoindex( std::string configLine );
		void	setScgiPass( std::string configLine );
		void	setTryFiles( std::string configLine );
		void	setLimitExcept( std::string configLine );
		void	setErrorPage( std::string configLine );
		void	setClientMaxBodySize( std::string configLine );

		void	setAttribute( std::string line );
		void	parsAttribute( std::string line );
		void	clearDirectives( void );
		void	showAttributes(void);
};
