/*#define PRINTER_TEST
*/

#include "global.h"
#include "drawscl.h"
#include "scalegem.h"
#include "ctorsc.h"

int text_notes=0;

void scale_offsets(ALWINDOW *alw, int *xoffset, int *yoffset)
	{
		GRECT c;

		wind_get(alw->handle,WF_WXYWH,&c.g_x,&c.g_y,&c.g_w,&c.g_h);

		if(c.g_w>(frets_length*global_frets))
			*xoffset=(c.g_w-(frets_length*global_frets))/2;
		else
			*xoffset=0;
			*xoffset=*xoffset+c.g_x;

		if(c.g_h>(frets_height*6))
			*yoffset=(c.g_h-(frets_height*6))/2;
		else
			*yoffset=0;		
			*yoffset=*yoffset+c.g_y;
	}

void refix_start_fret()
	{
		GRECT c;
		int no_frets_on;

		wind_get(scale_window.handle,WF_WXYWH,&c.g_x,&c.g_y,&c.g_w,&c.g_h);
		no_frets_on=c.g_w/frets_length;

		if(no_frets_on>(global_frets-start_fret))
			start_fret=global_frets-no_frets_on;
	}

short pos_from_start_fret()
	{
		short pos;
		float per;

		per=(float)start_fret/((float)global_frets-(float)frets_on());
		
		pos=(short)(per*1000.0);
		return(pos);

	}

int frets_on()
	{
		GRECT c;
		wind_get(scale_window.handle,WF_WXYWH,&c.g_x,&c.g_y,&c.g_w,&c.g_h);
		return(c.g_w/frets_length);
	}

void set_start_fret(short pos)
	{
		int no_frets_on;
		short size_of_hor,fret_hor,dummy;
		float per;

		no_frets_on=frets_on();

		wind_get(scale_window.handle,WF_HSLSIZE,&size_of_hor,&dummy,&dummy,&dummy);
		fret_hor=size_of_hor/no_frets_on;

		per=(float)pos/1000.0;

		start_fret=(((float)global_frets-(float)no_frets_on)*per);

	}

void set_hor(void)
	{

		GRECT c;
		int no_frets_on;
		short size_of_hor;
		float fnofrets;
		float per;

		wind_get(scale_window.handle,WF_WXYWH,&c.g_x,&c.g_y,&c.g_w,&c.g_h);

		no_frets_on=c.g_w/frets_length;

		fnofrets=(float)global_frets;
		per=no_frets_on/fnofrets;
		size_of_hor=(short)(per*1000.0);

		wind_set(scale_window.handle,WF_HSLSIZE,size_of_hor);

	}


short pos_from_y_dist()
	{
		short pos;
		float per;

		per=(float)big_y_offset/(float)total_y_dist;
		pos=(short)(1000.0*per);
		return(pos);
	}


void set_big_y_offset(short pos)
	{

		GRECT c;

		float fpos=(float)pos;
		float per=fpos/1000.0;
		float fydist=(float)total_y_dist;
		
		wind_get(scale_window.handle,WF_WXYWH,&c.g_x,&c.g_y,&c.g_w,&c.g_h);
		big_y_offset=((fydist-(float)c.g_h)*per)*-1.0;
	}




void set_ver(int total_y_dist)
	{

		GRECT c;
		short size_of_ver;
		float winh,ydist;

		wind_get(scale_window.handle,WF_WXYWH,&c.g_x,&c.g_y,&c.g_w,&c.g_h);

		ydist=(float)total_y_dist;
		winh=(float)c.g_h;

		if(c.g_h>total_y_dist)
			size_of_ver=1000;
		else
			size_of_ver=((winh/ydist)*1000.0);


		wind_set(scale_window.handle,WF_VSLSIZE,size_of_ver);

	}

