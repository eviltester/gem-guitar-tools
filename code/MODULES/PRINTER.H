#ifndef _PRINTER_H_
#define _PRINTER_H_

#include "global.h"
#include "error.h"
#include "tuning.h"
#include "fsel.h"

typedef struct {
						void *left;
						int  data;
						void *right;
					}list;

typedef struct {
						char *name;
						list *line_feed;
						list *new_page;

						list *fret;
						list *fret1;
						list *fret6;
						list *string;
						list *note;
						list *root;
						list *relative;
						list *normNrel;
						list *rootNrel;
						list *no_note;
						list *space;

						char *open_notes;
						char *fret_descr;
					}p_driver;

typedef struct{
					char *filename;
					FILE *output_file;
					}pto_disk_data;
	
extern p_driver *current_driver;

extern p_driver *create_printer_driver(void);

extern pto_disk_data *gprint_to_disk;

extern char *read_a_string(FILE *);
extern int read_a_number(FILE *);
extern list *intify_string(char *);
extern char skip_white_spaces( FILE *);
extern void skip_to(char , FILE *);
extern p_driver *load_printer_driver(char *);
extern void scrub_list( list *);
extern void destroy_driver(p_driver *);
extern char *get_token(char, FILE *);
extern void albf(char *, short);
extern void print_as_text( CURRENT_STATUS *);
extern int separator(char );
extern void output_header(CURRENT_STATUS *, short );
extern void output_trailer(CURRENT_STATUS *, short );
extern void output_text(CURRENT_STATUS *, char *, short );
extern void output_key_name(CURRENT_STATUS *,short );
extern void output_notes_string(CURRENT_STATUS *, short );
extern void output_symbols_string(CURRENT_STATUS *, short );
extern int is_string(char *, char *, int );
extern void load_new_driver( void );
#endif
