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
			if ((pos = line.find("location")) != std::string::npos) {
				pos += line.find_first_not_of(" \t", pos + 8);
				this->_locationName = block.substr(pos, block.find('{', pos) - pos);
				// togliere spazi da locationName
			}
			else if (line.find('}') == std::string::npos)
				this->_directives.setAttribute(line);
		}
		std::cout << '$' << this->_locationName << '$' << std::endl;
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
