/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Worker.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 18:45:31 by marvin            #+#    #+#             */
/*   Updated: 2024/01/07 18:45:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Worker.hpp"

Worker::Worker( void )
	: _listener (-1) {
}

Worker::Worker( const int &listener, const v_ser &servers )
	: _listener (listener), _servers (servers) {
}

Worker::Worker( const Worker &src ) {
	*this = src;
}

Worker	&Worker::operator=( const Worker &src ) {
	if (this == &src)
		return *this;
	_listener = src._listener;
	_servers = src._servers;
	_clients = src._clients;
	return *this;
}

Worker::~Worker( void ) {
}

const int &Worker::getListener( void ) const {
	return _listener;
}

const v_ser	&Worker::getServers( void ) const {
	return _servers;
}

const v_cli	&Worker::getClients( void ) const {
	return _clients;
}

void	Worker::setListener( const int &listener ) {
	_listener = listener;
}

void	Worker::setServers( const v_ser &servers ) {
	_servers = servers;
}

void	Worker::setClients( const v_cli &clients ) {
	_clients = clients;
}

int	Worker::nfds( void ) const {
	int temp = _listener;

	for (v_cli::const_iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (temp < it->getSocket())
			temp = it->getSocket();
		if (temp < it->getCgiFd())
			temp = it->getCgiFd();
	}
	return temp;
}

v_cli::iterator	Worker::removeClient( v_cli::iterator &it ) {
	std::cout << it->getSocket() << " is disconnected" << std::endl;
	close(it->getSocket());
	if (it->getCgiFd() != -1)
		close(it->getCgiFd());
	if (it->getCgiPid() != 0)
		kill(it->getCgiPid(), SIGKILL);
	return _clients.erase(it);
}

void	Worker::removeAllClients( void ) {
	v_cli::iterator it = _clients.begin();
	while (it != _clients.end())
		it = removeClient(it);
}

void	Worker::acceptNewClient( void ) {
	int socket = accept(_listener, NULL, NULL);
	if (socket == -1)
		return ;
	if (fcntl(socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)
	{
		close(socket);
		return ;
	}
	_clients.push_back(socket);
	std::cout << "Client " << socket << " on listener " << _listener << std::endl;
}

v_ser::const_iterator	Worker::serverLinker( const std::string &buffer ) const {

	std::string	host;
	v_ser::const_iterator it;
	size_t pos = buffer.substr(0, buffer.find("\r\n\r\n")).find("Host:");
	if (pos == std::string::npos)
		return _servers.begin();
	pos += 5;
	pos = buffer.find_first_not_of(" \t", pos);
	host = buffer.substr(pos, buffer.find_first_of(" \t\r\n", pos) - pos);
	it = std::find(_servers.begin(), _servers.end(), host);
	if (it != _servers.end())
		return it;
	return _servers.begin();
}

void	Worker::menageClient( const fd_set &read, const fd_set &write ) {
	v_cli::iterator it;

	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->getCgiFd() != -1 && FD_ISSET(it->getCgiFd(), &read))
		{
			if (kill(it->getCgiPid(), 0) != -1)
				continue ;
			if (it->buildBuffer(it->getCgiFd()))
			{
				if (it->getBuffer().empty())
					it->setBuffer("empty buffer");
				close(it->getCgiFd());
				it->setCgiFd(-1);
			}
			return ;
		}
	}
	
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		if (FD_ISSET(it->getSocket(), &read))
		{
			if (it->buildBuffer(it->getSocket()))
				removeClient(it);
			return ;
		}
		else if (FD_ISSET(it->getSocket(), &write) && !it->getBuffer().empty())
		{
			if (serverLinker(it->getBuffer())->writeResponse(it))
				removeClient(it);
			return ;
		}
	}
}

void	Worker::displayWorker( void ) const {
	std::cout << "[CLUSTER]" << std::endl;
	for (v_ser::const_iterator it = _servers.begin(); it != _servers.end(); it++)
		it->displayServer();
}