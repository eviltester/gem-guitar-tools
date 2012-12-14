#include "textwin.h"
#include "keys.h"

int total_items;
int max_on_screen;
float max_on_screenf;

int count_items(NODE *np)
{
	int count=0;

	while(np!=NULL)
	{
	count++;
	np=np->right;
	}

	return count;
}

void count_max_on_screen()
	{
	short x,y,w,h,tattributes[10];

	/* get the text attributes */
	vqt_attributes(handle,tattributes);

	/* text height is in tattributes[7] */

	/*get the window work area */
	wind_get(text_window.handle,WF_WXYWH,&x,&y,&w,&h);

	max_on_screen=h/(tattributes[7]*2);
	max_on_screenf=max_on_screen*1.0;
	}

void scale_side_set()
	{
	float per=max_on_screenf/((float)total_items-max_on_screenf);

	short new_pos=1000.0*per;

	if(total_items<max_on_screen)
		new_pos=1000;

	wind_set(text_window.handle,WF_VSLSIZE,new_pos);
	}
			

short first_scale_on_screen()
	{
	short pos=window_vert_pos(&text_window);
	float per=(float)pos/1000.0;
	short scale=(short)(per*((float)total_items-max_on_screenf));
	

	return scale;
	}

short side_part()
	{
	short vh=window_vert_height(&text_window);
	short ret=vh/max_on_screen;
	return ret;
	}


void print_scales_in_window(NODE *known_scales,int high_light)
	{
	int y_offset,loop;
	GRECT c;
	short dummy,first;
	NODE *np;
	SCALE *sc;

	np=known_scales;

	first=first_scale_on_screen();

	graf_mouse(M_OFF,NULL);

	for(loop=0;loop<first;loop++)
		np=np->right;

	wclip(&text_window);
	clearw(&text_window);

	wind_get(text_window.handle,WF_WXYWH,&c.g_x,&c.g_y,&c.g_w,&c.g_h);
	vst_height(handle,text_attributes[7],&dummy,&dummy,&dummy,&dummy);

	y_offset=(c.g_h-(26*text_attributes[7]))/2;

	for(loop=1;(loop<14 && np!=NULL);loop++)
		{

			if(loop==high_light)
				vswr_mode(handle,MD_ERASE);
			else
				vswr_mode(handle,MD_REPLACE);
			sc=np->data;
			v_gtext(handle,c.g_x+10,c.g_y+(text_attributes[7]*(loop*2)+y_offset),sc->name);
			np=np->right;
		}
			vswr_mode(handle,MD_REPLACE);

	graf_mouse(M_ON,NULL);

	}

NODE *scale_on_text_line(NODE *known_scales,int text_line)
	{
	int loop;
	short first;
	NODE *np;
	SCALE *sc;

	np=known_scales;

	first=first_scale_on_screen();


	for(loop=0;loop<first;loop++)
		np=np->right;


	for(loop=1;loop<text_line;loop++)
		{
			sc=np->data;
			np=np->right;
		}

	return(np);
	}

int is_text_line(int mousey)
	{
	GRECT c;
	int y_offset,loop;
	short dummy;

	wind_get(text_window.handle,WF_WXYWH,&c.g_x,&c.g_y,&c.g_w,&c.g_h);
	vst_height(handle,text_attributes[7],&dummy,&dummy,&dummy,&dummy);

	y_offset=(c.g_h-(26*text_attributes[7]))/2;

	for(loop=1;loop<14;loop++)
		{
		if((mousey>=
			(c.g_y+(text_attributes[7]*(loop*2)+y_offset)-text_attributes[7]))
			&&
			(mousey<=
			(c.g_y+(text_attributes[7]*(loop*2)+y_offset)))
		   )
		return loop;
		}
	return 0;
	}




