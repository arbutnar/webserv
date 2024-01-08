/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Master.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:59:48 by arbutnar          #+#    #+#             */
/*   Updated: 2024/01/07 18:56:28 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MASTER_HPP
# define MASTER_HPP

# include "../Worker/Worker.hpp"

typedef std::vector<Worker> v_wrk;

class Master {
	private:
		v_wrk	_workers;
	public:
		Master( void );
		Master( const char* path );
		Master( const Master &src );
		Master& operator=( const Master &src );
		~Master( );

		const v_wrk&	getWorkers( void ) const;
		void			setWorkers( const v_wrk &workers);
		
		void	configDivider( const char* path );
		Server	*serverParser( std::string &block );
		void	arrangeWorkers( v_ser &cluster );
		void	start( void );
		void	displayMaster( void ) const;
};

#endif
