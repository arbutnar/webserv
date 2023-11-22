#include "Directives.hpp"

void    Directives::setListenHost( const u_int32_t &listen_host ) {
    _listen_host = listen_host;
}

void    Directives::setListenPort( const u_int16_t &listen_port ) {
    _listen_port = listen_port;
}

void	Directives::setServerName( const std::string &server_name ) {
	_server_name = server_name;
}

void	Directives::setRoot( const std::string &root ) {
	_root = root;
}

void	Directives::setIndex( const v_str &index ) {
	_index = index;
}

void	Directives::setAutoindex( const bool &autoindex ) {
	_autoindex = autoindex;
}

void	Directives::setScgiPass( const std::string &scgi_pass ) {
	_scgi_pass = scgi_pass;
}

void	Directives::setTryFiles( const v_str &try_files ) {
	_try_files = try_files;
}

void	Directives::setLimitExcept( const m_strBool &limit_except ) {
    _limit_except = limit_except;
}

void	Directives::setErrorPage( const m_intStr &error_page ) {
	_error_page = error_page;
}

void	Directives::setClientMaxBodySize( const unsigned int &client_max_body_size ) {
	_client_max_body_size = client_max_body_size;
}
