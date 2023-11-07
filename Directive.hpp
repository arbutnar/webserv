/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 13:44:09 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/07 14:45:31 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Master.hpp"

enum	commonDirectives {
	ROOT,
	INDEX,
	AUTOINDEX,
	ERROR_PAGE,
	CLIENT_MAX_BODY_SIZE,
	TRY_FILES,
};

class Directive {
	protected:
		std::string		_root;
		std::string		_index;
		std::string		_autoindex;
		const char*		_error_page;
		unsigned int	_client_max_body_size;
		const char*		_try_files;             // cos'e' $uri
	public:
		Directive( void );
		Directive( const Directive &src );
		Directive& operator=( Directive &src ); //const
		~Directive( );
	
		// void	setRoot( std::string confLine );
		// void	setIndex( std::string confLine );
		// void	setAutoindex( std::string confLine );
		// void	setErrorPage( std::string confLine );
		// void	setClientMaxBodySize( std::string confLine );
		// void	setTryFiles( std::string confLine );

		// void	getRoot( void );
		// void	getIndex( void );
		// void	getAutoindex( void );
		// void	getErrorPage( void );
		// void	getClinetMaxBodySize( void );
		// void	getTryFiles( void );

		void	blockParse( std::string block );
};
