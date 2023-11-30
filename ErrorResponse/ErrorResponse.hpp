/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorResponse.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 17:43:40 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/30 18:35:16 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORRESPONSE_H
# define ERRORRESPONSE_H

#include "../Response/Response.hpp"

class ErrorResponse : protected Response {
	public:
		ErrorResponse( void );
		ErrorResponse( const std::string &code );
		ErrorResponse( const ErrorResponse &src );
		ErrorResponse &operator=( const ErrorResponse &src );
		~ErrorResponse( );

		// void	generateBody( void ) const;
		void	sendResponse( void ) const;
};

#endif