void small_scale_draw(ALWINDOW *alw, CURRENT_STATUS *cs)
	{
	register int loop1,loop2;
	int cf;
	short coord[4];
	register int curx,cury;

	int inc_res;
	int half_fret_height=frets_height/2,half_fret_length=frets_length/2;

	register int circle_radius;
	int h,saveh;
	int tadd;
	short ch,cw,clh,clw,maxcw;
	char albuff[10];
	char info_buffer[120];
	int combination_note,note_val;
	int frets_x_offset_and_frets_length;
	int half_fret_height_and_frets_y_offset;
	int frets_height_and_frets_y_offset;
	int frets_length_times_loop2;
	int frets_height_times_7;
	short tatts[10]; /* text attributes */
	NOTE *np;
	
#ifdef PRINTER_TEST
/* gdos test stuff*/
	short work_in[11],work_out[57],thandle,lop;

#endif

#define draw(X1,Y1,X2,Y2);	coord[0]=X1;coord[1]=Y1;coord[2]=X2;coord[3]=Y2;v_pline(handle,2,coord);

#ifdef PRINTER_TEST
if(vq_gdos()==0)
	fatal_error("GDOS NOT LOADED!");

#endif


#ifndef PRINTER_TEST
if(scale_name_flag && !key_name_flag)
	{
	sprintf(info_buffer," %s",cs->current_scale->name);
	wind_info(scale_window.handle,info_buffer);
	}
else
	{
	if(!scale_name_flag && key_name_flag)
		{
		sprintf(info_buffer," %s",cs->key->note_name);
		wind_info(scale_window.handle,info_buffer);
		}
	else
		{
		if(scale_name_flag && key_name_flag)
			{
			sprintf(info_buffer," %s in %s",cs->current_scale->name,cs->key->note_name);
			wind_info(scale_window.handle,info_buffer);
			}
		else
			{
			sprintf(info_buffer,"                                                                                       ");
			wind_info(scale_window.handle,info_buffer);
			}
		}
	}
#endif


	scale_offsets(alw,&frets_x_offset,&frets_y_offset);
	frets_y_offset=frets_y_offset+big_y_offset;
	frets_height_and_frets_y_offset=frets_height+frets_y_offset;
	frets_x_offset_and_frets_length=frets_length+frets_x_offset;
	half_fret_height_and_frets_y_offset=half_fret_height+frets_y_offset;


#ifndef PRINTER_TEST
h=0;cw=0;ch=0;maxcw=0;
while(cw<((frets_length/2)-2) && h<99)
	{
		
		if(cw>maxcw)
			saveh=h;
		h++;
		vst_height(handle,h,&cw,&ch,&clw,&clh);
	}

set_hor();


if(h>=99)
	vst_height(handle,saveh,&cw,&ch,&clw,&clh);
#endif

#ifdef PRINTER_TEST
clh=0;
#endif

	vqt_attributes(handle,tatts);

	frets_height_times_7=7*frets_height;

	total_y_dist=clh+(frets_height_times_7);


	set_ver(total_y_dist);


	if(frets_length>frets_height)
		circle_radius=(frets_height/2)-1-2;
	else
		circle_radius=(frets_length/2)-1-2;

	curx=0;

#ifdef PRINTER_TEST
work_in[0]=21; /*printer ?*/
for(lop=1;lop<10;lop++)
	work_in[lop]=1;
	work_in[10]=2;

for(lop=21;lop<31;lop++)
{
	work_in[0]=lop;
	v_opnwk(work_in,&handle,work_out);
	if(handle)
{
		error("GOT A PRINTER HANDLE");
		lop=32;
	}
	else
		error("NOT GOT A PRINTER HANDLE");
}


#endif

	wclip(alw);
	graf_mouse(M_OFF,NULL);
	for(loop2=0;loop2<global_frets && loop2+start_fret<global_frets;loop2++)
		{
		frets_length_times_loop2=frets_length*loop2;

		for(loop1=0;loop1<6;loop1++)
			{
			if(curx<desk_x+frets_height)
			{
				cf=loop2+start_fret;

				curx=/*alw->wx+*/(frets_length_times_loop2);
				cury=/*alw->wy+*/(loop1*frets_height);
				if(cf!=0)
					{

						draw(curx+frets_x_offset,cury+half_fret_height_and_frets_y_offset,
							 curx+frets_x_offset_and_frets_length,cury+half_fret_height_and_frets_y_offset);

					}


				switch(loop1)
				{
				case 0:				
					draw(curx+frets_x_offset_and_frets_length,cury+frets_y_offset+half_fret_height,
					 curx+frets_x_offset_and_frets_length,cury+frets_height_and_frets_y_offset);						
					break;

			 	case 5:					
					draw(curx+frets_x_offset_and_frets_length,cury+frets_y_offset,
					 curx+frets_x_offset_and_frets_length,cury+half_fret_height_and_frets_y_offset);						
					break;

				default:			
					draw(curx+frets_x_offset_and_frets_length,cury+frets_y_offset,
					 curx+frets_x_offset_and_frets_length,cury+frets_height_and_frets_y_offset);						
				}

#ifndef PRINTER_TEST
	if(numbers_flag==1)
	{

	if(cf==first_fret || (((inc_res=cf%fret_inc)==0) && cf>first_fret))
		{
		if((tadd=clh-frets_height)>0)
			{
			sprintf(albuff,"%d",cf);
			if(strlen(albuff)==1)
			v_gtext(handle,frets_x_offset+(frets_length_times_loop2)+half_fret_length-(tatts[6]/2),frets_y_offset+(frets_height_times_7)+tadd,albuff);
			else
			v_gtext(handle,frets_x_offset+(frets_length_times_loop2)+half_fret_length-tatts[6],frets_y_offset+(frets_height_times_7)+tadd,albuff);
			}
		else
			{
			sprintf(albuff,"%d",cf);
			if(strlen(albuff)==1)
			v_gtext(handle,frets_x_offset+(frets_length_times_loop2)+half_fret_length-(tatts[6]/2),frets_y_offset+(frets_height_times_7),albuff);
			else
			v_gtext(handle,frets_x_offset+(frets_length_times_loop2)+half_fret_length-tatts[6],
								frets_y_offset+(frets_height_times_7),albuff);
			}
		}
	}
#endif

		if(text_notes==1)
		{

			combination_note=0;
			if(cs->display_fretboard[loop1][cf]>RELATIVE_NOTE)
				{
					combination_note=1;
					note_val=cs->display_fretboard[loop1][cf]-RELATIVE_NOTE;
				}
			else
				{
					note_val=cs->display_fretboard[loop1][cf];
				}

			switch(note_val)
			{
						case NO_NOTE: break;
						case NORMAL_NOTE:
							if(combination_note)
								vst_effects(handle,0|UNDERLINED);
							else
								vst_effects(handle,0);
							break;
						case FILLED_NOTE:
							if(combination_note)
								vst_effects(handle,OUTLINE|UNDERLINED);
							else
								vst_effects(handle,OUTLINE);
							break;
						case RELATIVE_NOTE:
							vst_effects(handle,SHADED|UNDERLINED); 
							break;
			}

			/* draw as text */
			if(cs->display_fretboard[loop1][cf]!=NO_NOTE)
			{
				np=cs->current_fretboard[loop1][cf];
				sprintf(albuff,"%s",np->note_name);
				if(strlen(albuff)==1)
					v_gtext(handle,frets_x_offset+frets_length_times_loop2+half_fret_length-(tatts[6]/2),
										cury+half_fret_height_and_frets_y_offset+(tatts[7]/2)/*cury+frets_y_offset+frets_height*/,albuff);
				else
					v_gtext(handle,/*curx+frets_x_offset*/frets_x_offset+(frets_length_times_loop2)+half_fret_length-tatts[6],
										cury+half_fret_height_and_frets_y_offset+(tatts[7]/2)/*cury+frets_y_offset+frets_height*/,albuff);
			}
		
			vst_effects(handle,0);

		}
		else
		{
			combination_note=0;
			if(cs->display_fretboard[loop1][cf]>RELATIVE_NOTE)
				{
					combination_note=1;
					note_val=cs->display_fretboard[loop1][cf]-RELATIVE_NOTE;
				}
			else
				{
					note_val=cs->display_fretboard[loop1][cf];
				}

			if(combination_note)
				{
					vsf_color(handle,BLACK);
					vsf_interior(handle,FIS_HOLLOW);
					vsf_style(handle,0);

					alcircle(handle, 
								curx+half_fret_length+frets_x_offset,
								cury+half_fret_height+frets_y_offset, 
								circle_radius+2);
 		
					vsf_color(handle,BLACK);					
					vsf_interior(handle,8);
					vsf_style(handle,2);
				}

				switch(note_val)		
					{
						case NO_NOTE: break;
						case NORMAL_NOTE:
							vsf_color(handle,BLACK); 
							vsf_interior(handle,2);
							vsf_style(handle,2);

							alcircle(handle,
							curx+half_fret_length+frets_x_offset,
							cury+half_fret_height_and_frets_y_offset,
							circle_radius); 

							vsf_color(handle,BLACK);
							vsf_interior(handle,8);
							vsf_style(handle,2);
							break;
						case FILLED_NOTE:
							vsf_color(handle,BLACK);
							vsf_interior(handle,FIS_SOLID);
							vsf_style(handle,2);

							alcircle(handle, 
							curx+half_fret_length+frets_x_offset,
							cury+half_fret_height_and_frets_y_offset, 
							circle_radius);
 		
							vsf_color(handle,BLACK);					
							break;
						case RELATIVE_NOTE: 
							vsf_color(handle,BLACK);
							vsf_interior(handle,FIS_HOLLOW);
							vsf_style(handle,2);
							alcircle(handle, 
							curx+half_fret_length+frets_x_offset,
							cury+half_fret_height_and_frets_y_offset, 
							circle_radius);
 		
							vsf_color(handle,BLACK);					
							vsf_interior(handle,8);
							vsf_style(handle,2);
							break;
/*						case COMMON_NOTE: 
							vsf_color(handle,BLACK);
							vsf_interior(handle,4);
							vsf_style(handle,2);
							alcircle(handle, 
							curx+half_fret_length+frets_x_offset,
							cury+half_fret_height_and_frets_y_offset, 
							circle_radius);
 		
							vsf_color(handle,BLACK);					
							vsf_interior(handle,8);
							vsf_style(handle,2);
							break;*/
					}
			}

			}
			}

	
		}
	

graf_mouse(M_ON,NULL);
#ifdef PRINTER_TEST
v_clswk(handle);
#endif
	}
				
