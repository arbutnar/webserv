/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Master.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:59:48 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/14 19:12:36 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "../Server/Server.hpp"

typedef std::vector<Server> v_Ser;

class Master {
	private:
		v_Ser	_cluster;
		void	configCleaner( std::ifstream &configFile, std::string &content );
	public:
		Master( void );
		Master( const char* path );
		Master( const Master &src );
		Master& operator=( const Master &src );
		~Master( );

		void			setCluster( const v_Ser &cluster);
		const v_Ser&	getCluster( void ) const;

		void	configDivider( const char* path );
		void	serverParser( std::string &block );
		void	displayMaster( void ) const;
};
