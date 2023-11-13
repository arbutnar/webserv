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

void	Location::setLocationName( std::string name ) {
	this->_location_name = name;
}

const std::string&	Location::getLocationName( void ) {
	return this->_location_name;
}

void	Location::parseLocationName( const std::string &line ) {
	size_t pos = line.find("location") + 8;
	pos = line.find_first_not_of(" \t", pos);
	this->_location_name = line.substr(pos, line.find_last_not_of(" {\t\n") - pos + 1);
}

void	Location::clear( void ) {
	this->_location_name = "";
	Directives::clear();
}
