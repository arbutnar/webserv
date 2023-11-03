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
	// size_t				locationBlock;
	size_t				pos;
		std::string s;

	buffer << configFile.rdbuf();
	content = buffer.str();

	while ((pos = content.find_first_of('#')) != std::string::npos)
		content.erase(pos, content.find_first_of('\n', pos) - pos);
	while ((pos = content.find_first_of("location ")) != std::string::npos) {
		// locationBlock = content.find(static_cast<std::string>("location")) != std::string::npos;
		// if (locationBlock)
		s = content.substr(pos, content.find_first_of('}'));
	}
	std::cout << content << std::endl;
	std::cout << s << std::endl;
}

// void	Master::validateConfig( void ) {
// 	std::string		content;
// 	size_t			pos;
// 	unsigned int	locCount;
// 	bool			locationBlock;

// 	while (std::getline(configFile, content)) {
// 		while ((pos = content.find_first_of('#')) != std::string::npos)
// 			content.erase(pos, content.find_first_of('\n', pos) - pos);
// 		locationBlock = content.find(static_cast<std::string>("location")) != std::string::npos;
// 		if (locationBlock)
// 			locCount++;
// 	}
// 	std::cout << locCount << std::endl;
// }