#ifndef  _ALFSEL_H
#define  _ALFSEL_H
#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <aes.h>

extern char last_filenames[10][FNSIZE];	/* space for filenames with 10 extensions */
extern char *stored_paths[26];			/* the last paths on each drive */
extern int last_drive;					/* the number of the last drive */

extern void extract_extension(char *, char *);
extern void init_paths(void);
extern int select(char *, char *, char *);


#endif