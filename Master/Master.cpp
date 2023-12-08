/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Master.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:59:53 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/08 16:13:52 by arbutnar         ###   ########.fr       */
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
	_cluster = src._cluster;
	return *this;
}

Master::~Master() {
}

void	Master::setCluster( const v_ser &cluster) {
	_cluster = cluster;
}

const v_ser&	Master::getCluster( void ) const {
	return _cluster;
}

void	Master::configCleaner( std::ifstream &configFile, std::string &content) {
	std::stringstream	buffer;
	std::string			tmp;
	size_t				pos = 0;

	buffer << configFile.rdbuf();
	content = buffer.str();
	while ((pos = content.find('#')) != std::string::npos)
	{
		tmp = content.substr(pos, content.find_first_of("\n", pos) - pos);
		content.erase(pos, content.find_first_not_of(" \t\n", pos + tmp.size()) - pos);
	}
	pos = 0;
	while ((pos = content.find("\n", pos)) != std::string::npos)
	{
		tmp = content.substr(pos, content.find_first_of("\n", pos + 1) - pos);
		if (tmp.find_first_not_of(" \t\n") == std::string::npos)
			content.erase(pos, tmp.size());
		pos += 1;
	}
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
	for (v_ser::iterator it = _cluster.begin(); it != _cluster.end(); it++)
	{
		if (it->findRoot() != it->getLocations().end())
			continue ;
		Directives *root = new Location("/");
		Directives *server = &(*it);
		*root = *server;
		it->addLocation(*(dynamic_cast<Location *>(root)));
	}
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
			if (line.find("{") != line.length() - 1)
				throw Directives::SyntaxError();
			inLocation = true;
			locationPtr = new Location(line);
			*locationPtr = *serverPtr;
			locationPtr->setRoot("");
			locationPtr->setAlias("");
		}
		else if (line.find("}") != std::string::npos)
		{
			if (!inLocation)
				throw Directives::SyntaxError();
			inLocation = false;
			if (locationPtr->getRoot().empty() && locationPtr->getAlias().empty())
			{
				locationPtr->setRoot(serverPtr->getRoot());
				locationPtr->setAlias(serverPtr->getAlias());
			}
			serverPtr->addLocation(*(dynamic_cast<Location *>(locationPtr)));
			delete locationPtr;
		}
		else if (inLocation)
		{
			if (line.find("client_header_buffer_size") != std::string::npos)
				throw Directives::SyntaxError();
			locationPtr->directiveParser(line);
		}
		else
			serverPtr->directiveParser(line);
	}
	_cluster.push_back(*(dynamic_cast<Server *>(serverPtr)));
	delete serverPtr;
}

void	Master::displayMaster( void ) const {
	std::cout << "[MASTER]" << std::endl;
	for (v_ser::const_iterator it = _cluster.begin(); it != _cluster.end(); it++)
		it->displayServer();
}

void	Master::arrangeCluster( void ) {
	std::sort(_cluster.begin(), _cluster.end());
	v_ser::iterator it = _cluster.begin();
	while (it != _cluster.end())
	{
		try {
			it->ListenerInit();
			it++;
		} catch (std::exception &e) {
			std::cout << it->getServerName() << ": " << e.what() << std::endl;
			it = _cluster.erase(it);
		}
	}
	if (_cluster.size() == 0)
		throw std::runtime_error("No Server available");
}

void	Master::start( void ) {
	fd_set	active;
	int		max;
	struct timeval  tv;

	tv.tv_sec = 0;
	tv.tv_usec = 0;
	while (true)
	{
		for (v_ser::iterator it = _cluster.begin(); it != _cluster.end(); it++)
		{
			FD_ZERO(&active);
			FD_SET(it->getListener(), &active);
			for (v_cli::const_iterator c_it = it->getClients().begin(); c_it != it->getClients().end(); c_it++)
				FD_SET(c_it->getSocket(), &active);
			max = it->nfds();
			if (max == 0)
				max = it->getListener();
			if (select(max + 1, &active, NULL, NULL, &tv) < 1)
				continue ;
			if (FD_ISSET(it->getListener(), &active))
				it->newConnection();
			it->clientInteraction(active);
		}
	}
}
