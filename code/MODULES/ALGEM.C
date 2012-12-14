#include "algem.h"
/* This file contains the functions I wrote to access GEM easily!!
	many are still used and yet I suspect that many are obsolete
	after using GEM for a while */


/* v_opnvwk input array */
short work_in[11]={1,1,1,1,1,1,1,1,1,1,2};

/* v_opnvwk output array */
short work_out[57];
int message[8];

/* flag used to determine if gem was on or not!!*/
int gem_on=0;

/*working storage */
short handle,char_height,char_width,cell_height,cell_width;


/* creates an ALWINDOW structure - why?? I don't remember, perhaps
	GEM Windows were too hard to use initially, is it still used??
	possibly.

	Don't ask what the temptitle did, because it looks like ******
	code to me - create a temp string, copy in the title, and then
	don't do anything!!! so I commented it out 4/10/93*/

void init_alwindow(ALWINDOW *alw,char *title,int style,int wx,int wy,int ww, int wh)
	{
/*	char *temptitle=(char *)malloc(strlen(title)+1);

	temptitle=(char *)strcpy(temptitle,title);
*/
	alw->style=style;
	alw->wxbf=alw->wx=wx;
	alw->wybf=alw->wy=wy;
	alw->wwbf=alw->ww=ww;
	alw->whbf=alw->wh=wh;
	alw->fulled=0;
	alw->handle=-1;
	
	}

/* 
	set desk x gets the width in pixels of the desktop and places
	it in the supplied integer address
*/
void al_set_desk_x(int *limit)
	{
	GRECT p;
	wind_get(DESK,WF_WXYWH,&p.g_x,&p.g_y,&p.g_w,&p.g_h);
	*limit=p.g_w;
	}				
	
/*
 	open the window, either full size or at the initial size
	dictated by the ALWINDOW structure 
*/
int windopen( ALWINDOW *alw, int fullq )
	{
		int wr;
		GRECT p,q;
		wind_get(DESK,WF_WXYWH,&p.g_x,&p.g_y,&p.g_w,&p.g_h);
		wr=wind_calc(WC_WORK,alw->style,p.g_x,p.g_y,p.g_w,p.g_h,
					 &q.g_x,&q.g_y,&q.g_w,&q.g_h);
					 

		alw->handle=wind_create(alw->style,p.g_x,p.g_y,p.g_w,p.g_h);
		if(alw->handle<0) return(0);


		if(fullq)		
			{
				wr=wind_open(alw->handle,p.g_x,p.g_y,p.g_w,p.g_h);
				alw->wx=p.g_x;alw->wy=p.g_y;alw->ww=p.g_w;alw->wh=p.g_h;
			}
		else
			wr=wind_open(alw->handle,alw->wx,alw->wy,alw->ww,alw->wh);

		return(alw->handle);
	}


void windclose(ALWINDOW *alw)
	{
		int wr;

		if(alw->handle>0)
			{
				wr=wind_close(alw->handle);
				wr=wind_delete(alw->handle);
			}
	}

void clearw(ALWINDOW *alw)
	{
		short coord[4];
		GRECT c;

		wind_get(alw->handle,WF_WXYWH,&c.g_x,&c.g_y,&c.g_w,&c.g_h);

		coord[0]=c.g_x;
		coord[1]=c.g_y;
		coord[2]=c.g_x+c.g_w-1;
		coord[3]=c.g_y+c.g_h-1;

		
		graf_mouse(M_OFF,NULL);
		vsf_color(handle,0);
		vr_recfl(handle,coord);
		graf_mouse(M_ON,NULL);
	}

void wclip(ALWINDOW *alw)
	{
		short coord[4];

		GRECT c;

		wind_get(alw->handle,WF_WXYWH,&c.g_x,&c.g_y,&c.g_w,&c.g_h);


		coord[0]=c.g_x;
		coord[1]=c.g_y;
		coord[2]=c.g_x+c.g_w-1;
		coord[3]=c.g_y+c.g_h-1;
		
		vs_clip(handle,1,coord);

	}

void fullw(ALWINDOW *alw)
	{
	GRECT c,p,f;

	wind_get(alw->handle,WF_CXYWH,&c.g_x,&c.g_y,&c.g_w,&c.g_h);
	wind_get(alw->handle,WF_FXYWH,&f.g_x,&f.g_y,&f.g_w,&f.g_h);

	if(rc_equal(&c,&f))
		{
		wind_get(alw->handle,WF_PXYWH,&p.g_x,&p.g_y,&p.g_w,&p.g_h);
		if(!rc_equal(&p,&f))
			{
			wind_set(alw->handle,WF_CXYWH,p.g_x,p.g_y,p.g_w,p.g_h);
			alw->wx=p.g_x;alw->wy=p.g_y;alw->ww=p.g_w;alw->wh=p.g_h;
			}
		}
	else
		{
			wind_set(alw->handle,WF_CXYWH,f.g_x,f.g_y,f.g_w,f.g_h);
			alw->wx=f.g_x;alw->wy=f.g_y;alw->ww=f.g_w;alw->wh=f.g_h;
		}
	}		

