#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "aluno.h"

int main()
{

    char *nomeArquivo;
    int op;

    // Variaveis utilizadas nas opções 4 e 5
    int inicio, fim, pos;
    FILE *fp;

    nomeArquivo = getLine();
    scanf("%d", &op);
    fp = fopen(nomeArquivo, "rb");
    if(fp != NULL)
    {
        switch(op)
        {
            // Exibe todos os registros
            case 1:
                exibeTodos(fp);
            break;

            // Exibe metade dos registros do começo até o meio
            case 2:
                exibeInicioMetade(fp);
            break;

            // Exibe metade dos registros do meio até o fim
            case 3:
                exibeMetadeFim(fp);
            break;
            
            // Exibe uma faixa de registros
            case 4:
                scanf("%d %d", &inicio, &fim);
                exibeFaixa(fp, inicio, fim);
            break;
            
            // Exibe um registro específico
            case 5:
                scanf("%d", &pos);
                exibePosicao(fp, pos);
            break;
        }
        fclose(fp);
    }
    else
    {
        printf("Não foi possível abrir o arquivo\n");
    }

    free(nomeArquivo);
    return 0;
}
