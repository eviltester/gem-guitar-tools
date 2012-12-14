#define DEMO

#include "algem.h"
#include "scalegem.h"
#include <stdlib.h>
#include <dos.h>
#include "global.h"
#include <string.h>
#include "scales.h"
#include "alscale.h"
#include "tuning.h"
#include "paths.h"
#include "printer.h"
#include "aes.h"
#include "time.h"

ALWINDOW scale_window;
ALWINDOW text_window;
ALWINDOW graphic_save;

OBJECT *menu_ptr;

char Header[]="!AJR!";

int global_frets=MAX_FRETS;

SCALE test_scale={"TEST SCALE",1,1,0,0,0,0,0,1,0,0,0,0};


				/* E B G  D A E */
int open_notes[]={7,2,10,5,0,7}; /* positions in note_numbers */
int octaves[]={3,3,2,2,2,1}; /* octave numbers */

int stave_position[STRINGS][MAX_FRETS];   /*for display in music notation*/


/* GLOBAL DISPLAY OPTIONS */
int odisplay_scale=TRUE;
int odisplay_root=TRUE;
int odisplay_relative=FALSE;




int frets_x_offset=0;
int frets_y_offset=0;
int frets_length=20;
int frets_height=14;
int start_fret=0;
short text_attributes[10];
int desk_x;
int osort=0;
int first_fret=5;
int fret_inc=5;
int right_mouse_button=FALSE;
int total_scales=0;
int alredraw=TRUE;
int finished=0;


int total_y_dist;
int big_y_offset=0;


short screenx,screeny,screenw,screenh;

char sizememory[20];
char aldate[20];
char altime[20];
char alnumofscales[16];
char alnumoftunings[16];
char versionnumber[10];
char circanumber[10];

