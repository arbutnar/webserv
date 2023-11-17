/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Master.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:59:53 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/17 13:07:21 by arbutnar         ###   ########.fr       */
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

void	Master::setCluster( const v_Ser &cluster) {
	this->_cluster = cluster;
}

const v_Ser&	Master::getCluster( void ) const {
	return this->_cluster;
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
	Directives*			serverPtr = new Server();
	Directives*			locationPtr;
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
			locationPtr = new Location(line);
			*locationPtr = *serverPtr;
		}
		else if (line.find("}") != std::string::npos)
		{
			if (!inLocation)
				throw Directives::SyntaxError();
			inLocation = false;
			serverPtr->addLocation(*(dynamic_cast<Location *>(locationPtr)));
			delete locationPtr;
		}
		else if (inLocation)
			locationPtr->directiveParser(line);
		else
			serverPtr->directiveParser(line);
	}
	this->_cluster.push_back(*(dynamic_cast<Server *>(serverPtr)));
	delete serverPtr;
}

void	Master::displayMaster( void ) const {
	std::cout << "[MASTER]" << std::endl;
	for (v_Ser::const_iterator it = this->_cluster.begin(); it != this->_cluster.end(); it++)
		it->displayServer();
}

void	Master::start( void ) {
	for (v_Ser::iterator it = _cluster.begin(); it != _cluster.end(); it++)
		it->socketInit();
	fd_set	activeSockets;
	while (true)
	{
		for (v_Ser::iterator s_it = _cluster.begin(); s_it != _cluster.end(); s_it++)
		{
			FD_ZERO(&activeSockets);
			s_sock temp = s_it->getSockets();
			for (s_sock::const_iterator it = temp.begin(); it != temp.end(); it++)
				FD_SET(*it, &activeSockets);
			if (select(*temp.rbegin() + 1, &activeSockets, NULL, NULL, 0) == -1)
				throw std::runtime_error("Select function failed");
			for (s_sock::const_iterator it = temp.begin(); it != temp.end(); it++)
			{
				if (FD_ISSET(*it, &activeSockets))
				{
					if (it == temp.begin())
						s_it->newConnection();
					else
						s_it->readRequest(it, &activeSockets);
				}
			}
		}
	}		
}
