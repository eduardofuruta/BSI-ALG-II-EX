/*
Adaptado do código de:
Henrique Gomes Zanin NUSP: 10441321
Gabriel Guimaraes Vilas Boas Marin NUSP: 11218521

A estrutura do código foi baseado no arquivo disponibilizado este arquivo disponibilizado pela disciplina,
algumas funções foram alteradas para se adequarem a este projeto
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "btree.h"

struct _page_
{
    record *records;
    long *childs;
    short numberOfKeys;
    BOOL isLeaf;
    int pageRRN;
};

struct _promotedkey_
{
    int key;
    long recordRRN;
    long childs[2];
};

/*Lê os dados da árvore do arquivo
  Esta função considera que o ponteiro fp ja esteja posicionado no início dos dados a serem lidos
  ou seja, é necessário chamar o fseek antes de chamar essa função*/

btPage *readPageFromFile(FILE *fp){
    //Aloca espaço para carregar página
    btPage *page = malloc(sizeof(btPage));
    page->records = malloc(sizeof(record)*MAX_KEYS);
    page->childs = malloc(sizeof(long)*(MAX_KEYS + 1));
    page->pageRRN = ftell(fp)/PAGESIZE;

    //Lê dados da página do arquivo
    fread(&(page->numberOfKeys), sizeof(short), 1, fp);
    fread(&(page->isLeaf), sizeof(BOOL), 1, fp);
    fread(page->records, sizeof(record), MAX_KEYS, fp);
    fread(page->childs, sizeof(long), MAX_KEYS + 1, fp);

    return page;
}

/*Posiciona o ponteiro do arquivo no local correto de escrita a partir do RRN e escreve os dados da árvore no arquivo*/

ERROR writePageIntoFile(long rrn, btPage *page, FILE *fp){
    //Verifica se está tudo ok com os dados
    if (page == NULL || fp == NULL)
    {
        return FALHA;
    }

    //Encontra local para escrita baseado no RRN
    if (rrn != 0)
    {
        fseek(fp, rrn*PAGESIZE, SEEK_SET);
    }
    else
    {
        fseek(fp, 4, SEEK_SET);
    }

    //Escreve dados
    //Atualiza valor de espaço livre na página
    //Dica: você pode criar uma função que só lida com a escrita dos dados e chamar aqui
    fwrite(&(page->numberOfKeys), sizeof(short), 1, fp);    
    fwrite(&(page->isLeaf), sizeof(BOOL), 1, fp);    
    fwrite(page->records, sizeof(record), MAX_KEYS, fp);
    fwrite(page->childs, sizeof(long), MAX_KEYS + 1, fp);

    // Preenche o espaço que não é utilizada pelo dado no arquivo com qualquer coisa pra manter o tamanho da página fixa
    char *blank;
    if(rrn == 0)
    {
        blank = malloc(sizeof(char)*(FREE_SPACE_ON_PAGE - 4));
        fwrite(blank, sizeof(char), FREE_SPACE_ON_PAGE - 4, fp);
    }
    else
    {
        blank = malloc(sizeof(char)*FREE_SPACE_ON_PAGE);
        fwrite(blank, sizeof(char), FREE_SPACE_ON_PAGE, fp);
    }

    free(blank);
    fflush(fp);

    return OK;
}

/*Retorna a página do arquivo a partir do RRN*/
btPage *getPage(long RRN, FILE *fp){
    //Recupera uma página baseado no RRN
    //Procura e carrega seus dados
    if(RRN == 0)
    {
        fseek(fp, 4, SEEK_SET);
    }
    else
    {
        fseek(fp, RRN*PAGESIZE, SEEK_SET);
    }
    return readPageFromFile(fp);
}

/*Lê o RRN da raíz da árvore que se encontra nos primeiros 4 bytes do arquivo da árvore*/
long getTreeHeader(FILE *fp){
    //Carrega o cabeçalho da árvore, que está no início do arquivo
    int headerRRN;
    fseek(fp, 0, SEEK_SET);
    fread(&headerRRN, sizeof(int), 1, fp);
    return headerRRN;
}

