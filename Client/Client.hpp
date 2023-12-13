/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 16:38:05 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/13 17:57:15 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <sys/socket.h>
# include <unistd.h>

class Client {
	private:
		int			_socket;
		std::string	_buffer;
	public:
		Client( void );
		Client( int const &socket );
		Client( Client const &src );
		Client	&operator=( Client const &src );
		bool	operator==( Client const &src ) const ;
		~Client();

		int const			&getSocket( void ) const;
		std::string const	&getBuffer( void ) const;
		void				setSocket( int const &socket );
		void				setBuffer( std::string const &buffer );

		bool	writeRequest( void );
		void	clearBuffer( void );
};

#endif