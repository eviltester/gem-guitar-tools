#ifndef _ALERROR_H
#define _ALERROR_H

#include "algem.h"
#include <stdio.h>
#include "global.h"

extern void alerror(int,char *);

#define error(A)			alerror(0,A);
#define fatal_error(A)	alerror(1,A); 

#endif