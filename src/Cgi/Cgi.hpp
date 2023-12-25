#include <string.h>
#include <sys/wait.h>
#include "../Request/Request.hpp"

class Cgi {
	private:
		int	_output;
		int	_cliSock;
	public:
		Cgi( void );
		Cgi( const Cgi &src );
		Cgi &operator=( const Cgi &src );
		~Cgi();
		
		const int	&getOutput( void ) const;
		const int	&getCliSock( void ) const;
		void		setOutput( const int &output );
		void		setCliSock( const int &cliSock );
		void		handleCgi( int cliSock, Request &request );
};
