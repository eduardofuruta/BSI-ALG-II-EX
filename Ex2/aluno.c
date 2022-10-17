#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aluno.h"

struct _aluno
{
    int nUSP;
    char nome[SIZE_NOME];
    char sobrenome[SIZE_SOBRENOME];
    char curso[SIZE_CURSO];
    float nota;
};


ALUNO* aluno_criar()
{
    ALUNO *aluno = malloc(sizeof(ALUNO));
    if (aluno != NULL)
    {
        aluno->nUSP = -1;
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

void aluno_leDadosArquivo(ALUNO *aluno, FILE *fp)
{
    fread(&(aluno->nUSP), sizeof(int), 1, fp);
    fread(aluno->nome, SIZE_NOME*sizeof(char), 1, fp);
    fread(aluno->sobrenome, SIZE_SOBRENOME*sizeof(char), 1, fp);
    fread(aluno->curso, SIZE_CURSO*sizeof(char),1, fp);
    fread(&(aluno->nota), sizeof(float),1, fp);
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
    printf("USP number: %d\n", aluno->nUSP);
    printf("Name: %s\n", aluno->nome);
    printf("Surname: %s\n", aluno->sobrenome);
    printf("Course: %s\n", aluno->curso);
    printf("Test grade: %.2f\n", aluno->nota);
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

void aluno_salvaDados(ALUNO *aluno, FILE *fp, int offset)
{
    fseek(fp, offset, SEEK_SET);
    fwrite(aluno, sizeof(ALUNO), 1, fp);
    fflush(fp);
}

int aluno_busca(ALUNO *aluno, FILE *fp, int nUSP)
{
    // Busca desde o inicio do arquivo
    fseek(fp, 0, SEEK_SET);
    
    // Posição relativa do dado do aluno no arquivo
    int posicao;

    // Percorre o arquivo em busca do aluno
    while(!feof(fp))
    {
        aluno_leDadosArquivo(aluno, fp);
        if(aluno->nUSP == nUSP)
        {
            posicao = ftell(fp)/sizeof(ALUNO) - 1;
            return posicao*sizeof(ALUNO);    
        }
    }
    return -1;
}

void aluno_apagar(FILE *fp, int nUSP)
{
    // aluno temporário para verificar se o aluno existe no arquivo
    ALUNO *aluno = aluno_criar();

    // verifica se aluno existe
    int offset = aluno_busca(aluno, fp, nUSP); 
    if (offset != -1)
    {
        aluno->nUSP = -1;
        aluno_salvaDados(aluno, fp, offset);
    }
    aluno_destruir(&aluno);
}

int aluno_getNUSP(ALUNO *aluno)
{
    return aluno->nUSP;
}