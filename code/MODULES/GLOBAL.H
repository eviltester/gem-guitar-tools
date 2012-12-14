#ifndef _GLOBAL_H
#define _GLOBAL_H

#define FALSE 	0
#define TRUE	1

#include <stdio.h>
#include "algem.h"
#include "note.h"
#include "fsel.h"
#include "error.h"

#define SCALE_WINDOW 	0
 
#define NO_NOTE 	0
#define NORMAL_NOTE	1
#define FILLED_NOTE	2
#define RELATIVE_NOTE 10

#define STRINGS 6
#define MAX_FRETS	25
#define OUTFILENAME "i:\escales.st"
#define INFILENAME "i:\escales"

#define TEMPORARY_STRING_LENGTH	300

extern	char Gtemp_string[];


typedef struct{
				char *name;
				int notes[12];
		}SCALE;

typedef struct{
		void *left;
		void *data;
		void *right;
		}NODE;

typedef struct{
/*the fretboard to display*/	int display_fretboard[STRINGS][MAX_FRETS];
/*current scale*/		SCALE *current_scale;
/*copy of the scale*/	int backup_notes[12];
/*scales notes in current key*/	int current_notes[12];
/*fretboard notes*/		NOTE *current_fretboard[STRINGS][MAX_FRETS];
/*root of fretboard*/		NOTE *root;
/*current key*/			NOTE *key;
/*has scale changed*/	int changed_scale;
		}CURRENT_STATUS;


extern char Header[];

extern int global_frets;

extern SCALE test_scale;


				/* E B G  D A E */
extern int open_notes[]; /* positions in note_numbers */
extern int octaves[];    /* octave numbers for the open notes */

extern int stave_position[][];   /*for display in music notation*/


/* GLOBAL DISPLAY OPTIONS */
extern int odisplay_scale;
extern int odisplay_root;
extern int odisplay_relative;



extern int numbers_flag;
extern int key_name_flag;
extern int scale_name_flag;
extern int midi_sound_flag;
extern int channel;
extern int velocity;
extern int note_length;
extern int midi_note_off_flag; 
extern int frets_x_offset;
extern int frets_y_offset;
extern int frets_length;
extern int frets_height;
extern int start_fret;
extern short text_attributes[];
extern int desk_x;
extern int osort;
extern int first_fret;
extern int fret_inc;
extern int right_mouse_button;
extern int total_scales;
extern int alredraw;
extern int finished;
extern short chip_volume;


extern int total_y_dist;
extern int big_y_offset;

extern ALWINDOW scale_window;
extern ALWINDOW text_window;
extern ALWINDOW graphic_save;
extern OBJECT *menu_ptr;

extern 	int circle_flag; 
extern 	int tp_columns;
extern 	int tp_start_fret;
extern 	int tp_tab;
extern 	int tp_no_frets;
	/* all 40 */
extern 	char tp_header1[];
extern 	char tp_header2[];
extern 	char tp_trailer1[];
extern 	char tp_trailer2[];
	/* all 14 */
extern 	char tp_root_string[];
extern 	char tp_root_n_rel[];
extern 	char tp_norm_string[];
extern 	char tp_norm_n_rel[];
extern 	char tp_relative[];


/* about strings */
extern	char sizememory[];
extern	char aldate[];
extern	char altime[];
extern	char alnumofscales[];
extern	char alnumoftunings[];
extern	char versionnumber[];
extern	char circanumber[];

extern void save_defaults( void );
extern void load_defaults( char *);
extern int text_notes;

#endif