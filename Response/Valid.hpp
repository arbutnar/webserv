/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Valid.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 11:47:36 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/02 17:45:03 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALID_H
# define VALID_H

#include "Response.hpp"

class Valid : public Response {
	public:
		Valid( void );
		Valid( const std::string &code );
		Valid( const Valid &src );
		Valid &operator=( const Valid &src );
		~Valid( );

		void	generateBody( void );
};

#endif