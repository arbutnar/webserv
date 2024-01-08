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

static char buffer[PATH_MAX + 1];
std::string	absolutePath = static_cast<std::string>(realpath(".", buffer)) + "/";

Directives::Directives( void ) {
	_listen_host = 0;	// 127*2^24 + 0*2^16 + 0*2^8 + 1
	_listen_port = 8080;
	_server_name = "";
	_root = "";
	_alias = "";
	_index.clear();
	_autoindex = false;
	_cgi_alias = "";
	_error_page.clear();
	_client_max_body_size = 1000000;
	_client_header_buffer_size = 4000;
	_limit_except.clear();
	_limit_except.insert(std::make_pair("GET", true));
	_limit_except.insert(std::make_pair("HEAD", true));
	_limit_except.insert(std::make_pair("POST", true));
	_limit_except.insert(std::make_pair("PUT", true));
	_limit_except.insert(std::make_pair("DELETE", true));
	_return = std::make_pair(-1, "");
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
	_alias = src._alias;
	_index = src._index;
	_autoindex = src._autoindex;
	_cgi_alias = src._cgi_alias;
	_limit_except = src._limit_except;
	_error_page = src._error_page;
	_client_max_body_size = src._client_max_body_size;
	_client_header_buffer_size = src._client_header_buffer_size;
	_return = src._return;
	return *this;
}

void	Directives::checkDirective( const std::string &key, const bool &inLocation ) const {
	if (!inLocation && (key == "limit_except" || key == "alias" || key == "cgi_alias"))
		throw Directives::SyntaxError();
	else if (inLocation && (key == "server_name" || key == "listen" || key == "client_header_buffer_size"))
		throw Directives::SyntaxError();
}

void	Directives::directiveParser( std::string line, const bool &inLocation ) {
	std::string	arr[] = { "listen", "server_name", "root", "alias", "cgi_alias", "index", "autoindex", "limit_except",
							"error_page", "client_max_body_size", "client_header_buffer_size", "return" };
	std::string	key;
	std::string	value;
	size_t		pos;
	int 		i;

	pos = line.find_first_not_of(" \t");
	key = line.substr(pos, line.find_first_of(" \t", pos) - pos);
	for (i = 0; i < 12; i++) {
		if (arr[i] == key)
			break;
	}
	line.erase(0, line.find_first_not_of(" \t", key.length() + pos));
	pos = line.find(';');
	if (pos != line.size() - 1)
		throw Directives::SyntaxError();
	value = line.substr(0, pos);
	if (value.empty())
		throw Directives::SyntaxError();
	checkDirective(key, inLocation);
	switch (i) {
		case LISTEN:
			parseListen(value); break;
		case SERVER_NAME:
			_server_name = value; break;
		case ROOT:
			parseRoot(value); break;
		case ALIAS:
			parseAlias(value); break;
		case CGI_ALIAS:
			parseCgiAlias(value); break;
		case INDEX:
			parseIndex(value); break;
		case AUTOINDEX:
			parseAutoindex(value); break;
		case LIMIT_EXCEPT:
			parseLimitExcept(value); break;
		case ERROR_PAGE:
			parseErrorPage(value); break;
		case CLIENT_MAX_BODY_SIZE:
			parseClientMaxBodySize(value); break;
		case CLIENT_HEADER_BUFFER_SIZE:
			parseClientHeaderBufferSize(value); break;
		case RETURN:
			parseReturn(value); break;
		default:
			throw std::runtime_error("Syntax Error: not supported Directive");
	}
}

void	Directives::displayDirectives( void ) const {
	std::cout << "Host: " << _listen_host << std::endl;
	std::cout << "Port: " << _listen_port << std::endl;
	std::cout << "Server Name: " << _server_name << std::endl;
	std::cout << "Root: " << _root << std::endl;
	std::cout << "Alias: " << _alias << std::endl;
	std::cout << "Index: ";
	for (v_str::const_iterator it = _index.begin(); it != _index.end(); it++)
		std::cout << *it << ' ';
	std::cout << std::endl;
	std::cout << "Autoindex: " << _autoindex << std::endl;
	std::cout << "Cgi Pass: " << _cgi_alias << std::endl;
	std::cout << "Allowed Methods: ";
	for (m_strBool::const_iterator it = _limit_except.begin(); it != _limit_except.end(); it++)
		if (it->second == true)
			std::cout << it->first << ' ';
	std::cout << std::endl;
	std::cout << "Error Page: ";
	for (m_intStr::const_iterator it = _error_page.begin(); it != _error_page.end(); it++)
		std::cout << it->first << ' ' << it->second;
	std::cout << std::endl;
	std::cout << "Client Max Body Size: " << _client_max_body_size << std::endl;
	std::cout << "Client Header Buffer Size: " << _client_header_buffer_size << std::endl;
	std::cout << "Return: " << _return.first << " " << _return.second << std::endl;
}
