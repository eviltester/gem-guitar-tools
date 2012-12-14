#define DEMO
#include "printer.h"

p_driver *current_driver=NULL;
pto_disk_data *gprint_to_disk=NULL;

char default_pname[]="TEXT DRIVER EPSON";
list default_lfeed={NULL,10,NULL};
list default_npage={NULL,12,NULL};
list default_nfret={NULL,'|',NULL};
list default_fret1={NULL,'+',NULL};
list default_fret6={NULL,'+',NULL};
list default_string={NULL,'-',NULL};
list default_note={NULL,'.',NULL};
list default_root={NULL,',',NULL};
list default_rel={NULL,0x27,NULL};
list default_normNrel={NULL,':',NULL};
list default_rootnrel={NULL,';',NULL};
list default_nonote={NULL,' ',NULL};
list default_space={NULL,' ',NULL};

char default_opennotes[]="o |";
char default_fretdescr[]=" o |";


char skip_white_spaces(FILE *fp)
{
	char ch;

	ch=fgetc(fp);

	while(!feof(fp) && (ch==' ' || ch=='\t' || isspace(ch) || ch=='\n'))
		ch=fgetc(fp);

	return ch;
}

void skip_to(char ch, FILE *fp)
{
	char nc;

	nc=fgetc(fp);

	while(ch!=nc && !feof(fp))
		nc=fgetc(fp);

}

int separator(char ch)
{
	switch(ch)
	{
	case ' ':
	case ',':
	case '.':
	case ';':
		return(1);

	default:
		return(0);
	}
}

#define MAX_TOKEN_SIZE	100

char *get_token(char first_char, FILE *fp)
{
	char token_array[MAX_TOKEN_SIZE];
	int count=0;
	char fc,*fcp;

	count++;
	token_array[0]=first_char;

	fc=fgetc(fp);
	while(count<MAX_TOKEN_SIZE && !feof(fp) &&
			!separator(fc))
	{
		token_array[count]=fc;
		fc=fgetc(fp);
		count++;
	}

	token_array[count]='\0';
	fcp=(char *)malloc(count);
	strcpy(fcp,token_array);

	count=fseek(fp,-1,SEEK_CUR);

	return(fcp);
	

}

char *read_a_string(FILE *fp)
{
	char ch,*expand_buf,*cp;
	char *ret_buf;
	int eb_len,eb_size;


	expand_buf=(char *)malloc(11);
	if(expand_buf==NULL)
	{
		error("Cannot create expand buf 1");
		return(NULL);
	}
	expand_buf[0]='\0';
	eb_len=0;
	eb_size=11;

	ch=skip_white_spaces(fp);

	if(ch!='"')
	{
		error("field is not a quoted string");
		return( NULL );
	}

	ch=fgetc(fp);

	while(!feof(fp) && ch!='"')
	{
		expand_buf[eb_len]=ch;
		eb_len++;

		if(eb_len==eb_size)
		{
		/* resize */
		cp=(char *)realloc(expand_buf,eb_size+10);
		if(cp==NULL)
			fatal_error("Cannot Allocate expand_buf 2");

		eb_size+=10;
		expand_buf=cp;
		}
	
		expand_buf[eb_len]='\0';
		ch=fgetc(fp);
	}

	if(feof(fp))
		fatal_error("Premature eof reached in string read");

	ret_buf=(char *)malloc(strlen(expand_buf)+1);
	if(ret_buf==NULL)
		fatal_error("no room for return buf");

	strcpy(ret_buf,expand_buf);
	free(expand_buf);

/*	sprintf(ebuf,"read -%s- length is %d",ret_buf,strlen(ret_buf));
	error(ebuf);*/

	return(ret_buf);
}

int read_a_number(FILE *fp)
{
	char ch;
	char buf[20]; /* big number! */
	int count;

	ch=skip_white_spaces(fp);
	count=0;


	while(isdigit(ch) && !feof(fp))
	{
		buf[count]=ch;
		count++;
		ch=fgetc(fp);
	}

	buf[count]='\0';

	count=fseek(fp,-1,SEEK_CUR);

	stcd_i(buf,&count);

	return(count);
}

