/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:23:11 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/04 18:02:38 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/select.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <stdio.h>
# include <set>
# include <unistd.h>
# include "../Location/Location.hpp"
# include "../Client/Client.hpp"
# include "../Request/Request.hpp"
# include "../Response/Error.hpp"
# include "../Response/Valid.hpp"

typedef std::vector<Client> v_cli;

class Server : public Directives {
	private:
		s_locs	_locations;
		int		_listener;
		v_cli	_clients;
		fd_set	_fd_sets;
	public:
		Server( void );
		Server( std::string block );
		Server( const Server &src );
		Server& operator=( const Server &src );
		bool	operator<( const Server &src );
		~Server( );

		void			setLocations( const s_locs &locations );
		void			setListener( const int &listener );
		void			setClients( const v_cli &clients );
		void			setFdSets( const fd_set &fd_sets );
		const s_locs	&getLocations( void ) const;
		const int		&getListener( void ) const;
		v_cli			&getClients( void );
		const fd_set	&getFdSets( void ) const; 

		void					addLocation( const Location &location );
		s_locs::const_iterator	findRoot( void ) const;
		void					ListenerInit( void );
		int						nfds( void ) const;
		v_cli::iterator			eraseClient( v_cli::iterator &c_it );
		void					newConnection( void );
		v_cli::iterator			&writeResponse( v_cli::iterator &it );
		void					displayServer( void ) const;
};

#endif
