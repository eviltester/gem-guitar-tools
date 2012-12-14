#ifndef _DRAWSCL_H_
#define _DRAWSCL_H_
#include "global.h"
#include "scalegem.h"
#include "ctorsc.h"
 
extern void scale_offsets( ALWINDOW *, int *, int * );
extern void refix_start_fret( void );
extern short pos_from_start_fret( void );
extern int frets_on( void );
extern void set_start_fret( short );
extern void set_hor( void );
extern short pos_from_y_dist( void );
extern void set_big_y_offset( short );
extern void set_ver( int );
extern void small_scale_draw( ALWINDOW *, CURRENT_STATUS * );
extern void specific_note_draw( ALWINDOW *, CURRENT_STATUS *, int );


#endif