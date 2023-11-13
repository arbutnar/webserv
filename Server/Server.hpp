/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:23:11 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/08 16:41:27 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../Location/Location.hpp"

typedef std::vector<Location> v_locs;

class Server : public Directives {
	private:
		v_locs	_locations;
		// std::vector<Socket>		_sockets;
	public:
		Server( void );
		Server( std::string block );
		Server( const Server &src );
		Server& operator=( const Server &src );
		~Server( );

		void			setLocations( const v_locs &locations );
		const v_locs&	getLocations( void );

		void	addLocation( Location &location );
};
