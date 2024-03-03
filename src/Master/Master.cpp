/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Master.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:59:53 by arbutnar          #+#    #+#             */
/*   Updated: 2024/03/03 15:47:10 by arbutnar         ###   ########.fr       */
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
	_workers = src._workers;
	return *this;
}

Master::~Master() {
	for (v_wrk::iterator it = _workers.begin(); it != _workers.end(); it++)
	{
		close(it->getListener());
		it->removeAllClients();
	}
}

const v_wrk&	Master::getWorkers( void ) const {
	return _workers;
}

void	Master::setWorkers( const v_wrk &workers) {
	_workers = workers;
}

Server	*Master::serverParser( std::string &block ) {
	block.erase(block.find_last_of('\n', block.find_last_of('\n') - 1) + 1, std::string::npos);
	Directives*			serverPtr = new Server();
	Directives*			locationPtr = NULL;
	std::string			line;
	std::stringstream	ss;

	try {
		ss << block;
		while (std::getline(ss, line))
		{
			if (line.find("location ") != std::string::npos)
			{
				if (locationPtr != NULL)
					throw Directives::SyntaxError();
				if (line.find("{") != line.length() - 1)
					throw Directives::SyntaxError();
				locationPtr = new Location(line);
				*locationPtr = *serverPtr;
				locationPtr->setRoot("");
			}
			else if (line.find("}") != std::string::npos)
			{
				if (locationPtr == NULL)
					throw Directives::SyntaxError();
				if (locationPtr->getRoot().empty() && (locationPtr->getAlias().empty() && locationPtr->getCgiAlias().empty()))
					locationPtr->setRoot(serverPtr->getRoot());
				(dynamic_cast<Server *>(serverPtr))->addLocation(*(dynamic_cast<Location *>(locationPtr)));
				delete locationPtr;
				locationPtr = NULL;
			}
			else if (locationPtr != NULL)
				locationPtr->directiveParser(line, true);
			else
				serverPtr->directiveParser(line, false);
		}
	} catch (std::exception &e) {
		if (locationPtr != NULL)
			delete locationPtr;
		if (serverPtr != NULL)
			delete serverPtr;
		throw std::runtime_error(e.what());
	}
	return dynamic_cast<Server *>(serverPtr);
}

void	Master::configDivider( const char* path ) {
	std::ifstream		configFile(path);
	std::string			content;

	if (!configFile.is_open())
		throw std::runtime_error("Cannot open File");
	content = removeComments(toString(configFile.rdbuf()));
	content = removeEmptyLine(content);
	std::stringstream		ss(content);
	std::string				line;
	std::string				tmp;
	unsigned int 			brackets = 0;
	Directives*				serverPtr = NULL;
	v_ser					cluster;

	while (std::getline(ss, line))
	{
		size_t pos = line.find_first_not_of(" \t");
		line = line.substr(pos, line.find_last_not_of(" \t\r\n") - pos + 1);
		if (brackets == 0)
		{
			pos = line.find("server");
			if (pos == std::string::npos)
				throw Directives::SyntaxError();
			pos = line.find_first_not_of(" \t", pos + 6);
			if (pos != line.size() - 1 || line.at(pos) != '{')
				throw Directives::SyntaxError();
			brackets++;
			continue ;
		}
		else if (*line.rbegin() == '{')
			brackets++;
		else if (*line.rbegin() == '}')
		{
			if (brackets == 0)
				throw Directives::SyntaxError();
			brackets--;
		}
		tmp += line + "\n";
		if (brackets == 0)
		{
			serverPtr = serverParser(tmp);
			cluster.push_back(*(dynamic_cast<Server *>(serverPtr)));
			delete serverPtr;
			tmp.clear();
		}
	}
	configFile.close();
	if (brackets != 0)
		throw Directives::SyntaxError();
	for (v_ser::iterator it = cluster.begin(); it != cluster.end(); it++)
	{
		if (std::find(it->getLocations().begin(), it->getLocations().end(), "/") != it->getLocations().end())
			continue ;
		Directives *root = new Location("/");
		Directives *server = &(*it);
		*root = *server;
		it->addLocation(*(dynamic_cast<Location *>(root)));
	}
	arrangeWorkers(cluster);
}

void	Master::arrangeWorkers( v_ser &cluster ) {
	v_ser	subVector;
	int		listener;

	v_ser::iterator it = cluster.begin();
	while (it != cluster.end())
	{
		try {
			it->listenerInit(listener);
		} catch( std::exception &e ) {
			std::cout << e.what() << std::endl;
			if (listener != -1)
				close(listener);
			cluster.erase(it);
			continue ;
		}
		subVector.push_back(*it);
		cluster.erase(it);
		while ((it = std::find(cluster.begin(), cluster.end(), subVector.back())) != cluster.end())
		{
			if (std::find(subVector.begin(), subVector.end(), it->getServerName()) != subVector.end())
			{
				std::cout << "conflicting server name " << it->getServerName() << " on " << listener << ", ignored" << std::endl;
				cluster.erase(it);
				continue ;
			}
			subVector.push_back(*it);
			cluster.erase(it);
		}
		_workers.push_back(Worker(listener, subVector));
		subVector.clear();
		it = cluster.begin();
	}
}

void	Master::start( void ) {
	fd_set			read;
	fd_set			write;
	fd_set			active;
	struct timeval	tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	while(1)
	{
		for (v_wrk::iterator it = _workers.begin(); it != _workers.end(); it++)
		{
			FD_ZERO(&active);
			FD_SET(it->getListener(), &active);
			for (v_cli::const_iterator c_it = it->getClients().begin(); c_it != it->getClients().end(); c_it++)
			{
				FD_SET(c_it->getSocket(), &active);
				if (c_it->getCgiFd() != -1)
					FD_SET(c_it->getCgiFd(), &active);
			}
			read = write = active;
			if (!select(it->nfds() + 1, &read, &write, NULL, &tv))
				continue ;
			if (FD_ISSET(it->getListener(), &read))
				it->acceptNewClient();
			else
				it->menageClient(read, write);
		}
	}
}

void	Master::displayMaster( void ) const {
	std::cout << "[MASTER]" << std::endl;
	for (v_wrk::const_iterator it = _workers.begin(); it != _workers.end(); it++)
		it->displayWorker();
}