/*Escreve o RRN da raíz da árvore no início do arquivo*/
void writeTreeHeader(FILE *fp, long rootRRN){
    //Escreve no cabeçalho do arquivo da árvore, junto com o RRN do nó raíz
    fseek(fp, 0, SEEK_SET);
    int headerRRN = (int) rootRRN;
    fwrite(&headerRRN, sizeof(int), 1, fp);
    fflush(fp);
}

/* Cria uma página em branco*/
btPage *_createBlankPage(FILE *fp)
{
    btPage *blankPage = malloc(sizeof(btPage));
    blankPage->childs = malloc(sizeof(long)*(MAX_KEYS + 1));
    blankPage->numberOfKeys = 0;
    blankPage->records  = malloc(sizeof(record)*MAX_KEYS);
    fseek(fp, 0, SEEK_END);
    blankPage->pageRRN = ftell(fp)/PAGESIZE;
    for(int i = 0; i < MAX_KEYS; i++)
    {
        blankPage->records[i].key = -1;
        blankPage->records[i].recordRRN = -1;
        blankPage->childs[i] = -1;
    }
    blankPage->childs[MAX_KEYS] = -1;
    writePageIntoFile(blankPage->pageRRN, blankPage, fp);
    return blankPage;
}

/*Cria uma árvore nova*/
btPage *createTree(FILE *fp){
    //Aloca espaço pra raiz
    btPage *rootPage = _createBlankPage(fp);
    
    //Inicializa os valores
    rootPage->isLeaf = TRUE;
    
    //Escreve a raiz no cabeçalho
    setNodeAsRoot(rootPage, fp);
    return rootPage;
}

/*Função que retorna a raíz da árvore, se a árvore não existe ele cria uma nova*/
btPage *getOrCreateRoot(FILE *fp){
    //Verifica se a árvore já existe ou precisa criar uma nova
    fseek(fp, 0, SEEK_END);
    long rootRRN = getTreeHeader(fp);
    if(ftell(fp) > 0)
    {
        return getPage(rootRRN, fp);
    }
    //Se a árvore não existir, cria ela
    return createTree(fp);    
}

/*Insere uma chave no nó, se ocorrer o split retorna a chave promovida, se não ocorrer split retorna NULL*/
promotedKey *insertIntoNode(btPage *page, promotedKey *newKey, FILE *fp)
{
    //Procura local pra inserir nova chave na página
    int pos;
    for(pos = 0; pos < page->numberOfKeys; pos++)
    {
        if(newKey->key < page->records[pos].key || page->records[pos].key == -1)
        {
            break;
        }
    }
    if(page->numberOfKeys < MAX_KEYS)
    {
        //Verifica se a nova chave entra na última posição da lista ou se precisa encaixar ela no lugar
        if(pos == page->numberOfKeys)
        { 
            page->records[pos].key = newKey->key;
            page->records[pos].recordRRN = newKey->recordRRN;
            page->childs[pos] = newKey->childs[0];
            page->childs[pos + 1] = newKey->childs[1];
        }
        else
        {
            memcpy(&(page->records[pos + 1]), &(page->records[pos]), sizeof(record)*(page->numberOfKeys - pos));
            memcpy(&(page->childs[pos + 1]), &(page->childs[pos]), sizeof(long)*(page->numberOfKeys - pos + 1));
            page->records[pos].key = newKey->key;
            page->records[pos].recordRRN = newKey->recordRRN;
            page->childs[pos] = newKey->childs[0];
            page->childs[pos + 1] = newKey->childs[1];
        }
        page->numberOfKeys++;
        free(newKey);
        newKey = NULL;
    }
    else
    {
        //Se não couber, divide ele
        int inserirEsquerda = pos < page->numberOfKeys/2;
        btPage *newPage = splitAndCreateNewNode(&page, fp);
        if(inserirEsquerda)
        {
            insertIntoNode(page, newKey, fp);
        }
        else
        {
            insertIntoNode(newPage, newKey, fp);
        }
        newKey = extractpromotedKey(newPage);
        newKey->childs[0] = page->pageRRN;
        newKey->childs[1] = newPage->pageRRN;

        //Escreve dados na página nova
        writePageIntoFile(newPage->pageRRN, newPage, fp); 
    }
    
    //Escreve dados na página
    writePageIntoFile(page->pageRRN, page, fp);

    return newKey;
}

