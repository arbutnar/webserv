/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 17:51:54 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/09 13:32:56 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location( void ) {
}

// potrei decidere di togliere gli spazi vuoti
Location::Location( std::string block )
	: _locationName ("None") {
		std::stringstream	ss(block);
		std::string			line;
		size_t pos;
		while (std::getline(ss, line))
		{
			if ((pos = line.find("location")) != std::string::npos) {
				pos = line.find_first_not_of(" \t", pos + 8);
				this->_locationName = line.substr(pos, line.find_last_not_of(" \t{\n") - pos + 1);
			}
			else if (line.find('}') == std::string::npos)
				this->_directives.setAttribute(line);
		}
		// this->_directives.showAttributes();
}

Location::Location( const Location &src ) {
	(void)src;
}

Location& Location::operator=( Location &src ) { //const
	return src;
}

Location::~Location() {
}
