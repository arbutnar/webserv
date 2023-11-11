#include "Directives.hpp"

uint32_t    Directives::getListenHost( void ) {
    return this->_listenHost;
}

uint16_t    Directives::getListenPort( void ) {
    return this->_listenPort;
}

std::string	Directives::getServerName( void ) {
	return this->_server_name;
}

std::string	Directives::getRoot( void ) {
	return this->_root;
}

v_Str   Directives::getIndex( void ) {
	return this->_index;
}

bool	Directives::getAutoindex( void ) {
	return this->_autoindex;
}

std::string	Directives::getScgiPass( void ) {
	return this->_scgi_pass;
}

v_Str	Directives::getTryFiles( void ) {
	return this->_try_files;
}

v_Str	Directives::getLimitExcept( void ) {
    return this->_limit_except;
}

m_IntStr	Directives::getErrorPage( void ) {
	return this->_error_page;
}

unsigned int	Directives::getClientMaxBodySize( void ) {
	return this->_client_max_body_size;
}
