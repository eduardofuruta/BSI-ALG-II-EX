#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "aluno.h"

int main()
{
    // Operação: insert, search, delete, exit 
    char op[10];

    // Separador do csv
    char separador = ',';

    // Aluno para leitura e gravação dos dados
    ALUNO *aluno = aluno_criar();
    // Verifica se o aluno foi criado
    if(aluno == NULL)
    {
        return -1;
    }

    // Cria o arquivo onde os dados serão salvos
    FILE *fp = fopen("alunos.bin", "wb+");
    // Verifica se o arquivo foi criado com sucesso
    if(fp == NULL)
    {
        return -1;
    }
    
    while(1)
    {
        scanf("%s", op);   
        if (strcmp(op, "insert") == 0)
        {
            aluno_leDadosCSV(aluno, stdin, separador);

            // Verifica se aluno já existe
            ALUNO *tmp = aluno_criar();
            int nUSP = aluno_getNUSP(aluno);
            if(aluno_busca(tmp, fp, nUSP) == -1)
            {
                // Insere aluno no fim do arquivo
                fseek(fp, 0, SEEK_END);
                int offset = ftell(fp);
                aluno_salvaDados(aluno, fp, offset);
            }
            else
            {
                printf("O Registro ja existe!\n");
            }
            aluno_destruir(&tmp);
        }
        else if(strcmp(op, "search") == 0)
        {
            scanf("%s", op);
            if(aluno_busca(aluno, fp, atoi(op)) != -1)
            {
                aluno_imprimeDados(aluno);
            }
            else
            {
                printf("Registro nao encontrado!\n");
            }
        }
        else if(strcmp(op, "delete") == 0)
        {
            scanf("%s", op);
            aluno_apagar(fp, atoi(op));
        }
        else if(strcmp(op, "exit") == 0)
        {
            break;
        }

    }

    aluno_destruir(&aluno);
    fclose(fp); 
    return 0;
}