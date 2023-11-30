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
	_listen_host = 2130706433;	// 127*2^24 + 0*2^16 + 0*2^8 + 1
	_listen_port = 8080;
	_server_name = "";
	_root = "";
	_alias = "";
	_index.clear();
	_index.push_back("index.html");
	_autoindex = false;
	_scgi_pass = "";
	_try_files.clear();
	_error_page.clear();
	_client_max_body_size = 1000000;
	_return.clear();
	_limit_except.clear();
	_limit_except.insert(std::make_pair("GET", true));
	_limit_except.insert(std::make_pair("HEAD", true));
	_limit_except.insert(std::make_pair("POST", true));
	_limit_except.insert(std::make_pair("PUT", true));
	_limit_except.insert(std::make_pair("DELETE", true));
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
	_scgi_pass = src._scgi_pass;
	_try_files = src._try_files;
	_limit_except = src._limit_except;
	_error_page = src._error_page;
	_client_max_body_size = src._client_max_body_size;
	_return = src._return;
	return *this;
}

void	Directives::directiveParser( std::string line ) {
	std::string	arr[] = { "listen", "server_name", "root", "alias", "index", "autoindex", "scgi_pass", "try_files", 
							"limit_except", "error_page", "client_max_body_size", "return" };
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
	value = line.substr(0, line.find(';'));
	if (value.empty())
		throw Directives::SyntaxError();
	switch (i) {
		case LISTEN:
			parseListen(value); break;
		case SERVER_NAME:
			_server_name = value; break;
		case ROOT:
			parseRoot(value); break;
		case ALIAS:
			parseAlias(value); break;
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
			parseScgiPass(value); break;
		case CLI_MAX_SIZE:
			parseClientMaxBodySize(value); break;
		case RETURN:
			parseReturn(value); break;
		default:
			throw std::runtime_error("Syntax Error: not supported Directive");
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
	
	if (key.empty() || value.empty() || key.find_first_not_of("0123456789") != std::string::npos)
		throw std::runtime_error("Syntax Error: error_page Directive");
	_error_page.insert(std::make_pair(std::atoi(key.c_str()), value));
}

void	Directives::parseScgiPass( const std::string &attribute ) {
	_scgi_pass = attribute;
	if (*_scgi_pass.begin() == '/')
		_scgi_pass.erase(0, 1);
	_scgi_pass = absolutePath + _scgi_pass;
	if (access(_scgi_pass.c_str(), X_OK) == -1)
		throw std::runtime_error("Syntax Error: scgi_pass Directive");
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

void	Directives::parseReturn( const std::string &attribute ) {
	int count = 0;

	for (size_t i = 0; i < attribute.size(); i++)
		if (attribute.at(i) == ' ' || attribute.at(i) == '\t')
			count++;
	if (count == 1)
	{
		size_t pos = attribute.find_first_of(' ');
		std::string	code = attribute.substr(0, pos);
		if (std::atoi(code.c_str()))
			_return.push_back(attribute.substr(0, pos));
		else
			throw std::runtime_error("Syntax Error: return Directive");
		pos = attribute.find_first_not_of(' ', pos);
		std::string uri = attribute.substr(pos, std::string::npos);
		if (!std::atoi(uri.c_str()))
			_return.push_back(uri);
		else
			throw std::runtime_error("Syntax Error: return Directive");
	}
	else if (count < 1)
		_return.push_back(attribute);
	else
		throw std::runtime_error("Syntax Error: return Directive");
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
	std::cout << "Scgi Pass: " << _scgi_pass << std::endl;
	std::cout << "Try Files: ";
	for (v_str::const_iterator it = _try_files.begin(); it != _try_files.end(); it++)
		std::cout << *it << ' ';
	std::cout << std::endl;
	std::cout << "Allowed Methods: ";
	for (m_strBool::const_iterator it = _limit_except.begin(); it != _limit_except.end(); it++)
		if (it->second == true)
			std::cout << it->first << ' ';
	std::cout << std::endl;
	std::cout << "Error Page: " << std::endl;
	for (m_intStr::const_iterator it = _error_page.begin(); it != _error_page.end(); it++)
		std::cout << it->first << ' ' << it->second << std::endl;
	std::cout << "Client Max Body Size: " << _client_max_body_size << std::endl;
	std::cout << "Return: ";
	for (v_str::const_iterator it = _return.begin(); it != _return.end(); it++)
		std::cout << *it << ' ';
	std::cout << std::endl;
}

void	Directives::addLocation( const Location &location ) {		//implemented only to avoid hpp files recurion.
	(void) location;
}
