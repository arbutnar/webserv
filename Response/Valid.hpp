/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Valid.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 11:47:36 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/07 18:16:03 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALID_H
# define VALID_H

# include <sys/types.h>
# include <sys/stat.h>
# include <cerrno>
# include <cstring>
# include "Response.hpp"

class Valid : public Response {
	private:
		Request			_request;
		std::fstream	_file;
	public:
		Valid( void );
		Valid( const Request &request );
		Valid( const Valid &src );
		Valid &operator=( const Valid &src );
		~Valid( );

		const Request	&getRequest( void ) const;
		void			setRequest( const Request &request );
		void			generateBody( void );
};

#endif