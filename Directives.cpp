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
}

Directives::Directives( std::string block ) {
	(void)block;
	// std::cout << block << std::endl;
	// parseBlock(block);
}

Directives::Directives( const Directives &src ) {
	*this = src;
}

Directives& Directives::operator=( const Directives &src ) {
	if (this != &src)
	{
		this->_listen = src._listen;
		this->_server_name = src._server_name;
		this->_root = src._root;
		this->_index = src._index;
		this->_autoindex = src._autoindex;
		this->_scgi_pass = src._scgi_pass;
		this->_try_files = src._try_files;
		this->_limit_except = src._limit_except;
		this->_error_page = src._error_page;
		this->_client_max_body_size = src._client_max_body_size;
	}
	return (*this);
}

Directives::~Directives( void ) {
}

void	Directives::setListen( std::string configLine) {
	this->_listen = std::atoi(configLine);
}

void	Directives::setServerName( std::string configLine ) {
	this->_server_name = configLine;
}

void	Directives::setRoot( std::string configLine ) {
	this->_root = configLine;
}

void	Directives::setIndex( std::string configLine ) {
	this->_index = configLine;
}

void	Directives::setAutoindex( std::string configLine ) {
	this->_autoindex = configLine;
}

void	Directives::setScgiPass( std::string configLine ) {
	this->_scgi_pass = configLine;
}

void	Directives::setTryFiles( std::string configLine ) {
	std::stringstream 	ss(configLine);
	std::string			line;

	while (std::getline(ss, line, ' '))
		this->_try_files.push_back(line);
}

void	Directives::setLimitExcept( std::string configLine ) {
	std::stringstream 	ss(configLine);
	std::string			line;

	while (std::getline(ss, line, ' '))
		this->_limit_except.push_back(line);
}

void	Directives::setErrorPage( std::string configLine ) {
	std::stringstream 	ss(configLine);
	std::string			line;

	while (std::getline(ss, line, ' '))
		this->_error_page.push_back(line);
}

void	Directives::setClientMaxBodySize( std::string configLine ) {
	this->_client_max_body_size = std::atoi(configLine);
}

void	Directives::setAttribute( std::string line ) {
	std::string	arr[] = { "listen", "root", "index", "autoindex", "error_page", "client_max_body_size",
									"limit_except", "try_files", "cgi_pass" };
	// int 		i;
	size_t		pos = line.find_first_not_of(" \t");
	std::string	directive = line.substr(pos, line.find_first_of(" \t", pos) - pos);
	std::cout << '$' << directive << '$'<< std::endl;
	// for (i = 0; i < 10; i++)
	// {

	// }
	// 	if (arr[i] == line.substr(startPos, endPos - startPos))
	// 	{
	// 		line.erase(startPos, endPos - startPos);
	// 		startPos = line.find_first_not_of(' \t');
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
	// 					setAutoindex(line); break;
	// 			case ERROR_PAGE:
	// 					setErrorPage(line); break;
	// 			case LIMIT_EXCEPT:
	// 					setLimitExcept(line); break;
	// 			case TRY_FILES:
	// 					setTryFiles(line); break;
	// 			case SCGI_PASS:
	// 					setScgiPass(line); break;
	// 			default:
	// 					break;
	// 		}
	// 		break;
	// 	}
	// }
}

void	Directives::clearDirectives( void ) {
	this->_listen = 0;
	this->_server_name = "None";
	this->_root = "None";
	this->_index = "None";
	this->_autoindex = "None";
	this->_scgi_pass = "None";
	// this->_try_files = NULL;
	// this->_limit_except = NULL;
	// this->_error_page = NULL;
	this->_client_max_body_size = 0;
}

void	Directives::showAttributes(void) {
	std::cout << _listen << std::endl;
	std::cout << _server_name << std::endl;
	std::cout << _root << std::endl;
	std::cout << _index << std::endl;
	std::cout << _autoindex << std::endl;
	std::cout << _scgi_pass << std::endl;
	// std::cout << _try_files << std::endl;
	// std::cout << _limit_except << std::endl;
	// std::cout << _error_page << std::endl;
	std::cout << _client_max_body_size << std::endl;
}
