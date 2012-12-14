#define DEMO

#include "alscale.h"
#include "tuning.h"


/* dynamic memory allocation function */
void *allocation(int bytes)
	{ 
		void *al;
		char buf[60];

	/*	form_alert(1,"[1][Dynamic memory allocation in use][****]");
*/
		if((al=(void *)malloc(bytes))==NULL)
			{
			sprintf(buf,"NOT ENOUGH MEMORY - %d\n",bytes);
			fatal_error(buf);
/*			fprintf(stderr,"NOT ENOUGH MEMORY - %d\n",bytes);
				exit(1);
*/
			}

		return al;
	}




/* given a root note it fills the fretboard in standard tuning */
void set_frets(CURRENT_STATUS *cs )
	{
		int loop1,loop2,note_count=0;

		for(loop1=0;loop1<STRINGS;loop1++)
			{

				note_count=((NOTE *)(transpose_note(
												note_numbers[open_notes[loop1]],
												cs->root)
											)
								)->number;

			      /*	if(note_count<0)
					note_count=11; */

				for(loop2=0;loop2<global_frets;loop2++)
					{
						cs->current_fretboard[loop1][loop2]=
								note_numbers[note_count];

						note_count++;
						note_count = (note_count > 12) ? 1 : note_count;
					}
			}
	}

void print_frets(CURRENT_STATUS *cs,int frets)
	{
		int loop1,loop2;

		if(frets>global_frets)
			frets=global_frets;

		for(loop1=0;loop1<STRINGS;loop1++)
			{
				printf("%d - ",loop1+1);

				for(loop2=0;loop2<frets;loop2++)
					{
						printf("%s ",cs->current_fretboard[loop1][loop2]->note_name);
					}
				printf("\n");
			}

	}


void fill_current_notes(CURRENT_STATUS *cs)   /*uses current_scale * and current_notes */
	{
		fill_notes_from_scale(cs->current_scale,cs->current_notes,
					cs->key);
	}

/* return the relative note */
NOTE *get_relative(CURRENT_STATUS *cs, NOTE *np)
	{
		NOTE *np2;

		np2 = transpose_note( note_numbers[ 12 + (np->number) - 3 ], cs->key );

		return( np2 );
	}

/* set the relative notes on the fretboard */
void set_relative(CURRENT_STATUS *cs)
	{
	int nl,fl,sl;
	NOTE *np;

	for(nl=0; nl<12; nl++)
		{
			if( cs->backup_notes[ nl ] )
				{
				np = transpose_note( note_numbers[ 12 + nl - 3 ], cs->key );

				for( fl = 0; fl < MAX_FRETS; fl++ )
					{
					for( sl=0; sl < STRINGS; sl++ )
						{
						if(cs->current_fretboard [sl][fl] == np)
							{
							cs->display_fretboard [sl][fl] += RELATIVE_NOTE;
							}
						}
					}
				}
		}
	}

/* set up the display fretboard */
void set_display_frets( CURRENT_STATUS *cs )
	{
		int loop1,loop2;

		for(loop1=0;loop1<STRINGS;loop1++)
			{
				for(loop2=0;loop2<global_frets;loop2++)
					{
						if(cs->current_notes[cs->current_fretboard[loop1][loop2]->number])
							{
								if(((cs->current_fretboard[loop1][loop2])->number==cs->key->number) &&
								    odisplay_root==TRUE)
								    cs->display_fretboard[loop1][loop2]=FILLED_NOTE;
								else
								    cs->display_fretboard[loop1][loop2]=NORMAL_NOTE;
							}
						else
							cs->display_fretboard[loop1][loop2]=NO_NOTE;
					}
			}
		if(odisplay_relative)
			set_relative(cs);
	}


					
		

void print_display_frets(CURRENT_STATUS *cs /*int fretboard[][FRETS]*/)
	{
		int loop1,loop2;

		for(loop1=0;loop1<STRINGS;loop1++)
			{
				printf("%d - ",loop1);

				for(loop2=0;loop2<global_frets;loop2++)
					{
						switch(cs->display_fretboard[loop1][loop2])
							{
								case NO_NOTE: printf("-"); break;
								case NORMAL_NOTE: printf("o"); break;
								case FILLED_NOTE: printf("."); break;
								case RELATIVE_NOTE: printf("x"); break;
							}
					}
				printf("\n");
			}
	}

