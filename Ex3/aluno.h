/*A operações sobre os dados ALUNO foram reaproveitadas do exercício anterior, portanto algumas funções aqui
não foram utilizadas no projeto*/
#ifndef ALUNO_H
#define ALUNO_H
    #define SIZE_NOME       15
    #define SIZE_SOBRENOME  15
    #define SIZE_CURSO      50
    typedef struct _aluno ALUNO;
	
    // retorna objeto aluno
    ALUNO* aluno_criar(char *);

    // libera objeto aluno da memória
    void aluno_destruir(ALUNO **aluno);

    // Leitura dos dados do aluno de arquivo binário
    ALUNO *aluno_leDadosArquivo(FILE *fp, long RRN);

    // Leitura dos dados do aluno do arquivo csv separador pelo "delimiter"
    void aluno_leDadosCSV(ALUNO* aluno, FILE *fp, char delimiter);

    // Imprime dados do aluno
    void aluno_imprimeDados(ALUNO *aluno);

    // Retorna a quantidade de registros de alunos no arquivo
    long aluno_nRegistros(FILE *fp);

    // Salva dados do aluno no arquivo na posicao offset dada
    void aluno_salvaDados(ALUNO *aluno, FILE *fp, long RRN);
   
    // Apaga aluno pelo id do arquivo
    void aluno_apagar(FILE *fp, long RRN);

    // Retorna nUSP dp aluno
    int aluno_getNUSP(ALUNO *aluno);

#endif