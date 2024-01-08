/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Worker.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 18:45:41 by marvin            #+#    #+#             */
/*   Updated: 2024/01/07 18:45:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORKER_HPP
# define WORKER_HPP

# include "../Server/Server.hpp"

typedef std::vector<Server> v_ser;

class Worker {
	private:
		int			_listener;
		v_ser		_servers;
		v_cli		_clients;
	public:
		Worker( void );
		Worker( const int &listener, const v_ser &servers );
		Worker( const Worker &src );
		Worker &operator=( const Worker &src );
		~Worker();

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
		void					displayWorker( void ) const;
};

#endif
