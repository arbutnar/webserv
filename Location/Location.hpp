/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 17:16:45 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/14 19:34:08 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "../Directives/Directives.hpp"

class   Location : public Directives {
	private:
		std::string     _location_name;
	public:
		Location( void );
		Location( const Location &src );
		Location& operator=( const Location &src );
		~Location( );
		
		void				setLocationName( std::string name );
		const std::string&	getLocationName( void ) const;

		void	parseLocationName( const std::string &line );
		void	clear( void );
		void	displayLocation( void ) const;
};
