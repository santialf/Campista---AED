/******************************************************************************
* NAME
*   structInfo.c
*
* DESCRIPTION
*   Implementation of basic and necessary struct functions, operations such as
*counters, allocation and releasing of memory, access to the struct's values are in this file
*
* COMMENTS
*   Functions are placed in this file whenever their operations consist of just
*reading structure data or the allocation and releasing of memory for said structure
*
******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "structInfo.h"
/* GR27 */

typedef struct _game_info {
  int L;            /* number of lines */
  int C;            /* number of columns */
  int *lt;          /* array of integers counting tents per line */
  int *ct;          /* array of integers counting tents per column */
  char **gameMap;   /* table of chars with the game map */
} game_info;

game_info * currGame = (game_info *) NULL;

/******************************************************************************
* currGameL()
*
* Arguments: (none)
* Returns: currGame->L - number of lines
* Side-Effects: (none)
*
* Description: returns the number of lines in the current game
*****************************************************************************/
int currGameL() {
  return currGame->L;
}

/******************************************************************************
* currGameC()
*
* Arguments: (none)
* Returns: currGame->C - number of columns
* Side-Effects: (none)
*
* Description: returns the number of columns in the current game
*****************************************************************************/
int currGameC() {
  return currGame->C;
}

/******************************************************************************
* currGameLines()
*
* Arguments: (none)
* Returns: currGame->lt[n] - number of tents in specified line
* Side-Effects: (none)
*
* Description: returns number of tents in specified line
*****************************************************************************/
int currGameLine(int line) {
  return currGame->lt[line];
}

/******************************************************************************
* currGameColumns()
*
* Arguments: (none)
* Returns: currGame->ct[n] - number of tents in specified column
* Side-Effects: (none)
*
* Description: returns number of tents in specified column
*****************************************************************************/
int currGameColumn(int column) {
  return currGame->ct[column];
}

/******************************************************************************
* currGameColumns()
*
* Arguments: (none)
* Returns: currGame->gameMap - game map table
* Side-Effects: (none)
*
* Description: returns table with the game map
*****************************************************************************/
char currGameMap(int l, int c) {
  return currGame->gameMap[l][c];
}

/******************************************************************************
* fillPosWithGrassOrTent()
*
* Arguments: int - line number
*            int - column number
*            char - tells us if we want to place a tent or grass
* Returns: (none)
* Side-Effects: (none)
*
* Description: fills specified position with grass or a tent
*****************************************************************************/
void fillPosWithGrassOrTent(int l, int c, char got) {
    currGame->gameMap[l][c] = got;
  return;
}

/******************************************************************************
* adjacentTreeCounter()
*
* Arguments: coordL - line number
*            coordC - column number 
* Returns: treeSum - number of trees in orthogonal adjacent cells
* Side-Effects: (none)
*
* Description:
*   Counts number of trees in orthogonal adjacent cells
*****************************************************************************/
int adjacentTreeOrOpenCounter (int coordL, int coordC, char too) {
  int i, j, sum = 0;

  for (i = -1; i < 2; i++) {
    if ((coordL + i >= currGame->L) || (coordL + i < 0))
	continue; 
    for (j = -1; j < 2; j++) {
      if (((i != 0) && (j != 0)) || (coordC + j > currGame->C) || (coordC + j < 0))
	continue;  
      if (currGame->gameMap[coordL + i][coordC + j] == too)
	sum++;
    }
  }
  return sum;
}

/******************************************************************************
* loneliestTree()
*
* Arguments: (none)
* Returns: coord - pointer to the coordinates of the loneliest tree :(
* Side-Effects: allocates memory for the coordinates
*
* Description:
*   Checks every tree and returns the one that has the least amount of openSpaces
*around her
*****************************************************************************/
int *loneliestTree() {
  int i, j, min = 5, openSum = 0;
  int *coord = NULL;

  coord = (int*) calloc(2, sizeof(int));
  if (coord == NULL) exit(0);

  for(i = 0; i < currGameL(); i++)
    for(j = 0; j < currGameC(); j++) {
      if (currGameMap(i, j) == 'A') {
	openSum = (adjacentTreeOrOpenCounter (i, j, 'O'));
	if ((openSum < min) && (openSum != 0)) {
	  min = openSum;
	  coord[0] = i;
	  coord[1] = j;
	}
      }
    }

  return coord;
}