/*
	this would be used to build up the stave positions for the notes on
	the fret board for displaying the scale in musical notation.
	unused and doesn't take into account the different tunings.
*/
void fill_stave_array(int sfretboard[][MAX_FRETS],NOTE *fretboard[][MAX_FRETS])
	{
		int stave_positions[]={13,10,8,5,2,-1};
		int loop1,loop2,pos;

		for(loop1=0;loop1<STRINGS;loop1++)
			{
			pos=stave_positions[loop1];

			for(loop2=0;loop2<global_frets;loop2++)
				{
					if((fretboard[loop1][loop2])->note_name[1]!='#')
					       pos++;


					sfretboard[loop1][loop2]=pos;
				}
			}
	}

void seek_and_replace(char *cp,char s,char r)
	{
		while(*cp!=s)
			cp++;

		*cp=r;
	}

void ags(char *dest,FILE *fp)
	{
		char ch;

		while((ch=getc(fp))!='\n' && !feof(fp))
			{
			*dest=ch;
			dest++;
			}
		*dest='\0';
	 }

void convert_old_to_new(char *filename,char *filename2)
	{
		FILE *fp,*outfp;
		char /*buffer[200],*/target[3],note_buffer[100]/*,ch*/;
		int /*buf,*/loop;
		char *cp,*cp2,*buffer;
		SCALE al;

		buffer=Gtemp_string;

		if((fp=fopen(filename,"r"))==NULL)
			{
				sprintf(buffer,"error cannot open| %s ",filename);
				fatal_error(buffer);
			}

		if((outfp=fopen(filename2,"w"))==NULL)
			{
				sprintf(buffer,"error cannot create| %s",filename2);
				fatal_error(buffer);
			}

		ags(buffer,fp);
  /*		seek_and_replace(buffer,'\n','\0');
  */		printf("%s %d\n",buffer,strlen(buffer));

		write_header(outfp);


		while(!feof(fp))
			{
				ags(buffer,fp);
				printf("%s of length %d\n",buffer,strlen(buffer));

				if(strlen(buffer)!=0)
				{

				ags(note_buffer,fp);
				printf("%s\n",note_buffer);

				for(loop=0;loop<12;loop++)
					al.notes[loop]=0;

				cp=note_buffer;
				cp2=target;

				while(*cp!='\0')
					{
					while(*cp!='.')
						{
						*cp2=*cp;
						cp++;
						cp2++;
						}
					*cp2='\0';
					loop=0;

					while(loop<12 && strcmp(note_numbers[loop]->note_name,target)
						&& strcmp(note_numbers[loop]->note_name_alias,target))
						loop++;

					al.notes[loop]=1;

					while(*cp!=',')
						cp++;

					printf("- %s -",target);
					cp++;
					cp2=target;
					}
				list_notes(&al);
				fprintf(outfp,"%s\n",buffer);
				fwrite((char *)al.notes,sizeof(int),12,outfp);
				ags(buffer,fp);
				getchar();
				}
			  }


		fclose(fp);
		fclose(outfp);
	}


void save_tunings(NODE *known_tunings,char *filename)
	{

#ifdef DEMO
		OBJECT *objptr;
		int result;
			Crsrc_gaddr(R_TREE,NAG_SCREEN,&objptr);
			set_tedinfo(objptr,Please_Wait_Butt," OK ");
			result=handle_dialog(objptr,0,0);
			set_tedinfo(objptr,Please_Wait_Butt,"Please Wait");

#else
		FILE *fp;
/*		char buffer[200];
*/		int ret;

		if((fp=fopen(filename,"r"))!=NULL)
			{
			ret=form_alert(1,"[1][ The file already exists! ][ SAVE | CANCEL ]");
			if(ret==2)
				goto end_save;
			}
							
		if((fp=fopen(filename,"w"))==NULL)
			{
				sprintf(Gtemp_string,"error cannot open| %s |as output",filename);
				error(Gtemp_string);
			}

		write_header(fp);

		while(known_tunings)
			{
				fprintf(fp,"%s\n",((tuning *)known_tunings->data)->name);
				fwrite((char *)((tuning *)known_tunings->data)->notes,sizeof(int),6,fp);
				fwrite((char *)((tuning *)known_tunings->data)->octaves,sizeof(int),6,fp);
				known_tunings=known_tunings->right;
			}


	end_save:
		fclose(fp);
#endif
	}

