/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 17:51:54 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/18 17:05:35 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location( void ) {
}

Location::Location( const Location &src )
	: Directives(src) {
		_location_name = src._location_name;
}

Location::Location( const std::string &locationName ) {
	size_t pos = locationName.find("location") + 8;
	pos = locationName.find_first_not_of(" \t", pos);
	_location_name = locationName.substr(pos, locationName.find_last_not_of(" {\t\n") - pos + 1);
}

Location& Location::operator=( const Location &src ) {
	if (this == &src)
		return *this;
	_location_name = src._location_name;
	Directives::operator=(src);
	return *this;
}

Location::~Location() {
}

void	Location::setLocationName( std::string name ) {
	_location_name = name;
}

const std::string&	Location::getLocationName( void ) const {
	return _location_name;
}

void	Location::displayLocation( void ) const {
	std::cout << "location " << _location_name << " {" << std::endl;
	displayDirectives();
	std::cout << "}" << std::endl;
}