/******************************************************************************
* lineOrColumnTentCounter()
*
* Arguments: loc - char that tells us to read a column or a line
*            coordC - column number 
* Returns: sum - total number of tents in lines or columns
* Side-Effects: (none)
*
* Description:
*   Adds together the number of tents in every line or column
*****************************************************************************/
int lineOrColumnTentCounter (char loc) {
  int i, sum = 0;

  if (loc == 'L')
    for (i = 0; i < currGame->L; i++) 
      sum += currGame->lt[i];
  else
    for (i = 0; i < currGame->C; i++) 
      sum += currGame->ct[i];

  return sum;
}

/******************************************************************************
* sumTentsInLineOrColumn()
*
* Arguments: loc - char that tells us if we want to sum a line or a column
*            n - number of the line or column 
* Returns: sum - number of tents in specified line or column
* Side-Effects: (none)
*
* Description:
*   Function that counts the number of tents in a line or column 
*****************************************************************************/
int sumTentsInLineOrColumn(char loc, int n) {
  int i, sum = 0;

  if (loc == 'C') {
    for (i = 0; i < currGame->L; i++)
      if (currGame->gameMap[i][n] == 'T') 
	sum++;
  } else {
    for (i = 0; i < currGame->C; i++)
      if (currGame->gameMap[n][i] == 'T')
	sum++;
  }
  return sum;
}

/******************************************************************************
* sumOpenSpacesInLineOrColumn()
*
* Arguments: loc - char that tells us if we want to sum a line or a column
*            n - number of the line or column 
* Returns: sum - number of tents that a certain line or column can sustain
* Side-Effects: (none)
*
* Description:
*   Function that counts the number of tents that a certain line or column can
*sustain
*****************************************************************************/
int sumOpenSpacesInLineOrColumn(char loc, int n) {
  int i, sum = 0, sum2 = 0;
  
  if (loc == 'C') {
    for (i = 0; i < currGame->L; i++) {
      if ((currGame->gameMap[i][n] == 'O') && (sideOpenCounter('C', i, n) == 0)) 
	sum++;
      /* if there's a "train" of open spaces */
      if ((currGame->gameMap[i][n] == 'O') && (sideOpenCounter('C', i, n) != 0)) {
	/* lenght of the train */
	sum2 = trainCounter('C', i, n);
	i += sum2;
	/* calculates how many tents can fit in that train of spaces, for ex: 3 adjacent open spaces can only fit max 2 tents */
	sum = sum + (sum2/2 + (sum2 % 2)); 
	sum2 = 0;
      }
    }
  } else {
    for (i = 0; i < currGame->C; i++) {
      if ((currGame->gameMap[n][i] == 'O') && (sideOpenCounter('L', n, i) == 0)) 
	sum++;
      /* if there's a "train" of open spaces */
      if ((currGame->gameMap[n][i] == 'O') && (sideOpenCounter('L', n, i) != 0)) {
	/* lenght of the train */
	sum2 = trainCounter('L', n, i);
	i += sum2;
	/* calculates how many tents can fit in that train of spaces, for ex: 3 adjacent open spaces can only fit max 2 tents */
	sum = sum + (sum2/2 + (sum2 % 2)); 
	sum2 = 0;
      }
    }
  }
  return sum;
}

/******************************************************************************
* trainCounter()
*
* Arguments: loc - char that tells us if the train is in a line or a column
*            l - number of the line
*            c - number of the column 
* Returns: tCounter - length of the train (number of adjacent openSpaces) 
* Side-Effects: (none)
*
* Description:
*   Function that returns the size of a train of openSpaces 
*****************************************************************************/
int trainCounter(char loc, int l, int c) {
  int i = 0, tCounter = 0;

  if (loc == 'L') {
    while (currGame->gameMap[l][c + i] == 'O') { 
      tCounter++;
      i++;
      if ((i + c) >= currGame->C)
	break;
    }
  } else {
    while (currGame->gameMap[l + i][c] == 'O') {
      tCounter++;
      i++;
      if ((i + l) >= currGame->L)
	break;
    }
  }
  return tCounter;
}

