/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 17:51:54 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/08 18:17:02 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location( void ) {
}

Location::Location( const Location &src )
	: Directives(src) {
		this->_location_name = src._location_name;
}

Location& Location::operator=( const Location &src ) {
	if (this == &src)
        return *this;
    this->_location_name = src._location_name;
    Directives::operator=(src);
    return *this;
}

Location::~Location() {
}
