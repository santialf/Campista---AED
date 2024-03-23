#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "structInfo.h"
#include "fileOp.h"
#include "chainEffect.h"
#include "stack.h"
#include "recursiveAndABC.h"
/* GR27 */

/******************************************************************************
* main()
*
* Arguments: argc - counter of number of arguments in call
*            argv - pointer to array of strings holding the arguments
* Returns: exit status
* Side-Effects: (none)
*
* Description:
*
*****************************************************************************/
int main(int argc, char *argv[]) {
  FILE *fp, *fp2;
  Stack *stack = NULL;
  int i, j, checkVal = 1, err = 0, trash = 0;
  char mtz = ' ', epoch = 'A';

  /* must always have one argument besides the exec name */
  if (argc != 2) {
    exit(0);
  }
  /* file name must end with .camp */
  i = nameCheck(argv[1]);
  if (i != 0) {
    exit(0);
  } 
  /* open file and read array size */
  fp = openFile(argv[1], "r");
  /* creates new file with .tents extension */
  fp2 = createFile(argv[1]);

  /* goes through the file */
  while (1) {
    trash = loadGameInfo(fp, &mtz);
    /* if end of file is reached */
    if (trash == 1)
      break;

    if (mtz != 'e')
      /* checks if the game is admissable */
      checkVal = problemA(&epoch);
    else
      checkVal = -1;
    /* if the game is admissable */
    if (checkVal == 1) {
      /* every cell that can contain a tent will become 'O' */
      fillEmptySpacesWithGrass();
      /* initializes the stack */
      stack = createStack();
      /* checks if any tents or grass can be placed right away */
      if (currGameL() == currGameC()) {
	for (i = 0; i < currGameL(); i++)
	  for (j = 0; j < currGameC(); j++)
	    if (i == j) {
	      err = checkLineOrColumnArray(i, j, stack);
	      if (err == 1)
	      checkVal = -1;
	    }
      /* same thing but if Lines != Columns */
      } else {
	for (i = 0; i < currGameL(); i++) {
	  err = checkLineOrColumnArray(i, 0, stack);
	  if (err == 1) {
	    checkVal = -1;
	    break;
	  }
	}
	for (i = 0; i < currGameC(); i++) {
	  err = checkLineOrColumnArray(0, i, stack);
	  if (err == 1) {
	    checkVal = -1;
	    break;
	  }
	}
      }
      /* check if placed tents contain any logical errors */
      if (checkVal == 1) {
	err = problemC();
	if (err == 1)
	  checkVal = -1;	  
      }

      /* notice that if err == 1, we know for a fact the game doesnt have a solution */
      /* because all we did was use logic to fill the openSpaces ('O') */

      freeStack(stack);
      stack = createStack();
      
      if (checkVal == 1) {
	/* does DFS to find the solution, if it returns 1, there is no solution */
	err = randomRecursiveSolve(stack, epoch);
	if (err == 1)
	  checkVal = -1;
      }
      freeStack(stack);
      }
    /* writes down game info and frees mem. allocated by struct */
    writeFile(fp2, checkVal);
    freeStruct(mtz);
    mtz = ' ';
  }
  fclose(fp2);
  fclose(fp);
  return(0);
}