NODE *read_tunings(NODE *known_tunings,char *filename)
	{
#ifdef DEMO
		OBJECT *objptr;
		int result;
			Crsrc_gaddr(R_TREE,NAG_SCREEN,&objptr);
			set_tedinfo(objptr,Please_Wait_Butt," OK ");
			result=handle_dialog(objptr,0,0);
			set_tedinfo(objptr,Please_Wait_Butt,"Please Wait");
#else
		FILE *fp;
		char check_header[6]/*,buffer[200]*/;
		tuning *sp;
		int total_tunings;
		NODE *np,*ksp,*np2;


		if((fp=fopen(filename,"r"))==NULL)
			{
			sprintf(Gtemp_string,"Could not open| %s",filename);
			error(Gtemp_string);
			return(known_tunings);
			}

		fread(check_header,1,6,fp);
		if(strcmp(check_header,Header))
			{
			fclose(fp);
			sprintf(Gtemp_string,"Not a valid tunings file");
			error(Gtemp_string);
			return(known_tunings);
			}

			
		if(known_tunings)
			{
			np2=known_tunings->right;
			np=known_tunings;

			while(np!=NULL)
				{
					free((char *)((tuning *)np->data)->name);
					free((tuning *)np->data);
					free(np);
					np=np2;
					if(np2!=NULL)
						np2=np2->right;
				}
			known_tunings=NULL;
			}
			

		ags(Gtemp_string,fp);
		sp=allocation(sizeof(tuning));
		sp->name=(char *)allocation(strlen(Gtemp_string)+1);
		strcpy(sp->name,Gtemp_string);


		fread((char *)sp->notes,sizeof(int),6,fp);
		fread((char *)sp->octaves,sizeof(int),6,fp);

		known_tunings=allocation(sizeof(NODE));
		known_tunings->left=NULL;
		known_tunings->right=NULL;
		known_tunings->data=sp;
		total_tunings=1;

		ksp=known_tunings;

		while(!feof(fp))
			{
			ags(Gtemp_string,fp);
			if(strlen(Gtemp_string)!=0)
			{
			sp=allocation(sizeof(tuning));
			sp->name=(char *)allocation(strlen(Gtemp_string)+1);
			strcpy(sp->name,Gtemp_string);

			fread((char *)sp->notes,sizeof(int),6,fp);
			fread((char *)sp->octaves,sizeof(int),6,fp);

			np=allocation(sizeof(NODE));
			np->left=ksp;
			np->data=sp;
			np->right=NULL;
			ksp->right=np;
			ksp=np;
			total_tunings++;

			}
			}

		fclose(fp);
#endif
		return(known_tunings);

	}

void save_scales(NODE *known_scales,char *filename)
	{
#ifdef DEMO
		OBJECT *objptr;
		int result;
			Crsrc_gaddr(R_TREE,NAG_SCREEN,&objptr);
			set_tedinfo(objptr,Please_Wait_Butt," OK ");
			result=handle_dialog(objptr,0,0);
			set_tedinfo(objptr,Please_Wait_Butt,"Please Wait");
#else
		FILE *fp;
/*		char buffer[200];
*/		int ret;

		if((fp=fopen(filename,"r"))!=NULL)
			{
			ret=form_alert(1,"[1][ The file already exists! ][ SAVE | CANCEL ]");
			if(ret==2)
				goto end_save;
			}
							
		if((fp=fopen(filename,"w"))==NULL)
			{
				sprintf(Gtemp_string,"error cannot open| %s |as output",filename);
				error(Gtemp_string);
			}

		write_header(fp);

		while(known_scales)
			{
				fprintf(fp,"%s\n",((SCALE *)known_scales->data)->name);
				fwrite((char *)((SCALE *)known_scales->data)->notes,sizeof(int),12,fp);
				known_scales=known_scales->right;
			}


	end_save:
		fclose(fp);
#endif
	}
		

NODE *read_scales(NODE *known_scales,char *filename)
	{
#ifdef DEMO
		OBJECT *objptr;
		int result;
			Crsrc_gaddr(R_TREE,NAG_SCREEN,&objptr);
			set_tedinfo(objptr,Please_Wait_Butt," OK ");
			result=handle_dialog(objptr,0,0);
			set_tedinfo(objptr,Please_Wait_Butt,"Please Wait");
#else
		FILE *fp;
		char check_header[6]/*,buffer[200]*/;
		SCALE *sp;
		NODE *np,*ksp,*np2;
/*		int buf;
*/
		if((fp=fopen(filename,"r"))==NULL)
			{
			sprintf(Gtemp_string,"Could not open| %s",filename);
			error(Gtemp_string);
			return(known_scales);
			}

		fread(check_header,1,6,fp);
		if(strcmp(check_header,Header))
			{
			fclose(fp);
			sprintf(Gtemp_string,"Not a valid scales file");
			error(Gtemp_string);
			return(known_scales);
			}

			
		if(known_scales)
			{
			np2=known_scales->right;
			np=known_scales;

			while(np!=NULL)
				{
					free((char *)((SCALE *)np->data)->name);
					free((SCALE *)np->data);
					free(np);
					np=np2;
					if(np2!=NULL)
						np2=np2->right;
				}
			known_scales=NULL;
			}
			

		ags(Gtemp_string,fp);
		sp=allocation(sizeof(SCALE));
		sp->name=(char *)allocation(strlen(Gtemp_string)+1);
		strcpy(sp->name,Gtemp_string);
/*		printf("%s\n",sp->name);
*/
		fread((char *)sp->notes,sizeof(int),12,fp);

		known_scales=allocation(sizeof(NODE));
		known_scales->left=NULL;
		known_scales->right=NULL;
		known_scales->data=sp;
		total_scales=1;

		ksp=known_scales;

		while(!feof(fp))
			{
			ags(Gtemp_string,fp);
			if(strlen(Gtemp_string)!=0)
			{
			sp=allocation(sizeof(SCALE));
			sp->name=(char *)allocation(strlen(Gtemp_string)+1);
			strcpy(sp->name,Gtemp_string);
	/*		printf("%s %s\n",sp->name,Gtemp_string);
*/
			fread((char *)sp->notes,sizeof(int),12,fp);

			np=allocation(sizeof(NODE));
			np->left=ksp;
			np->data=sp;
			np->right=NULL;
			ksp->right=np;
			ksp=np;
/*			printf("in node name is %s \n",((SCALE *)np->data)->name);
*/			total_scales++;
			/*getchar();*/
/*			list_notes(sp);
*/
			}
			}

		fclose(fp);
/*		printf("scales read %d",total_scales);
*/
#endif
		return(known_scales);
	}



