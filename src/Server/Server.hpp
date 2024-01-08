/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:23:11 by arbutnar          #+#    #+#             */
/*   Updated: 2024/01/08 18:52:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/select.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <set>
# include <unistd.h>
# include <fcntl.h>
# include "../Location/Location.hpp"
# include "../Response/Response.hpp"
# include "../Client/Client.hpp"

typedef std::vector<Client> v_cli;

class Server : public Directives {
	private:
		s_locs		_locations;
	public:
		Server( void );
		Server( const Server &src );
		Server& operator=( const Server &src );
		bool	operator==( const Server &src ) const;
		bool	operator==( const std::string &serverName ) const;
		~Server( );

		const s_locs	&getLocations( void ) const;
		void			setLocations( const s_locs &locations );

		void	listenerInit( int &listener ) const;
		void	addLocation( const Location &location );
		bool	requestParser( Request &request, v_cli::iterator &it ) const;
		bool	writeResponse( v_cli::iterator &it ) const;
		void	displayServer( void ) const;
};

#endif