list *intify_string(char *ch)
{
	list *lp1,*head,*spare;

	lp1=NULL;
	spare=(list *)malloc(sizeof(list));
	if(spare==NULL)
		fatal_error("cannot create spare");

	spare->left=NULL;
	spare->right=NULL;
	
	head=NULL;

	while(*ch!='\0')
	{
		spare->data=*ch;

		if(head==NULL)
		{
			head=spare;
		}
		else
		{
			lp1->right=spare;
			spare->left=lp1;
		}

		lp1=spare;

		spare=(list *)malloc(sizeof(list));
		if(spare==NULL)
			fatal_error("Cannot create spare 2");
		spare->left=NULL;
		spare->right=NULL;

		ch++;
	}

	free(spare);

	return (head);
}

void show_list(list *l)
{
	char buf[100];
	char buf2[10];

	sprintf(buf,"list = ");

	while(l)
	{
		sprintf(buf2,"%c,",l->data);
		strcat(buf,buf2);
		l=l->right;
	}

	error(buf);
}
		
list *read_a_list(FILE *fp)
{
	list *lp,*lp2,*head;
	char ch,*cp;
	int count;

/*	lp=(list *)malloc(sizeof(list));
	if(lp==NULL)
		fatal_error("cannot allocate list memory A1");

	lp->left=3;
	lp->right=3;
	lp->data=3;
*/
	lp=NULL;
	ch='a';

	while(ch!='.' && !feof(fp))
	{
		ch=skip_white_spaces(fp);

/*		sprintf(buf,"switch on %c",ch);
		error(buf);
*/
		switch(ch)
		{
		case '"':
			count=fseek(fp,-1,SEEK_CUR);
			cp=read_a_string(fp);
			lp2=intify_string(cp);
			free(cp);
	
			if(lp==NULL)
			{
		
				lp=lp2;
				head=lp;
				while(lp2->right!=NULL)
					lp2=lp2->right;
				lp=lp2;
			}
			else
			{
				lp->right=lp2;
				lp2->left=lp;
				while(lp2->right!=NULL)
					lp2=lp2->right;
				lp=lp2;
			}

			break;
				
		case ',':
			break;
		
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			count=fseek(fp,-1,SEEK_CUR);
			count=read_a_number(fp);
/*			error("read number");*/
			lp2=(list *)malloc(sizeof(list));
			if(lp2==NULL)
				fatal_error("cannot create list A2");

			lp2->data=count;
			lp2->right=NULL;
			lp2->left=NULL;

			if(lp==NULL)
			{
				lp=lp2;
				head=lp;
			}
			else
			{
				lp2->left=lp;
				lp->right=lp2;
				lp=lp2;
			}
			break;
		}			
	}
/*error("read list");
show_list(head);*/
	return(head);
}

	
void process_token(FILE *fp, char *ts, p_driver *pd)
{
	char *cp;
	char ch;

	if(is_string(ts,"NM",1))
		{
		/* get string -> name */
		cp=read_a_string(fp);
		if(cp!=NULL)
			pd->name=cp;

		/* get fullstop */
		ch=skip_white_spaces(fp);
/*		sprintf(buf,"%c",ch);
		error(buf);*/

		if(ch!='.')
			fatal_error("NAME FIELD IS NOT A STRING | FOLLOWED BY A FULLSTOP ");

		/* flag any errors */

		}

	if(is_string(ts,"LF",1))
		{
		/* read a list and assign it to carraige return */
		pd->line_feed=read_a_list(fp);
		}
	
	if(is_string(ts,"NP",1))
		{
		/* read a list and assign it to new page */
		pd->new_page=read_a_list(fp);
		}

	if(is_string(ts,"FS",1))
		{
		pd->fret=read_a_list(fp);
		}

	if(is_string(ts,"F1",1))
		{
		pd->fret1=read_a_list(fp);
		}

	if(is_string(ts,"F6",1))
		{
		pd->fret6=read_a_list(fp);
		}

	if(is_string(ts,"SS",1))
		{
		pd->string=read_a_list(fp);
		}

	if(is_string(ts,"NS",1))
		{
		pd->note=read_a_list(fp);
		}

	if(is_string(ts,"RS",1))
		{
		pd->root=read_a_list(fp);
		}

	if(is_string(ts,"LS",1))
		{
		pd->relative=read_a_list(fp);
		}

	if(is_string(ts,"NL",1))
		{
		pd->normNrel=read_a_list(fp);
		}

	if(is_string(ts,"RL",1))
		{
		pd->rootNrel=read_a_list(fp);
		}

	if(is_string(ts,"NN",1))
		{
		pd->no_note=read_a_list(fp);
		}

	if(is_string(ts,"SP",1))
		{
		pd->space=read_a_list(fp);
		}

	if(is_string(ts,"F0",1))
		{
		/* get string -> name */
		cp=read_a_string(fp);
		if(cp!=NULL)
			pd->open_notes=cp;

		/* get fullstop */
		ch=skip_white_spaces(fp);
		if(ch!='.')
			fatal_error("OPEN NOTES IS NOT A STRING | FOLLOWED BY A FULLSTOP ");

		/* flag any errors */

		}

	if(is_string(ts,"FD",1))
		{
		/* get string -> name */
		cp=read_a_string(fp);
		if(cp!=NULL)
			pd->fret_descr=cp;

		/* get fullstop */
		ch=skip_white_spaces(fp);
		if(ch!='.')
			fatal_error("FRET DESCRIPTION IS NOT A STRING | FOLLOWED BY A FULLSTOP ");

		/* flag any errors */

		}		
}	

