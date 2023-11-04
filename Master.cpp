/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Master.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:59:53 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/03 19:03:26 by arbutnar         ###   ########.fr       */
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

void	Master::validateConfig( void ) {
	std::string			content;
	std::stringstream	buffer;
	size_t				pos;

	buffer << configFile.rdbuf();
	content = buffer.str();
	while ((pos = content.find_first_of('#')) != std::string::npos)
		content.erase(pos, content.find_first_of('\n', pos) - pos);
	while ((pos = content.find("server")) != std::string::npos) {
		size_t	posServer = pos;
		Server currentServer;
		cluster.push_back(currentServer);
		while ((pos = content.find("location")) != std::string::npos) {
			size_t	posLocation = pos;
			currentServer.locations.push_back(content.substr(posLocation, content.find('}', posLocation) - posLocation + 1));
			content.erase(posLocation, content.find('}', posLocation) - posLocation + 1);
		}
		currentServer.main = content.substr(posServer, content.find('}', posServer) + 1);
		//std::cout << currentServer.main << std::endl;
		//for (std::vector<std::string>::iterator it = currentServer.locations.begin(); it != currentServer.locations.end(); it++) {
		//	std::cout << *it << std::endl;
		//}
		content.erase(posServer, content.find('}', posServer) - posServer + 1);
	}
}
