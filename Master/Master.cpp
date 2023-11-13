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

std::string		Master::configCleaner( std::ifstream &configFile) {
	std::stringstream	buffer;
	std::string			content;
	size_t				pos;

	buffer << configFile.rdbuf();
	content = buffer.str();
	while ((pos = content.find_first_of('#')) != std::string::npos)
		content.erase(pos, content.find_first_of('\n', pos) - pos);
	while ((pos = content.find("\n\n")) != std::string::npos)
		content.erase(pos, 1);
	return content;
}

void	Master::configDivider( const char* path ) {
	std::ifstream		configFile(path);
	std::string			content;

	if (!configFile.is_open())
		throw std::runtime_error("Cannot open File");
	content = configCleaner(configFile);

	std::vector<std::string>ServerBlocks;
	std::stringstream		ss(content);
	std::string				line;
	std::string				tmp;
	unsigned int 			brackets;
	
	brackets = 0;
	while (std::getline(ss, line))
	{
		if (line.find('{') != std::string::npos)
		{
			if (brackets == 0 && line.find("server") == std::string::npos)
				throw std::runtime_error("Incomplete Config");
			brackets++;
		}
		else if (line.find('}') != std::string::npos)
			brackets--;
		else if (line[line.length() - 1] != ';')
			throw std::runtime_error("Incomplete Config");
		tmp += line + '\n';
		if (tmp.find("server {") == std::string::npos)
			throw std::runtime_error("Incomplete Config");
		if (brackets == 0)
		{
			ServerBlocks.push_back(tmp);
			tmp.clear();
		}
	}
	if (brackets != 0)
		throw std::runtime_error("Incomplete Config");
	for (std::vector<std::string>::iterator it = ServerBlocks.begin(); it != ServerBlocks.end(); it++)
		std::cout << *it << std::endl;
}