void load_new_driver()
{
	p_driver *newd;
	char filename[FMSIZE];
	int result;

	result=select("DRV",filename,"LOAD PRINTER DRIVER");

	if(result)
	{

	newd=load_printer_driver(filename);

	if(newd!=NULL && newd!=current_driver)
		destroy_driver(current_driver);

	current_driver=newd;

	}
}


p_driver *load_printer_driver(char *filename)
{
	FILE *fp;
	p_driver *tpd;
	int local_finished=0;
	char nc;
	char *token_string,*buf;


	fp=fopen(filename,"r");

	if(fp==NULL)
	{
		error(filename);
		error("cannot open printer driver file");
		return(current_driver);
	}

	tpd=create_printer_driver();

	while(!feof(fp) && !local_finished)
	{
		nc=skip_white_spaces(fp);

		switch(nc)
		{
		case 	'%':
					skip_to('\n',fp);
					break;

		case	'F':
		case	'N':
		case	'R':
		case 	'L':
		case	'S':
		case	'E':
			token_string=get_token(nc,fp);
			/*error(token_string);*/

			if(is_string(token_string,"END",1))
			{
				local_finished=1;
				free(token_string);
			}
			else
			{
			process_token(fp,token_string,tpd);
			free(token_string);
			}
			break;

		default:
			token_string=get_token(nc,fp);		
			buf=(char *)malloc(100);
			sprintf(buf,"invalid token %s",token_string);
			error(buf);
			free(buf);
			free(token_string);
			destroy_driver(tpd);
			fclose(fp);
			return(current_driver);
		}				

	/*process token_string */

	}		


	fclose(fp);
	return(tpd);

}

void scrub_list(list *lp)
{
	list *nlp;

	while(lp!=NULL)
	{
	nlp=lp;
	lp=lp->right;
	free(nlp);
	}
}

