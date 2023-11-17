/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:23:11 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/17 12:26:08 by arbutnar         ###   ########.fr       */
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

typedef std::vector<Location> v_locs;
typedef std::set<int> s_sock;

class Server : public Directives {
	private:
		v_locs	_locations;
		s_sock	_sockets;
	public:
		Server( void );
		Server( std::string block );
		Server( const Server &src );
		Server& operator=( const Server &src );
		~Server( );

		void			setLocations( const v_locs &locations );
		void			setSockets( const s_sock &sockets );
		const v_locs&	getLocations( void ) const;
		s_sock			getSockets( void ) const;

		void	addLocation( const Location &location );
		void	socketInit( void );
		void	newConnection( void );
		void	readRequest( s_sock::iterator &socket, fd_set* const activeSockets );
		void	displayServer( void ) const;
};

#endif
