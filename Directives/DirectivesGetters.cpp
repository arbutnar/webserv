#include "Directives.hpp"

const u_int32_t	&Directives::getListenHost( void ) const {
    return _listen_host;
}

const u_int16_t	&Directives::getListenPort( void ) const {
    return _listen_port;
}

const std::string	&Directives::getServerName( void ) const {
	return _server_name;
}

const std::string	&Directives::getRoot( void ) const {
	return _root;
}

const std::string	&Directives::getAlias( void ) const {
	return _alias;
}

const v_str	&Directives::getIndex( void ) const {
	return _index;
}

const bool	&Directives::getAutoindex( void ) const {
	return _autoindex;
}

const std::string	&Directives::getScgiPass( void ) const {
	return _scgi_pass;
}

const v_str	&Directives::getTryFiles( void ) const {
	return _try_files;
}

const m_strBool	&Directives::getLimitExcept( void ) const {
    return _limit_except;
}

const m_intStr	&Directives::getErrorPage( void ) const {
	return _error_page;
}

const unsigned int	&Directives::getClientMaxBodySize( void ) const {
	return _client_max_body_size;
}

const unsigned int	&Directives::getClientHeaderBufferSize( void ) const {
	return _client_header_buffer_size;
}

const v_str	&Directives::getReturn( void ) const {
	return _return;
}
