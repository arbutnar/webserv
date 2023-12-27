#include <stdio.h>

int main(int argc, char **argv, char **env) {
	printf("Status: 400\n");
	printf("Connection: close\n\n");
	for (int i = 0; env[i]; i++)
		printf("%d: %s\n", i, env[i]);
	return 0;
}
