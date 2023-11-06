/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 14:12:52 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/06 17:19:14 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Directive.hpp"

Directive::Directive( void ) {
}

Directive::Directive( const Directive &src ) {
	(void)src;
}

Directive& Directive::operator=( Directive &src ) { //const
	return (src);
}

Directive::~Directive( void ) {
}

void	Directive::blockParse( std::string block ) {
	// std::string	arr[] = { "listen", "root", "index", "autoindex", "error_page", "client_max_body_size",
	// 								"allowed_methods", "try_files", "cgi_pass", "return" };
	// std::string	line;
	// size_t  pos;

	// size_t	startPos = line.find_not_first_of(' ');
	// size_t	endPos = line.find(' ');
	// for (int i = 0; i < arr.length; i++)
	// {
	// 	if (attribute[i] == line.substr(startPos, endPos - startPos));
	// 	{
	// 		line.erase(startPos, endPos - startPos);
	// 		startPos = line.find_not_first_of(' ');
	// 		endPos = line.find(';');
	// 		switch (i) {
	// 			case LISTEN:
	// 					setListen(line); break;
	// 			case SERVER_NAME:
	// 					setServerName(line); break;
	// 			case ROOT:
	// 					setRoot(line); break;
	// 			case INDEX:
	// 					setIndex(line); break;
	// 			case AUTOINDEX:
	// 					setAutoIndex(line); break;
	// 			case ERROR_PAGE:
	// 					setErrorPage(line); break;
	// 			case ALLOWED_METHODS:
	// 					setAllowedMethods(line); break;
	// 			case TRY_FILES:
	// 					setTryFiles(line); break;
	// 			case CGI_PASS:
	// 					setCgiPass(line); break;
	// 			case RETURN:
	// 					setReturn(line); break;
	// 			default:
	// 					break;
	// 		}
	// 		line.erase(startPos, endPos - startPos + 1);
	// 		break;
	// 	}
	// }
	std::cout <<block << std::endl;
}
