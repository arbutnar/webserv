#include "Cluster.hpp"

Cluster::Cluster( void )
	: _listener (-1) {
}

Cluster::Cluster( const int &listener, const v_ser &servers )
	: _listener (listener), _servers (servers) {
}

Cluster::Cluster( const Cluster &src ) {
	*this = src;
}

Cluster	&Cluster::operator=( const Cluster &src ) {
	if (this == &src)
		return *this;
	_listener = src._listener;
	_servers = src._servers;
	_clients = src._clients;
	return *this;
}

Cluster::~Cluster( void ) {
}

const int &Cluster::getListener( void ) const {
	return _listener;
}

const v_ser	&Cluster::getServers( void ) const {
	return _servers;
}

const v_cli	&Cluster::getClients( void ) const {
	return _clients;
}

void	Cluster::setListener( const int &listener ) {
	_listener = listener;
}

void	Cluster::setServers( const v_ser &servers ) {
	_servers = servers;
}

void	Cluster::setClients( const v_cli &clients ) {
	_clients = clients;
}

int	Cluster::nfds( void ) const {
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

v_cli::iterator	Cluster::removeClient( v_cli::iterator &it ) {
	std::cout << it->getSocket() << " is disconnected" << std::endl;
	close(it->getSocket());
	close(it->getCgiFd());
	if (it->getCgiPid() != 0)
		kill(it->getCgiPid(), SIGKILL);
	return _clients.erase(it);
}

void	Cluster::removeAllClients( void ) {
	v_cli::iterator it = _clients.begin();
	while (it != _clients.end())
		it = removeClient(it);
}

void	Cluster::acceptNewClient( void ) {
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

// void	Cluster::serverIdentifier( const std::string &buffer ) {
// 	size_t	pos;

// 	if ((pos = buffer.find("Host: ")) == buffer.end())
// 	{
// 		Error	error("400");
// 	}
// 	for (v_ser::iterator it = _servers.begin(); it != _servers)
// }

void	Cluster::menageClient( const fd_set &read, const fd_set &write ) {
	v_cli::iterator it;

	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		if (FD_ISSET(it->getCgiFd(), &read))
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
			//serverIdentifier(it->getBuffer());
			if (_servers.begin()->writeResponse(it))
				it = removeClient(it);
			return ;
		}
	}
}

void	Cluster::displayCluster( void ) const {
	std::cout << "[CLUSTER]" << std::endl;
	for (v_ser::const_iterator it = _servers.begin(); it != _servers.end(); it++)
		it->displayServer();
}