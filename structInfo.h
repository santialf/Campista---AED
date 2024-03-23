#ifndef _STRUCT_H
#define _STRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

/* game info */
int currGameL();
int currGameC();
int currGameLine(int);
int currGameColumn(int);
char currGameMap(int, int);
void fillPosWithGrassOrTent(int, int, char);

/* counters and onthers */
int adjacentTreeOrOpenCounter (int, int, char);
int *loneliestTree();
int lineOrColumnTentCounter (char);
int sumTentsInLineOrColumn(char, int);
int sumOpenSpacesInLineOrColumn(char, int);
int trainCounter(char, int, int);
int treeCounter();
int sideOpenCounter(char, int, int);
int win();
int checkIfTentIsInsideMap(int, int);

/* allocation and free of memory */
int loadGameInfo(FILE *, char *);
int readGameMapFromFile (FILE *, int);
void freeTable(char **);
void freeStruct (char);
char **allocTable();

void writeFile (FILE *, int);

#endif
