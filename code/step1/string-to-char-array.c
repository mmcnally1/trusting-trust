// convert the contents of stdin to their ASCII values (e.g.,
// '\n' = 10) and spit out the <prog> array used in Figure 1 in
// Thompson's paper.
#include <stdio.h>
#define FILE_SIZE 1024 * 8

int main(void) {
	char buffer[FILE_SIZE];
	for (int i = 0; i < FILE_SIZE; i++) {
		buffer[i] = '\0';
	}
	fread(buffer, FILE_SIZE, sizeof(char), stdin);
	int i = 0;
	printf("char prog[] = {\n");
	while (buffer[i] != '\0') {
		printf("\t%d,%c", buffer[i], (i+1)%8==0 ? '\n' : ' ');
		i++;
	}
	printf("0 };\n");
	return 0;
}
