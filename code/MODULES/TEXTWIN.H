#ifndef _TEXTWIN_H_
#define _TEXTWIN_H_
#include "global.h"
#include "scalegem.h"
#include "ctorsc.h"
extern void scale_side_set( void );
extern short first_scale_on_screen( void );
extern short side_part( void );
extern void print_scales_in_window( NODE *, int );
extern NODE *scale_on_text_line( NODE *, int );
extern int is_text_line( int );
extern NODE *restore_scale( CURRENT_STATUS *, NODE * );
extern NODE *delete_scale_window( CURRENT_STATUS *, NODE * ); 
extern NODE *select_tuning_window( CURRENT_STATUS *, NODE * ); 
extern NODE *delete_tuning_window( CURRENT_STATUS *, NODE * ); 


#endif