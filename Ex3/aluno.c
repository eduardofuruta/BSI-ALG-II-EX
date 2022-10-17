#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aluno.h"
#include "utils.h"

struct _aluno
{
    int nUSP;
    char nome[SIZE_NOME];
    char sobrenome[SIZE_SOBRENOME];
    char curso[SIZE_CURSO];
    float nota;
};

/* Funções auxiliares para ler os dados do aluno*/
char *_getID(char *args)
{
    char *id = strtok(args, ",");
    return id;
}

char *_getNextArg()
{
    char *next = strtok(NULL, ",");
    return next;
}

char *_getString(char *args)
{
    int index = 1;
    if(args[0] == '"')
    {
        while(TRUE)
        {
            if(args[index] == '\0')
            {
                args[index] = ' ';
            }
            else if(args[index] == '"')
            {
                args[index] = '\0';
                break;
            }
            index++;
        }
    }

    return args;
}

char *_getGrade(char *args)
{
    int index = 0;
    while(TRUE)
    {
        //printf("%c", args[index]);
        index++;
        if(args[index] == ',')
        {
            return &args[index + 1];
        }
    }
    return 0;
}

ALUNO* aluno_criar(char *args)
{
    ALUNO *aluno = malloc(sizeof(ALUNO));
    if (aluno != NULL)
    {
        char *id = _getID(args);
        aluno->nUSP = atoi(id);

        char *next = _getNextArg();
        next = _getString(next);
        strcpy(aluno->nome, &next[1]);

        next = _getNextArg();
        next = _getString(next);
        strcpy(aluno->sobrenome, &next[1]);

        next = _getNextArg();
        next = _getString(next);
        strcpy(aluno->curso, &next[1]);

        next = _getGrade(next);
        aluno->nota = atof(next);

        return aluno;
    }

  return NULL;
}

void aluno_destruir(ALUNO **aluno)
{
    if(*aluno != NULL)
    {
        free(*aluno);
        *aluno = NULL;
    }
}

ALUNO *aluno_leDadosArquivo(FILE *fp, long RRN)
{
    fseek(fp, RRN, SEEK_SET);
    ALUNO *aluno = malloc(sizeof(ALUNO));
    fread(&(aluno->nUSP), sizeof(int), 1, fp);
    fread(aluno->nome, SIZE_NOME*sizeof(char), 1, fp);
    fread(aluno->sobrenome, SIZE_SOBRENOME*sizeof(char), 1, fp);
    fread(aluno->curso, SIZE_CURSO*sizeof(char),1, fp);
    fread(&(aluno->nota), sizeof(float),1, fp);
    return aluno;
}

// Função auxiliar para leitura do CSV
static char* _leCharCSV(FILE *fp, char separador)
{
    char *line = NULL, *tmp = NULL;
    size_t size = 0, index = 0;
    char ch = EOF;

    while (ch) {
        fread(&ch, sizeof(char), 1, fp);

        //Verifica se é o fim da linha, separador ou fim do arquivo
        if (feof(fp) || ch == '\n' || ch == separador)
        {
            ch = 0;
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

void aluno_leDadosCSV(ALUNO *aluno, FILE *fp, char delimiter)
{
    char *dados;

	dados = _leCharCSV(fp, delimiter);
	aluno->nUSP = atoi(dados);
	free(dados);

	dados = _leCharCSV(fp, delimiter);
    strcpy(aluno->nome, dados);
	free(dados);

    dados = _leCharCSV(fp, delimiter);
    strcpy(aluno->sobrenome, dados);
	free(dados);

	dados = _leCharCSV(fp, delimiter);
    strcpy(aluno->curso, dados);
	free(dados);

	dados = _leCharCSV(fp, delimiter);
    aluno->nota = atof(dados);
	free(dados);
}

void aluno_imprimeDados(ALUNO *aluno)
{
    printf("-------------------------------\n");
    printf("nUSP: %d\n", aluno->nUSP);
    printf("Nome: %s\n", aluno->nome);
    printf("Sobrenome: %s\n", aluno->sobrenome);
    printf("Curso: %s\n", aluno->curso);
    printf("Nota: %.2f\n", aluno->nota);
    printf("-------------------------------\n");
}

long aluno_nRegistros(FILE *fp)
{
    // Posiciona ponteiro no final do arquivo
    fseek(fp, 0, SEEK_END);
    
    // Posição do último byte do arquivo (tamanho do arquivo)
    long size = ftell(fp);

    return size/sizeof(ALUNO);
    
}

void aluno_salvaDados(ALUNO *aluno, FILE *fp, long RRN)
{
    fseek(fp, RRN, SEEK_SET);
    fwrite(aluno, sizeof(ALUNO), 1, fp);
    fflush(fp);
}

void aluno_apagar(FILE *fp, long RRN)
{
    // aluno temporário para verificar se o aluno existe no arquivo
    ALUNO *aluno = aluno_leDadosArquivo(fp, RRN);
    aluno->nUSP = -1;
    aluno_salvaDados(aluno, fp, RRN);
    
}

int aluno_getNUSP(ALUNO *aluno)
{
    return aluno->nUSP;
}