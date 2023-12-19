/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:23:11 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/14 15:18:46 by arbutnar         ###   ########.fr       */
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
# include <fcntl.h>
# include "../Location/Location.hpp"
# include "../Request/Request.hpp"
# include "../Response/Error.hpp"
# include "../Response/Valid.hpp"

typedef std::vector<Request> v_req;

class Server : public Directives {
	private:
		s_locs		_locations;
		int			_listener;
		m_intStr	_connections;
	public:
		Server( void );
		Server( const Server &src );
		Server& operator=( const Server &src );
		bool	operator<( const Server &src );
		~Server( );

		void			setLocations( const s_locs &locations );
		void			setListener( const int &listener );
		void			setConnections( const m_intStr &connections );
		const s_locs	&getLocations( void ) const;
		const int		&getListener( void ) const;
		const m_intStr	&getConnections( void ) const;

		void	addLocation( const Location &location );
		int		nfds( void ) const;
		void	ListenerInit( void );
		void	newConnection( void );
		bool 	buildBuffer( m_intStr::iterator &c_it );
		void	eraseConnection( m_intStr::iterator &c_it );
		void	menageConnection( const fd_set &read, const fd_set &write );
		Request requestParser( m_intStr::iterator &c_it );
		bool	writeResponse( m_intStr::iterator &c_it );
		void	displayServer( void ) const;
};

#endif
