/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Master.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:59:53 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/04 17:43:14 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Master.hpp"
#include "Server.hpp"

class Master::FileError : public std::exception {
	public:
		virtual const char* what() const throw() { return ("File Error"); }
};

Master::Master( void )
	: configFile (NULL) {
}

Master::Master( const char* path )
	: configFile (NULL) {
		configFile.open(path);

		if (!(configFile.is_open()))
			throw Master::FileError();
		validateConfig();
}

bool	Master::checkSyntax( void ) {
	unsigned int 	openBrackets = 0;
	unsigned int	closeBrackets = 0;
	std::string		line;

	while (std::getline(configFile, line)) {
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

void	Master::validateConfig( void ) {
	std::string			content;
	std::stringstream	buffer;
	size_t				pos;

	if (!checkSyntax())
		throw Master::FileError();
	buffer << configFile.rdbuf();
	content = buffer.str();
	while ((pos = content.find_first_of('#')) != std::string::npos)
		content.erase(pos, content.find_first_of('\n', pos) - pos);
	while ((pos = content.find("\n\n")) != std::string::npos)
		content.erase(pos, 1);
	while ((pos = content.find("server")) != std::string::npos) {
		size_t	posServer = pos;
		Server currentServer;
		cluster.push_back(currentServer);
		while ((pos = content.find("location")) != std::string::npos) {
			size_t		posLocation = pos;
			std::string	locationBlock;
			locationBlock = content.substr(posLocation, content.find('}', posLocation) - posLocation + 1);
			currentServer.locations.push_back(locationBlock);
			content.erase(posLocation, content.find('}', posLocation) - posLocation);
		}
		currentServer.main = content.substr(posServer, content.find('}', posServer) - posServer + 1);
		content.erase(posServer, content.find('}', posServer) - posServer);
	}
}
