#ifndef _ALGEM_H
#define _ALGEM_H

#include <aes.h>		/* get the AES prototypes and definitions */
#include <vdi.h>		/* get the VDI prototypes and definitions */
#include <osbind.h>
#include <stdio.h>
#include "fsel.h"


typedef struct{
				char *title;
				int style;
				int fulled;
				int wx;
				int wy;
				int ww;
				int wh;
				int handle;
				int wxbf;
				int wybf;
				int wwbf;
				int whbf;
			}ALWINDOW;

/* v_opnvwk input array */
extern short work_in[]/*[11]={1,1,1,1,1,1,1,1,1,1,2}*/;

/* v_opnvwk output array */
extern short work_out[57];
extern int message[8];
extern int gem_on;

/*working storage */
extern short handle,char_height,char_width,cell_height,cell_width;

extern void init_alwindow( ALWINDOW *, char *,int ,int ,int ,int , int );
extern void al_set_desk_x( int * );
extern int windopen( ALWINDOW *, int );
extern void windclose( ALWINDOW * );
extern void clearw( ALWINDOW * );
extern void wclip( ALWINDOW * );
extern void fullw( ALWINDOW * );
extern short window_height( ALWINDOW * );
extern short window_vert_height( ALWINDOW * );
extern short window_work_y( ALWINDOW * );
extern short window_vert_pos( ALWINDOW * );
extern void set_tedinfo( OBJECT *, int, char * );
extern void get_tedinfo( OBJECT *, int, char * );
extern int handle_dialog( OBJECT *, int, int );
extern void draw_dialog( OBJECT * );
extern void set_button( OBJECT *, int, int );
extern void set_normal_button( OBJECT *, int, int );
extern void unset_normal_button( OBJECT *, int, int );
extern int get_normal_button( OBJECT *, int, int );
extern int get_button( OBJECT *, int );

#endif
