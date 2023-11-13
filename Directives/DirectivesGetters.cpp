#include "Directives.hpp"

const u_int32_t&	Directives::getListenHost( void ) const {
    return this->_listen_host;
}

const u_int16_t&	Directives::getListenPort( void ) const {
    return this->_listen_port;
}

const std::string&	Directives::getServerName( void ) const {
	return this->_server_name;
}

const std::string&	Directives::getRoot( void ) const {
	return this->_root;
}

const v_Str&	Directives::getIndex( void ) const {
	return this->_index;
}

const bool&	Directives::getAutoindex( void ) const {
	return this->_autoindex;
}

const std::string&	Directives::getScgiPass( void ) const {
	return this->_scgi_pass;
}

const v_Str&	Directives::getTryFiles( void ) const {
	return this->_try_files;
}

const v_Str&	Directives::getLimitExcept( void ) const {
    return this->_limit_except;
}

const m_IntStr&	Directives::getErrorPage( void ) const {
	return this->_error_page;
}

const unsigned int&	Directives::getClientMaxBodySize( void ) const {
	return this->_client_max_body_size;
}
