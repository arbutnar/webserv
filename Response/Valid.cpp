/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Valid.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 11:48:05 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/02 17:48:56 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Valid.hpp"

Valid::Valid( void )
	: Response() {
}

Valid::Valid( const std::string &code )
	: Response(code) {
}

Valid::Valid( const Valid &src )
	: Response(src) {
}

Valid	&Valid::operator=( const Valid &src ) {
	if (this == &src)
		return *this;
	Response::operator=(src);
	return *this;
}

Valid::~Valid( ) {
}

void	Valid::generateBody( void ) {
	//roba
	_body = "Valid request\n";
}
