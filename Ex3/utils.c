#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

char *getLine()
{
    char *line = NULL, *tmp = NULL;
    size_t size = 0, index = 0;
    char ch = EOF;

    while (ch) {
        ch = getchar();

        //Verifica se é o fim da linha
        if (ch == EOF || ch == '\n' || feof(stdin))
        {
            ch = '\0';
        }        
        
        //Redimensiona o buffer da string
        if (size <= index) {
            size += 1;
            tmp = realloc(line, size);
            if (tmp == NULL) {
                free(line);
                line = NULL;
                break;
            }
            line = tmp;
        }

        line[index++] = ch;
    }

    return line;
}

char *getComando(char *linha)
{
    char *comando;
    comando = strtok(linha, " ");
    return comando;
}

char *getArgs()
{
    char *args;
    args = strtok(NULL, " ");
    return args;
}
