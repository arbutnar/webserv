/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:23:11 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/16 16:08:41 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include "../Location/Location.hpp"

typedef std::vector<Location> v_locs;

class Server : public Directives {
	private:
		v_locs	_locations;
		fd_set	_sockets;
		int 	_listener;
	public:
		Server( void );
		Server( std::string block );
		Server( const Server &src );
		Server& operator=( const Server &src );
		~Server( );

		void			setLocations( const v_locs &locations );
		void			setSockets( const fd_set &sockets );
		void			setListener( const int &listener );
		const v_locs&	getLocations( void ) const;
		const fd_set&	getSockets( void ) const;
		const int &	getListener( void ) const;

		void	addLocation( const Location &location );
		void	socketInit( void );
		int		nfds( void );
		void	addSocket( const int &socket );
		void	displayServer( void ) const;
};

#endif
