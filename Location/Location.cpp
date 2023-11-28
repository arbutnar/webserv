/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 17:51:54 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/28 14:59:34 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location( void )
	:_modifier (0), _location_name ("") {
}

Location::Location( const std::string &line )
	: _modifier (0), _location_name ("") {
		std::string	arr[] = { "=", "~", "~*", "^~" };
		size_t		pos = 0;

		if (line.find("location") != std::string::npos)
			pos = line.find("location") + 8;
		pos = line.find_first_not_of(" \t", pos);
		std::string tmp = line.substr(pos, line.find_last_not_of(" {\t\n") - pos + 1);
		if (tmp.find(" ") != std::string::npos)
		{
			int	i;
			for (i = 0; i < 4; i++)
				if (arr[i] == tmp.substr(0, tmp.find(" ")))
					break;
			if (i == 4)
				throw Directives::SyntaxError();
			_modifier = i + 1;
			tmp.erase(0, tmp.find_last_of(" \t") + 1);
		}
		if (tmp.empty() || tmp.find_first_of(" \t") != std::string::npos)
			throw Directives::SyntaxError();
		if (tmp != "/") {
			if (tmp.at(0) == '/')
				tmp.erase(0, 1);
			if (tmp.at(tmp.size() - 1) == '/')
				tmp.erase(tmp.size() -1, 1);
		}
		else
			if (_modifier != 0)
				throw Directives::SyntaxError();
		_location_name = tmp;
}

Location::Location( const Location &src )
	: Directives(src) {
		_modifier = src._modifier;
		_location_name = src._location_name;
}

Location	&Location::operator=( const Location &src ) {
	if (this == &src)
		return *this;
	_modifier = src._modifier;
	_location_name = src._location_name;
	Directives::operator=(src);
	return *this;
}

bool	Location::operator<( const Location &src ) const {
	if (_location_name.compare(src._location_name) < 0)
		return true;
	return false;
}

Location::~Location() {
}

const int	&Location::getModifier( void ) const {
	return _modifier;
}

const std::string&	Location::getLocationName( void ) const {
	return _location_name;
}

void	Location::setModifier( const int &modifier ) {
	_modifier = modifier;
}

void	Location::setLocationName( const std::string &name ) {
	_location_name = name;
}

void	Location::displayLocation( void ) const {
	std::cout << "location " << _modifier << " " << _location_name << " {" << std::endl;		// modifier is a number
	displayDirectives();
	std::cout << "}" << std::endl;
}