int compare_scale_names(char *first,char *second)
	{
	char *cp1,*cp2;

	cp1=first;
	cp2=second;

	while(*cp1==' ')
		cp1++;

	while(*cp2==' ')
		cp2++;

	while(*cp1!='\0' && *cp2!='\0')
		{
			if(*cp1<*cp2)
				return(-1);
		
			if(*cp1>*cp2)
				return(1);

			cp1++;
			cp2++;

	while(*cp1==' ')
		cp1++;

	while(*cp2==' ')
		cp2++;
		}

	if(*cp2!='\0')
		return(-1);

	if(*cp1!='\0')
		return(1);
	
	return(0);
	}

void exchange_nodes(NODE *np1,NODE *np2)
	{
	SCALE *data=(SCALE *)np2->data;

	(SCALE *)np2->data=(SCALE *)np1->data;
	(SCALE *)np1->data=data;
	}

void sort_names(NODE *known_scales)
	{
		int exchanges=0;
		int local_finished=0;
		NODE *np=known_scales;
/*		int ret;
*/
		while(!local_finished)
			{
				exchanges=0;
				while(np->right!=NULL)
					{
					if(compare_scale_names(((SCALE *)np->data)->name,
						((SCALE *)((NODE *)np->right)->data)->name)==1)
						{
							exchange_nodes((NODE *)np,(NODE *)np->right);
							exchanges++;
						}
					np=np->right;
					}
				if(exchanges==0)
					local_finished=1;
				np=known_scales;
			}
	}

void list_notes(SCALE *scale)
	{
		int loop;

		for(loop=0;loop<12;loop++)
			if(scale->notes[loop])
				printf("%s ",note_numbers[loop]->note_name);
		printf("\n");
	}

void fill_notes_from_scale(SCALE *current_scale, int current_notes[],NOTE *key)
	{
		int pos,loop,diff=(note8.number)-(key->number);
		int addfret=12-diff;
		int offset;

		for(loop=0;loop<12;loop++)
			{
				offset=(note_numbers[loop]->number)+addfret;
				pos=(note_numbers[offset])->number;
				if(current_scale->notes[loop])
					current_notes[pos]=TRUE;
				else
					current_notes[pos]=FALSE;
			}
	}

void fill_notes_from_store(int backup_notes[], int current_notes[],NOTE *key)
	{
		int pos,loop,diff=(note8.number)-(key->number);
		int addfret=12-diff;
		int offset;

		for(loop=0;loop<12;loop++)
			{
				offset=(note_numbers[loop]->number)+addfret;
				pos=(note_numbers[offset])->number;
				if(backup_notes[loop])
					current_notes[pos]=TRUE;
				else
					current_notes[pos]=FALSE;
			}
	}

void current_notes_to_store(int backup_notes[], int current_notes[],NOTE *key)
	{
		int pos,loop,diff=(key->number)-(note8.number);
/*		int addfret=12+diff;*/
		int offset;

		if(diff<0)
			diff=diff*-1;

		for(loop=0;loop<12;loop++)
			{
				offset=(note_numbers[loop]->number)+diff;
				pos=(note_numbers[offset])->number;
				if(current_notes[loop])
					backup_notes[pos]=TRUE;
				else
					backup_notes[pos]=FALSE;
			}
	}

void copy_scales(int from[],int to[])
	{
		int loop;
		for(loop=0;loop<12;loop++)
			to[loop]=from[loop];
	}