void specific_note_draw(ALWINDOW *alw, CURRENT_STATUS *cs,int specific_note)
	{
	register int loop1,loop2;
	int cf;
	short coord[4];
	register int curx,cury;
	int half_fret_height=frets_height/2,half_fret_length=frets_length/2;
	register int circle_radius;
	int combination_note,note_val;
	int frets_length_times_loop2,half_fret_height_and_frets_y_offset;
	short tatts[10]; /* text attributes */
	NOTE *np;
	char albuff[10];
	
#define draw(X1,Y1,X2,Y2);	coord[0]=X1;coord[1]=Y1;coord[2]=X2;coord[3]=Y2;v_pline(handle,2,coord);



	scale_offsets(alw,&frets_x_offset,&frets_y_offset);
	frets_y_offset=frets_y_offset+big_y_offset;
	half_fret_height_and_frets_y_offset=half_fret_height+frets_y_offset;



	if(frets_length>frets_height)
		circle_radius=(frets_height/2)-1-2;
	else
		circle_radius=(frets_length/2)-1-2;

	vqt_attributes(handle,tatts);

	wclip(alw);
	graf_mouse(M_OFF,NULL);
	for(loop2=0;loop2<global_frets && loop2+start_fret<global_frets;loop2++)
		{
		frets_length_times_loop2=frets_length*loop2;

		for(loop1=0;loop1<6;loop1++)
			{
				cf=loop2+start_fret;

				curx=(loop2*frets_length);
				cury=(loop1*frets_height);




	if(cs->current_fretboard[loop1][cf]->number==specific_note)
		{

/*							vsf_color(handle,WHITE);
							vsf_interior(handle,FIS_SOLID);
							vsf_style(handle,1);

							alcircle(handle,
							curx+half_fret_length+frets_x_offset,
							cury+half_fret_height+frets_y_offset,
							circle_radius+2); 

							vsf_color(handle,BLACK);
							vsf_interior(handle,8);
							vsf_style(handle,2);

*/

		/* Wipe out the old note */
				coord[0]=curx+frets_x_offset+1;
				coord[1]=cury+frets_y_offset;
				coord[2]=curx+frets_x_offset+frets_length-1;
				coord[3]=cury+frets_height+frets_y_offset;
				vsf_color(handle,WHITE);
				vsf_interior(handle,FIS_SOLID);
				vsf_style(handle,1);
				vr_recfl(handle,coord);

				vsf_color(handle,BLACK);
				vsf_interior(handle,8);
				vsf_style(handle,2);


				/* if the current fret is 0 then it is the open notes
					only draw the string if it is not the open note */

						if(cf!=0)
							{
							draw(curx+frets_x_offset,
								cury+half_fret_height+frets_y_offset,
							 	curx+frets_length+frets_x_offset,
								cury+half_fret_height+frets_y_offset);
							}

	/* draw the note */
		if(text_notes==1)
		{

			combination_note=0;
			if(cs->display_fretboard[loop1][cf]>RELATIVE_NOTE)
				{
					combination_note=1;
					note_val=cs->display_fretboard[loop1][cf]-RELATIVE_NOTE;
				}
			else
				{
					note_val=cs->display_fretboard[loop1][cf];
				}

			switch(note_val)
			{
						case NO_NOTE: break;
						case NORMAL_NOTE:
							if(combination_note)
								vst_effects(handle,0|UNDERLINED);
							else
								vst_effects(handle,0);
							break;
						case FILLED_NOTE:
							if(combination_note)
								vst_effects(handle,OUTLINE|UNDERLINED);
							else
								vst_effects(handle,OUTLINE);
							break;
						case RELATIVE_NOTE:
							vst_effects(handle,UNDERLINED); 
							break;
			}

			/* draw as text */
			if(cs->display_fretboard[loop1][cf]!=NO_NOTE)
			{
				np=cs->current_fretboard[loop1][cf];
				sprintf(albuff,"%s",np->note_name);
				if(strlen(albuff)==1)
					v_gtext(handle,frets_x_offset+frets_length_times_loop2+half_fret_length-(tatts[6]/2),
										cury+half_fret_height_and_frets_y_offset+(tatts[7]/2)/*cury+frets_y_offset+frets_height*/,albuff);
				else
					v_gtext(handle,/*curx+frets_x_offset*/frets_x_offset+(frets_length_times_loop2)+half_fret_length-tatts[6],
										cury+half_fret_height_and_frets_y_offset+(tatts[7]/2)/*cury+frets_y_offset+frets_height*/,albuff);
			}
		
			vst_effects(handle,0);

		}
		else
		{

			combination_note=0;
			if(cs->display_fretboard[loop1][cf]>RELATIVE_NOTE)
				{
					combination_note=1;
					note_val=cs->display_fretboard[loop1][cf]-RELATIVE_NOTE;
				}
			else
				{
					note_val=cs->display_fretboard[loop1][cf];
				}

			if(combination_note)
				{
					vsf_color(handle,BLACK);
					vsf_interior(handle,FIS_HOLLOW);
					vsf_style(handle,0);

					alcircle(handle, 
								curx+half_fret_length+frets_x_offset,
								cury+half_fret_height+frets_y_offset, 
								circle_radius+2);
 		
					vsf_color(handle,BLACK);					
					vsf_interior(handle,8);
					vsf_style(handle,2);
				}

				switch(note_val)		
					{
						case NO_NOTE: 
/*							vsf_color(handle,WHITE);
							vsf_interior(handle,FIS_SOLID);
							vsf_style(handle,1);

							alcircle(handle,
							curx+half_fret_length+frets_x_offset,
							cury+half_fret_height+frets_y_offset,
							circle_radius); 

							vsf_color(handle,BLACK);
							vsf_interior(handle,8);
							vsf_style(handle,2);
						if(cf!=0)
							{
							draw(curx+frets_x_offset,
								cury+half_fret_height+frets_y_offset,
							 	curx+frets_length+frets_x_offset,
								cury+half_fret_height+frets_y_offset);
							}*/
							break;
						case NORMAL_NOTE:
/*						if(cf!=0)
							{
							draw(curx+frets_x_offset,
								cury+half_fret_height+frets_y_offset,
							 	curx+frets_length+frets_x_offset,
								cury+half_fret_height+frets_y_offset);
							}
*/							vsf_color(handle,BLACK); 
							vsf_interior(handle,2);
							vsf_style(handle,2);

							alcircle(handle,
							curx+half_fret_length+frets_x_offset,
							cury+half_fret_height+frets_y_offset,
							circle_radius); 

							vsf_color(handle,BLACK);
							vsf_interior(handle,8);
							vsf_style(handle,2);
							break;
						case FILLED_NOTE:
/*						if(cf!=0)
							{
							draw(curx+frets_x_offset,
								cury+half_fret_height+frets_y_offset,
							 	curx+frets_length+frets_x_offset,
								cury+half_fret_height+frets_y_offset);
							}
*/							vsf_color(handle,BLACK);
							vsf_interior(handle,FIS_SOLID);
							vsf_style(handle,2);

							alcircle(handle, 
							curx+half_fret_length+frets_x_offset,
							cury+half_fret_height+frets_y_offset, 
							circle_radius);
 		
							vsf_color(handle,BLACK);					
							break;
						case RELATIVE_NOTE: 
/*						if(cf!=0)
							{
							draw(curx+frets_x_offset,
								cury+half_fret_height+frets_y_offset,
							 	curx+frets_length+frets_x_offset,
								cury+half_fret_height+frets_y_offset);
							}
*/							vsf_color(handle,BLACK);
							vsf_interior(handle,FIS_HOLLOW);
							vsf_style(handle,0);

							alcircle(handle, 
							curx+half_fret_length+frets_x_offset,
							cury+half_fret_height+frets_y_offset, 
							circle_radius);
 		
							vsf_color(handle,BLACK);					
							vsf_interior(handle,8);
							vsf_style(handle,2);
							break;
/*						case COMMON_NOTE: 
							vsf_color(handle,BLACK);
							vsf_interior(handle,4);
							vsf_style(handle,2);
							alcircle(handle, 
							curx+half_fret_length+frets_x_offset,
							cury+half_fret_height+frets_y_offset, 
							circle_radius);
 		
							vsf_color(handle,BLACK);					
							vsf_interior(handle,8);
							vsf_style(handle,2);
							break;
*/					}

			}

			}			
			}

	
		}
	

graf_mouse(M_ON,NULL);


	}
