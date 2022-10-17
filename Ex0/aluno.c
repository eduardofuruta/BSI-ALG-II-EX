#include <stdio.h>
#include <stdlib.h>
#include "aluno.h"

struct _aluno
{
    int nUSP;
    char nome[50];
    char curso[50];
    float nota;
};

void leDadosAluno(ALUNO *aluno, FILE *fp)
{
    fread(&(aluno->nUSP), sizeof(int), 1, fp);
    fread(aluno->nome, 50*sizeof(char), 1, fp);
    fread(aluno->curso, 50*sizeof(char),1, fp);
    fread(&(aluno->nota), sizeof(float),1, fp);
}

void imprimeDadosAluno(ALUNO *aluno)
{
    printf("nUSP: %d\n", aluno->nUSP);
    printf("Nome: %s\n", aluno->nome);
    printf("Curso: %s\n", aluno->curso);
    printf("Nota: %.2f\n", aluno->nota);
}

long nRegistros(FILE *fp)
{
    // Posiciona ponteiro no final do arquivo
    fseek(fp, 0, SEEK_END);
    
    // Posição do último byte do arquivo (tamanho do arquivo)
    long size = ftell(fp);

    return size/sizeof(ALUNO);
    
}

void exibeTodos(FILE *fp)
{
    long n = nRegistros(fp);
    ALUNO *alunos = (ALUNO *)malloc(n*sizeof(ALUNO));
    fseek(fp, 0, SEEK_SET);
    
    // Lê os dados do arquivo
    for(long i = 0; i < n; i++)
    {
        leDadosAluno(&alunos[i], fp);
    }    
    
    // Imprime os dados dos alunos
    for(long i = 0; i < n - 1; i++)
    {
        imprimeDadosAluno(&alunos[i]);
        printf("\n");
    }
    
    // Imprime o último registro sem pular a última linha
    imprimeDadosAluno(&alunos[n - 1]);

    // Libera o vetor de alunos da memória
    for(long i = n - 1; i <= 0; i--)
    {
        free(&alunos[i]);
    }
	free(alunos);
}

void exibeInicioMetade(FILE *fp)
{
    long n = nRegistros(fp)/2;
    ALUNO *alunos = (ALUNO *)malloc(n*sizeof(ALUNO));
    fseek(fp, 0, SEEK_SET);
    
    // Lê os dados do arquivo
    for(long i = 0; i < n; i++)
    {
        leDadosAluno(&alunos[i], fp);
    }    
    
    // Imprime os dados dos alunos
    for(long i = 0; i < n - 1; i++)
    {
        imprimeDadosAluno(&alunos[i]);
        printf("\n");
    }
    
    // Imprime o último registro sem pular a última linha
    imprimeDadosAluno(&alunos[n - 1]);

    // Libera o vetor de alunos da memória
    for(long i = n - 1; i <= 0; i--)
    {
        free(&alunos[i]);
    }
	free(alunos);
}

void exibeMetadeFim(FILE *fp)
{
    long n = nRegistros(fp)/2;
    ALUNO *alunos = (ALUNO *)malloc(n*sizeof(ALUNO));
    fseek(fp, n*sizeof(ALUNO), SEEK_SET);
    
    // Lê os dados do arquivo
    for(long i = 0; i < n; i++)
    {
        leDadosAluno(&alunos[i], fp);
    }    
    
    // Imprime os dados dos alunos
    for(long i = 0; i < n - 1; i++)
    {
        imprimeDadosAluno(&alunos[i]);
        printf("\n");
    }
    
    // Imprime o último registro sem pular a última linha
    imprimeDadosAluno(&alunos[n - 1]);

    // Libera o vetor de alunos da memória
    for(long i = n - 1; i <= 0; i--)
    {
        free(&alunos[i]);
    }
	free(alunos);
}

void exibeFaixa(FILE *fp, int inicio, int fim)
{
    long n;

    // Verifica se a posição final está dentro da quantidade de registros
    if (fim > nRegistros(fp))
    {
        n = nRegistros(fp) - inicio + 1;
    }
    else
    {
        n = fim - inicio + 1;
    }

    ALUNO *alunos = (ALUNO *)malloc(n*sizeof(ALUNO));
    fseek(fp, (inicio - 1)*sizeof(ALUNO), SEEK_SET);
    
    // Lê os dados do arquivo
    for(long i = 0; i < n; i++)
    {
        leDadosAluno(&alunos[i], fp);
    }    
    
    // Imprime os dados dos alunos
    for(long i = 0; i < n - 1; i++)
    {
        imprimeDadosAluno(&alunos[i]);
        printf("\n");
    }
    
    // Imprime o último registro sem pular a última linha
    imprimeDadosAluno(&alunos[n - 1]);

    // Libera o vetor de alunos da memória
    for(long i = n - 1; i <= 0; i--)
    {
        free(&alunos[i]);
    }
	free(alunos);
}

void exibePosicao(FILE *fp, int pos)
{
   
    ALUNO *aluno = (ALUNO *)malloc(sizeof(ALUNO));
    fseek(fp, (pos - 1)*sizeof(ALUNO), SEEK_SET);
    
    // Lê os dados do arquivo
    leDadosAluno(aluno, fp);    
    
    // Imprime os dados dos alunos
    imprimeDadosAluno(aluno);
    
    // Libera aluno da memória
    free(aluno);
}