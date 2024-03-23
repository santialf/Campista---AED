/******************************************************************************
* NAME
*   chainEffect.c
*
* DESCRIPTION
*   Implementation of functions with a particular set of skills, that are in charge
*of operations that consist in knowing when to turn openSpaces into tents or grass
*
* COMMENTS
*   The majority of these functions operate as a whole, in other words, they are a
*cluster of operations that repeatedly call eachother to ensure the well behavior of
*the gameMap, and to check the impact each change has on the map, basically these functions
*are used to see how the map reacts to every change that occurs.
*   Sometimes the consequences of a change are small, other times not...
*
*   "It has been said that something as small as the flutter of a butterfly's wing
*can ultimately cause a typhoon halfway around the gameMap" - unknown :)
*
*
*   Notice that every change we make is stored in a stack, so we can later go back
*if any anomaly happens
******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "recursiveAndABC.h"
#include "structInfo.h"
#include "chainEffect.h"
#include "stack.h"
/* GR27 */

/******************************************************************************
* fillEmptySpacesWithGrass()
*
* Arguments: (none)
* Returns: (none)
* Side-Effects: (none)
*
* Description:
*   Puts 'O' in areas where there may be tents
*****************************************************************************/
void fillEmptySpacesWithGrass() {
  int i, j;
  
  for (i = 0; i < currGameL(); i++)
    for (j = 0; j < currGameC(); j++)
      if ((problemB(i, j) == 0) && (currGameMap(i, j) != 'T'))
	fillPosWithGrassOrTent(i, j, 'O');

  return;
}

/******************************************************************************
* checkLineOrColumnArray()
*
* Arguments: l - line we want to check
*            c - column we want to check
*            stack - struct with pointer to the last and first member of the stack
* Returns: err - 1 if the way we are taking is wrong
*              - 0 if not
* Side-Effects: (none)
*
* Description:
*   Checks if specified line and (and != or) column can be filled with grass or tents,
*for example if a certain line can only have 1 tent, and there's only 1 open space,
*that open space can only be a tent
*   Returns 1 when the number of openSpaces is smaller than the number of tents
*that need placing in that line/column
*****************************************************************************/
int checkLineOrColumnArray(int l, int c, Stack* stack) {
  int open, tents, err = 0;

  /* analyze line vector */
  open = sumOpenSpacesInLineOrColumn('L', l);
  tents = sumTentsInLineOrColumn('L', l);
  /* if we can fill a line with tents */
  if ((open == (currGameLine(l) - tents)) && (open != 0)) {
    err = fillWithTentsLoC(l, 'L', stack);
  /* if we can fill a line with grass */
  } else if ((currGameLine(l) - tents == 0) && (open != 0)) {
    err = fillWithGrassLoC(l, 'L', stack);
  /* if there's not enough space for tents (we are going the wrong way...) */
  } else if (open < currGameLine(l) - tents) {
    return 1;
  }
  if (err == 1)
    return 1;
  /* analyze column vector */
  open = sumOpenSpacesInLineOrColumn('C', c);
  tents = sumTentsInLineOrColumn('C', c);
  /* if we can fill a column with tents */
  if ((open == (currGameColumn(c) - tents)) && (open != 0)) {
    err = fillWithTentsLoC(c, 'C', stack);
  /* if we can fill a column with grass */
  } else if ((currGameColumn(c) - tents == 0) && (open != 0)) {
    err = fillWithGrassLoC(c, 'C', stack);
  /* if there's not enough space for tents (we are going the wrong way...) */
  } else if (open < currGameColumn(c) - tents) {
    return 1;
  }
  if (err == 1)
    return 1;
  return 0;
}

