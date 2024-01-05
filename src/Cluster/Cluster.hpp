#ifndef CLUSTER_HPP
# define CLUSTER_HPP

# include "../Server/Server.hpp"

typedef std::vector<Server> v_ser;

class Cluster {
	private:
		int			_listener;
		v_ser		_servers;
		v_cli		_clients;
	public:
		Cluster( void );
		Cluster( const int &listener, const v_ser &servers );
		Cluster( const Cluster &src );
		Cluster &operator=( const Cluster &src );
		~Cluster();

		const int 	&getListener( void ) const;
		const v_ser	&getServers( void ) const;
		const v_cli	&getClients( void ) const;
		void		setListener( const int &listener );
		void		setServers( const v_ser &servers );
		void		setClients( const v_cli &clients );

		int						nfds( void ) const;
		v_cli::iterator			removeClient( v_cli::iterator &it );
		void					removeAllClients( void );
		void					acceptNewClient( void );
		v_ser::const_iterator	serverLinker( const std::string &buffer ) const;
		void					menageClient( const fd_set &read, const fd_set &write );
		void					displayCluster( void ) const;
};

#endif
