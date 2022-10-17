#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aluno.h"

struct _aluno
{
    int nUSP;
    char nome[50];
    char curso[50];
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
    else
    {
        return NULL;
    }
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
    fread(aluno->nome, 50*sizeof(char), 1, fp);
    fread(aluno->curso, 50*sizeof(char),1, fp);
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
    strcpy(aluno->curso, dados);
	free(dados);
	dados = _leCharCSV(fp, delimiter);
    aluno->nota = atof(dados);
	free(dados);

    // Verifica a validade dos dados do aluno caso inválido atribui -1 para o nUSP
    if(strlen(aluno->nome) == 0 || strlen(aluno->curso) == 0)
    {
        aluno->nUSP = -1;
    }
}

void aluno_imprimeDados(ALUNO *aluno)
{
    printf("nUSP: %d\n", aluno->nUSP);
    printf("Nome: %s\n", aluno->nome);
    printf("Curso: %s\n", aluno->curso);
    printf("Nota: %.2f\n", aluno->nota);
}

long aluno_nRegistros(FILE *fp)
{
    // Posiciona ponteiro no final do arquivo
    fseek(fp, 0, SEEK_END);
    
    // Posição do último byte do arquivo (tamanho do arquivo)
    long size = ftell(fp);

    return size/sizeof(ALUNO);
    
}

void aluno_salvaDados(ALUNO *aluno, FILE *fp)
{
    // Só salva se for um dado válido
    if(aluno->nUSP != -1)
    {
        fwrite(aluno, sizeof(ALUNO), 1, fp);
        fflush(fp);
    }
}

void aluno_imprime10ultimos(FILE *fp)
{
    // Posiciona o ponteiro de arquivos a 10 registros do fim do arquivo
    fseek(fp, -10*sizeof(ALUNO), SEEK_END);
    ALUNO *aluno = aluno_criar();
    if (aluno == NULL)
    {
        return;
    }
    for(int i = 0; i < 9; i++)
    {
        aluno_leDadosArquivo(aluno, fp);
        aluno_imprimeDados(aluno);
        printf("\n");
    }
    
    // Imprime o último aluno sem pular a última linha
    aluno_leDadosArquivo(aluno, fp);
    aluno_imprimeDados(aluno);

    // destrói aluno
    aluno_destruir(&aluno);
}
