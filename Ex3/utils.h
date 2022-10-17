//Funções auxiliares do exercício

#ifndef UTILS_H
#define UTILS_H
#define TRUE 1
#define FALSE 0

    typedef char BOOL;
    
    // Função auxiliar para leitura do nome do arquivo dada pelo usuário (entrada do usuário)
    char *getLine();

    // Códigos de erros
    typedef enum _erro_{
        FALHA,
        OK
    } ERROR;
    
    // Função para extrair o comando digitado
    char *getComando(char *);

    // Função para extrair os argumentos da instrução digitada
    char *getArgs();
#endif