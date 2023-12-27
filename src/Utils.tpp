template <typename T>
std::string toString(const T val)
{
    std::stringstream stream;
    stream << val;
    return stream.str();
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