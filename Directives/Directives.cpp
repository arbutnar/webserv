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
	this->_index = src._index;
	this->_autoindex = src._autoindex;
	this->_scgi_pass = src._scgi_pass;
	this->_try_files = src._try_files;
	this->_limit_except = src._limit_except;
	this->_error_page = src._error_page;
	this->_client_max_body_size = src._client_max_body_size;
	return *this;
}

Directives::~Directives( void ) {
}

void	Directives::clear( void ) {
	this->_listen_host = 2130706433;	// 127*2^24 + 0*2^16 + 0*2^8 + 1
	this->_listen_port = 8080;
	this->_server_name = "";
	this->_root = "";
	this->_index.clear();
	this->_autoindex = false;
	this->_scgi_pass = "";
	this->_try_files.clear();
	this->_error_page.clear();
	this->_client_max_body_size = 1000000;
	this->_limit_except.clear();
	this->_limit_except.insert(std::make_pair("GET", false));
	this->_limit_except.insert(std::make_pair("HEAD", false));
	this->_limit_except.insert(std::make_pair("POST", false));
	this->_limit_except.insert(std::make_pair("PUT", false));
	this->_limit_except.insert(std::make_pair("DELETE", false));
}

void	Directives::directiveParser( std::string line ) {
	std::string	arr[] = { "listen", "server_name", "root", "index", "autoindex", "scgi_pass", "try_files", 
							"limit_except", "error_page", "client_max_body_size"};
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
	if (value.empty())
		throw Directives::SyntaxError();
	switch (i) {
		case LISTEN:
			parseListen(value); break;
		case SERVER_NAME:
			this->_server_name = value; break;
		case ROOT:
			this->_root = value; break;
		case INDEX:
			parseIndex(value); break;
		case AUTOINDEX:
			parseAutoindex(value); break;
		case ERROR_PAGE:
			parseErrorPage(value); break;
		case LIMIT_EXCEPT:
			parseLimitExcept(value); break;
		case TRY_FILES:
			parseTryFiles(value); break;
		case SCGI_PASS:
			this->_scgi_pass = value; break;
		default:
			throw Directives::SyntaxError;
	}
}

void	Directives::parseListen( const std::string &attribute ) {
	size_t pos = attribute.find(':');
	if (pos != std::string::npos)
	{
		parseListenHost(attribute.substr(0, pos));
		parseListenPort(attribute.substr(pos + 1, std::string::npos));
	}
	else
		parseListenPort(attribute);
}

void 	Directives::parseListenHost( const std::string &attribute ) {
	if (attribute != "127.0.0.1" && attribute != "localhost")
		throw Directives::SyntaxError();
}

void	Directives::parseListenPort( const std::string &attribute ) {
	if (attribute.find_first_not_of("0123456789") != std::string::npos)
		throw Directives::SyntaxError();
	this->_listen_port = std::atoi(attribute.c_str());
}

void	Directives::parseIndex( const std::string &attribute ) {
	size_t	pos = 0;

	while ((pos = attribute.find_first_not_of(' ', pos)) != std::string::npos)
	{
		std::string tmp = attribute.substr(pos, attribute.find_first_of(' ', pos) - pos);
		this->_index.push_back(tmp);
		pos += tmp.length();
		std::cout << tmp <<  std::endl;
	}
}

void	Directives::parseAutoindex( const std::string &attribute ) {
	if (attribute == "on")
		this->_autoindex = true;
	else if (attribute != "off")
		throw Directives::SyntaxError();
}

void	Directives::parseTryFiles( const std::string &attribute ) {
	size_t	pos = 0;

	while ((pos = attribute.find_first_not_of(' ', pos)) != std::string::npos)
	{
		std::string tmp = attribute.substr(pos, attribute.find_first_of(' ', pos) - pos);
		this->_try_files.push_back(tmp);
		pos += tmp.length();
		std::cout << tmp <<  std::endl;
	}
}

void	Directives::parseLimitExcept( const std::string &attribute ) {
	size_t	pos = 0;

	while ((pos = attribute.find_first_not_of(' ', pos)) != std::string::npos)
	{
		std::string tmp = attribute.substr(pos, attribute.find_first_of(' ', pos) - pos);
		tmp = toupper(tmp)
		pos += tmp.length();
		std::cout << tmp <<  std::endl;
	}
}

void	Directives::parseErrorPage( const std::string &attribute ) {
	size_t pos = attribute.find_first_of(" \t");
	std::string	key = attribute.substr(0, pos);
	std::string	value = attribute.substr(attribute.find_first_not_of(" \t", pos), std::string::npos);
	
	if (key.empty() || value.empty() || key.find_first_not_of("0123456789") != std::string::npos)
		throw Directives::SyntaxError();
	this->_error_page.insert(std::make_pair(std::atoi(key.c_str()), value));
}

void	Directives::parseClientMaxBodySize( const std::string &attribute ) {
	setClientMaxBodySize(std::atoi(attribute.c_str()));
}
