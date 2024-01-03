/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Master.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:59:53 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/14 18:50:34 by arbutnar         ###   ########.fr       */
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
	_clusters = src._clusters;
	return *this;
}

Master::~Master() {
}

const v_cluster&	Master::getClusters( void ) const {
	return _clusters;
}

void	Master::setClusters( const v_cluster &clusters) {
	_clusters = clusters;
}

void	Master::configCleaner( std::ifstream &configFile, std::string &content) {
	std::stringstream	ss;
	std::string			tmp;
	size_t				pos = 0;

	ss << configFile.rdbuf();
	content = removeComments(ss.str());
	//togliere \n
	std::cout << content << std::endl;
	exit(0);
}

Server	*Master::serverParser( std::string &block ) {
	block.erase(0, block.find_first_of('\n') + 1);
	block.erase(block.find_last_of('\n', block.find_last_of('\n') - 1) + 1, std::string::npos);
	std::stringstream	ss(block);
	std::string			line;
	Directives*			serverPtr = new Server();
	Directives*			locationPtr = NULL;
	bool				inLocation = false;

	try {
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
			}
			else if (line.find("}") != std::string::npos)
			{
				if (!inLocation)
					throw Directives::SyntaxError();
				inLocation = false;
				if (locationPtr->getRoot().empty() && (locationPtr->getAlias().empty() && locationPtr->getCgiAlias().empty()))
					locationPtr->setRoot(serverPtr->getRoot());
				(dynamic_cast<Server *>(serverPtr))->addLocation(*(dynamic_cast<Location *>(locationPtr)));
				delete locationPtr;
			}
			else if (inLocation)
				locationPtr->directiveParser(line, inLocation);
			else
				serverPtr->directiveParser(line, inLocation);
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
	configCleaner(configFile, content);
	std::stringstream		ss(content);
	std::string				line;
	std::string				tmp;
	unsigned int 			brackets = 0;
	Directives*				serverPtr = NULL;
	v_ser					serverVec;

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
			serverPtr = serverParser(tmp);
			serverVec.push_back(*(dynamic_cast<Server *>(serverPtr)));
			delete serverPtr;
			tmp.clear();
		}
	}
	configFile.close();
	if (brackets != 0)
		throw Directives::SyntaxError();
	for (v_ser::iterator it = serverVec.begin(); it != serverVec.end(); it++)
	{
		if (std::find(it->getLocations().begin(), it->getLocations().end(), "/") != it->getLocations().end())
			continue ;
		Directives *root = new Location("/");
		Directives *server = &(*it);
		*root = *server;
		it->addLocation(*(dynamic_cast<Location *>(root)));
	}
	arrangeClusters(serverVec);
}

void	Master::arrangeClusters( v_ser &serverVec ) {
	v_ser	subVector;
	int		listener;

	v_ser::iterator it = serverVec.begin();
	while (it != serverVec.end())
	{
		listener = it->listenerInit();
		std::cout << listener << std::endl;
		if (listener == -1)
		{
			serverVec.erase(it);
			continue ;
		}
		subVector.push_back(*it);
		serverVec.erase(it);
		while ((it = std::find(serverVec.begin(), serverVec.end(), subVector.back())) != serverVec.end())
		{
			subVector.push_back(*it);
			serverVec.erase(it);
		}
		_clusters.push_back(Cluster());
		_clusters.back().setListener(listener);
		subVector.clear();
		it = serverVec.begin();
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
		for (v_cluster::iterator it = _clusters.begin(); it != _clusters.end(); it++)
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
			select(it->nfds() + 1, &read, &write, NULL, &tv);
			if (FD_ISSET(it->getListener(), &read))
				it->acceptNewClient();
			else
				it->menageClient(read, write);
		}
	}
}

// void	Master::displayMaster( void ) const {
// 	std::cout << "[MASTER]" << std::endl;
// 	for (v_ser::const_iterator it = _cluster.begin(); it != _cluster.end(); it++)
// 		it->displayServer();
// }
