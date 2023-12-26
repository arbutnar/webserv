/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                             			    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 17:46:34 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/30 18:35:12 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Error.hpp"

Error::Error( void )
	: Response() {
}

Error::Error( const std::string &code, const Request &request ) 
	: Response(code, request) {
		int c = std::atoi(code.c_str());
		switch (c)
		{
			case MOVED_PERMANENTLY:
				_status += " Moved Permanently"; break ;
			case MOVED_TEMPORARLY:
				_status += " Moved temporarily"; break ;
			case SEE_OTHER:
				_status += " See Other"; break ;
			case NOT_MODIFIED:
				_status += " Not Modified"; break ;
			case TEMPORARY_REDIRECT:
				_status += " Temporary Redirect"; break ;
			case PERMANENT_REDIRECT:
				_status += " Permanent Redirect"; break ;
			case BAD_REQUEST:
				_status += " Bad Request"; break ;
			case FORBIDDEN:
				_status += " Forbidden"; break ;
			case NOT_FOUND:
				_status += " Not Found"; break ;
			case METHOD_NOT_ALLOWED:
				_status += " Method Not Allowed"; break ;
			case CONFLICT:
				_status += " Conflict"; break ;
			case REQUEST_ENTITY_TOO_LARGE:
				_status += " Request Entity Too Large"; break ;
			case URI_TOO_LONG:
				_status += " URI Too Long"; break ;
			case UNSUPPORTED_MEDIA_TYPE:
				_status += " Unsupported Media Type"; break ;
			case REQUEST_HEADER_TOO_LARGE:
				_status += " Request header too large"; break ;
			case CLIENT_CLOSED_REQUEST:
				_status += " Request Closed Request"; break ;
			case INTERNAL_SERVER:
				_status += " Internal Server Error"; break ;
			case NOT_IMPLEMENTED:
				_status += " Not Implemented"; break;
			case HTTP_NOT_SUPPORTED:
				_status += " HTTP Not Supported"; break ;
			case INSUFFICIENT_STORAGE:
				_status += " Insufficient Storage"; break ;
			default:
				throw std::runtime_error(code);
		}
}

Error::Error( const Error &src )
	: Response(src) {
}

Error	&Error::operator=( const Error &src ) {
	if (this == &src)
		return *this;
	Response::operator=(src);
	return *this;
}

Error::~Error( ) {
}

void	Error::defaultErrorPage( void ) {
	int status = atoi(_status.c_str());

	m_intStr::const_iterator it = _request.getMatch().getErrorPage().find(status);
	if (it != _request.getMatch().getErrorPage().end())
	{
		std::ifstream		error_page(it->second.c_str());
		std::stringstream	buffer;
		buffer << error_page.rdbuf();
		_body = buffer.str();
		return ;
	}
	_body = "<!DOCTYPE HTML>\n";
	_body += "<html>\n<head>\n\t<title>" + _status + "</title>\n</head>\n<body>\n\t";
	if ((status < 301 || status > 308) && _request.getMatch().getReturn().first != -1)
		_body += _request.getMatch().getReturn().second;
	else
		_body += "<h1>" + _status + "</h1>";
	_body += "\n</body>\n</html>\n";
}
