/******************************************************************************
* NAME
*   recursiveAndABC.c (we ran out of ideas...)
*
* DESCRIPTION
*   Implementation of complex functions with the objective of validating the map
*and a recursive operation that helps us find it's solution (if there's one...) 
* COMMENTS
*   To be honest, this file was created to get together the leftover functions who
*didn't belong in with the other files, so to not create unecessary extra files,
*we placed them here.
*   Some functions here are used to validate certain aspects of the game,
*and we also placed here the recursive that finds a solution to the game by guessing.
******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "recursiveAndABC.h"
#include "structInfo.h"
#include "chainEffect.h"
/* GR27 */

/******************************************************************************
* randomRecursiveSolve()
*
* Arguments: stack - struct with pointer to the last and first member of the stack
*            char - indicates the season (High or Low)
* Returns: err - 1 if the way we are taking is wrong
*              - 0 if not
* Side-Effects: (none)
*
* Description:
*   Implementation of a DFS (Depth first search) to find a solution to unsolved
*or partially solved map, the operation consists of making a guess, this means putting
*a tent in an openSpace that we cannot confirm to be a tent yet, if nothing goes wrong
*the function calls itself again to make another guess, else we use the operations in stack.c
*to undoo the alterations that were consequence of this decision.
*   These guesses are aimed at trees with the least amount of openSpaces adjacent to them
*so we can reduce the chances of placing a tent in the wrong place, and consequently making
*the search less wide.
*****************************************************************************/
int randomRecursiveSolve(Stack *stack, char season) {
  int i, j, err = 0;
  int *coord = NULL;
  /* checks if the map is solved, return 0 if yes */
  if (win() == 0)
    return 0;
  /* finds the coordinates to the loneliest tree */
  coord = loneliestTree();
  /* visits every adjacent openSpace to the tree */
  for (i = -1; i < 2; i++) {
    for (j = -1; j < 2; j++) {
      if ((j != 0) && (i != 0)) continue;
      if (checkIfTentIsInsideMap(coord[0] + i, coord[1] + j) == -1) continue;
      if (currGameMap(coord[0] + i, coord[1] + j) == 'O') {
	/* turns into a tent said position and stores the change in stack */
	fillPosWithGrassOrTent(coord[0] + i, coord[1] + j, 'T');
	pushStack(stack, coord[0] + i, coord[1] + j);
	/* checks if any changes can be made in the gameMap with this alteration */
	err = fillAroundTent(coord[0] + i, coord[1] + j, stack);
	if (err == 0)
	  err = checkLineOrColumnArray(coord[0] + i, coord[1] + j, stack);
	if (err == 0)
	  err = problemC();
	/* if something goes wrong with consequences of the decision */
	if (err == 1) {
	  /* undoo the consequences of the decision (using the stack) */
	  while (currGameMap(coord[0] + i, coord[1] + j) != 'O')
	    popStack(stack);
	  continue;
	/* if nothing is wrong with the decision (so far...) */
	} else {
	  /* calls itself to make another guess */
	  err = randomRecursiveSolve(stack, season);
	  /* if the map is solved returns 0 until it reaches the surface */
	  if (err == 0) {
	    free(coord);
	    return 0;
	  }
	  /* if the path we took was wrong */
	  if (err == 1) {
	    /* undoo the consequences of the decision (using the stack) */
	    while (currGameMap(coord[0] + i, coord[1] + j) != 'O')
	      popStack(stack);
	  }
	}	
      }
    }
  }
  /* extra condition if we are in low season (no tents around the tree) */
  /* this condition also applies if we are in high season and the tree already has an adjacent tent */
  if ((season == 'L') || ((season == 'H') && (adjacentTreeOrOpenCounter(coord[0], coord[1], 'T') > 0))) {
    /* turns to grass said positions */
    err = fillAroundTreeWithGrass(coord[0], coord[1], stack);
    if (err == 0)
      err = problemC();
    if (err == 0) {
      /* calls itself to make another guess */
      err = randomRecursiveSolve(stack, season);
      /* if the map is solved returns 0 until it reaches the surface */
      if (err == 0) {
	free(coord);
	return 0;
      }
    }
  }
  free(coord);
  return 1;
}

