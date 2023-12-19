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
		_requests = src._requests;
		_listener = src._listener;
}

Server& Server::operator=( const Server &src ) {
    if (this == &src)
        return *this;
    _locations = src._locations;
	_requests = src._requests;
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
	if (_listener != 0)
		close(_listener);
	v_req::iterator it = _requests.begin();
	while ( it != _requests.end() )
		it = eraseRequest(it);
}

void    Server::setLocations( const s_locs &locations ) {
    _locations = locations;
}

void	Server::setListener( const int &listener ) {
	_listener = listener;
}

void	Server::setRequests( const v_req &connections ) {
	_requests = connections;
}

const s_locs	&Server::getLocations( void ) const {
    return _locations;
}

const int &Server::getListener( void ) const {
	return _listener;
}

const v_req	&Server::getRequests( void ) const {
	return _requests;
}

void    Server::addLocation( const Location &location ) {
	if (_locations.insert(location).second == false)
		throw Directives::SyntaxError();
}

int	Server::nfds( void ) const {
	int temp = 0;

	for (v_req::const_iterator it = _requests.begin(); it != _requests.end(); it++)
		if (temp < it->getSocket())
			temp = it->getSocket();
	return temp;
}

v_req::iterator	Server::eraseRequest( v_req::iterator &r_it ) {
	std::cout << r_it->getSocket() << " is disconnected" << std::endl;
	close(r_it->getSocket());
	return _requests.erase(r_it);
}

void	Server::displayServer( void ) const {
	std::cout << "Server {" << std::endl;
	displayDirectives();
	for (s_locs::const_iterator it = _locations.begin(); it != _locations.end(); it++)
		it->displayLocation();
	std::cout << "}" << std::endl;
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

void	Server::newRequest( void ) {
	int socket = accept(_listener, NULL, NULL);
	if (socket == -1)
		return ;
	if (fcntl(socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)
		return ;
	_requests.push_back(socket);
	std::cout << "Request " << socket << std::endl;
}

void	Server::menageRequest( const fd_set &read, const fd_set &write ) {
	v_req::iterator r_it = _requests.begin();
	while ( r_it != _requests.end() )
	{
		if (FD_ISSET(r_it->getSocket(), &write) && !r_it->getBuffer().empty())
		{
			writeResponse(r_it);
			r_it->displayRequest();
			exit(0);
			r_it++;
		}
		else if (FD_ISSET(r_it->getSocket(), &read))
		{
			if (r_it->buildBuffer() == false)
				r_it = eraseRequest(r_it);
			else
				r_it++;
		}
		else
			r_it++;
	}
}

void	Server::requestParser( v_req::iterator &r_it ) {
	size_t pos = r_it->getBuffer().find("\r\n");
	std::string line = r_it->getBuffer().substr(0, pos);
	if (line.length() > _client_header_buffer_size)
		throw std::runtime_error("414");
	r_it->firstLineParser(line);
	pos += 2;
	line = r_it->getBuffer().substr(pos, r_it->getBuffer().find("\r\n\r\n") - pos);
	if (line.length() > _client_header_buffer_size)
		throw std::runtime_error("400");
	r_it->headersParser(line);
	pos = r_it->getBuffer().find("\r\n\r\n") + 4;
	line = r_it->getBuffer().substr(pos, std::string::npos);
	r_it->bodyParser(line);
	if (r_it->getBody().length() > _client_max_body_size)
		throw std::runtime_error("413");
	std::cout << r_it->getBody() << std::endl;
	// if (_client_max_body_size == 0) _client_max_body_size viene disabilitato;
}

void	Server::writeResponse( v_req::iterator &r_it ) {
	try
	{
		requestParser(r_it);
	} catch(std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}
