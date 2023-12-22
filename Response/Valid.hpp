/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Valid.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 11:47:36 by arbutnar          #+#    #+#             */
/*   Updated: 2023/12/12 10:58:37 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALID_H
# define VALID_H

# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <cerrno>
# include <cstring>
# include <string.h>
# include <dirent.h>
# include "Response.hpp"

class Valid : public Response {
	private:
		std::fstream	_file;
		std::string		_cgiBuffer;
	public:
		Valid( void );
		Valid( const Request &request );
		Valid( const Valid &src );
		Valid &operator=( const Valid &src );
		~Valid( );

		const std::string	&getCgiBuffer( void ) const;
		void			handleAutoindex( void );
		void			generateBody( void );
};

#endif