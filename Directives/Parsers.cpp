/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:40:37 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/12 19:26:01 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Directives.hpp"

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

void	Directives::parseRoot( const std::string &attribute ) {
	if (!_alias.empty())
		throw std::runtime_error("Syntax Error: root Directive");
	_root = attribute;
	if (_root != "/") {
		if (_root.at(0) == '/')
			_root.erase(0, 1);
		if (*_root.rbegin() != '/')
			_root += "/";
	}
	if (_root.empty())
		throw std::runtime_error("Syntax Error: root Directive");

	struct stat	st;
	if (stat(_root.c_str(), &st) == -1 || !(st.st_mode & S_IFDIR))
		throw std::runtime_error("Syntax Error: root Directive");
}

void	Directives::parseAlias( const std::string &attribute ) {
	if (!_root.empty())
		throw std::runtime_error("Syntax Error: alias Directive");
	_alias = attribute;
	if (_alias != "/" && _alias.at(0) == '/')
		_alias.erase(0, 1);
	if (_alias.empty())
		throw std::runtime_error("Syntax Error: alias Directive");
	struct stat	st;
	if (stat(_alias.c_str(), &st) == -1 || !(st.st_mode & S_IFDIR))
		throw std::runtime_error("Syntax Error: alias Directive");
}

void 	Directives::parseListenHost( const std::string &attribute ) {
	if (attribute != "127.0.0.1" && attribute != "localhost")
		throw std::runtime_error("Syntax Error: host Directive");
}

void	Directives::parseListenPort( const std::string &attribute ) {
	if (attribute.find_first_not_of("0123456789") != std::string::npos)
		throw std::runtime_error("Syntax Error: port Directive");
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
		throw std::runtime_error("Syntax Error: autoindex Directive");
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
	for (m_strBool::iterator it = _limit_except.begin(); it != _limit_except.end(); it++)
		it->second = false;
	size_t	pos = 0;
	while ((pos = attribute.find_first_not_of(' ', pos)) != std::string::npos)
	{
		std::string tmp = attribute.substr(pos, attribute.find_first_of(' ', pos) - pos);
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);			// is case sensitive?
		if (tmp != "GET" && tmp != "HEAD" && tmp != "POST" && tmp != "PUT" && tmp != "DELETE")
			throw std::runtime_error("Syntax Error: limit_except Directive");
		for (m_strBool::iterator it = _limit_except.begin(); it != _limit_except.end(); it++)
			if (it->first == tmp)
				it->second = true;
		pos += tmp.length();
	}
	if (_limit_except["GET"] == true)
		_limit_except["HEAD"] = true;
}

void	Directives::parseErrorPage( const std::string &attribute ) {
	size_t pos = attribute.find_first_of(" \t");
	std::string	key = attribute.substr(0, pos);
	std::string	value = attribute.substr(attribute.find_first_not_of(" \t", pos), std::string::npos);
	
	if (key.empty() || value.empty())
		throw std::runtime_error("Syntax Error: error_page Directive");
	int	code = atoi(key.c_str());
	if (key.find_first_not_of("0123456789") != std::string::npos || code < 300 || code > 600)
		throw std::runtime_error("Syntax Error: error_page Directive");
	if (*value.begin() == '/')
		value.erase(0, 1);
	value = absolutePath + value;
	if (access(value.c_str(), R_OK) == -1)
		throw std::runtime_error("Syntax Error: error_page Directive");
	_error_page.insert(std::make_pair(code, value));
}

void	Directives::parseCgiPass( const std::string &attribute ) {
	_cgi_pass = attribute;
	if (*_cgi_pass.begin() == '/')
		_cgi_pass.erase(0, 1);
	_cgi_pass = absolutePath + _cgi_pass;
	if (access(_cgi_pass.c_str(), X_OK) == -1)
		throw std::runtime_error("Syntax Error: cgi_pass Directive");
}

void	Directives::parseClientMaxBodySize( const std::string &attribute ) {
	size_t			pos = attribute.find_first_not_of("0123456789");
	char			c;

	_client_max_body_size = std::atoi(attribute.substr(0, pos).c_str());
	if (pos == std::string::npos)
		return ;
	else if (pos != attribute.length() - 1)
		throw std::runtime_error("Syntax Error: max_body_size Directive");
	c = attribute[pos];
	c = std::toupper(c);
	if (c == 'K')
		_client_max_body_size *= 1000;
	else if (c == 'M')
		_client_max_body_size *= 1000000;
	else
		throw std::runtime_error("Syntax Error: max_body_size Directive");
}

void	Directives::parseClientHeaderBufferSize( const std::string &attribute ) {
	size_t			pos = attribute.find_first_not_of("0123456789");
	char			c;

	_client_header_buffer_size = std::atoi(attribute.substr(0, pos).c_str());
	if (pos == std::string::npos)
		return ;
	else if (pos != attribute.length() - 1)
		throw std::runtime_error("Syntax Error: header_buffer_size Directive");
	c = attribute[pos];
	c = std::toupper(c);
	if (c == 'K')
		_client_header_buffer_size *= 1000;
	else if (c == 'M')
		_client_header_buffer_size *= 1000000;
	else
		throw std::runtime_error("Syntax Error: header_buffer_size Directive");
}

void	Directives::parseReturn( const std::string &attribute ) {
	int			code;
	std::string str;
	
	size_t pos = attribute.find_first_of(" \t");
	str = attribute.substr(0, pos);
	if (str.find_first_not_of("0123456789") != std::string::npos)
		throw std::runtime_error("Syntax Error: return Directive");
	code = atoi(str.c_str());
	if (code < 0 || code > 999)
		throw std::runtime_error("Syntax Error: return Directive");
	_return.first = code;
	if (pos == std::string::npos)
		return ;
	pos = attribute.find_first_not_of(" \t", pos);
	str = attribute.substr(pos, std::string::npos);
	if (str.find_first_of(" \t") != std::string::npos)
		throw std::runtime_error("Syntax Error: return Directive");
	_return.second = str;
}
