/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directives.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 14:12:52 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/08 15:22:23 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Directives.hpp"

Directives::Directives( void ) {
	clear();
}

Directives::Directives( const Directives &src ) {
	*this = src;
}

Directives& Directives::operator=( const Directives &src ) {
	if (this == &src)
		return *this;
	this->_listen_host = src._listen_host;
	this->_listen_port = src._listen_port;
	this->_server_name = src._server_name;
	this->_root = src._root;
	// this->_index = src._index;
	this->_autoindex = src._autoindex;
	this->_scgi_pass = src._scgi_pass;
	// this->_try_files = src._try_files;
	// this->_limit_except = src._limit_except;
	// this->_error_page = src._error_page;
	this->_client_max_body_size = src._client_max_body_size;
	return *this;
}

Directives::~Directives( void ) {
}

void	Directives::clear( void ) {
	this->_listen_host = 2130706433;	// 127*2^24 + 0*2^16 + 0*2^8 + 1
	this->_listen_port = 0;
	this->_server_name = "";
	this->_root = "";
	this->_index.clear();
	this->_autoindex = false;
	this->_scgi_pass = "";
	this->_try_files.clear();
	this->_limit_except.clear();
	this->_error_page.clear();
	this->_client_max_body_size = 1000000;
}

void	Directives::directiveParser( std::string line ) {
	std::string	arr[] = { "listen", "root", "index", "autoindex", "error_page",
							"client_max_body_size", "limit_except", "try_files", "cgi_pass" };
	std::string	key;
	std::string	value;
	size_t		pos;
	int 		i;

	pos = line.find_first_not_of(" \t");
	key = line.substr(pos, line.find_first_of(" \t", pos) - pos);
	for (i = 0; i < 10; i++) {
		if (arr[i] == key)
			break;
	}
	line.erase(0, line.find_first_not_of(" \t", key.length() + pos));
	value = line.substr(0, line.find(';'));
	switch (i) {
		case LISTEN:
			parseListen(line); break;
		// case SERVER_NAME:
		// 		setServerName(line); break;
		// case ROOT:
		// 		setRoot(line); break;
		// case INDEX:
		// 		setIndex(line); break;
		// case AUTOINDEX:
		// 		setAutoindex(line); break;
		// case ERROR_PAGE:
		// 		setErrorPage(line); break;
		// case LIMIT_EXCEPT:
		// 		setLimitExcept(line); break;
		// case TRY_FILES:
		// 		setTryFiles(line); break;
		// case SCGI_PASS:
		// 		setScgiPass(line); break;
		default:
				break;
	}
}
