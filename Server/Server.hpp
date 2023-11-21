/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:23:11 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/21 14:47:50 by arbutnar         ###   ########.fr       */
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

typedef std::vector<Location> v_locs;
typedef std::vector<Client> v_cli;

class Server : public Directives {
	private:
		v_locs	_locations;
		int		_listener;
		v_cli	_clients;
	public:
		Server( void );
		Server( std::string block );
		Server( const Server &src );
		Server& operator=( const Server &src );
		~Server( );

		void			setLocations( const v_locs &locations );
		void			setListener( const int &listener );
		void			setClients( const v_cli &clients );
		const v_locs	&getLocations( void ) const;
		const int		&getListener( void ) const;
		const v_cli		&getClients( void ) const;

		void	addLocation( const Location &location );
		void	ListenerInit( void );
		int		nfds( void ) const;
		void	newConnection( void );
		void	readRequest( v_cli::iterator &it );
		void	writeResponse( v_cli::iterator &it );
		void	displayServer( void ) const;
};

#endif
