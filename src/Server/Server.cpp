/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:24:46 by arbutnar          #+#    #+#             */
/*   Updated: 2024/01/08 19:09:20 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server( void ) {

}

Server::Server( const Server &src )
	: Directives() {
		*this = src;
}

Server& Server::operator=( const Server &src ) {
	if (this == &src)
		return *this;
	_locations = src._locations;
	Directives::operator=(src);
	return *this;
}

bool	Server::operator==( const Server &src ) const {
	if (src._listen_host != _listen_host || src._listen_port != _listen_port)
		return false;
	return true;
}

bool	Server::operator==( const std::string &serverName ) const {
	if (serverName != _server_name)
		return false;
	return true;
}

Server::~Server() {
}

const s_locs	&Server::getLocations( void ) const {
	return _locations;
}

void    Server::setLocations( const s_locs &locations ) {
    _locations = locations;
}

void    Server::addLocation( const Location &location ) {
	if (_locations.insert(location).second == false)
		throw Directives::SyntaxError();
}

void	Server::listenerInit( int &listener ) const {
	listener = -1;
	listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener == -1)
		throw std::runtime_error("Cannot create socket");
	if (fcntl(listener, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)
		throw std::runtime_error("Unable to make socket non-blocking");
	const int enable = 1;
	if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
		throw std::runtime_error("setsockopt(SO_REUSEADDR) failed");
	struct sockaddr_in	sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(_listen_port);
	sockaddr.sin_addr.s_addr = _listen_host;	// htonl();
	if (bind(listener, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) == -1)
		throw std::runtime_error("Cannot bind socket");
	if (listen(listener, 10) == -1)
		throw std::runtime_error("Socket cannot Listen");
	struct in_addr	addr;
	addr.s_addr = _listen_host;
	std::cout << "Socket " << listener << " binded to " << inet_ntoa(addr) << ":" << _listen_port << std::endl;
}

bool	Server::requestParser( Request &request, v_cli::iterator &it ) const {
	size_t		pos = it->getBuffer().find("\r\n");
	std::string line = it->getBuffer().substr(0, pos);
	if (std::count(line.begin(), line.end(), ' ') != 2)
		throw std::runtime_error("400");
	if (line.length() > _client_header_buffer_size)
		throw std::runtime_error("414");
	request.firstLineParser(line);
	pos += 2;
	line = it->getBuffer().substr(pos, it->getBuffer().find("\r\n\r\n") - pos + 2);
	if (line.length() > _client_header_buffer_size)
		throw std::runtime_error("400");
	request.headersParser(line);
	pos = it->getBuffer().find("\r\n\r\n") + 4;
	line = it->getBuffer().substr(pos, std::string::npos);
	if (!line.empty())
		request.bodyParser(line);
	if (_client_max_body_size != 0 && request.getBody().length() > _client_max_body_size)
		throw std::runtime_error("413");
	request.uriMatcher(_locations);
	request.translateUri();
	if (request.getMatch().getCgiAlias().empty())
		return false;
	it->handleCgi(request);
	it->setBuffer("");
	return true;
}

bool	Server::writeResponse( v_cli::iterator &it ) const {
	Response	response;

	try {
		if (it->getBuffer() == "empty buffer")
			throw std::runtime_error("500");
		if (it->getCgiPid() != 0)
		{
			response.cgiOutputParser(it->getBuffer());
		}
		else
		{
			if (requestParser(response, it))
				return false;
			response.methodHandler();
		}
	} catch( std::exception &e ) {
		response.setStatusCode(std::atoi(e.what()));
		response.generateErrorPage();
	}
	response.generateHeaders();
	it->setBuffer("");
	if (it->getCgiPid() != 0)
		it->setCgiPid(0);
	if (response.send(it->getSocket()))
		return true;
	if (response.getHeaders().at("Connection") == "close")
		return true;
	return false;
}

void	Server::displayServer( void ) const {
	std::cout << "Server {" << std::endl;
	displayDirectives();
	for (s_locs::const_iterator it = _locations.begin(); it != _locations.end(); it++)
		it->displayLocation();
	std::cout << "}" << std::endl;
}
