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
		s_locs	_locations;
		int		_listener;
		v_req	_requests;
	public:
		Server( void );
		Server( const Server &src );
		Server& operator=( const Server &src );
		bool	operator<( const Server &src );
		~Server( );

		void			setLocations( const s_locs &locations );
		void			setListener( const int &listener );
		void			setRequests( const v_req &connections );
		const s_locs	&getLocations( void ) const;
		const int		&getListener( void ) const;
		const v_req		&getRequests( void ) const;

		void			addLocation( const Location &location );
		int				nfds( void ) const;
		void			ListenerInit( void );
		void			newRequest( void );
		v_req::iterator	eraseRequest( v_req::iterator &r_it );
		void			menageRequest( const fd_set &read, const fd_set &write );
		void			bufferChecker( const std::string &buffer ) const;
		void			requestParser( v_req::iterator &r_it );
		void			writeResponse( v_req::iterator &it );
		void			displayServer( void ) const;
};

#endif
