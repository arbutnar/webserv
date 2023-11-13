/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 17:16:45 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/08 18:16:50 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "../Directives/Directives.hpp"

class   Location : protected Directives {
	private:
		std::string     _location_name;
	public:
		Location( void );
		Location( std::string block );
		Location( const Location &src );
		Location& operator=( const Location &src );
		~Location( );

};
