#ifndef _SCALEGEM_H
#define _SCALEGEM_H

#include "algem.h"
#include "global.h"
#include "alscale.h"
#include "error.h"
#include "textwin.h"
#include "drawscl.h"
#include "scales.h"

extern int start_gem( void );
extern int stop_gem( void );
extern void alcircle( int, int, int, int );

extern void display_sizes_form( CURRENT_STATUS * );
extern void fret_numbers_form( CURRENT_STATUS * );
extern void display_frets_form( CURRENT_STATUS * );

extern void display_text_print_form( CURRENT_STATUS * );
extern void display_options_form( CURRENT_STATUS * );
extern void rename_form( CURRENT_STATUS *, NODE * );
extern char *get_name_of_scale_form( CURRENT_STATUS *, NODE * );
extern void note_on_fretboard_form( CURRENT_STATUS *, NODE *, int, int );
extern void mouse_click_on_fret( ALWINDOW *, CURRENT_STATUS *, 
											NODE *, int, int );
extern void note_on_fretboard_do( CURRENT_STATUS *, NODE *, int, int );
extern void mouse_double_click_on_fret( ALWINDOW *, CURRENT_STATUS *, 
													NODE *, int, int );
extern int store_scale( CURRENT_STATUS *, NODE * );
extern int store_scale_with_this_name( CURRENT_STATUS *, NODE * , char *);
extern void store_scale_as( CURRENT_STATUS *, NODE * );
extern void rename_form_small( CURRENT_STATUS *, NODE *);
extern NODE *store_tuning_form( CURRENT_STATUS */*, NODE **/, int *, int */*, int **/);
extern void set_key_from_menu( CURRENT_STATUS *, int );
extern int resolve_clash_form_small(NODE *, NODE *);
extern int resolve_clash_form_normal(NODE *, NODE *);
extern void set_key_tick( int );
extern void set_note_from_menu( CURRENT_STATUS *, int );
extern void set_scales_ticks( int note_array[] );

extern int scale_changed( CURRENT_STATUS * );
extern int is_quit( void );
extern NODE *handle_menu( int, int, CURRENT_STATUS *, NODE * );
extern void handle_events( CURRENT_STATUS *, NODE * );
extern void help_key( void );
#endif





































