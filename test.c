#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {

    int choice = -1;
    char *line, *linep;
    size_t lenmax = 100, inlen = lenmax;
    int c;

    while(choice < 0) {
        line = (char *)malloc(100);
        linep = line;
        printf("linep is @ %p and line is @ %p\n", line, linep);
        lenmax = 100;
        inlen = lenmax;

        for(;;) {

            c = fgetc(stdin);
            if (c == EOF) {
                break;
            }

            if (--inlen == 0) {
                inlen = lenmax;
                char *linen = realloc(linep, lenmax *= 2);

                if(linen == NULL) {
                    free(linep);
                    perror("fail");
                    exit(1);
                }
                line = linen + (line - linep);
                linep = linen;
            }

            if ((*line++ = c) == '\n') {
                break;
            }
        }
        *line = '\0';
        printf("You Chose %s\n", linep);
        free(linep);
    }

    return 0;
}
