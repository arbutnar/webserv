/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 17:16:45 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/07 14:04:13 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Directive.hpp"

class   Location : protected Directive {
		friend void Master::configParse( void );

	private:
		std::string     _locationName;
		std::string		_scgi_pass;
		const char*		_limit_except;
	public:
		Location( void );
		Location( std::string block);
		Location( const Location &src );
		Location& operator=( Location &src ); // const
		~Location( );
};
