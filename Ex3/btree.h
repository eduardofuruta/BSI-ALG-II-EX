#ifndef BTREE_H
#define BTREE_H

#include <stdio.h>
#include "utils.h"
#include "aluno.h"

#define PAGESIZE 4096
#define MAX_KEYS 170
#define AUX_FIELDS_SIZE_ON_PAGE (MAX_KEYS*16 + (MAX_KEYS + 1)*8 + 3)
#define FREE_SPACE_ON_PAGE (PAGESIZE - AUX_FIELDS_SIZE_ON_PAGE)

typedef struct _record_
{
    int key;
    long recordRRN;
} record;

typedef struct _page_ btPage;
typedef struct _promotedkey_ promotedKey;

btPage *readPageFromFile(FILE *fp);
ERROR writePageIntoFile(long rrn, btPage *page, FILE *fp);
btPage *getPage(long RRN, FILE *fp);
long getTreeHeader(FILE *fp);
void writeTreeHeader(FILE *fp, long rootRRN);
btPage *createTree(FILE *fp);
btPage *getOrCreateRoot(FILE *fp);
promotedKey *insertIntoNode(btPage *page, promotedKey *newKey, FILE *fp);
btPage *searchPositionOnPage(btPage *page, promotedKey *newKey, FILE *fp);
btPage *splitAndCreateNewNode(btPage **page, FILE *fp);
promotedKey *extractpromotedKey(btPage *page);
promotedKey *_split(btPage *page,FILE *fp, promotedKey *newKey);
btPage *createNodeWithPromotedKey(promotedKey *promoKey);
ERROR setNodeAsRoot(btPage *page, FILE *fp);
promotedKey *_bTreeInsert(btPage *node, promotedKey *key, FILE *fp);
ERROR bTreeInsert(record *newRecord, btPage *root, FILE *fp);
long bTreeSelect(btPage *node, int key, FILE *fp);

#endif