NODE *restore_scale(CURRENT_STATUS *cs,NODE *known_scales)
	{
	short mbuff[8],dummy,new_pos;
	int high_light_text_line=0,text_line;
	OBJECT *dlog;
	int res;
	int max_items;
/*	char buf[100];*/
	int scan_code;
	int ascii_code;
	int local_finished=0,little_skip=0;
	short mousex,mousey,mouse_button,key_status,key_return,num_clicks;
	NODE *ret_scale=NULL;

/*						sprintf(buf,"[1][known %d][ok]",known_scales);
						form_alert(1,buf);
*/

	if(known_scales==NULL)
		return NULL;

	new_pos=1;

	total_items=count_items(known_scales);



	Crsrc_gaddr(R_TREE,MENU1,&menu_ptr);
	menu_tnormal(menu_ptr,MTScales,1);
	menu_bar(menu_ptr,0);

	init_alwindow(&text_window,
				  "SCALES",
				  	NAME|CLOSE/*|FULL*//*|MOVE*//*|INFO*//*|SIZE*/|UPARROW|
				  	DNARROW|VSLIDE/*|LFARROW|RTARROW|HSLIDE*/,
				  60,60,400,250);

	windopen(&text_window,1);

	if(total_items<max_on_screen)
		max_items=total_items;
	else
		max_items=max_on_screen;

	count_max_on_screen();

	wind_title(text_window.handle,text_window.title/*"SCALES"*/);
	scale_side_set();	

	Crsrc_gaddr(R_TREE,TextWinHelp,&dlog);

	print_scales_in_window(known_scales,high_light_text_line);

	for (; !local_finished && ret_scale==NULL;)
	{

		res=evnt_multi(	MU_KEYBD|MU_BUTTON|MU_MESAG,2,1,1,
						NULL,NULL,NULL,NULL,NULL,
						NULL,NULL,NULL,NULL,NULL,
						mbuff,
						NULL,NULL,
						&mousex,&mousey,&mouse_button,
						&key_status,&key_return,&num_clicks);
								

		if(alredraw==TRUE && mbuff[0]!=WM_REDRAW)
			{
				alredraw=FALSE;
			}

	if(res & MU_KEYBD)
	{
		scan_code=key_return ^ 0xFF00;
		ascii_code=key_return ^ 0x00FF;
		scan_code = scan_code >> 8;

		if(high_light_text_line==0)
		{
			if(scan_code==HelpKey)
			{
			  /* Help   */
					handle_dialog(dlog,0,0);
					little_skip=1;
					print_scales_in_window(known_scales,high_light_text_line);
					goto end_if2;	
			}

			if(scan_code==ReturnKey || scan_code==EscapeKey)	/* return */
				{
					ret_scale=NULL;			
					local_finished=1;
					goto end_if2;
				}
			
			if(scan_code==UndoKey)
				{	/* undo */
						little_skip=1;
				}
			else{
							/*default:*/
						high_light_text_line=1;
					print_scales_in_window(known_scales,high_light_text_line);
				}

			
		}
		
end_if1:;
		
		if(scan_code==WindowDownKey)
		{
				/* down line */

			if(key_status==2) /*page down*/
			{
					mbuff[0]=WM_ARROWED;
					mbuff[3]=text_window.handle;
					mbuff[4]=WA_DNPAGE;

			}
			else
			{				
				high_light_text_line++;

				if(high_light_text_line>max_items)
				{
					high_light_text_line=max_items;

				/*force window move down */

					mbuff[0]=WM_ARROWED;
					mbuff[3]=text_window.handle;
					mbuff[4]=WA_DNLINE;
				}
				else
					{
					little_skip=1;
					print_scales_in_window(known_scales,high_light_text_line);
					}
			}
			goto end_if2;	
		}

		if(scan_code==EscapeKey)	/* Escape */
			{
				ret_scale=NULL;			
				local_finished=1;
				goto end_if2;
			}

		if(scan_code==HelpKey)
		{

			  /* Help   */
					handle_dialog(dlog,0,0);
					little_skip=1;
					print_scales_in_window(known_scales,high_light_text_line);
				goto end_if2;
		}


		if(scan_code==ReturnKey)
		{
					/* return */
					ret_scale=scale_on_text_line(known_scales,high_light_text_line);			
					local_finished=1;
					goto end_if2;
		}
					

		if(scan_code==UndoKey)
		{	/* undo */
				high_light_text_line=0;
				print_scales_in_window(known_scales,high_light_text_line);
				little_skip=1;
				goto end_if2;
		}

		if(scan_code==WindowUpKey)
		{	/* up line */
	
			if(key_status==2) /*page up*/
			{
					mbuff[0]=WM_ARROWED;
					mbuff[3]=text_window.handle;
					mbuff[4]=WA_UPPAGE;

			}
			else
			{				
				high_light_text_line--;

					if(high_light_text_line==0)
					{	
						high_light_text_line=1;
						mbuff[0]=WM_ARROWED;
						mbuff[3]=text_window.handle;
						mbuff[4]=WA_UPLINE;
					}
					else
					{
						little_skip=1;
						print_scales_in_window(known_scales,high_light_text_line);
					}
			}
		
			goto end_if2;
			
		}
		else
		{
		/* default unknown key*/
			little_skip=1;
		}

end_if2:;

	}

	if(local_finished==1 || little_skip==1)
		goto skip_loop_end;

	if(res & MU_BUTTON)
		{
		if((text_line=is_text_line(mousey))!=0)
			{
			if(num_clicks==2)
				{
				ret_scale=scale_on_text_line(known_scales,text_line);
				}
			else
				{
				if(num_clicks==1)
					{
/*	sprintf(buf,"[1][text line %d  hitext  %d][ok]",text_line,high_light_text_line);
	form_alert(1,buf);

	form_alert(1,"[1][clicks 1][ok]");
*/					if(high_light_text_line!=0)
						{
/*	form_alert(1,"[1][clicks in][ok]");
*/
						if(text_line==high_light_text_line)
							high_light_text_line=0;
						else
							high_light_text_line=text_line;
						}
					else
						high_light_text_line=text_line;
					}
				}
			print_scales_in_window(known_scales,high_light_text_line);

			}
		}
	else
		{	
		switch(mbuff[0])
			{
			case WM_VSLID:
				if(mbuff[3]==text_window.handle)
					{
					wind_set(mbuff[3],WF_VSLIDE,mbuff[4]);
					print_scales_in_window(known_scales,high_light_text_line);
					alredraw=TRUE;
					}
				break;

			case WM_CLOSED:
				if(high_light_text_line!=0)
					ret_scale=scale_on_text_line(known_scales,high_light_text_line);			
				local_finished=1;
				break;
			case WM_ARROWED:
				if(mbuff[3]==text_window.handle)
					{
					switch(mbuff[4])
						{
						case WA_DNPAGE:
							new_pos=window_vert_height(&text_window)+
								window_vert_pos(&text_window);
		
							if(new_pos>1000)
								new_pos=1000;

							wind_set(text_window.handle,WF_VSLIDE,new_pos);

							print_scales_in_window(known_scales,high_light_text_line);
							break;
						case WA_DNLINE:
							new_pos=window_vert_pos(&text_window)+
									side_part();
					
							if((dummy=first_scale_on_screen())==0)
								new_pos+=side_part();
		
							if(new_pos>1000)
								new_pos=1000;

							wind_set(text_window.handle,WF_VSLIDE,new_pos);

							print_scales_in_window(known_scales,high_light_text_line);

							break;
						case WA_UPPAGE:
							new_pos=window_vert_pos(&text_window)-
									window_vert_height(&text_window);

		
							if(new_pos<1)
								new_pos=1;

							wind_set(text_window.handle,WF_VSLIDE,new_pos);

							print_scales_in_window(known_scales,high_light_text_line);

							break;
						case WA_UPLINE:
							new_pos=window_vert_pos(&text_window)-
									side_part();

							if((dummy=first_scale_on_screen())==1)
								new_pos-=side_part();
		
							if(new_pos<1)
								new_pos=1;

							wind_set(text_window.handle,WF_VSLIDE,new_pos);

							print_scales_in_window(known_scales,high_light_text_line);

							break;
						}
					}

			default:
				break;
						
			}
	skip_loop_end: little_skip=0;;
		}


	}



	Crsrc_gaddr(R_TREE,MENU1,&menu_ptr);
	menu_bar(menu_ptr,1);
	windclose(&text_window);

	return(ret_scale);

	}

