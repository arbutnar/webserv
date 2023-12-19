/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Valid.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 11:48:05 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/14 18:54:02 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Valid.hpp"

Valid::Valid( void )
	: Response ("200 Ok") {
}

Valid::Valid( const Request &request )
	:  Response ("200 Ok", request) {
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
	return *this;
}

Valid::~Valid( ) {
}

void	Valid::handleAutoindex( void ) {
	DIR				*dir;
	struct dirent	*entry;
	std::string		uri = _request.getUri();
	dir = opendir(_request.getTranslate().c_str());
	_body = "<!DOCTYPE HTML>\n";
	_body += "<html>\n<head>\n\t<title>" + uri + "</title>\n</head>\n<body>\n";
	_body += "\t<h1>Index of " + uri + "</h1>\n\t<hr>\n";
	if (*uri.rbegin() != '/')
		uri += "/";
	if (*uri.begin() != '/')
		uri = "/" + uri;
	while ((entry = readdir(dir)) != NULL)
	{
		_body += "\t<a href=\"";
		_body += uri + entry->d_name;
		_body += "\">";
		_body += entry->d_name;
		_body += "</a><br>\n";
	}
	_body += "\t<hr>\n</body>\n</html>\n";
	closedir(dir);
}

void	Valid::handleCgi( void ) {
	char*		args[3];
	char*		env[4];
	int			pipe_in[2];
	int			pipe_out[2];
	pid_t		pid;

	pipe(pipe_in);
	pipe(pipe_out);
	args[0] = strdup(_request.getMatch().getCgiPass().c_str());
	args[1] = strdup(_request.getTranslate().c_str());
	args[2] = NULL;
	env[0] = strdup("SERVER_PROTOCOL=HTTP/1.1");
	env[1] = strdup(("REQUEST_METHOD=" + _request.getMethod()).c_str());
	env[2] = strdup(("PATH_INFO=" + _request.getUri()).c_str());
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
	write(pipe_in[1], _body.c_str(), _body.size());
	close(pipe_in[1]);
	_body.clear();
	char c;
	while (read(pipe_out[0], &c, 1))
		_body += c;
	close(pipe_out[0]);
	for(int i = 0; args[i]; i++)
		free(args[i]);
	for(int i = 0; env[i]; i++)
		free(env[i]);
}


void	Valid::generateBody( void ) {
	if(_request.getMethod() == "GET")
	{
		if (_request.getMatch().getAutoindex() == false && *_request.getTranslate().rbegin() != '/')
		{
			std::stringstream ss;
			ss << _file.rdbuf();
			_body = ss.str();
			if (!_request.getMatch().getCgiPass().empty())
				handleCgi();
		}
		else
			handleAutoindex();
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
