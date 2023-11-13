#include "Directives.hpp"

void	Directives::parseListen( const std::string &attribute ) {
	if (attribute.find(':') != std::string::npos)
	{
		std::string tmp = attribute.substr(0, attribute.find(':'));
		if (tmp != "localhost" || std::atoi(tmp.c_str()) != 2130706433)
			throw Directives::SyntaxError();
		// parseHost(tmp);
		// attribute.erase(0, attribute.find(':') - tmp.length());
	}

	setListenPort(std::atoi(attribute.c_str()));
}

void	Directives::parseServerName( const std::string &attribute ) {
	setServerName(attribute);
}

void	Directives::parseRoot( const std::string &attribute ) {
	setRoot(attribute);
}

void	Directives::parseIndex( const std::string &attribute ) {
	(void)attribute;
}

void	Directives::parseAutoindex( const std::string &attribute ) {
	setAutoindex(std::atoi(attribute.c_str()));
}

void	Directives::parseScgiPass( const std::string &attribute ) {
	setScgiPass(attribute);
}

void	Directives::parseTryFiles( const std::string &attribute ) {
		(void)attribute;
}

void	Directives::parseLimitExcept( const std::string &attribute ) {
		(void)attribute;
}

void	Directives::parseErrorPage( const std::string &attribute ) {
	std::string	key = attribute.substr(0, attribute.find(' '));
	
}

void	Directives::parseClientMaxBodySize( const std::string &attribute ) {
	setClientMaxBodySize(std::atoi(attribute.c_str()));
}
 