NODE *delete_scale_window(CURRENT_STATUS *cs,NODE *known_scales)
	{	
	NODE *np;
	char *old_name=text_window.title;
	char new_title[]="DELETE SCALE";
	/*char buf[100];*/
/*						sprintf(buf,"[1][known %d][ok]",known_scales);
						form_alert(1,buf);
*/


	if(known_scales==NULL)
		return NULL;

	text_window.title=new_title;
	np=restore_scale(cs,known_scales);

	text_window.title=old_name;
	return(np);
	}

NODE *select_tuning_window(CURRENT_STATUS *cs,NODE *known_tunings)
	{	
	NODE *np;
	char *old_name=text_window.title;
	char new_title[]="SELECT TUNING";
	/*char buf[100];*/
/*						sprintf(buf,"[1][known %d][ok]",known_scales);
						form_alert(1,buf);
*/


	if(known_tunings==NULL)
		return NULL;

	text_window.title=new_title;
	np=restore_scale(cs,known_tunings);

	text_window.title=old_name;
	return(np);
	}

NODE *delete_tuning_window(CURRENT_STATUS *cs,NODE *known_tunings)
	{	
	NODE *np;
	char *old_name=text_window.title;
	char new_title[]="DELETE TUNING";
	/*char buf[100];*/
/*						sprintf(buf,"[1][known %d][ok]",known_scales);
						form_alert(1,buf);
*/


	if(known_tunings==NULL)
		return NULL;

	text_window.title=new_title;
	np=restore_scale(cs,known_tunings);

	text_window.title=old_name;
	return(np);
	}
