/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.tpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 18:45:48 by marvin            #+#    #+#             */
/*   Updated: 2024/01/07 18:45:48 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

template <typename T>
std::string toString(const T val)
{
    std::stringstream ss;
    ss << val;
    return ss.str();
}

template <typename T>
void	closePipe( T pipe[2] ) {
	close(pipe[0]);
	close(pipe[1]);
}

template <typename T>
void	freeMatrix( T **matrix ) {
	for(int i = 0; matrix[i]; i++)
		free(matrix[i]);
}

template <typename T>
std::string	removeComments(T str) {
	size_t		pos;

	while ((pos = str.find('#')) != std::string::npos)
		str.erase(pos, str.find_first_of("\n", pos) - pos);
	return str;
}

template <typename T>
std::string	removeEmptyLine(T str) {
	std::string	tmp;
	size_t		pos = 0;

	while ((pos = str.find_first_of("\n")) != std::string::npos)
	{
		pos += 1;
		if (str.substr(0, pos).find_first_not_of(" \t\n\r") != std::string::npos)
			tmp += str.substr(0, pos);
		str.erase(0, pos);
	}
	tmp += str;
	return tmp;
}