/******************************************************************************
* treeCounter()
*
* Arguments: (none)
* Returns: treeSum - number of trees in the game
* Side-Effects: (none)
*
* Description:
*   Function that counts the number of trees in the current game 
*****************************************************************************/
int treeCounter() {
  int i, j, treeSum = 0;

  for (i = 0; i < currGame->L; i++)
    for (j = 0; j < currGame->C; j++) 
      if (currGame->gameMap[i][j] == 'A') 
	treeSum++;

  return treeSum;
}

/******************************************************************************
* sideOpenCounter()
*
* Arguments: loc - char that tells us if we want to return the number of openSpaces
*above and below or sides
*            coordL - number of the line
*            coordC - number of the column 
* Returns: sum - number of openSpaces in its (specified cell) sides or above and below 
* Side-Effects: (none)
*
* Description:
*   Function that counts and returns the number of openSpaces in the sides of the cell
*or above and below
*****************************************************************************/
int sideOpenCounter(char loc, int coordL, int coordC) {
  int i, sum = 0;

  if (loc == 'L') {
    for (i = -1; i < 2; i++) {
      if (i == 0)
      	continue;
      if (checkIfTentIsInsideMap(coordL, coordC + i) == -1)
	continue;
      if (currGame->gameMap[coordL][coordC + i] == 'O') 
	sum++;
    }
  } else {
    for (i = -1; i < 2; i++) {
      if (i == 0)
      	continue;
      if (checkIfTentIsInsideMap(coordL + i, coordC) == -1)
	continue;
      if (currGame->gameMap[coordL + i][coordC] == 'O')
	sum++;
    }
  }
  return sum;
}

/******************************************************************************
* win()
*
* Arguments: (none)
* Returns: openSum - number of openSpaces in the game
* Side-Effects: (none)
*
* Description:
*   Counts the number of openSpaces in the whole map
*****************************************************************************/
int win() {
  int i, j, openSum = 0;

  for (i = 0; i < currGame->L; i++)
    for (j = 0; j < currGame->C; j++)
      if (currGame->gameMap[i][j] == 'O') 
	openSum++;

  return openSum;
}

/******************************************************************************
* checkIfTentIsInsideMap()
*
* Arguments: coordL - line number
*            coordC - column number 
* Returns: int - 0 if tent is inside gameMap
*              - -1 if tent is outside gameMap
* Side-Effects: (none)
*
* Description:
*   Checks if selected tent is inside the gameMap limits
*****************************************************************************/
int checkIfTentIsInsideMap(int coordL, int coordC) { 

  if ((coordL >= currGame->L) || (coordC >= currGame->C) || (coordL < 0) || (coordC < 0))
    return (-1);
  return 0;
}

/******************************************************************************
* loadGameInfo()
*
* Arguments: fp - pointer to opened file
*            mtz - char sent by reference to check for gameMap allocation
* Returns: - 1 if something went wrong reading the file
*          - 0 if not
* Side-Effects: allocates memory for struct of type game_info
*
* Description:
*   Loads game info from file into struct
*****************************************************************************/
int loadGameInfo(FILE *fp, char *mtz) {

  int i, err = 0, lSum = 0, cSum = 0;
  /* allocation of memory the size of a struct game_info */
  currGame = (game_info*) malloc (sizeof(game_info));
  if (currGame == NULL)
    exit (0);

  /* reading of first line */
  if (fscanf(fp, "%d %d", &currGame->L, &currGame->C) != 2) {
    free(currGame);
    return 1;
  }
  /* allocation and reading of number of tents in each line/column */
  currGame->lt = (int *) malloc(sizeof(int) * currGame->L);
  if (currGame->lt == NULL)
    exit (0);
  currGame->ct = (int *) malloc(sizeof(int) * currGame->C);
  if (currGame->ct == NULL)
    exit (0);
  for (i = 0; i < currGame->L; i++) {
    if (fscanf(fp, "%d", &currGame->lt[i]) != 1)
      return 1;
  } 
  for (i = 0; i < currGame->C; i++) {
    if (fscanf(fp, "%d", &currGame->ct[i]) != 1)
      return 1;
  }
  /* allocation and reading of game map (sometimes there's memory allocation...other times not...) */
  lSum = lineOrColumnTentCounter('L');
  cSum = lineOrColumnTentCounter('C');
  
  if (lSum != cSum){
    *mtz = 'e';
    err = readGameMapFromFile (fp, 0);
  } else {
    err = readGameMapFromFile (fp, 1);
  }
  if (err == 1)
    return 1;
  
  return 0;
}

