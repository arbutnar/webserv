#include "Cgi.hpp"

Cgi::Cgi( void )
	: _output (0), _cliSock (0) {
}

Cgi::Cgi( const Cgi &src ) {
	*this = src;
}

Cgi	&Cgi::operator=( const Cgi &src ) {
	if (this == &src)
		return *this;
	_output = src._output;
	_cliSock = src._cliSock;
	return *this;
}

~Cgi::Cgi( void ) {
	if (_output != 0)
		close(_output);
}

const int	&Cgi::getOutput( void ) const {
	return _output;
}

const int	&Cgi::getCliSock( void ) const {
	return _cliSock;
}

void	Cgi::setOutput( const int &output ) {
	_output = output;
}

void	Cgi::setCliSock( const int &cliSock ) {
	_cliSock = cliSock;
}

void	Cgi::handleCgi( int cliSock, Request &request ) {
	char*		args[3];
	char*		env[4];
	int			pipe_in[2];
	int			pipe_out[2];
	pid_t		pid;

	pipe(pipe_in);
	pipe(pipe_out);
	args[0] = strdup(request.getMatch().getCgiPass().c_str());
	args[1] = strdup(request.getTranslate().c_str());
	args[2] = NULL;
	env[0] = strdup("SERVER_PROTOCOL=HTTP/1.1");
	env[1] = strdup(("REQUEST_METHOD=" + request.getMethod()).c_str());
	env[2] = strdup(("PATH_INFO=" + request.getUri()).c_str());
	env[3] = NULL;
	pipe(pipe_in);
	pipe(pipe_out);
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
	_output = pipe_out[0];
	_cliSock = cliSock;
	for(int i = 0; args[i]; i++)
		free(args[i]);
	for(int i = 0; env[i]; i++)
		free(env[i]);
}