/******************************************************************************
* fillAroundTent()
*
* Arguments: l - line number
*            c - column number
*            stack - struct with pointer to the last and first member of the stack
* Returns: err - 1 if the way we are taking is wrong
*              - 0 if not
* Side-Effects: (none)
*
* Description:
*   Fills adjacent cells with grass (because the specified position is a tent)
*****************************************************************************/
int fillAroundTent(int l, int c, Stack *stack) {
  int i, j, err = 0;
  /* first checks if the placed tent is OK (no other tents around) */
  for (i = -1; i < 2; i++)
    for (j = -1; j < 2; j++) {
      if (checkIfTentIsInsideMap(l + i, c + j) == -1)
	continue;
      if ((i == 0) && (j == 0))
	continue;
      /* if there is already a tent around the cell, return 1 bcs we are going the wrong way */
      if (currGameMap(l + i, c + j) == 'T')
	return 1;
    }
  /* proceeds to turn every adjacent cell into grass */
  for (i = -1; i < 2; i++)
    for (j = -1; j < 2; j++) {
      if (checkIfTentIsInsideMap(l + i, c + j) == -1)
	continue;
      if ((i == 0) && (j == 0))
	continue;
      if (currGameMap(l + i, c + j) == 'O') {
	/* turns to grass said position and stores the change in stack */ 
	fillPosWithGrassOrTent(l + i, c + j, '.');
	pushStack(stack, l + i, c + j);
	/* checks if there are any changes in the gameMap with this alteration */
	err = checkLineOrColumnArray(l + i, c + j, stack);
	if (err == 1)
	  return 1;
      }
    } 

  return 0;
}

/******************************************************************************
* fillAroundTreeWithGrass()
*
* Arguments: l - line number
*            c - column number
*            stack - struct with pointer to the last and first member of the stack
* Returns: err - 1 if the way we are taking is wrong
*              - 0 if not
* Side-Effects: (none)
*
* Description:
*   Fills adjacent cells with grass (assuming that the specified position is a tree)
*****************************************************************************/
int fillAroundTreeWithGrass(int l, int c, Stack *stack) {
  int i, j, err = 0;

  for (i = -1; i < 2; i++)
    for (j = -1; j < 2; j++) {
      if (checkIfTentIsInsideMap(l + i, c + j) == -1)
	continue;
      if ((i != 0) && (j != 0))
	continue;
      if (currGameMap(l + i, c + j) == 'O') {
	/* turns to grass said position and stores the change in stack */
	fillPosWithGrassOrTent(l + i, c + j, '.');
	pushStack(stack, l + i, c + j);
	/* checks if there are any changes in the gameMap with this alteration */
	err = checkLineOrColumnArray(l + i, c + j, stack);
	if (err == 1) {
	  return 1;
	}
      }
    }

  return 0;
}

/******************************************************************************
* fillWithGrassLoC()
*
* Arguments: n - number of line or column to be filled
*            loc - tells us if it is a line or a column
*            stack - struct with pointer to the last and first member of the stack
* Returns: err - 1 if the way we are taking is wrong
*              - 0 if not
* Side-Effects: (none)
*
* Description:
*   Fills specified line or column with grass, for example:
*
* 1- ..AT.A0..AOOA.AOOOA. -> ..AT.A...A..A.A...A.   because that line doesnt
*                                                   have more capacity for tents  
*****************************************************************************/
int fillWithGrassLoC(int n, char loc, Stack *stack) {
  int i, err = 0;
  /* if we are analysing a line */
  if (loc == 'L') {
    for (i = 0; i < currGameC(); i++)
      if (currGameMap(n, i) == 'O') {
	/* turns to grass said position and stores the change in stack */
	fillPosWithGrassOrTent(n, i, '.');
	pushStack(stack, n, i);
	/* checks if there are any changes in the gameMap with this alteration */
	err = checkLineOrColumnArray(n, i, stack);
	if (err == 1)
	  return err;
      }
  /* if we are analysing a column */
  } else {
    for (i = 0; i < currGameL(); i++)
      if (currGameMap(i, n) == 'O') {
	/* turns to grass said position and stores the change in stack */
	fillPosWithGrassOrTent(i, n, '.');
	pushStack(stack, i, n);
	/* checks if there are any changes in the gameMap with this alteration */
	err = checkLineOrColumnArray(i, n, stack);
	if (err == 1)
	    return err;
      }
  }
  return err;
}

