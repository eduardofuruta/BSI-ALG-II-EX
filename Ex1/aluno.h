#ifndef ALUNO_H
#define ALUNO_H
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

    // Salva dados do aluno no arquivo
    void aluno_salvaDados(ALUNO *aluno, FILE *fp);

    // Imprime os dez últimos registros do arquivo
    void aluno_imprime10ultimos(FILE *fp);
#endif