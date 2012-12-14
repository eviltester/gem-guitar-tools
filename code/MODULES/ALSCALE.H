#ifndef _ALSCALE_H
#define _ALSCALE_H
#include "global.h"
#include "error.h"
#include "printer.h"


/* FILE STUFF */

#define write_header(_X) fwrite(Header,1,6,_X);

/* GLOBALS */





/*functions */

extern void *allocation( int );
extern void set_frets( CURRENT_STATUS * );
extern void print_frets( CURRENT_STATUS *, int );
extern void fill_current_notes( CURRENT_STATUS *);
extern void set_relative( CURRENT_STATUS * );
extern NOTE *get_relative( CURRENT_STATUS * , NOTE * );
extern void set_display_frets( CURRENT_STATUS * );
extern void print_display_frets( CURRENT_STATUS * );
extern void fill_stave_array( int sfretboard[][MAX_FRETS],
										NOTE *fretboard[][MAX_FRETS] );
extern void seek_and_replace( char *, char, char );
extern void ags( char *, FILE *);
extern void convert_old_to_new( char *, char *);
extern NODE *read_scales( NODE *, char * );
extern NODE *read_tunings( NODE *, char * );
extern void save_scales( NODE *, char * );
extern void save_tunings( NODE *, char * );
extern int compare_scale_names( char *, char * );
extern void exchange_nodes( NODE *, NODE *);
extern void sort_names( NODE * );
extern void list_notes( SCALE * );
extern void fill_notes_from_scale( SCALE *, int current_notes[],NOTE * );
extern void fill_notes_from_store(	int backup_notes[], 
												int current_notes[],NOTE * );
extern void current_notes_to_store( int backup_notes[], 
												int current_notes[], NOTE * );
extern void copy_scales( int from[], int to[] );




#endif


















