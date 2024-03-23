#ifndef _CHAIN_H
#define _CHAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "stack.h"

void fillEmptySpacesWithGrass();
int checkLineOrColumnArray(int, int, Stack*);

int fillAroundTent(int, int, Stack*);
int fillAroundTreeWithGrass(int, int, Stack*);
int fillWithGrassLoC(int, char, Stack*);
int fillWithTentsLoC(int, char, Stack*);

int fillOddTrainWithTents(int, int, char, int, Stack*);
int fillEvenTrainWithTents(int, int, char, int, Stack*);
#endif