void destroy_driver( p_driver *tpd)
{

	if(tpd->name!=default_pname)
		free(tpd->name);

	if(tpd->line_feed!=&default_lfeed)
		scrub_list(tpd->line_feed);

	if(tpd->new_page!=&default_npage)
		scrub_list(tpd->new_page);

	if(tpd->fret!=&default_nfret)
		scrub_list(tpd->fret);

	if(tpd->fret1!=&default_fret1)
		scrub_list(tpd->fret1);

	if(tpd->fret6!=&default_fret6)
		scrub_list(tpd->fret6);

	if(tpd->string!=&default_string)
		scrub_list(tpd->string);

	if(tpd->note!=&default_note)
		scrub_list(tpd->note);

	if(tpd->root!=&default_root)
		scrub_list(tpd->root);

	if(tpd->relative!=&default_rel)
		scrub_list(tpd->root);

	if(tpd->normNrel!=&default_normNrel)
		scrub_list(tpd->normNrel);

	if(tpd->rootNrel!=&default_rootnrel)
		scrub_list(tpd->rootNrel);

	if(tpd->no_note!=&default_nonote)
		scrub_list(tpd->no_note);

	if(tpd->space!=&default_space)
		scrub_list(tpd->space);

	if(tpd->open_notes!=default_opennotes)
		free(tpd->open_notes);

	if(tpd->fret_descr!=default_fretdescr)
		free(tpd->fret_descr);

	free(tpd);
}

p_driver *create_printer_driver()
{
	p_driver *tpd=(p_driver *)malloc(sizeof(p_driver));

	if(!tpd)
	{
		error("Cannot create printer driver");
	}
	else
	{
		tpd->name=default_pname;
		tpd->line_feed=&default_lfeed;
		tpd->new_page=&default_npage;
		tpd->fret=&default_nfret;
		tpd->fret1=&default_fret1;
		tpd->fret6=&default_fret6;
		tpd->string=&default_string;
		tpd->note=&default_note;
		tpd->root=&default_root;
		tpd->relative=&default_rel;
		tpd->normNrel=&default_normNrel;
		tpd->rootNrel=&default_rootnrel;
		tpd->no_note=&default_nonote;
		tpd->space=&default_space;

		tpd->open_notes=default_opennotes;
		tpd->fret_descr=default_fretdescr;
	}

	return tpd;
}


void albf(char *ch,short dev)
	{

	long error=1;
	char *error_text;

	while(*ch!='\0')
		{
			if(gprint_to_disk!=NULL)
				{
				error=fputc(*ch,gprint_to_disk->output_file);

				if(error!=*ch)
				{
					*ch='\0';
					error_text=(char *)malloc(70);

					if(error_text)
					{
						sprintf(error_text,"File Error Value %d",error);
						alerror(0,error_text);
						free(error_text);
					}
					else
					{
						alerror(0,"File Error");
					}
				}
				ch++;
			}
#ifndef DEMO
			else
			{
				error=Bconout(dev,*ch);
				ch++;
				if(!error)
					*ch='\0';
			}
#endif
		}
	}

void albf1(char *ch,short dev)
	{

	long error=1;
	char *error_text;

			if(gprint_to_disk!=NULL)
				{
				error=fputc(*ch,gprint_to_disk->output_file);
				if(error!=*ch)
				{

					error_text=(char *)malloc(70);

					if(error_text)
					{
						sprintf(error_text,"File Error Value %d during print",error);
						alerror(0,error_text);
						free(error_text);
					}
					else
					{
						alerror(0,"File Error during print");
					}
				}
			}
#ifndef DEMO
			else
			{

			error=Bconout(dev,*ch);
			}
#endif
	}


void output_key_name(CURRENT_STATUS *cs,short dev)
	{
		albf(cs->key->note_name,dev);
	}		

void output_notes_string(CURRENT_STATUS *cs, short dev)
	{
	int loop,first;
	first=0;

	for(loop=0;loop<12;loop++)
		{
		
		if(cs->current_notes[loop])
			{
			if(first!=0)
				albf(", ",dev);


			first=1;
			albf(note_numbers[loop]->note_name,dev);
			}
		}
	}
			
void output_name_string(CURRENT_STATUS *cs, short dev)
	{
	albf(cs->current_scale->name,dev);
	}

void output_header(CURRENT_STATUS *cs, short dev)
	{
		output_text(cs,tp_header1,dev);
		output_text(cs,tp_header2,dev);
	}

void output_trailer(CURRENT_STATUS *cs, short dev)
	{
		output_text(cs,tp_trailer1,dev);
		output_text(cs,tp_trailer2,dev);
	}

