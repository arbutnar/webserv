/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 17:51:54 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/06 17:46:23 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location( void ) {
}

Location::Location( std::string block) {
	std::stringstream	ss(block);
	std::string			line;
	size_t				pos;

	while (std::getline(ss, line))
	{
		if (pos = line.find("location") != std::string::npos)
		{
			line.substr(pos, line.find('{') - pos)
			pos 
		}
	}
	blockParse(block);
}

Location::Location( const Location &src )
	: Directive() {
	(void)src;
}

Location& Location::operator=( Location &src ) { //const
	return src;
}

Location::~Location() {
}
