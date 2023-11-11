#include "Directives.hpp"

void    Directives::setListenHost( uint32_t listen_host ) {
    this->_listen_host = listen_host;
}

void    Directives::setListenPort( uint16_t listen_port ) {
    this->_listen_port = listen_port;
}

void	Directives::setServerName( std::string server_name ) {
	this->_server_name = server_name;
}

void	Directives::setRoot( std::string root ) {
	this->_root = root;
}

void	Directives::setIndex( v_Str index ) {
	this->_index = index;
}

void	Directives::setAutoindex( bool autoindex ) {
	this->_autoindex = autoindex;
}

void	Directives::setScgiPass( std::string scgi_pass ) {
	this->_scgi_pass = scgi_pass;
}

void	Directives::setTryFiles( v_Str try_files ) {
	this->_try_files = try_files;
}

void	Directives::setLimitExcept( v_Str limit_except ) {
    this->_limit_except = limit_except;
}

void	Directives::setErrorPage( m_IntStr error_page ) {
	this->_error_page = error_page;
}

void	Directives::setClientMaxBodySize( unsigned int client_max_body_size ) {
	this->_client_max_body_size = client_max_body_size;
}
