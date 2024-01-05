#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv, char **env) {
	printf("Status: 200 Ok\n\n");
	for (int i = 0; env[i]; i++)
		printf("%d: %s\n", i, env[i]);
	char c = *argv[1];
	return 0;
}