/******************************************************************************
* fillWithTentsLoC()
*
* Arguments: n - number of line or column to be filled
*            loc - tells us if it is a line or a column
*            stack - struct with pointer to the last and first member of the stack
* Returns: err - 1 if the way we are taking is wrong
*              - 0 if not
* Side-Effects: (none)
*
* Description:
*   Fills specified line or column with tents, taking into account the openSpaces
*in that line/column, for example:
*
* 5- ..A0.A0..AOOA.AOOOA. -> ..AT.AT..AOOA.AT.TA.   if we know there are
*                                                   5 tents in that line
*****************************************************************************/
int fillWithTentsLoC(int n, char loc, Stack *stack) {
  int i, err = 0, train = 0;
  /* if we are analysing a line */
  if (loc == 'L') {
    for (i = 0; i < currGameC(); i++) {
      /* checks and counts the length of the "trains" (adjacent openSpaces) */
      train = trainCounter('L', n, i);
      /* if the openSpace is isolated */
      if ((currGameMap(n, i) == 'O') && (sideOpenCounter('L', n, i) == 0)) {
	/* turns into a tent said position and stores the change in stack */
	fillPosWithGrassOrTent(n, i, 'T');
	pushStack(stack, n, i);
	/* checks if there are any changes in the gameMap with this alteration */
	err = fillAroundTent(n, i, stack);
	if (err == 1)
	  return err;
	/* checks if there are any changes in the gameMap with this alteration */
	err = checkLineOrColumnArray(n, i, stack);
	if (err == 1)
	  return err;
      }
      /* if we encounter a odd train (ex: ..OOO..) */ 
      if ((train > 2) && ((train % 2) != 0)) {
	err = fillOddTrainWithTents(n, i, 'L', train, stack);
	if (err == 1)
	  return err;
	i += train;
      }
      /* if we encounter an even train (ex: ..OOOO..) */
      if ((train > 1) && ((train % 2) == 0)) {
	err = fillEvenTrainWithTents(n, i, 'L', train, stack);
	if (err == 1)
	  return err;
	i += train;
      }
    }
  /* if we are analysing a column */
  } else {
    for (i = 0; i < currGameL(); i++) {
      /* checks and counts the length of the "trains" (adjacent openSpaces) */
      train = trainCounter('C', i, n);
      if ((currGameMap(i, n) == 'O') && (sideOpenCounter('C', i, n) == 0)) {
	/* turns into a tent said position and stores the change in stack */
	fillPosWithGrassOrTent(i, n, 'T');
	pushStack(stack, i, n);
	/* checks if there are any changes in the gameMap with this alteration */
	err = fillAroundTent(i, n, stack);
	if (err == 1)
	  return err;
	/* checks if there are any changes in the gameMap with this alteration */
	err = checkLineOrColumnArray(i, n, stack);
	if (err == 1)
	  return err;
      }
      /* if we encounter a odd train (ex: ..OOO..) */
      if ((train > 2) && ((train % 2) != 0)) {
	err = fillOddTrainWithTents(i, n, 'C', train, stack);
	if (err == 1)
	  return err;
	i += train;
      }
      /* if we encounter an even train (ex: ..OOOO..) */
      if ((train > 1) && ((train % 2) == 0)) {
	err = fillEvenTrainWithTents(i, n, 'C', train, stack);
	if (err == 1)
	  return err;
	i += train;
      }   
    }
  }
  return err;
}

