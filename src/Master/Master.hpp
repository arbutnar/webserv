/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Master.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:59:48 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/13 17:52:53 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MASTER_HPP
# define MASTER_HPP

# include "../Cluster/Cluster.hpp"

typedef std::vector<Cluster> v_cluster;

class Master {
	private:
		v_cluster	_clusters;
	public:
		Master( void );
		Master( const char* path );
		Master( const Master &src );
		Master& operator=( const Master &src );
		~Master( );

		const v_cluster&	getClusters( void ) const;
		void				setClusters( const v_cluster &clusters);
		
		void	configCleaner( std::ifstream &configFile, std::string &content );
		void	configDivider( const char* path );
		Server	*serverParser( std::string &block );
		void	displayMaster( void ) const;
		void	arrangeClusters( v_ser &serverVec );
		void	start( void );
};

#endif
