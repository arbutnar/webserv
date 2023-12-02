/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp 			                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbutnar <arbutnar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 17:43:40 by arbutnar          #+#    #+#             */
/*   Updated: 2023/11/30 18:35:16 by arbutnar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

#include "Response.hpp"

class Error : public Response {
	public:
		Error( void );
		Error( const std::string &code );
		Error( const Error &src );
		Error &operator=( const Error &src );
		~Error( );

		void	generateContent( const bool &isConnected );
};

#endif