/******************************************************************************
* readGameMapFromFile()
*
* Arguments: fp - pointer to file
*            alloc - int that decides if there is map allocation or not 
* Returns: err - 1 if fscanf returns a result not expected
* Side-Effects: Allocation of memory for a game map
*               Allocation and release of memory for pointer "trash"
*
* Description:
*   Reads array with the game map information from file to struct, or simply 
*goes through this info without saving it
*****************************************************************************/
int readGameMapFromFile (FILE *fp, int alloc) {
  int err = 0, i;
  char *trash = NULL;

  if (alloc == 1) {
    currGame->gameMap = allocTable();
    for (i = 0; i < currGame->L; i++)
      if (fscanf(fp, "%s", currGame->gameMap[i]) != 1)
	err = 1;
  } else {
    trash = (char *) malloc(sizeof(char) * currGame->C + 1);
    for (i = 0; i < currGame->L; i++)
      if (fscanf(fp, "%s", trash) != 1)
	err = 1;
    free(trash);
  }
  return err;
}

/******************************************************************************
* freeTable()
*
* Arguments: auxMap - table with auxiliary game map
* Returns: (none)
* Side-Effects: auxiliary game table gets cleared
*
* Description:
*
*****************************************************************************/
void freeTable(char *auxMap[]) {
  int i;

  for (i = 0; i < currGame->L; i++)
    free(auxMap[i]);
  free(auxMap);

  return;
}

/******************************************************************************
* freeStruct()
*
* Arguments: mtz - checks if there's an allocated table to be released
* Returns: (none)
* Side-Effects: frees allocated struct for game info
*
* Description:
*
*****************************************************************************/
void freeStruct (char mtz) {
  
  /* frees game table using freeTable() */
  if (mtz == ' ')
    freeTable(currGame->gameMap);

  /* frees arrays of columns and lines */
  free(currGame->lt);
  free(currGame->ct);

  /* frees struct */
  free(currGame);
  
  return;
}

/******************************************************************************
* allocTable()
*
* Arguments: (none)
* Returns: table - pointer to allocated table LxC
* Side-Effects: allocates memory for a table with specified size
*
* Description:
*
*****************************************************************************/
char **allocTable() {
  char **table;
  int i;

  table = (char **) malloc(sizeof(char *) * currGame->L);
  if (table == NULL)
    exit (0);

  for (i = 0; i < currGame->L; i++) {
    table[i] = (char *) malloc(sizeof(char) * currGame->C + 1);
    if (table[i] == NULL)
      exit (0);
  }
  return table;
}

/******************************************************************************
* writeFile()
*
* Arguments: f - pointer to created file
*            checkVal - integer with the admissibility value of the problem
* Returns: (none)
* Side-Effects: (none)
*
* Description:
*   Writes down in file the solution to each game
*****************************************************************************/
void writeFile (FILE *f, int checkVal) {
  
  int i;

  fprintf (f, "%d %d %d\n", currGame->L, currGame->C, checkVal);
  if (checkVal == 1)
    for (i = 0; i < currGame->L; i++)
      fprintf (f, "%s\n", currGame->gameMap[i]);

  fprintf (f, "\n");
  return;
}
