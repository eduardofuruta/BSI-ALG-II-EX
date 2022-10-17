#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "aluno.h"

int main()
{   
    // Arquivo de entrada
    FILE *fp;
    // Arquivo de saída
    FILE *saida;
    
    fseek(stdin, 0, SEEK_SET);
    ALUNO *aluno = aluno_criar();
    saida = fopen("saida.bin", "wb");
    // Verifica se o arquivo de saída foi criada
    if(saida == NULL)
    {
        printf("Houve erro ao criar o arquivo de saída\n");
        return -1;
    }

    // Lê cada registro de aluno do CSV e salva no arquivo de saída em formato binário
    while(!feof(stdin))
    {
        aluno_leDadosCSV(aluno, stdin, ',');
        aluno_salvaDados(aluno, saida);
    }
    
    fp = fopen("saida.bin", "rb");
    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo binário\n");
        return -1;
    }

    // Imprime os 10 últimos registros
    aluno_imprime10ultimos(fp);

    // Destrói o aluno
    aluno_destruir(&aluno);

    // Fecha o arquivo csv e o arquivo de saída
    fclose(fp);
    fclose(saida);

    return 0;
}