#ifndef _STACK_H
#define _STACK_H

typedef struct _element {
  int coord[2];            /* coordinates to position */
  struct _element *next;   /* pointer to next element of the stack */
} Element;


typedef struct _stack {
  Element *first;          /* pointer to first element of the stack */
  Element *last;           /* pointer to last element of the stack */
} Stack;


Stack *createStack();             /* creates empty list */
void popStack(Stack*);            /* get LAST element of the stack */
void pushStack(Stack*, int, int); /* insertion at the END of the stack */
void freeStack(Stack *);          /* frees stack */

#endif
