#ifndef _PATHS_H_
#define _PATHS_H_

#include "scalegem.h"
#include <osbind.h>
#include "ctorsc.h"
#include <stdio.h>
#include "fsel.h"
#include "error.h"

extern char *prog_name_path;
extern char scales_path[FMSIZE];
extern char defaults_path[FMSIZE];
extern char tunings_path[FMSIZE];
extern char printer_path[FMSIZE];
extern char paths_file[FMSIZE];

extern void load_paths(void);
extern void save_paths(void);
extern void paths_form(CURRENT_STATUS *);

#endif