int is_string(char *orig, char *test, int caps_dist)
	{

	char c1,c2;

	while(*orig !='\0' && *test!='\0')
		{
		if(caps_dist)
			{
			c1=*orig;c2=*test;
			}
		else
			{
			c1=toupper(*orig);c2=toupper(*test);
			}

		if(c1!=c2)
			return(0);

		orig++;test++;
		}

	if(*test=='\0')
		return(1);

	return(0);
	}
		
void output_symbols_string(CURRENT_STATUS *cs, short dev)
	{
	
	char buffer[80];
	int tab_loop;
	list *nlp;

	for(tab_loop=0;tab_loop<tp_tab;tab_loop++)
		{
			sprintf(buffer," ");
			albf(buffer,dev);
		}


	sprintf(buffer,"Root note:       ");
	albf(buffer,dev);

	nlp=current_driver->root;
	while(nlp!=NULL)
		{
			sprintf(buffer,"%c",nlp->data);
			albf1(buffer,dev);
			nlp=nlp->right;
		}


	sprintf(buffer,"   Scale note:            ");
	albf(buffer,dev);

	nlp=current_driver->note;
	while(nlp!=NULL)
		{
			sprintf(buffer,"%c",nlp->data);
			albf1(buffer,dev);
			nlp=nlp->right;
		}

	nlp=current_driver->line_feed;
	while(nlp!=NULL)
		{
			sprintf(buffer,"%c",nlp->data);
			albf1(buffer,dev);
			nlp=nlp->right;
		}

	for(tab_loop=0;tab_loop<tp_tab;tab_loop++)
	{
	sprintf(buffer," ");
	albf(buffer,dev);
	}


	sprintf(buffer,"Root & Relative: ");
	albf(buffer,dev);

	nlp=current_driver->rootNrel;
	while(nlp!=NULL)
		{
			sprintf(buffer,"%c",nlp->data);
			albf1(buffer,dev);
			nlp=nlp->right;
		}

	sprintf(buffer,"   Scale & Relative note: ");
	albf(buffer,dev);

	nlp=current_driver->normNrel;
	while(nlp!=NULL)
		{
			sprintf(buffer,"%c",nlp->data);
			albf1(buffer,dev);
			nlp=nlp->right;
		}

	nlp=current_driver->line_feed;
	while(nlp!=NULL)
		{
			sprintf(buffer,"%c",nlp->data);
			albf1(buffer,dev);
			nlp=nlp->right;
		}

	for(tab_loop=0;tab_loop<tp_tab;tab_loop++)
	{
	sprintf(buffer," ");
	albf(buffer,dev);
	}


	sprintf(buffer,"Relative note:   ",tp_relative);
	albf(buffer,dev);

	nlp=current_driver->relative;
	while(nlp!=NULL)
		{
			sprintf(buffer,"%c",nlp->data);
			albf1(buffer,dev);
			nlp=nlp->right;
		}

	nlp=current_driver->line_feed;
	while(nlp!=NULL)
		{
			sprintf(buffer,"%c",nlp->data);
			albf1(buffer,dev);
			nlp=nlp->right;
		}

	}