/******************************************************************************
* fillOddTrainWithTents()
*
* Arguments: l - number of the line
*            c - number of the column
*            loc - tells us if we want to go through a line or a column
*            length - length of the train
*            stack - struct with pointer to the last and first member of the stack
* Returns: err - 1 if the way we are taking is wrong
*              - 0 if not
* Side-Effects: (none)
*
* Description:
*   Fills a train with a odd number of openSpaces with tents, for example:
*
*   ..AOOOA.. -> ..AT.TA..          because we know if those openSpaces
*   ..AOOOOOA.. -> ..AT.T.TA..      reach the maximum capacity of tents,
*   ..AOOOOOOOA.. -> ..AT.T.T.TA..  then these formations are the only 
*                                   ones who can sustain such capacity
*  
*****************************************************************************/
int fillOddTrainWithTents(int l, int c, char loc, int length, Stack *stack) {
  int i = 0, err = 0;
  /* if we are analysing a line */
  if (loc == 'L') {
    while(length > 0) {
      if ((length % 2) != 0) {
	/* turns into a tent said position and stores the change in stack */
	fillPosWithGrassOrTent(l, c + i, 'T');
	pushStack(stack, l, c + i);
	/* checks if there are any changes in the gameMap with this alteration */
	err = fillAroundTent(l, c + i, stack);
	if (err == 1)
	  return err;
	/* checks if there are any changes in the gameMap with this alteration */
	err = checkLineOrColumnArray(l, c + i, stack);
	if (err == 1)
	  return err;
	length--;
	i++;
      } else {
	length--;
	i++;
      }
    }
  /* if we are analysing a column */
  } else {
    while(length > 0) {
      if ((length % 2) != 0) {
	/* turns into a tent said position and stores the change in stack */
	fillPosWithGrassOrTent(l + i, c, 'T');
	pushStack(stack, l + i, c);
	/* checks if there are any changes in the gameMap with this alteration */
	err = fillAroundTent(l + i, c, stack);
	if (err == 1)
	  return err;
	/* checks if there are any changes in the gameMap with this alteration */
	err = checkLineOrColumnArray(l + i, c, stack);
	if (err == 1)
	  return err;
	length--;
	i++;
      } else {
	length--;
	i++;
      }
    }
  }
  
  return 0;
}

/******************************************************************************
* fillEvenTrainWithTents()
*
* Arguments: l - number of the line
*            c - number of the column
*            loc - tells us if we want to go through a line or a column
*            length - length of the train
*            stack - struct with pointer to the last and first member of the stack
* Returns: err - 1 if the way we are taking is wrong
*              - 0 if not
* Side-Effects: (none)
*
* Description:
*   Although this function is called "fillEvenTrainWithTents", the operation
*doesnt consist in placing tents, instead, what it does is turn into grass
*the upper and lower adjacent openSpace of that train, for example:
*
*...AAOOA..                ...AA..A..   if we know the train in the middle
*..AOOOOA..       ->       ..AOOOOA..   has 2 tents
*..AOOA....                ..A..A....
*****************************************************************************/
int fillEvenTrainWithTents(int l, int c, char loc, int length, Stack *stack) {
  int i, j, err = 0;
  /* if we are analysing a line */
  if (loc == 'L') {
    for (i = 0; i < length; i++) {
      for (j = -1; j < 2; j++) {
	if (j == 0) continue;
	if (checkIfTentIsInsideMap(l + j, c + i) == -1) continue;
	if (currGameMap(l + j, c + i) == 'O') {
	  /* turns to grass said position and stores the change in stack */
	  fillPosWithGrassOrTent(l + j, c + i, '.');
	  pushStack(stack, l + j, c + i);
	  /* checks if there are any changes in the gameMap with this alteration */
	  err = checkLineOrColumnArray(l + j, c + i, stack);
	  if (err == 1)
	    return err;
	}
      }
    }
  /* if we are analysing a column */
  } else {
    for (i = 0; i < length; i++) {
      for (j = -1; j < 2; j++) {
	if (j == 0) continue;
	if (checkIfTentIsInsideMap(l + i, c + j) == -1) continue;
	if (currGameMap(l + i, c + j) == 'O') {
	  /* turns to grass said position and stores the change in stack */
	  fillPosWithGrassOrTent(l + i, c + j, '.');
	  pushStack(stack, l + i, c + j);
	  /* checks if there are any changes in the gameMap with this alteration */
	  err = checkLineOrColumnArray(l + i, c + j, stack);
	  if (err == 1)
	    return err;
	}
      }
    }
  }
  return 0;
}
