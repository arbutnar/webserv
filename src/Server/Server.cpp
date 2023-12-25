/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:24:46 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/14 15:18:46 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server( void ) {
	_listener = 0;
}

Server::Server( const Server &src )
	: Directives(src) {
		_locations = src._locations;
		_listener = src._listener;
		_connections = src._connections;
}

Server& Server::operator=( const Server &src ) {
	if (this == &src)
		return *this;
	_locations = src._locations;
	_listener = src._listener;
	_connections = src._connections;
	Directives::operator=(src);
	return *this;
}

bool	Server::operator<( const Server &src ) {
	if (_server_name.compare(src._server_name) < 0)
		return true;
	return false;
}

Server::~Server() {
	if (_listener != 0)
		close(_listener);
	m_intStr::iterator it = _connections.begin();
	while ( it != _connections.end() )
		eraseConnection(it);
}

const s_locs	&Server::getLocations( void ) const {
	return _locations;
}

const int &Server::getListener( void ) const {
	return _listener;
}

const m_intStr	&Server::getConnections( void ) const {
	return _connections;
}

const Cgi	&Server::getCgi( void ) const {
	return _cgi;
}

void    Server::setLocations( const s_locs &locations ) {
    _locations = locations;
}

void	Server::setListener( const int &listener ) {
	_listener = listener;
}

void	Server::setConnections( const m_intStr &connections ) {
	_connections = connections;
}

void	Server::setCgi( const Cgi &cgi ) {
	_cgi = cgi;
}

void    Server::addLocation( const Location &location ) {
	if (_locations.insert(location).second == false)
		throw Directives::SyntaxError();
}

int	Server::nfds( void ) const {
	int temp = 0;

	for (m_intStr::const_iterator it = _connections.begin(); it != _connections.end(); it++)
		if (temp < it->first)
			temp = it->first;
	if (temp < _cgi.getOutput())
		temp = _cgi.getOutput();
	if (temp < _listener)
		temp = _listener;
	return temp;
}

void	Server::eraseConnection( m_intStr::iterator &c_it ) {
	int	socket = c_it->first;
	std::cout << socket << " is disconnected" << std::endl;
	c_it++;
	_connections.erase(socket);
	close(socket);
}

void	Server::ListenerInit( void ) {
	_listener = socket(AF_INET, SOCK_STREAM, 0);
	if (_listener == -1)
		throw std::runtime_error("Cannot create socket");
	if (fcntl(_listener, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)
		throw std::runtime_error("Unable to make socket non-blocking");
	const int enable = 1;
	if (setsockopt(_listener, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		throw std::runtime_error("setsockopt(SO_REUSEADDR) failed");
	struct sockaddr_in	sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(_listen_port);
	sockaddr.sin_addr.s_addr = htonl(_listen_host);
	if (bind(_listener, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) == -1)
		throw std::runtime_error("Cannot bind socket");
	if (listen(_listener, 10) == -1)
		throw std::runtime_error("Socket cannot Listen");
}

void	Server::newConnection( void ) {
	int socket = accept(_listener, NULL, NULL);
	if (socket == -1)
		return ;
	if (fcntl(socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)
		return ;
	_connections.insert(std::make_pair(socket, ""));
	std::cout << "Connection " << socket << std::endl;
}

bool	Server::buildBuffer( const int &socket, std::string &buffer ) {
	char	tmp_buffer[200001];
	int		n_bytes = 0;
	n_bytes = read(socket, tmp_buffer, 200000);
	if (n_bytes <= 0)
		return false;
	tmp_buffer[n_bytes] = '\0';
	buffer += tmp_buffer;
	return true;
}

void	Server::menageConnection( const fd_set &read, const fd_set &write ) {
	m_intStr::iterator c_it;
	if (FD_ISSET(_cgi.getOutput(), &read))
	{
		c_it = _connections.find(_cgi.getCliSock());
		if (c_it == _connections.end())
			close(_cgi.getOutput());
		else if (buildBuffer(_cgi.getOutput(), c_it->second) == false)
		{
			close(_cgi.getOutput());
			_cgi.setOutput(0);
		}
		return ;
	}
	c_it = _connections.begin();
	while (c_it != _connections.end())
	{
		if (FD_ISSET(c_it->first, &read))
		{
			if (buildBuffer(c_it->first, c_it->second) == false)
				eraseConnection(c_it);
			break ;
		}
		else if (FD_ISSET(c_it->first, &write) && !c_it->second.empty())
		{
			if (writeResponse(c_it) == false)
				eraseConnection(c_it);
			break ;
		}
		else
			c_it++;
	}
}

bool	Server::requestParser( Request &request, m_intStr::iterator &c_it ) {
	size_t		pos = c_it->second.find("\r\n");
	std::string line = c_it->second.substr(0, pos);
	if (std::count(line.begin(), line.end(), ' ') != 2)
		throw std::runtime_error("400");
	if (line.length() > _client_header_buffer_size)
		throw std::runtime_error("414");
	request.firstLineParser(line);
	pos += 2;
	line = c_it->second.substr(pos, c_it->second.find("\r\n\r\n") - pos);
	if (line.length() > _client_header_buffer_size)
		throw std::runtime_error("400");
	request.headersParser(line);
	pos = c_it->second.find("\r\n\r\n") + 4;
	line = c_it->second.substr(pos, std::string::npos);
	request.bodyParser(line);
	if (_client_max_body_size != 0 && request.getBody().length() > _client_max_body_size)
		throw std::runtime_error("413");
	request.uriMatcher(_locations);
	request.translateUri();
	if (!request.getMatch().getCgiAlias().empty())
		return true;
	return false;
}

bool	Server::writeResponse( m_intStr::iterator &c_it ) {
	Request		request;
	Response	*response = NULL;
	try
	{
		std::cout << c_it->second << std::endl;
		if (requestParser(request, c_it))
		{
			_cgi.handleCgi(c_it->first, request);
			c_it->second.clear();
			return true;
		}
		response = new Valid(request);
		response->handleByMethod();
	} catch(std::exception &e) {
		response = new Error(e.what(), request);
		response->generateBody();
	}
	response->generateHeaders();
	response->send(c_it->first);
	c_it->second.clear();
	if (response->getHeaders().at("Connection") == "close")
	{
		delete response;
		return false;
	}
	delete response;
	return true;
}

void	Server::displayServer( void ) const {
	std::cout << "Server {" << std::endl;
	displayDirectives();
	for (s_locs::const_iterator it = _locations.begin(); it != _locations.end(); it++)
		it->displayLocation();
	std::cout << "}" << std::endl;
}
