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
	_listen_host = 2130706433;	// 127*2^24 + 0*2^16 + 0*2^8 + 1
	_listen_port = 8080;
	_server_name = "";
	_root = "";
	_index.clear();
	_autoindex = false;
	_scgi_pass = "";
	_try_files.clear();
	_error_page.clear();
	_client_max_body_size = 1000000;
	_limit_except.clear();
	_limit_except.insert(std::make_pair("GET", false));
	_limit_except.insert(std::make_pair("HEAD", false));
	_limit_except.insert(std::make_pair("POST", false));
	_limit_except.insert(std::make_pair("PUT", false));
	_limit_except.insert(std::make_pair("DELETE", false));
}

Directives::Directives( const Directives &src ) {
	*this = src;
}

Directives::~Directives( ) {
}

Directives& Directives::operator=( const Directives &src ) {
	if (this == &src)
		return *this;
	_listen_host = src._listen_host;
	_listen_port = src._listen_port;
	_server_name = src._server_name;
	_root = src._root;
	_index = src._index;
	_autoindex = src._autoindex;
	_scgi_pass = src._scgi_pass;
	_try_files = src._try_files;
	_limit_except = src._limit_except;
	_error_page = src._error_page;
	_client_max_body_size = src._client_max_body_size;
	return *this;
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
			_server_name = value; break;
		case ROOT:
			_root = value; break;
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
			_scgi_pass = value; break;
		case CLIENT_MAX_BODY_SIZE:
			parseClientMaxBodySize(value); break;
		default:
			throw Directives::SyntaxError();
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
	_listen_port = std::atoi(attribute.c_str());
}

void	Directives::parseIndex( const std::string &attribute ) {
	size_t	pos = 0;
	while ((pos = attribute.find_first_not_of(' ', pos)) != std::string::npos)
	{
		std::string tmp = attribute.substr(pos, attribute.find_first_of(' ', pos) - pos);
		_index.push_back(tmp);
		pos += tmp.length();
	}
}

void	Directives::parseAutoindex( const std::string &attribute ) {
	if (attribute == "on")
		_autoindex = true;
	else if (attribute != "off")
		throw Directives::SyntaxError();
}

void	Directives::parseTryFiles( const std::string &attribute ) {
	size_t	pos = 0;
	while ((pos = attribute.find_first_not_of(' ', pos)) != std::string::npos)
	{
		std::string tmp = attribute.substr(pos, attribute.find_first_of(' ', pos) - pos);
		_try_files.push_back(tmp);
		pos += tmp.length();
	}
}

void	Directives::parseLimitExcept( const std::string &attribute ) {
	size_t	pos = 0;
	while ((pos = attribute.find_first_not_of(' ', pos)) != std::string::npos)
	{
		std::string tmp = attribute.substr(pos, attribute.find_first_of(' ', pos) - pos);
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
		if (tmp != "GET" && tmp != "HEAD" && tmp != "POST" && tmp != "PUT" && tmp != "DELETE")
			throw Directives::SyntaxError();
		for (std::map<std::string, bool>::iterator it = _limit_except.begin(); it != _limit_except.end(); it++)
			if (it->first == tmp)
				it->second = true;
		pos += tmp.length();
	}
}

void	Directives::parseErrorPage( const std::string &attribute ) {
	size_t pos = attribute.find_first_of(" \t");
	std::string	key = attribute.substr(0, pos);
	std::string	value = attribute.substr(attribute.find_first_not_of(" \t", pos), std::string::npos);
	
	if (key.empty() || value.empty() || key.find_first_not_of("0123456789") != std::string::npos)
		throw Directives::SyntaxError();
	_error_page.insert(std::make_pair(std::atoi(key.c_str()), value));
}

void	Directives::parseClientMaxBodySize( const std::string &attribute ) {
	size_t			pos = attribute.find_first_not_of("0123456789");
	char			c;

	_client_max_body_size = std::atoi(attribute.substr(0, pos).c_str());
	if (pos == std::string::npos)
		return ;
	else if (pos != attribute.length() - 1)
		throw Directives::SyntaxError();
	c = attribute[pos];
	c = std::toupper(c);
	if (c == 'K')
		_client_max_body_size *= 1000;
	else if (c == 'M')
		_client_max_body_size *= 1000000;
	else
		throw Directives::SyntaxError();
}

void	Directives::displayDirectives( void ) const {
	std::cout << "Host: " << _listen_host << std::endl;
	std::cout << "Port: " << _listen_port << std::endl;
	std::cout << "Server Name: " << _server_name << std::endl;
	std::cout << "Root: " << _root << std::endl;
	std::cout << "Index: ";
	for (v_Str::const_iterator it = _index.begin(); it != _index.end(); it++)
		std::cout << *it << ' ';
	std::cout << std::endl;
	std::cout << "Autoindex: " << _autoindex << std::endl;
	std::cout << "Scgi Pass: " << _scgi_pass << std::endl;
	std::cout << "Try Files: ";
	for (v_Str::const_iterator it = _try_files.begin(); it != _try_files.end(); it++)
		std::cout << *it << ' ';
	std::cout << std::endl;
	std::cout << "Allowed Methods: ";
	for (m_StrBool::const_iterator it = _limit_except.begin(); it != _limit_except.end(); it++)
		if (it->second == true)
			std::cout << it->first << ' ';
	std::cout << std::endl;
	std::cout << "Error Page: " << std::endl;
	for (m_IntStr::const_iterator it = _error_page.begin(); it != _error_page.end(); it++)
		std::cout << it->first << ' ' << it->second << std::endl;
	std::cout << "Client Max Body Size: " << _client_max_body_size << std::endl;
}

void	Directives::addLocation( const Location &location ) {
	(void) location;
}
