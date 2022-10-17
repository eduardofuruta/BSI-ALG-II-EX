#ifndef ALUNO_H
#define ALUNO_H
    #define SIZE_NOME       15
    #define SIZE_SOBRENOME  15
    #define SIZE_CURSO      50
    typedef struct _aluno ALUNO;
	
    // retorna objeto aluno
    ALUNO* aluno_criar();

    // libera objeto aluno da memória
    void aluno_destruir(ALUNO **aluno);

    // Leitura dos dados do aluno de arquivo binário
    void aluno_leDadosArquivo(ALUNO *aluno, FILE *fp);

    // Leitura dos dados do aluno do arquivo csv separador pelo "delimiter"
    void aluno_leDadosCSV(ALUNO* aluno, FILE *fp, char delimiter);

    // Imprime dados do aluno
    void aluno_imprimeDados(ALUNO *aluno);

    // Retorna a quantidade de registros de alunos no arquivo
    long aluno_nRegistros(FILE *fp);

    // Salva dados do aluno no arquivo na posicao offset dada
    void aluno_salvaDados(ALUNO *aluno, FILE *fp, int offset);

    // Busca aluno por nUSP, retorna o byte offset caso o aluno exista, senão retorna -1
    int aluno_busca(ALUNO *aluno, FILE *fp, int nUSP);

    // Apaga aluno pelo id do arquivo
    void aluno_apagar(FILE *fp, int nUSP);

    // Retorna nUSP dp aluno
    int aluno_getNUSP(ALUNO *aluno);

#endif