/******************************************************************************
* checkForTreeOrTent()
*
* Arguments: tot - guards the value of current position ('T' or 'A') 
*            L - line number
*            C - column number
*            auxMap - auxiliary table with info of game map (used as a draft) 
*            difCounter - > 0 when number of trees is bigger than number of tents 
*                       - = 0 when number of trees is equal than number of tents
*                       - < 0 when number of trees is smaller than number of tents
* Returns: difCounter ^^
* Side-Effects: (none)
*
* Description:
*   Recursive function that "counts" number of trees and tents connected orthogonaly
*to eachother, does this by decrementing "difCounter" when the current position
*is a tent (tot == 'T') and incrementing it when it is a tree (tot == 'A')
*****************************************************************************/
int checkForTreeOrTent (char tot, int L, int C, char *auxMap[], int difCounter) {
  int i, j;
  auxMap[L][C] = '-';
  
  if (tot == 'T') {
    difCounter--;
    for (i = -1; i < 2; i++) {
      if ((L + i >= currGameL()) || (L + i < 0))
	continue;
      for (j = -1; j < 2; j++) {
	if (((i != 0) && (j != 0)) || (C + i > currGameC()) || (C + j < 0))
	  continue;
	if ((currGameMap(L + i, C + j) == 'A') && (auxMap[L + i][C + j] != '-')) /* watch out: '-' != '.' */
	  difCounter = checkForTreeOrTent ('A', L + i, C + j, auxMap, difCounter);
      }
    }
  } else {
    difCounter++;
    for (i = -1; i < 2; i++) {
      if ((L + i >= currGameL()) || (L + i < 0))
	continue;
      for (j = -1; j < 2; j++) {
	if (((i != 0) && (j != 0)) || (C + i > currGameC()) || (C + j < 0))
	  continue;
	if ((currGameMap(L + i, C + j) == 'T') && (auxMap[L + i][C + j] != '-')) /* watch out: '-' != '.' */
	  difCounter = checkForTreeOrTent ('T', L + i, C + j, auxMap, difCounter);
      }
    }
  }
  return difCounter; 
}

/******************************************************************************
* problemC()
*
* Arguments: (none)
* Returns:
* Side-Effects: allocates space in memory for auxiliary table with the game map
*initialized with '.'
*               frees auxiliary table
*
* Description:
*   Checks if the map contains any logical errors, in other words, if every tree
*is associated with 1 tent maximum
*****************************************************************************/
int problemC() {
  int i, j, answer = 0;
  char **auxMap;

  /* allocates memory for auxMap */
  auxMap = allocTable();

  /* initializes auxMap */
  for (i = 0; i < currGameL(); i++)
    for (j = 0; j < currGameC(); j++)
      auxMap[i][j] = '.';
  
  /* calls recursive function when cycle finds a tent with only one tree in its sides */
  for (i = 0; i < currGameL(); i++)
    for (j = 0; j < currGameC(); j++) {
      if ((currGameMap(i, j) == 'T') && (adjacentTreeOrOpenCounter(i, j, 'A') == 1) && (auxMap[i][j] != '-')) /* watch out: '-' != '.' */
	answer = checkForTreeOrTent ('T', i, j, auxMap, 0);
      if (answer < 0) {
	freeTable(auxMap);
	return (1); 
      }
    }

  freeTable(auxMap);
  return (0);
}

/******************************************************************************
* problemB()
*
* Arguments: coordL - line number
*            coordC - column number
* Returns: admissibility test response for tent placement
* Side-Effects: (none)
*
* Description:
*   Checks if provided cell can have a tent
*****************************************************************************/
int problemB(int coordL, int coordC) {
  int tent = 0;

  /* verifies if coordinates are inside the borders of the map */
  if (checkIfTentIsInsideMap(coordL, coordC) == -1)
    return (-1);
  
  /* verifies if coordinates point to a tree or a tent */
  if (currGameMap(coordL, coordC) == 'A')
    return (1);
  if (currGameMap(coordL, coordC) == 'T')
    tent = 1;
  
  /* verifies if there are no tents in adjacent cells */
  if (adjacentTreeOrOpenCounter (coordL, coordC, 'T') > 0)
    return (1);

  /* verifies if there's atleast 1 tree in orthogonal adjacent cells */
  if (adjacentTreeOrOpenCounter(coordL, coordC, 'A') == 0)
    return (1);

  /* verifies if tent's limit for specified column has been reached */
  if (sumTentsInLineOrColumn('C', coordC) - tent >= currGameColumn(coordC))
    return (1);
  
  /* verifies if tent's limit for specified line has been reached */
  if (sumTentsInLineOrColumn('L', coordL) - tent >= currGameLine(coordL))
    return (1);

  return (0);
}

/******************************************************************************
* problemA()
*
* Arguments: epoch - char that will hold the value of the season ('H' or 'L')
* Returns: admissibility test response
* Side-Effects: (none)
*
* Description:
*   Checks if provided map is admissible
*****************************************************************************/
int problemA(char *epoch) {
  int lSum = 0, treeSum = 0 ;
  
  /* counts number of tents in game */
  lSum = lineOrColumnTentCounter('L');

  /* counts trees in game */
  treeSum = treeCounter();
      
  /* checks if number of trees is less than number of tents */
  if (treeSum < lSum)
    return (-1);
  if (treeSum == lSum)
    *epoch = 'H';
  if (treeSum > lSum)
    *epoch = 'L';

  return (1);
}
