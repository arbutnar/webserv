/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 17:16:45 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/25 16:59:24 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "../Directives/Directives.hpp"

class   Location : public Directives {
	private:
		int			_modifier;
		std::string	_location_name;
	public:
		Location( void );
		Location( const std::string &line );
		Location( const Location &src );
		Location	&operator=( const Location &src );
		bool		operator<( const Location &src ) const;
		~Location( );
		
		const int			&getModifier( void ) const;
		const std::string	&getLocationName( void ) const;
		void				setModifier( const int &modifier );
		void				setLocationName( const std::string &name );

		void	displayLocation( void ) const;
};

#endif