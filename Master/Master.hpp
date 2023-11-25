/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Master.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:59:48 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/25 13:34:09 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MASTER_HPP
# define MASTER_HPP

# include "../Server/Server.hpp"

typedef std::vector<Server> v_ser;

class Master {
	private:
		v_ser	_cluster;
		void	configCleaner( std::ifstream &configFile, std::string &content );
	public:
		Master( void );
		Master( const char* path );
		Master( const Master &src );
		Master& operator=( const Master &src );
		~Master( );

		void			setCluster( const v_ser &cluster);
		const v_ser&	getCluster( void ) const;

		void	configDivider( const char* path );
		void	serverParser( std::string &block );
		void	displayMaster( void ) const;
		void	arrangeCluster( void );
		void	start( void );
};

#endif
