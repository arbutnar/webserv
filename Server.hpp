/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:23:11 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/03 18:34:00 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Master.hpp"

class Server {
	friend void Master::validateConfig( void );
	private:
		std::string					main;
		std::vector<std::string> 	locations; // in realta' probabilmente un vettore di mappe
	public:
		Server( void );
		
		void    	setLocationBlock( std::string block );
		std::string	getMainDirectives( void );
};

std::ostream &operator<<(std::ostream &out, Server &server );
