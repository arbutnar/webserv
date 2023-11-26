/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:24:46 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/26 17:19:43 by arbutnar         ###   ########.fr       */
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

void	Server::ListenerInit( void ) {
	_listener = socket(AF_INET, SOCK_STREAM, 0);
	if (_listener == -1)
		throw std::runtime_error("Cannot create socket");
	const int enable = 1;
	if (setsockopt(_listener, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    	throw std::runtime_error("setsockopt(SO_REUSEADDR) failed");
	struct sockaddr_in	sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(_listen_port);
	sockaddr.sin_addr.s_addr = htonl(_listen_host);
	if (bind(_listener, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) == -1)
		throw std::runtime_error("Cannot bind socket");
	if (listen(_listener, 128) == -1)
		throw std::runtime_error("Socket cannot Listen");
}

int	Server::nfds( void ) const {
	int temp = 0;

	for (v_cli::const_iterator it = _clients.begin(); it != _clients.end(); it++)
		if (temp < it->getSocket())
			temp = it->getSocket();
	return temp;
}

void	Server::newConnection( void ) {
	int client = accept(_listener, NULL, NULL);
	if (client == -1)
		throw std::runtime_error("Cannot create Client socket");
	_clients.push_back(client);
	std::cout << "ciao " << client << std::endl;
}

void	Server::readRequest( v_cli::iterator &it ) {
	char	c;
	int		nBytes = recv(it->getSocket(), &c, 1, 0);
	if (nBytes == -1)
		throw std::runtime_error("Cannot read from socket");
	else if (nBytes == 0) // forse <= a 0 perche' l'errore non serve
	{
		close(it->getSocket());
		_clients.erase(std::find(_clients.begin(), _clients.end(), *it));
		std::cout << it->getSocket() << " is disconnected" << std::endl;
	}
	else
		std::find(_clients.begin(), _clients.end(), *it)->buildBuffer(c);
}

void	Server::writeResponse( v_cli::iterator &it ) {
	Request 	request;
	Location	match;
	std::string	translation;
	try {
		request.parser(it->getBuffer());
		request.setIsValid(true);
		match = request.uriMatcher(_locations);
		if (match.getLocationName().empty())
			match = *findRoot();
		translation = request.translateUri(match);
		std::cout << translation << std::endl;

		// request.displayRequest();
	} catch(std::exception &e) {
		std::cout << "Bad Client request" << std::endl;
	}
	std::find(_clients.begin(), _clients.end(), *it)->clearBuffer();
}

void	Server::displayServer( void ) const {
	std::cout << "Server {" << std::endl;
	displayDirectives();
	for (s_locs::const_iterator it = _locations.begin(); it != _locations.end(); it++)
		it->displayLocation();
	std::cout << "}" << std::endl;
}
