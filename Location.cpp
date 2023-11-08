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

// potrei decidere di togliere gli spazi vuoti
Location::Location( std::string block )
	: _locationName ("None") {
		std::stringstream	ss(block);
		std::string			line;
		size_t pos;
		while (std::getline(ss, line))
		{
			if (line.find_first_of("location")) {
				pos = line.find_first_of("location") + 9;
				this->_locationName = block.substr(pos, block.find_first_of('{', pos) - pos - 1);
			}
			this->_directives.setAttribute(line);
		}
		std::cout << this->_locationName << std::endl;
		// Directives	directives(block);
		this->_directives.showAttributes();
}

Location::Location( const Location &src ) {
	(void)src;
}

Location& Location::operator=( Location &src ) { //const
	return src;
}

Location::~Location() {
}