void main()
	{
	CURRENT_STATUS cs;
	NODE *known_scales=NULL;
	NODE *kp;
	char info_buffer[120];
	char restore_text[]="SELECT SCALE";
	SCALE *default_scale;
	char *cp;
	GRECT p;
	OBJECT *dlog;	
	OBJECT *objptr;
	int result;
	short x,y,w,h;
	extern int _XMODE;
	long temp_long;
	size_t temp_sizet;
	time_t temp_time;
	struct tm *tmpoint;

#ifndef DEMO
	sprintf(versionnumber,"1.1");
#else
	sprintf(versionnumber,"DEMO");
#endif

	sprintf(circanumber,"04/02/94");


	
	switch(_XMODE)
	{
		case 0:
/*			appl_init();
			form_alert(1,"[1][I am a GEM Program][OK]");
			appl_exit();*/
			break;

		case 1:
			printf("\n\nThank you for running the Guitar\n");
			printf("reference program. Sadly you have \n");
			printf("used a .TOS or .TTP extension.\n");
			printf("\nThis program (for your convenience)\n");
			printf("uses the GEM portion of the ST system\n");
			printf("please rename this program with a .PRG\n");
			printf("or .APP extension and start again.\n\n Thank you :->");
			getch();
			exit(1);
			break;

		case 2:
/*			menu_register(appl_init(),"  My DA Entry");
			for (;;)
			{
				evnt_mesag(msg);
				if (msg[0]==AC_OPEN)
					form_alert(1,"[1][I am a Desk Accessory][OK]");
			}*/
			break;

		case 3:
			printf("\n\nThank you for running the Guitar\n");
			printf("reference program. Sadly you have \n");
			printf("put it in an AUTO Folder.\n");
			printf("\nThis program (for your convenience)\n");
			printf("uses the GEM portion of the ST system\n");
			printf("as a result, please run me from the\n");
			printf("desktop and start again.\n\n Thank you :->");
			getch();
			exit(1);
			break;
	}



	start_gem();
	al_set_desk_x(&desk_x);
	init_paths();

	graf_mouse(HOURGLASS,NULL);

	Crsrc_gaddr(R_TREE,WaitForm,&dlog);
	set_tedinfo(dlog,WaitMessage,"LOADING PATHS FILE");

	form_center(dlog,&x,&y,&w,&h);
	form_dial(FMD_START,0,0,0,0,x,y,w,h);
	objc_draw(dlog,0,10,x,y,w,h);

#ifndef DEMO
	load_paths();
#endif

	text_window.title=restore_text;

	if(scales_path[0]=='\0')
	{
	set_tedinfo(dlog,WaitMessage,"SETTING DEFAULT SCALE");
	objc_draw(dlog,0,10,x,y,w,h);


	default_scale=(SCALE *)malloc(sizeof(SCALE));
	cp=(char *)malloc(80);
	strcpy(cp,"DEFAULT SCALE - pentatonic blues scale");
	default_scale->name=cp;
	default_scale->notes[0]=1;
	default_scale->notes[1]=0;
	default_scale->notes[2]=1;
	default_scale->notes[3]=0;
	default_scale->notes[4]=0;
	default_scale->notes[5]=1;
	default_scale->notes[6]=0;
	default_scale->notes[7]=1;
	default_scale->notes[8]=0;
	default_scale->notes[9]=0;
	default_scale->notes[10]=1;
	default_scale->notes[11]=0;

	known_scales=(NODE *)malloc(sizeof(NODE));
	known_scales->left=NULL;
	known_scales->right=NULL;
	known_scales->data=(void *)default_scale;

	default_scale=(SCALE *)malloc(sizeof(SCALE));
	cp=(char *)malloc(80);
	strcpy(cp,"DEFAULT SCALE - Double Harmonic Oriental (mode 5)");
	default_scale->name=cp;
	default_scale->notes[0]=1;
	default_scale->notes[1]=1;
	default_scale->notes[2]=0;
	default_scale->notes[3]=0;
	default_scale->notes[4]=1;
	default_scale->notes[5]=1;
	default_scale->notes[6]=0;
	default_scale->notes[7]=1;
	default_scale->notes[8]=1;
	default_scale->notes[9]=0;
	default_scale->notes[10]=0;
	default_scale->notes[11]=1;

	kp=known_scales;

	kp->right=(NODE *)malloc(sizeof(NODE));
	((NODE*)(kp->right))->left=kp;
	kp=kp->right;
	kp->right=NULL;
	kp->data=(void *)default_scale;

	default_scale=(SCALE *)malloc(sizeof(SCALE));
	cp=(char *)malloc(80);
	strcpy(cp,"DEFAULT SCALE - Harmonic Minor Spanish Gypsy Scale (mode 5)");
	default_scale->name=cp;
	default_scale->notes[0]=1;
	default_scale->notes[1]=0;
	default_scale->notes[2]=1;
	default_scale->notes[3]=1;
	default_scale->notes[4]=0;
	default_scale->notes[5]=1;
	default_scale->notes[6]=0;
	default_scale->notes[7]=1;
	default_scale->notes[8]=1;
	default_scale->notes[9]=0;
	default_scale->notes[10]=0;
	default_scale->notes[11]=1;

	kp->right=(NODE *)malloc(sizeof(NODE));
	((NODE*)(kp->right))->left=kp;
	kp=kp->right;
	kp->right=NULL;
	kp->data=(void *)default_scale;

	default_scale=(SCALE *)malloc(sizeof(SCALE));
	cp=(char *)malloc(80);
	strcpy(cp,"DEFAULT SCALE - Pentatonic Scale - Indian ");
	default_scale->name=cp;
	default_scale->notes[0]=1;
	default_scale->notes[1]=0;
	default_scale->notes[2]=1;
	default_scale->notes[3]=0;
	default_scale->notes[4]=0;
	default_scale->notes[5]=1;
	default_scale->notes[6]=0;
	default_scale->notes[7]=1;
	default_scale->notes[8]=0;
	default_scale->notes[9]=0;
	default_scale->notes[10]=0;
	default_scale->notes[11]=1;

	kp->right=(NODE *)malloc(sizeof(NODE));
	((NODE*)(kp->right))->left=kp;
	kp=kp->right;
	kp->right=NULL;
	kp->data=(void *)default_scale;

	default_scale=(SCALE *)malloc(sizeof(SCALE));
	cp=(char *)malloc(80);
	strcpy(cp,"DEFAULT SCALE - Eight tone Spanish Scale");
	default_scale->name=cp;
	default_scale->notes[0]=1;
	default_scale->notes[1]=1;
	default_scale->notes[2]=0;
	default_scale->notes[3]=1;
	default_scale->notes[4]=0;
	default_scale->notes[5]=1;
	default_scale->notes[6]=0;
	default_scale->notes[7]=1;
	default_scale->notes[8]=1;
	default_scale->notes[9]=0;
	default_scale->notes[10]=1;
	default_scale->notes[11]=1;

	kp->right=(NODE *)malloc(sizeof(NODE));
	((NODE*)(kp->right))->left=kp;
	kp=kp->right;
	kp->right=NULL;
	kp->data=(void *)default_scale;
	}
	else
	{
		set_tedinfo(dlog,WaitMessage,"LOADING SCALES FILE");
		known_scales=read_scales(known_scales,scales_path);
	}

	if(tunings_path[0]=='\0')
	{
	set_tedinfo(dlog,WaitMessage,"SETTING DEFAULT TUNING");
	objc_draw(dlog,0,10,x,y,w,h);


	cp=(char *)malloc(80);
	strcpy(cp,current_tuning->name);
	current_tuning->name=cp;
	}
	else
	{
	/* load the tunings */
	set_tedinfo(dlog,WaitMessage,"LOADING TUNING FILE");
	objc_draw(dlog,0,10,x,y,w,h);






	known_tunings=read_tunings(known_tunings,tunings_path);
	current_tuning=(tuning *)known_tunings->data;

	}

/*	convert_old_to_new(INFILENAME,OUTFILENAME);*/ 
/*	known_scales=read_scales(known_scales,OUTFILENAME);
*/

	cs.root=&note8;
	cs.key=&note8;

	if(defaults_path[0]=='\0')
	{
		set_tedinfo(dlog,WaitMessage,"USING INTERNAL DEFAULTS");
		objc_draw(dlog,0,10,x,y,w,h);
	}
	else
	{
		set_tedinfo(dlog,WaitMessage,"LOADING DEFAULTS");
		objc_draw(dlog,0,10,x,y,w,h);

		load_defaults(defaults_path);
		/* load the default values from the file */
	}

	if(printer_path[0]=='\0')
	{
	set_tedinfo(dlog,WaitMessage,"SETTING DEFAULT PRINTER");
	objc_draw(dlog,0,10,x,y,w,h);


	current_driver=create_printer_driver();
	}
	else
	{
	set_tedinfo(dlog,WaitMessage,"LOADING PRINTER FILE");
	objc_draw(dlog,0,10,x,y,w,h);


	current_driver=load_printer_driver(printer_path);
	}

	form_dial(FMD_FINISH,0,0,0,0,x,y,w,h);


	cs.current_scale=(SCALE *)known_scales->data;
	cs.changed_scale=FALSE;
	copy_scales(cs.current_scale->notes,cs.backup_notes);
	kp=known_scales;
	set_frets(&cs);
	fill_notes_from_store(cs.backup_notes,cs.current_notes,cs.key);
	set_display_frets(&cs);


	wind_get(DESK,WF_CXYWH,&screenx,&screeny,&screenw,&screenh);

	init_alwindow(&scale_window,
				  "SCALE LAYOUT",
				  	NAME|CLOSE|FULL|/*MOVE|*/INFO/*|SIZE*/|UPARROW|
				  	DNARROW|VSLIDE|LFARROW|RTARROW|HSLIDE,
				  60,60,400,250);

	windopen(&scale_window,1);
	sprintf(info_buffer," %s",cs.current_scale->name);
	wind_info(scale_window.handle,info_buffer);
	wind_title(scale_window.handle,"SCALE LAYOUT");

	wind_get(DESK,WF_CXYWH,&p.g_x,&p.g_y,&p.g_w,&p.g_h);
	init_alwindow(&graphic_save," NEVER SEEN",0,
						p.g_x,p.g_y,p.g_w,p.g_h);

	set_scales_ticks(cs.current_notes);
	set_key_tick(cs.key->number);

/*	menu_icheck(menu_ptr,MRootoption,odisplay_root);
*/

	graf_mouse(ARROW,NULL);

/*	temp_long=sizmem();
	sprintf(sizememory,"%ld",temp_long);
	sprintf(Gtemp_string,"[1][memory %s][OK]",sizememory);
	form_alert(1,Gtemp_string);
*/

#ifdef DEMO
			time(&temp_time);
			tmpoint=gmtime(&temp_time);

			temp_sizet= strftime(aldate,20,"%x",tmpoint);
			temp_sizet= strftime(altime,20,"%X",tmpoint);

			Crsrc_gaddr(R_TREE,FAbout,&objptr);
			set_tedinfo(objptr,about_time,altime);
			set_tedinfo(objptr,about_date,aldate);
			set_tedinfo(objptr,about_circa,circanumber);
			set_tedinfo(objptr,about_version,versionnumber);

			sprintf(alnumofscales,"%d",count_items(known_scales));
			set_tedinfo(objptr,about_scalesmem,alnumofscales);

			sprintf(alnumoftunings,"%d",count_items(known_tunings));
			set_tedinfo(objptr,about_tuningsmem,alnumoftunings);

			result=handle_dialog(objptr,0,0);

			Crsrc_gaddr(R_TREE,NAG_SCREEN,&objptr);
			set_tedinfo(objptr,Please_Wait_Butt," OK ");
			result=handle_dialog(objptr,0,0);
			set_tedinfo(objptr,Please_Wait_Butt,"Please Wait");

#endif



	handle_events(&cs,known_scales);

	windclose(&scale_window);

	stop_gem();
	}
