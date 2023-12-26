#include <string.h>
#include <sys/wait.h>
#include "../Request/Request.hpp"

class Cgi {
	private:
		int	_readFd;
		int	_cliSock;
	public:
		Cgi( void );
		Cgi( const Cgi &src );
		Cgi &operator=( const Cgi &src );
		~Cgi();
		
		const int	&getReadFd( void ) const;
		const int	&getCliSock( void ) const;
		void		setReadFd( const int &readFd );
		void		setCliSock( const int &cliSock );
		bool		writeResponse( const std::string &clientBuffer );
		void		handleCgi( Request &request );
		void		clear( void );
};