/*Busca a página onde será inserida a nova chave*/
btPage *searchPositionOnPage(btPage *page, promotedKey *newKey, FILE *fp)
{
    btPage *paginaEncontrada = NULL;

    //Encontra a posição para inserir a chave
    if(page->isLeaf == TRUE)
    {
        paginaEncontrada = page;

    }
    else
    {
        int pos;
        for(pos = 0; pos < page->numberOfKeys; pos++)
        {
            if(newKey->key < page->records[pos].key)
            {
                break;
            }
        }
        btPage *childPage = getPage(page->childs[pos], fp);
        paginaEncontrada = searchPositionOnPage(childPage, newKey, fp);
    }

    return paginaEncontrada;
}

/*
    Função que divide o nó no meio, copia metade dos dados pro novo nó e apaga os dados tranferidos do nó antigo, retorna o novo
*/
btPage *splitAndCreateNewNode(btPage **page, FILE *fp)
{
    //Pega a referência do ponteiro da página a ser dividida
    btPage *pPage = *page;

    //Encontra a posição do meio das chaves
    int meio = pPage->numberOfKeys/2;

    //Cria espaço pra nova página
    btPage *newPage = _createBlankPage(fp);
    newPage->isLeaf = pPage->isLeaf;

    //Copia metade das chaves pra página nova
    memcpy(newPage->records, &(pPage->records[meio]), sizeof(record)*(pPage->numberOfKeys - meio));
    memcpy(newPage->childs, &(pPage->childs[meio]), sizeof(long)*(pPage->numberOfKeys + 1 - meio));
    newPage->numberOfKeys = pPage->numberOfKeys - meio;
    
    //Limpa elas do nó antigo
    for(int i = meio; i < pPage->numberOfKeys; i++)
    {
        pPage->records[i].key = -1;
        pPage->records[i].recordRRN = -1;
        pPage->childs[i + 1] = -1;
    }

    //Atualiza o número de chaves da página dividida
    pPage->numberOfKeys = meio;
    return newPage;
}

/*Depois de chamar a função splitAndCreateNewNode a página nova da direita é passada para esta função e a chave promovida é extraída dela*/
promotedKey *extractpromotedKey(btPage *page){
    //Aloca espaço pra chave
    promotedKey *newKey = malloc(sizeof(promotedKey));
    
    //Considerando que essa função é chamada na nova folha da direita
    //Seleciona a primeira chave como a promotedKey
    newKey->childs[0] = -1;
    newKey->childs[1] = -1;
    newKey->key = page->records[0].key;
    newKey->recordRRN = page->records[0].recordRRN;
    
    //Move os registros e seus filhos pra esquerda
    memcpy(page->records, &(page->records[1]), sizeof(record)*(page->numberOfKeys - 1));
    memcpy(page->childs, &(page->childs[1]), sizeof(long)*(page->numberOfKeys));
    page->records[page->numberOfKeys - 1].key = -1;
    page->records[page->numberOfKeys - 1].recordRRN = -1;
    page->childs[page->numberOfKeys] = -1;
    page->numberOfKeys--;

    return newKey;
}

