#ifndef DEMO
#define DEMO
#endif

#include "algem.h"
#include "i:\scales.h"
#include "ctorsc.h"


void nag_about(int time)
{

		OBJECT *dlog;	
		short x,y,w,h;

#ifdef DEMO

	Crsrc_gaddr(R_TREE,NAG_SCREEN,&dlog);
	set_tedinfo(dlog,Please_Wait_Butt,"Please Wait");

	form_center(dlog,&x,&y,&w,&h);
	form_dial(FMD_START,0,0,0,0,x,y,w,h);
	objc_draw(dlog,0,10,x,y,w,h);

	/*pause for supplied time_value*/

	set_tedinfo(dlog,Please_Wait_Butt," OK ");
	objc_draw(dlog,0,10,x,y,w,h);

	/* handle form */
	handle_dialog(dlog,0,0);

	set_tedinfo(dlog,Please_Wait_Butt,"Please Wait");
	form_dial(FMD_FINISH,0,0,0,0,x,y,w,h);

#endif
}
