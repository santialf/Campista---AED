/******************************************************************************
* NAME
*   fileOp.c
*
* DESCRIPTION
*   Implementation of operations that wiggle with files. 
* COMMENTS
*   Every operation that consists of creating, validating or openning files,
*are located in this .c
*
******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "fileOp.h"
/* GR27 */

/******************************************************************************
* openFile()
*
* Arguments: nome - pointer to string holding name of file to open
*            mode - pointer to string with 'r'/'w' etc mode for file open
* Returns: pointer to opened file
* Side-Effects: exits if given file cannot be opened with given mode
*
* Description:
*   Open named file in requested mode, exit if open fails 
*****************************************************************************/
FILE *openFile (char *nome, char *mode)
{
    FILE *fp;
    fp = fopen(nome, mode);
    if (fp == NULL) {
      exit (0);
    }
    return (fp);
}

/******************************************************************************
* createFile()
*
* Arguments: file - pointer to string holding name of file to save
* Returns: f - pointer to new file
* Side-Effects: allocates memory for the name of new file
*
* Description:
*   Creates a file with the same name as the original but instead of ending with
*"camp", it ends with "tents"
*****************************************************************************/
FILE * createFile (char *file)
{
  FILE *f;
  char *name = NULL;
  /* puts '\0' at the '.' */
  file[strlen(file) - 4] = '\0';
  name = (char *) malloc(strlen(file) + strlen("tents") + 1);
  if (name == NULL) {
    exit (0);
  }
  /* adds the name "tents" to the filename */
  strcpy(name, file);
  strcat(name, "tents");

  f = openFile(name, "w");
  free(name);
  return f;
}

/******************************************************************************
* nameCheck()
*
* Arguments: fname - file name (argument 1)
* Returns: integer 1 or 0
* Side-Effects: (none)
*
* Description:
*   Checks if name of file ends with .camp          
*****************************************************************************/
int nameCheck (char *fname) {    
  
  const char s[2] = ".";
  char *str = NULL, *token = NULL, *aux = NULL;
 
  if (strlen(fname) < 4) {
    return (1);
  } 
  /* allocates memory for aux pointer */
  aux = (char *) malloc(sizeof(char) * strlen(fname) + 1);
  if (aux == NULL)
    exit (0);
  strcpy(aux, fname);

  /* divides aux in tokens */
  token = strtok(aux, s);  
  while ( token != NULL ) {
    str = token;
    token = strtok(NULL, s);
  } 
  /* compares last token with camp */
  if (strcmp(str, "camp") != 0) {
    free(aux);
    return (1);
  }
  free(aux);
  return (0);
}
