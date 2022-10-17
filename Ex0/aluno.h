#ifndef ALUNO_H
#define ALUNO_H
    typedef struct _aluno ALUNO;
	    // Leitura dos dados do aluno
    void leDadosAluno(ALUNO *aluno, FILE *fp);

    // Imprime dados do aluno
    void imprimeDadosAluno(ALUNO *aluno);

    // Retorna a quantidade de registros de alunos no arquivo
    long nRegistros(FILE *fp);

    // Exibe todos os registros de aluno do arquivo
    void exibeTodos(FILE *fp);

    // Exibe os registros dos alunos do início até a metade do arquivo
    void exibeInicioMetade(FILE *fp);

    // Exibe os registros de alunos da metade até o fim
    void exibeMetadeFim(FILE *fp);

    // Exibe faixa de registros de alunos do arquivo
    void exibeFaixa(FILE *fp, int inicio, int fim);

    // Exibe registro do aluno em posição específica do arquivo
    void exibePosicao(FILE *fp, int pos);
#endif