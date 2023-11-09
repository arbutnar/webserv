/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 17:16:45 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/09 12:02:37 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Directives.hpp"
#include <algorithm>

class   Location {
		friend void Master::configParse( void );

	private:
		std::string     _locationName;
		Directives		_directives;
	public:
		Location( void );
		Location( std::string block );
		Location( const Location &src );
		Location& operator=( Location &src ); // const
		~Location( );

};
