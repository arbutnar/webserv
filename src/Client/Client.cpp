#include "Client.hpp"

Client::Client( void )
	: _socket (-1), _buffer (""), _cgiPid (0), _cgiFd (-1) {
}

Client::Client( const int &socket )
	: _socket (socket), _buffer (""), _cgiPid (0), _cgiFd (-1) {
}

Client::Client( const Client &src ) {
	*this = src;
}

Client &Client::operator=( const Client &src ) {
	if (this == &src)
		return *this;
	_socket = src._socket;
	_buffer = src._buffer;
	_cgiPid = src._cgiPid;
	_cgiFd = src._cgiFd;
	return *this;
}

Client::~Client() {
	std::cout << _socket << " is disconnected" << std::endl;
	close(_socket);
	close(_cgiFd);
	if (!_cgiPid)
		kill(_cgiPid, SIGKILL);
}

const int	&Client::getSocket( void ) const {
	return _socket;
}

std::string	&Client::getBuffer( void ) {
	return _buffer;
}

const int	&Client::getCgiPid( void ) const {
	return _cgiPid;
}

const int	&Client::getCgiFd( void ) const {
	return _cgiFd;
}

void	Client::setSocket( const int &socket ) {
	_socket = socket;
}

void	Client::setBuffer( const std::string &buffer ) {
	_buffer = buffer;
}

void	Client::setCgiPid( const int &cgiPid ) {
	_cgiPid = cgiPid;
}

void	Client::setCgiFd( const int &cgiFd ) {
	_cgiFd = cgiFd;
}

bool	Client::buildBuffer( const int &socket ) {
	char	tmpBuffer[200001];
	int		nBytes = 0;

	nBytes = read(socket, tmpBuffer, 200000);
	if (nBytes <= 0)
		return true;
	tmpBuffer[nBytes] = '\0';
	_buffer += tmpBuffer;
	return false;
}

void	Client::handleCgi( Request &request ) {
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
			_cgiFd = pipeOut[0];
			return ;
		}
	}
	close(pipeOut[0]);
	kill(pid, SIGKILL);
	freeMatrix(args);
	freeMatrix(env);
	throw std::runtime_error("500");
}
