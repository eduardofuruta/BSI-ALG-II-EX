#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "btree.h"
#include "aluno.h"

/*O main trata basicamente da entrada dos dados e dos comandos.*/

int main()
{
    char *linha;
    char *comando;
    char *args;
    btPage *rootPage = NULL;
    FILE *pTree; /* Arquivo da árvore */
    FILE *pData; /* Arquivo dos dados */
    pTree = fopen("tree.bin", "wb+");
    pData = fopen("data.bin", "wb+");
    if(pTree == NULL || pData == NULL)
    {
        printf("Não foi possível abrir o arquivo de dados\n");
        return -1;
    }
    while(TRUE)
    {
        rootPage = getOrCreateRoot(pTree);
        linha = getLine();
        comando = getComando(linha);
        args = getArgs();
        if(strcmp(comando, "exit") == 0)
        {
            free(linha);
            fclose(pTree);
            fclose(pData);
            return 0;
        }
        else if(strcmp(comando, "insert") == 0)
        {
            ALUNO *aluno = aluno_criar(args);         
            /* Salva os dados do aluno no arquivo de dados se for registro novo*/
            int id = aluno_getNUSP(aluno);
            long RRN = bTreeSelect(rootPage, id, pTree);
            if(RRN == -1)
            {
                RRN = ftell(pData);
                record dados;
                dados.key = id;
                dados.recordRRN = RRN;
                bTreeInsert(&dados, rootPage, pTree);
                aluno_salvaDados(aluno, pData, RRN);
            }
            else
            {
                printf("O Registro ja existe!\n");
            }
            aluno_destruir(&aluno);
        }
        else if(strcmp(comando, "search") == 0)
        {
            int id = atoi(args);
            long RRN = bTreeSelect(rootPage, id, pTree);
            if(RRN == -1)
            {
                printf("Registro nao encontrado!\n");
            }
            else
            {
                ALUNO *aluno = aluno_leDadosArquivo(pData, RRN);
                aluno_imprimeDados(aluno);
                aluno_destruir(&aluno);
            }
        }
        else if (strcmp(comando, "update") == 0)
        {
            ALUNO *aluno = aluno_criar(args);
            int id = aluno_getNUSP(aluno);
            long RRN = bTreeSelect(rootPage, id, pTree);
            if(RRN != -1)
            {
                 aluno_salvaDados(aluno, pData, RRN);
            }
        }
        if (linha != NULL)
        {
            free(linha);
        }
        linha = NULL;
    }
    
    return 0;
}