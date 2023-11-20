/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 16:38:05 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/20 16:56:40 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>

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

		void				buildBuffer( const char &c );
		void				clearBuffer( void );
};
