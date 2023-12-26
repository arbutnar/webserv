#include "Cgi.hpp"

Cgi::Cgi( void )
	: _readFd (0), _cliSock (0) {
}

Cgi::Cgi( const Cgi &src ) {
	*this = src;
}

Cgi	&Cgi::operator=( const Cgi &src ) {
	if (this == &src)
		return *this;
	_readFd = src._readFd;
	_cliSock = src._cliSock;
	return *this;
}

Cgi::~Cgi( void ) {
	if (_readFd != 0)
		close(_readFd);
	if (_cliSock != 0)
		close(_cliSock);
}

const int	&Cgi::getReadFd( void ) const {
	return _readFd;
}

const int	&Cgi::getCliSock( void ) const {
	return _cliSock;
}

void	Cgi::setReadFd( const int &readFd ) {
	_readFd = readFd;
}

void	Cgi::setCliSock( const int &cliSock ) {
	_cliSock = cliSock;
}

// bool	Cgi::writeResponse( const std::string &clientBuffer ) {
// 	Response	*response = NULL;
// 	std::string	key;
// 	std::string	value;
// 	m_strStr	headers;

// 	try
// 	{
// 		if (clientBuffer == "empty buffer")
// 			throw std::runtime_error("500");
// 		while (std::getline(std::getline(ss, key, ':') >> std::ws, value))
// 		{
// 			if (value.at(value.size() - 1) == '\r')
// 				value.erase(value.size() - 1, 1);
// 			if (headers.find(key) != headers.end() || (key.empty() || value.empty()))
// 				throw std::runtime_error("500");
// 			if (key.find_first_of(" \t") != std::string::npos)
// 				throw std::runtime_error("500");
// 			headers.insert(std::make_pair(key, value));
// 		}
// 		if ("")
// 		response = new Valid(request);
// 		response->handleByMethod();
// 	} catch(std::exception &e) {
// 		response = new Error(e.what(), request);
// 		response->defaultErrorPage();
// 	}
// 	std::time_t	now = time(0);
// 	struct tm	tstruct;
// 	headers.insert(std::make_pair("Server", "42webserv"));
// 	tstruct = *localtime(&now);
// 	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S", &tstruct);
// 	headers.insert(std::make_pair("Date", buf));
// 	headers.insert(std::make_pair("Content-Type", "text/html"));
// 	std::stringstream ss;
// 	ss << _body.length();
// 	headers.insert(std::make_pair("Content-Length", ss.str()));
// 	response->setHeaders(headers);
// 	response->send(c_it->first);
// 	c_it->second.clear();
// 	delete response;
// 	return false;
// }

void	Cgi::handleCgi( Request &request ) {
	char*		args[2];
	char*		env[4];
	int			pipe_in[2];
	int			pipe_out[2];
	pid_t		pid;

	if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1)
		throw std::runtime_error("500");
	args[0] = strdup(request.getTranslate().c_str());
	args[1] = NULL;
	env[0] = strdup("SERVER_PROTOCOL=HTTP/1.1");
	env[1] = strdup(("REQUEST_METHOD=" + request.getMethod()).c_str());
	env[2] = strdup(("PATH_INFO=" + request.getUri()).c_str());
	env[3] = NULL;
	pid = fork();
	if (pid < 0)
		throw std::runtime_error("500");
	else if (pid == 0)
	{
		dup2(pipe_in[0], 0);
		dup2(pipe_out[1], 1);
		close(pipe_in[0]);
		close(pipe_in[1]);
		close(pipe_out[0]);
		close(pipe_out[1]);
		execve(args[0], args, env);
		throw std::runtime_error("500");
	}
	close(pipe_in[0]);
	close(pipe_out[1]);
	write(pipe_in[1], request.getBody().c_str(), request.getBody().size());
	close(pipe_in[1]);
	_readFd = pipe_out[0];
	for(int i = 0; args[i]; i++)
		free(args[i]);
	for(int i = 0; env[i]; i++)
		free(env[i]);
	waitpid(-1, NULL, 0);
}

void	Cgi::clear( void ) {
	if (_readFd != 0)
		close(_readFd);
	if (_cliSock != 0)
		close(_cliSock);
	_readFd = 0;
	_cliSock = 0;
}