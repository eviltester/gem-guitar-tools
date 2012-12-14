#ifndef _TUNING_H_
#define _TUNING_H_

#include "global.h"
#include "textwin.h"

typedef struct{
					char *name;
					int	octaves[6];
					int	notes[6];
					}tuning;


extern NODE *known_tunings;
extern tuning *current_tuning;

#endif