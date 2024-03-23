/******************************************************************************
* NAME
*   stack.c
*
* DESCRIPTION
*   Implementation of the basic operations of a functional stack (LIFO). 
* COMMENTS
*   When we use the operation pushStack() we only store the coordinates
*of the cell and not what it was before the change, because every change
*is from 'O' to 'T' or '.', so when using popStack() we always know the cell
*was 'O' before the change 
*
******************************************************************************/
#include <stdlib.h>
#include <stdio.h>

#include "stack.h"
#include "structInfo.h"

/******************************************************************************
* createStack()
*
* Arguments: (none)
* Returns: stack - pointer to the newly created stack
* Side-Effects: (none)
*
* Description:
*   creates and initializes a stack struct
*
*****************************************************************************/
Stack *createStack(int n)
{
  Stack *stack = (Stack*)NULL;

  /* allocation of memory */
  stack = (Stack*) malloc (sizeof(Stack));
  if (stack == NULL)
    exit (0);
  /* initialization */
  stack->first = NULL;
  stack->last = NULL;
   
  return(stack);
}


/******************************************************************************
* popStack()
*
* Arguments: stack - struct with pointer to the last and first member of the stack
* Returns: (none)
* Side-Effects: (none)
*
* Description:
*   Picks the last element of the stack and turns the cell into an openSpace,
*this is the operation we use to "rewind" the map
*
*****************************************************************************/
void popStack(Stack *stack)
{
  Element *elem = (Element*) NULL;

  if (stack->first == NULL)
    return;

  elem = stack->last;
  /* incase there's only 1 element in the list */
  if (elem == stack->first) {
    /* changes cell back to what it was */
    fillPosWithGrassOrTent(elem->coord[0], elem->coord[1], 'O');
    free(elem);
    stack->first = NULL;
    stack->last = NULL;
    return;
  }
  /* the last member in the stack becomes the next one */
  stack->last = elem->next;
  /* changes cell back to what it was */
  fillPosWithGrassOrTent(elem->coord[0], elem->coord[1], 'O');
  free(elem);
  
  return;
}

/******************************************************************************
* pushStack()
*
* Arguments: stack - struct with pointers to the last and first member of the stack
*            l - number of the line the cell is in
*            c - number of the column the cell is in
* Returns: (none)
* Side-Effects: allocation of memory to a new element
*
* Description:
*   Creates a new element and stores inside this element the coordinates to the changed cell,
*also inserts this new element at the end of the stack
*
*****************************************************************************/
void pushStack(Stack *stack, int l, int c)
{
  Element *elem = (Element*) NULL;
 
  /* mem allocation for new Element struct */
  elem = (Element*) malloc (sizeof(Element));
  if (elem == NULL)
    exit (0);
  
  /* init of Element struct */
  elem->coord[0] = l;
  elem->coord[1] = c;
  elem->next = NULL;

  /* inserts as the last element in the stack */
  if (stack->first == NULL) {
    stack->first = elem;
    stack->last = elem;
  } else {
    elem->next = stack->last;
    stack->last = elem;
  }
  
  return;
}

/******************************************************************************
* freeStack()
*
* Arguments: stack - struct with pointers to the last and first member of the stack
* Returns: (none)
* Side-Effects: frees the memory allocated by the stack
*
* Description:
*
*****************************************************************************/
void freeStack(Stack *stack) {
  Element *elem = NULL;
  
  while(stack->last != NULL) {
    elem = stack->last;;
    stack->last = elem->next;
    free(elem);
  }
  free(stack);
  return;
}