/*Se o novo nó criado for raíz, esta função atualiza o HEADER do arquivo para apontar pra esse nó como raíz*/
ERROR setNodeAsRoot(btPage *page, FILE *fp){
    //Escreve página nova e atualiza o cabeçalho para conter ela como raiz
    //Deveria ser chamada junto com criação de novo nó quando promoção cria uma nova raiz
    writeTreeHeader(fp, page->pageRRN);
    return writePageIntoFile(page->pageRRN, page, fp);        
}

/*Recursive insertion
O conjunto de funções abaixo são os principais na inserção de uma chave na árvore*/

//Função que carrega a próxima página caso o nó atual não seja folha
btPage *_loadNextPage(btPage *node, promotedKey *key, FILE *fp)
{
    int pos;
    for(pos = 0; pos < node->numberOfKeys; pos++)
    {
        if(key->key < node->records[pos].key)
        {
            break;
        }
    }
    return getPage(node->childs[pos], fp);
}

//Inserção recursiva de uma chave na árvore
promotedKey *_bTreeInsert(btPage *node, promotedKey *key, FILE *fp){
    //Se nó a ser inserido a chave é folha, tenta inserir
    if(node->isLeaf == TRUE)
    {
        return insertIntoNode(node, key, fp);
    }
    else
    {
        btPage *insertPage = _loadNextPage(node, key, fp);
        key = _bTreeInsert(insertPage, key, fp);
    }
    //Caso a inserção crie uma promoção, precisa retornar a chave promovida para a recursão
    if(key != NULL)
    {
        return insertIntoNode(node, key, fp);
    }

    return NULL;   
}

//Função mais abstrata de inserção, é chamada na raíz da árvore e esta função chama a função recursiva
ERROR bTreeInsert(record *newRecord, btPage *root, FILE *fp){
    //Prepara os dados da nova chave
    if(newRecord == NULL || root == NULL || fp == NULL)
    {
        return FALHA;
    }
    //Tenta inserir recursivamente
    promotedKey *pKey = malloc(sizeof(promotedKey));
    pKey->key = newRecord->key;
    pKey->recordRRN = newRecord->recordRRN;
    pKey->childs[0] = -1;
    pKey->childs[1] = -1;
    pKey = _bTreeInsert(root, pKey, fp);
   
    
    //Se tiver chave promovida no final da recursão, é que existe nova raiz
    if(pKey != NULL)
    {
        //Chama as funções pra criar nova raiz e atualizar o cabeçalho
        btPage *newRoot = _createBlankPage(fp);
        newRoot->isLeaf = FALSE;
        insertIntoNode(newRoot, pKey, fp);
        setNodeAsRoot(newRoot,fp);
        writePageIntoFile(newRoot->pageRRN, newRoot, fp);
    }
    return writePageIntoFile(root->pageRRN, root, fp);
}

/*Faz uma busca de uma chave na árvore, se for encontrada retorna o RRN da chave, se não retorna -1*/
long bTreeSelect(btPage *node, int key, FILE *fp){
    //Procura no nó atual se a chave existe
    long RRN = -1;
    for(int i = 0; i < node->numberOfKeys; i++)
    {
        if(node->records[i].key == key)
        {
            return node->records[i].recordRRN;    
        }
    }
    if(node->isLeaf == TRUE)
    {
        //Se não encontrar (chegar num nó folha e não estiver lá), retorna -1
        return -1;
    }
    else
    {
        //Se não existir, tenta procurar no filho adequado, recursivamente
        //Se encontrar a chave, retorna RRN dela    
        for(int i = 0; i < node->numberOfKeys; i++)
        {
            if(key < node->records[i].key)
            {
                btPage *tmp = getPage(node->childs[i], fp);
                RRN = bTreeSelect(tmp, key, fp);
                free(tmp);
                break;
            }
        }
        //Verifica se não está no último filho da direita
        if(RRN == -1 && (key > node->records[node->numberOfKeys - 1].key))
        {
            btPage *tmp = getPage(node->childs[node->numberOfKeys], fp);
            RRN = bTreeSelect(tmp, key, fp);
        }
        return RRN;
    }    
}