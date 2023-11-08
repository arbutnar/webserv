/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Master.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:59:53 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/08 16:58:42 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Master.hpp"
#include "Server.hpp"

class Master::FileError : public std::exception {
	public:
		virtual const char* what() const throw() { return ("File Error"); }
};

Master::Master( void )
	: _configFile (NULL) {
}

Master::Master( const char* path )
	: _configFile (path) {
	
		if (!(_configFile.is_open()))
			throw Master::FileError();
		configParse();
}

Master::Master( const Master &src ) {
	(void)src;
}

Master& Master::operator=( Master &src ) { // const
	return src;
}

Master::~Master() {
}

bool	Master::configSyntax( std::string content ) {
	std::stringstream	ss(content);
	std::string		line;
	unsigned int 	openBrackets = 0;
	unsigned int	closeBrackets = 0;

	while (std::getline(ss, line))
	{
		if (line.find('{') != std::string::npos)
			openBrackets++;
		else if (line.find('}') != std::string::npos)
			closeBrackets++;
		else if (line.find('{') == std::string::npos || line.find('}') == std::string::npos)
			if (!line.empty() && line[line.length() - 1] != ';')
				return 0;
	}
	if (openBrackets != closeBrackets)
		return 0;
	return 1;
}

void	Master::configParse( void ) {
	std::string			content;
	std::stringstream	buffer;
	size_t				pos;

	buffer << _configFile.rdbuf();
	content = buffer.str();
	while ((pos = content.find_first_of('#')) != std::string::npos)
		content.erase(pos, content.find_first_of('\n', pos) - pos);
	while ((pos = content.find("\n\n")) != std::string::npos)
		content.erase(pos, 1);
	if (!configSyntax(content))
		throw Master::FileError();

	while ((pos = content.find("server")) != std::string::npos)
	{
		size_t	posServer = pos;
		Server	currentServer;
		while ((pos = content.find("location")) != std::string::npos) {
			size_t		posLocation = pos;
			Location	currentLocation(content.substr(posLocation, content.find('}', posLocation) - posLocation + 1));
			currentServer._locations.push_back(currentLocation);
			content.erase(posLocation, content.find('}', posLocation) - posLocation);
		}
		Directives currentDirectives(content.substr(posServer, content.find('}', posServer) - posServer + 1));
		currentServer._directives = currentDirectives;
		_cluster.push_back(currentServer);
		content.erase(posServer, content.find('}', posServer) - posServer);
	}
}
