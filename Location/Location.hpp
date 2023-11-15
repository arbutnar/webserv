/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 17:16:45 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/15 15:09:35 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "../Directives/Directives.hpp"

class   Location : public Directives {
	private:
		std::string     _location_name;
	public:
		Location( void );
		Location( const std::string &locationName );
		Location( const Location &src );
		Location& operator=( const Location &src );
		~Location( );
		
		void				setLocationName( std::string name );
		const std::string&	getLocationName( void ) const;

		void	displayLocation( void ) const;
};

#endif