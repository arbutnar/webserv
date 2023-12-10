/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Valid.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 11:48:05 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/09 18:14:30 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Valid.hpp"

Valid::Valid( void )
	: Response ("200 Ok") {
}

Valid::Valid( const Request &request )
	:  Response ("200 Ok"), _request(request) {
		if (_request.getMatch().getLimitExcept().at(_request.getMethod()) == false)
			throw std::runtime_error("405");
		if (_request.getMethod() == "GET" || _request.getMethod() == "HEAD")
			_file.open(request.getTranslate().c_str(), std::fstream::in);
		if (_request.getMethod() == "PUT" || _request.getMethod() == "POST")
		{
			struct stat st;
			if (stat(request.getTranslate().c_str(), &st) == 0 && st.st_mode & S_IFDIR)
				throw std::runtime_error("409");
			_status = "201 Created";
			if (stat(request.getTranslate().c_str(), &st) == 0)
				_status = "204 No Content";
			_file.open(request.getTranslate().c_str(), std::fstream::out);
		}
		if (!_file.is_open() && _request.getMethod() != "DELETE")
			throw std::runtime_error("500");
		else if (_request.getMethod() == "DELETE")
			if ((remove(_request.getTranslate().c_str())) == 0)
				_status = "204 No Content";
}

Valid::Valid( const Valid &src )
	: Response(src) {
}

Valid	&Valid::operator=( const Valid &src ) {
	if (this == &src)
		return *this;
	Response::operator=(src);
	_request = src._request;
	return *this;
}

Valid::~Valid( ) {
}

const Request	&Valid::getRequest( void ) const {
	return _request;
}

void	Valid::setRequest( const Request &request ) {
	_request = request;
}

void	Valid::handleCgi( void ) {
	int			fd_in = fileno(tmpfile());
	int			fd_out = fileno(tmpfile());
	pid_t		pid;
	std::string	path;
	char*		args[3];
	char*		env[10];

	write(fd_in, _body.c_str(), _body.size());
	lseek(fd_in, 0, SEEK_SET);
	path = _request.getMatch().getCgiPass();
	args[0] = strdup(_request.getMatch().getCgiPass().c_str());
	args[1] = strdup(_request.getTranslate().c_str());
	args[2] = NULL;
	env[0] = strdup("SERVER_PROTOCOL=HTTP/1.1");
	env[1] = strdup(("REQUEST_METHOD=" + _request.getMethod()).c_str());
	env[2] = strdup(("PATH_INFO=" + _request.getUri()).c_str());
	env[3] = NULL;
	pid = fork();
	if (pid < 0)
		throw std::runtime_error("Error in handling CGI");
	else if (pid == 0)
	{
		if (dup2(fd_in, 0) == -1 || dup2(fd_out, 1) == -1)
			throw std::runtime_error("Error in handling CGI");
		execve(args[0], args, env);
		throw std::runtime_error("Error in handling CGI");
	}
	else
	{
		waitpid(-1, NULL, 0);	// aspetta che qualsiasi figlio termini o cambi di stato, non gli interessa quale cambiamento di stato e' avvennuto e non e' importante l'opzione con la quale il processo padre ritorna ad essere
		lseek(fd_out, 0, SEEK_SET);
		struct stat st;
		fstat(fd_out, &st);
		size_t	size = st.st_size;
		char*	buffer = (char *) calloc (size + 1, sizeof(char));
		size_t	nBytes;
		size_t	i = 0;
		while (i < size)
		{
			nBytes = read(fd_out, &buffer[i], size);		// typically send()/recv() are used for sockets, write()/read() for files.
			if (nBytes <= 0)
				throw std::runtime_error("Error in handling CGI");
			i += nBytes;
		}
		buffer[i] = '\0';
		_body = buffer;
		free(buffer);
	}
	close(fd_in);
	close(fd_out);
	for(int i = 0; args[i]; i++)
		delete[] args[i];
	for(int i = 0; env[i]; i++)
		delete[] env[i];
}


void	Valid::generateBody( void ) {
	if(_request.getMethod() == "GET")
	{
		std::stringstream ss;
		ss << _file.rdbuf();
		_body = ss.str();
	}
	else if (_request.getMethod() == "PUT" || _request.getMethod() == "POST")
	{
		_body = _request.getBody();
		if (!_request.getMatch().getCgiPass().empty())
			handleCgi();
		_file << _body;
		if (_request.getMethod() == "PUT")
			_body.clear();
	}
}
