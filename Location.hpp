/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 17:16:45 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/05 18:30:11 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Master.hpp"

class   Location {
	private:
			friend void Master::configParse( void );

		std::string     _locationName;
		unsigned int	_listen;
		std::string		_root;
		const char*		_index;
		const char*		_error_page;
		unsigned int	_client_max_body_size;
		const char*		_allowed_methods;
		const char*		_try_files;             // cos'e' $uri
		std::string		_cgi_pass;
		const char*		_return;
	public:
		Location( void );
		Location( std::string block);
		Location( const Location &src );
		Location& operator=( Location &src ); // const
		~Location( );
};
