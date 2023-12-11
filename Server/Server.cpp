/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:24:46 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/11 17:41:10 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server( void ) {
}

Server::Server( std::string block ) {
    std::cout << block << std::endl;
}

Server::Server( const Server &src )
    : Directives(src) {
        _locations = src._locations;
		_clients = src._clients;
		_listener = src._listener;
}

Server& Server::operator=( const Server &src ) {
    if (this == &src)
        return *this;
    _locations = src._locations;
	_clients = src._clients;
	_listener = src._listener;
    Directives::operator=(src);
    return *this;
}

bool	Server::operator<( const Server &src ) {
	if (_server_name.compare(src._server_name) < 0)
		return true;
	return false;
}

Server::~Server() {
}

void    Server::setLocations( const s_locs &locations ) {
    _locations = locations;
}

void	Server::setListener( const int &listener ) {
	_listener = listener;
}

void	Server::setClients( const v_cli &clients ) {
	_clients = clients;
}

const s_locs	&Server::getLocations( void ) const {
    return _locations;
}

const int &Server::getListener( void ) const {
	return _listener;
}

const v_cli	&Server::getClients( void ) const {
	return _clients;
}

void    Server::addLocation( const Location &location ) {
	if (_locations.insert(location).second == false)
		throw Directives::SyntaxError();
}

s_locs::const_iterator	Server::findRoot( void ) const {
	for (s_locs::const_iterator it = _locations.begin(); it != _locations.end(); it++)
		if (it->getLocationName() == "/")
			return it;
	return _locations.end();
}

int	Server::nfds( void ) const {
	int temp = 0;

	for (v_cli::const_iterator it = _clients.begin(); it != _clients.end(); it++)
		if (temp < it->getSocket())
			temp = it->getSocket();
	return temp;
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
	int client = accept(_listener, NULL, NULL);
	if (client == -1)
		return ;
	if (fcntl(client, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)
		return ;
	_clients.push_back(client);
	std::cout << "ciao " << client << std::endl;
}

void	Server::clientInteraction( const fd_set &active ) {
	v_cli::iterator it = _clients.begin();
	while (it != _clients.end())
	{
		if (FD_ISSET(it->getSocket(), &active) && !it->writeRequest())
			it = eraseClient(it);
		else if (it->getBuffer().find("\r\n\r\n") != std::string::npos && !writeResponse(it))
			it = eraseClient(it);
		else
			it++;
	}
}

bool	Server::writeResponse( v_cli::iterator &c_it ) {
	Request 	request;
	Response	*response = NULL;
	bool		ret = true;

	try {
		std::cout << c_it->getBuffer() << std::endl;
		bufferChecker(c_it->getBuffer());
		request.headersParser(c_it->getBuffer());
		request.headersChecker();
		request.uriMatcher(_locations);
		request.matchChecker();
		request.translateUri();
		request.bodyParser(c_it->getSocket());
		response = new Valid(request);
		response->generateBody();
	} catch(std::exception &e) {
		response = new Error(e.what());
		response->generateBody();
	}
	response->generateHeaders(request);
	response->send(c_it->getSocket());
	if (response->getHeaders().at("Connection") == "close")
		ret = false;
	c_it->clearBuffer();
	delete response;
	return ret;
}

void	Server::bufferChecker( const std::string &buffer ) const {
	std::stringstream	ss(buffer);
	std::string			line;

	std::getline(ss, line);
	if (line.find_first_of(" ") == std::string::npos)
		throw std::runtime_error("400");
	if (line.length() > _client_header_buffer_size)
		throw std::runtime_error("414");
	if (ss.str().length() - line.length() > _client_header_buffer_size)
		throw std::runtime_error("494");
}

v_cli::iterator	Server::eraseClient( v_cli::iterator &c_it ) {
	std::cout << c_it->getSocket() << " is disconnected" << std::endl;
	close(c_it->getSocket());
	return _clients.erase(c_it);
}

void	Server::displayServer( void ) const {
	std::cout << "Server {" << std::endl;
	displayDirectives();
	for (s_locs::const_iterator it = _locations.begin(); it != _locations.end(); it++)
		it->displayLocation();
	std::cout << "}" << std::endl;
}
