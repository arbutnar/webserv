/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Master.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:59:48 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/08 13:56:03 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "../Server/Server.hpp"

class Master {
	private:
		std::vector<Server>	_cluster;
	public:
		Master( void );
		Master( const char* path );
		Master( const Master &src );
		Master& operator=( Master &src );	// const
		~Master( );

		bool	configSyntax( std::string content );
		void	configParse( void );
};
