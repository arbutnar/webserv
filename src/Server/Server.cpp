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

Server::Server( void )
	: _listener (0) {
}

Server::Server( const Server &src )
	: Directives() {
		*this = src;
}

Server& Server::operator=( const Server &src ) {
	if (this == &src)
		return *this;
	_locations = src._locations;
	_listener = src._listener;
	_connections = src._connections;
	_cgi = src._cgi;
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

const p_intInt	&Server::getCgi( void ) const {
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

void	Server::setCgi( const p_intInt &cgi ) {
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
	if (temp < _cgi.first)
		temp = _cgi.first;
	if (temp < _listener)
		temp = _listener;
	return temp;
}

void	Server::eraseConnection( m_intStr::iterator &c_it ) {
	int	socket = c_it->first;
	std::cout << socket << " is disconnected" << std::endl;
	c_it++;
	_connections.erase(socket);
	if (_cgi.second == socket)
	{
		_cgi.second = 0;
		close(_cgi.first);
		_cgi.first = 0;
	}
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
	sockaddr.sin_addr.s_addr = _listen_host;	// htonl();
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
	std::cout << "Connection " << socket << " on port " << _listen_port << std::endl;
}

bool	Server::buildBuffer( const int &socket, std::string &buffer ) {
	char	tmpBuffer[200001];
	int		nBytes = 0;

	nBytes = read(socket, tmpBuffer, 200000);
	if (nBytes <= 0)
		return true;
	tmpBuffer[nBytes] = '\0';
	buffer += tmpBuffer;
	return false;
}

void	Server::menageConnection( const fd_set &read, const fd_set &write ) {
	m_intStr::iterator c_it;

	if (FD_ISSET(_cgi.first, &read))
	{
		c_it = _connections.find(_cgi.second);
		if (c_it == _connections.end())
		{
			close(_cgi.first);
			_cgi.first = 0;
			_cgi.second = 0;
		}
		else if (buildBuffer(_cgi.first, c_it->second))
		{
			if (c_it->second.empty())
				c_it->second = "empty buffer";
			close(_cgi.first);
			_cgi.first = 0;
		}
		return ;
	}
	c_it = _connections.begin();
	while (c_it != _connections.end())
	{
		if (FD_ISSET(c_it->first, &read))
		{
			if (buildBuffer(c_it->first, c_it->second))
				eraseConnection(c_it);
			break ;
		}
		else if (FD_ISSET(c_it->first, &write) && !c_it->second.empty())
		{
			if (writeResponse(c_it))
				eraseConnection(c_it);
			break ;
		}
		else
			c_it++;
	}
}

void	Server::handleCgi( Request &request ) {
	char*		args[2];
	char*		env[4];
	int			pipeIn[2];
	int			pipeOut[2];
	int			nBytes;
	int			status;
	pid_t		pid;

	struct stat	st;
	if (stat(request.getTranslate().c_str(), &st) == -1)
		throw std::runtime_error("404");
	if (!(st.st_mode & S_IFREG))
		throw std::runtime_error("403");
	if (pipe(pipeIn) == -1 || pipe(pipeOut) == -1)
		throw std::runtime_error("500");
	args[0] = strdup(request.getTranslate().c_str());
	args[1] = NULL;
	env[0] = strdup("SERVER_PROTOCOL=HTTP/1.1");
	env[1] = strdup(("REQUEST_METHOD=" + request.getMethod()).c_str());
	env[2] = strdup(("PATH_INFO=" + request.getUri()).c_str());
	env[3] = NULL;
	pid = fork();
	if (pid < 0)
	{
		freeMatrix(args);
		freeMatrix(env);
		closePipe(pipeIn);
		closePipe(pipeOut);
		throw std::runtime_error("500");
	}
	else if (pid == 0)
	{
		dup2(pipeIn[0], 0);
		dup2(pipeOut[1], 1);
		closePipe(pipeIn);
		closePipe(pipeOut);
		execve(args[0], args, env);
		exit(-1);
	}
	close(pipeIn[0]);
	close(pipeOut[1]);
	nBytes = write(pipeIn[1], request.getBody().c_str(), request.getBody().size());
	close(pipeIn[1]);
	if (nBytes >= 0)
	{
		waitpid(pid, &status, 0);
		if(WIFEXITED(status) && WEXITSTATUS(status) == 0)
		{
			freeMatrix(args);
			freeMatrix(env);
			_cgi.first = pipeOut[0];
			return ;
		}
	}
	close(pipeOut[0]);
	kill(pid, SIGKILL);
	freeMatrix(args);
	freeMatrix(env);
	throw std::runtime_error("500");
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
	if (!line.empty())
		request.bodyParser(line);
	if (_client_max_body_size != 0 && request.getBody().length() > _client_max_body_size)
		throw std::runtime_error("413");
	request.uriMatcher(_locations);
	request.translateUri();
	if (request.getMatch().getCgiAlias().empty())
		return false;
	handleCgi(request);
	_cgi.second = c_it->first;
	c_it->second.clear();
	return true;
}

bool	Server::writeResponse( m_intStr::iterator &c_it ) {
	bool		isClose = false;
	Request		request;
	Response	*response = NULL;

	try {
		std::cout << c_it->second << std::endl;
		if (c_it->second == "empty buffer")
			throw std::runtime_error("500");
		if (_cgi.second != c_it->first)
		{
			if (requestParser(request, c_it))
				return false;
			response = new Valid(request);
			(dynamic_cast<Valid *>(response))->methodHandler();
		}
		else
		{
			response = new Valid();
			(dynamic_cast<Valid *>(response))->cgiOutputParser(c_it->second);
		}
	} catch( std::exception &e ) {
		if (response != NULL)
			delete response;
		response = new Error(e.what(), request);
		(dynamic_cast<Error *>(response))->defaultErrorPage();
	}
	response->generateHeaders();
	if (response->send(c_it->first))
		isClose = true;
	c_it->second.clear();
	if (_cgi.second != 0)
		_cgi.second = 0;
	if (response->getHeaders().at("Connection") == "close")
		isClose = true;
	delete response;
	return isClose;
}

void	Server::displayServer( void ) const {
	std::cout << "Server {" << std::endl;
	displayDirectives();
	for (s_locs::const_iterator it = _locations.begin(); it != _locations.end(); it++)
		it->displayLocation();
	std::cout << "}" << std::endl;
}
