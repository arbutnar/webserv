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

Master::Master( void ) {
}

Master::Master( const Master &src ) {
	*this = src;
}

Master& Master::operator=( const Master &src ) {
	if (this == &src)
		return *this;
	this->_cluster = src._cluster;
	return *this;
}

Master::~Master() {
}

void	Master::configCleaner( std::ifstream &configFile, std::string &content) {
	std::stringstream	buffer;
	size_t				pos;

	buffer << configFile.rdbuf();
	content = buffer.str();
	while ((pos = content.find_first_of('#')) != std::string::npos)
		content.erase(pos, content.find_first_of('\n', pos) - pos);
	while ((pos = content.find("\n\n")) != std::string::npos)
		content.erase(pos, 1);
}

void	Master::configDivider( const char* path ) {
	std::ifstream		configFile(path);
	std::string			content;

	if (!configFile.is_open())
		throw std::runtime_error("Cannot open File");
	configCleaner(configFile, content);
	std::stringstream		ss(content);
	std::string				line;
	std::string				tmp;
	unsigned int 			brackets = 0;
	
	while (std::getline(ss, line))
	{
		if (line.find('{') != std::string::npos)
		{
			if (brackets == 0 && line.find("server") == std::string::npos)
				throw Directives::SyntaxError();
			brackets++;
		}
		else if (line.find('}') != std::string::npos)
			brackets--;
		else if (line[line.length() - 1] != ';')
			throw Directives::SyntaxError();
		tmp += line + '\n';
		if (tmp.find("server {") == std::string::npos)
			throw Directives::SyntaxError();
		if (brackets == 0)
		{
			serverParser(tmp);
			tmp.clear();
		}
	}
	if (brackets != 0)
		throw Directives::SyntaxError();
}

void	Master::serverParser( std::string &block ) {
	block.erase(0, block.find_first_of('\n') + 1);
	block.erase(block.find_last_of('\n', block.find_last_of('\n') - 1) + 1, std::string::npos);
	std::stringstream	ss(block);
	std::string			line;
	Server				server;
	Location			location;
	bool				inLocation = false;

	while (std::getline(ss, line))
	{
		if (line.find("location ") != std::string::npos)
		{
			if (inLocation)
				throw Directives::SyntaxError();
			else if (line.find("{") != line.length() - 1)
				throw Directives::SyntaxError();
			inLocation = true;
			location.parseLocationName(line);
		}
		else if (line.find("}") != std::string::npos)
		{
			if (!inLocation)
				throw Directives::SyntaxError();
			inLocation = false;
			server.addLocation(location);
			location.clear();
		}
		// else if (inLocation)
		// 	location.directivesParser(line);
		else
			server.directiveParser(line);
	}
}
