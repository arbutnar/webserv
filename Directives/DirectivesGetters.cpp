#include "Directives.hpp"

const u_int32_t&	Directives::getListenHost( void ) const {
    return _listen_host;
}

const u_int16_t&	Directives::getListenPort( void ) const {
    return _listen_port;
}

const std::string&	Directives::getServerName( void ) const {
	return _server_name;
}

const std::string&	Directives::getRoot( void ) const {
	return _root;
}

const v_Str&	Directives::getIndex( void ) const {
	return _index;
}

const bool&	Directives::getAutoindex( void ) const {
	return _autoindex;
}

const std::string&	Directives::getScgiPass( void ) const {
	return _scgi_pass;
}

const v_Str&	Directives::getTryFiles( void ) const {
	return _try_files;
}

const m_StrBool&	Directives::getLimitExcept( void ) const {
    return _limit_except;
}

const m_IntStr&	Directives::getErrorPage( void ) const {
	return _error_page;
}

const unsigned int&	Directives::getClientMaxBodySize( void ) const {
	return _client_max_body_size;
}