void output_text(CURRENT_STATUS *cs, char *text, short dev)
	{
	
		char *tp,buffer[20];
		list *nlp;		


		while(*text!='\0')
			{
				switch(*text)
					{
					case '\\':
/*						sprintf(buffer,"[1][case %c][ok]",*text);
						form_alert(1,buffer);
*/
						tp=text;
						tp++;
					
/*						sprintf(buffer,"[1][case %c][ok]",*tp);
						form_alert(1,buffer);
*/
						if(*tp=='n')
							{

								nlp=current_driver->line_feed;
								while(nlp!=NULL)
								{
								sprintf(buffer,"%c",nlp->data);
								albf1(buffer,dev);
								nlp=nlp->right;
								}

							tp++;
							text=tp;
							}
						else
							{
/*							form_alert(1,"[1][Doing a back slash][ok]");*/
							sprintf(buffer,"\\");
							albf(buffer,dev);
							text++;
							}
						break;
					
					case '&':
						tp=text;
						tp++;
			
						if(is_string(tp,"key",0))
							{
							output_key_name(cs,dev);
							text+=4;
							}
						else{
								if(is_string(tp,"notes",0))
									{
									output_notes_string(cs,dev);
									text+=6;
									}
								else{
										if(is_string(tp,"name",0))
											{
											output_name_string(cs,dev);
											text+=5;
											}
										else{
											if(is_string(tp,"symbols",0))
												{
												output_symbols_string(cs,dev);
												text+=8;
												}
											else{
													albf("&",dev);
													text++;
												}
											}
									}
							}
					default:
						if(*text=='\\')
							{
							tp=text;
							tp++;
						
							if(*tp=='n')
								{

								nlp=current_driver->line_feed;
								while(nlp!=NULL)
								{
								sprintf(buffer,"%c",nlp->data);
								albf1(buffer,dev);
								nlp=nlp->right;
								}


								tp++;
								text=tp;
								}
							else
								{
/*								form_alert(1,"[1][Doing a back slash][ok]");
*/								sprintf(buffer,"\\");
								albf(buffer,dev);
								text++;
								}
	
							}
						else
							{
/*							sprintf(buffer,"[2][Doing a %c][ok]",*text);
							form_alert(1,buffer);
*/	
							sprintf(buffer,"%c",*text);
							albf1(buffer,dev);
							text++;
							}
						}
			}
	}

void print_as_text(CURRENT_STATUS *cs)
	{
	
		int loop1,loop2,tab_loop;
		int combination_note;
		int note_val;
/*		int max_note_len;*/
		char tb[200];
		short dev=0;
		char *frdsc;
		list *nlp;

/*	if(Cprnos()!=0)
		form_alert(1,"[1][printer not ready][ok]");
	else
		{*/

		output_header(cs,dev);

		for(loop1=0;loop1<STRINGS;loop1++)
			{

				for(tab_loop=0;tab_loop<tp_tab;tab_loop++)
				{
				sprintf(tb," ");
				albf(tb,dev);
				}
	
/*				sprintf(tb,"%d - ",loop1+1);
				albf(tb,dev);
*/

				for(loop2=tp_start_fret;loop2<tp_start_fret+tp_no_frets
												&& loop2<MAX_FRETS;loop2++)
					{


						combination_note=0;
						if(cs->display_fretboard[loop1][loop2]>RELATIVE_NOTE)
						{
							combination_note=1;
							note_val=cs->display_fretboard[loop1][loop2]-RELATIVE_NOTE;
						}
						else
						{
							note_val=cs->display_fretboard[loop1][loop2];
						}

/*						if(combination_note)
						{
						}
*/


						if(loop2==0)
							frdsc=current_driver->open_notes;
						else
							frdsc=current_driver->fret_descr;

						while(*frdsc!='\0')
						{
						switch(*frdsc)
						{
		
						case '|':
								nlp=current_driver->fret;
								if(loop1==0)
									nlp=current_driver->fret1;
								if(loop1==5)
									nlp=current_driver->fret6;
								break;
									
						case ' ':
								nlp=current_driver->space;
								break;

						case '-':
								nlp=current_driver->string;
								break;	
	
						case 'o':
						case 'O':					
							switch(note_val)
								{
									case NO_NOTE:
											nlp=current_driver->no_note; 
											break;
									case NORMAL_NOTE:
											if(combination_note)
												nlp=current_driver->normNrel;
											else	
												nlp=current_driver->note;
											break;
									case FILLED_NOTE: 
										if(combination_note)
											{
											nlp=current_driver->rootNrel;
											}
										else		 
											{
											nlp=current_driver->root;
												}
											break;
									case RELATIVE_NOTE: 
											nlp=current_driver->relative;
											break;
								}
								break;
						}
						while(nlp!=NULL)
							{
							sprintf(tb,"%c",nlp->data);
							albf1(tb,dev);
							nlp=nlp->right;
							}
						frdsc++;
						}

					}
		
			nlp=current_driver->line_feed;
			while(nlp!=NULL)
				{
					sprintf(tb,"%c",nlp->data);
					albf1(tb,dev);
					nlp=nlp->right;
				}

			}

		output_trailer(cs,dev);

	/*	}*/
	} 