/*
	size of window - vertically
*/
short window_height(ALWINDOW *alw)
	{
		GRECT c;
		wind_get(alw->handle,WF_WXYWH,&c.g_x,&c.g_y,&c.g_w,&c.g_h);
		return(c.g_h);
	}

/*
	size of the vertical scroll bar
*/
short window_vert_height(ALWINDOW *alw)
	{
		GRECT c;
		wind_get(alw->handle,WF_VSLSIZE,&c.g_x,&c.g_y,&c.g_w,&c.g_h);
		return(c.g_x);
	}

short window_work_y(ALWINDOW *alw)
	{
		GRECT c;
		wind_get(alw->handle,WF_WXYWH,&c.g_x,&c.g_y,&c.g_w,&c.g_h);
		return(c.g_y);
	}

short window_vert_pos(ALWINDOW *alw)
	{
		GRECT c;
		wind_get(alw->handle,WF_VSLIDE,&c.g_x,&c.g_y,&c.g_w,&c.g_h);
		return(c.g_x);
	}

/*
 * copy a string into a TEDINFO structure.
 */
void set_tedinfo(OBJECT *tree,int obj,char *source)
{
	char *dest;
	TEDINFO *tp;

	tp=(TEDINFO *)tree[obj].ob_spec;

	dest=/*((TEDINFO *)tree[obj].ob_spec)*/tp->te_ptext;
	strcpy(dest,source);
}

/*
 * copy the string from a TEDINFO into another string
 */
void get_tedinfo(OBJECT *tree, int obj, char *dest)
{
	char *source;

	source=((TEDINFO *)tree[obj].ob_spec)->te_ptext;	/* extract address */
	strcpy(dest,source);
}

int handle_dialog(OBJECT *dlog,int editnum,int zoom)
{
	short x,y,w,h;
	int but;
	form_center(dlog,&x,&y,&w,&h);


	form_dial(FMD_START,0,0,0,0,x,y,w,h);
if(zoom)
	form_dial(FMD_GROW,x+w/2,y+h/2,0,0,x,y,w,h);
	objc_draw(dlog,0,10,x,y,w,h);
	but=form_do(dlog,editnum);
if(zoom)
	form_dial(FMD_SHRINK,x+w/2,y+h/2,0,0,x,y,w,h);
	form_dial(FMD_FINISH,0,0,0,0,x,y,w,h);
	dlog[but].ob_state&=~SELECTED;	/* de-select exit button */
	return but;
}

void draw_dialog(OBJECT *dlog)
{
	short x,y,w,h;

	form_center(dlog,&x,&y,&w,&h);


	form_dial(FMD_START,0,0,0,0,x,y,w,h);
	objc_draw(dlog,0,10,x,y,w,h);
	form_dial(FMD_FINISH,0,0,0,0,x,y,w,h);

}

void set_normal_button(OBJECT *tree,int parent,int button)
{
	int b;

	for (b=tree[parent].ob_head; b!=parent; b=tree[b].ob_next)
		{

		if (b==button)
			tree[b].ob_state|=SELECTED;
		}
}

void unset_normal_button(OBJECT *tree,int parent,int button)
{
	int b;

	for (b=tree[parent].ob_head; b!=parent; b=tree[b].ob_next)
		{

		if (b==button)
			tree[b].ob_state&=~SELECTED;
		}
}

void set_button(OBJECT *tree,int parent,int button)
{
	int b;

	for (b=tree[parent].ob_head; b!=parent; b=tree[b].ob_next)
		{

		if (b==button)
			tree[b].ob_state|=SELECTED;
		else
			tree[b].ob_state&=~SELECTED;
		}
}

int get_button(OBJECT *tree,int parent)
{
	int b;

	b=tree[parent].ob_head;
	for (; b!=parent && !(tree[b].ob_state&SELECTED); b=tree[b].ob_next)
		;

	return b;
}

int get_normal_button(OBJECT *tree,int parent,int button)
{
	int b;

	for (b=tree[parent].ob_head; b!=parent; b=tree[b].ob_next)
		{

		if (b==button)
			return(tree[b].ob_state&SELECTED);
		}
}
