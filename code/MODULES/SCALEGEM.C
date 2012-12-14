/*#define ALTESTING*/
#define DEMO

#include "scalegem.h"
#include "time.h"
#include <osbind.h>
#include "almidi.h"
#include "ctorsc.h"
#include "tuning.h"
#include "paths.h"
#include "printer.h"
#include "keys.h"
#include <aes.h>



#define DEF_COLUMNS		80
#define DEF_START_FRET	0
#define DEF_TAB			5
#define DEF_NO_FRETS		12
#define DEF_HEADER1		"\\nScale: &name in &key\\n"
#define DEF_HEADER2		"Notes: &notes\\n\\n"
#define DEF_TRAILER1		"\\n&symbols\\n"
#define DEF_TRAILER2		""
#define DEF_ROOT			"."
#define DEF_ROOT_N_REL	":"
#define DEF_NORM			","
#define DEF_NORM_N_REL	";"
#define DEF_RELATIVE		"`"

#define MIN_TEXT_COLUMNS		20
#define MAX_TEXT_COLUMNS		132
#define MIN_TEXT_START_FRET	0
#define MAX_TEXT_START_FRET	24
#define MIN_TEXT_NO_FRETS		0
#define MAX_TEXT_NO_FRETS		24
#define MIN_TEXT_TAB				0
#define MAX_TEXT_TAB				80


int numbers_flag=1;
int key_name_flag=1;
int scale_name_flag=1;
int midi_sound_flag=1;
int channel=0;
int velocity=127;
int note_length=1;
int midi_note_off_flag=1;
int circle_flag=0;

int tp_columns=DEF_COLUMNS;
int tp_start_fret=DEF_START_FRET;
int tp_tab=DEF_TAB;
int tp_no_frets=DEF_NO_FRETS;
/* all 40 */
char tp_header1[40]=DEF_HEADER1;
char tp_header2[40]=DEF_HEADER2;
char tp_trailer1[40]=DEF_TRAILER1;
char tp_trailer2[40]=DEF_TRAILER2;
/* all 14 */
char tp_root_string[14]=DEF_ROOT;
char tp_root_n_rel[14]=DEF_ROOT_N_REL;
char tp_norm_string[14]=DEF_NORM;
char tp_norm_n_rel[14]=DEF_NORM_N_REL;
char tp_relative[14]=DEF_RELATIVE;
/* end of future globals */



void alcircle(int handle, int x, int y, int radius)
	{

	short pxyarray[8];

	if(circle_flag)
		v_circle(handle,x,y,radius);
	else
		{
		pxyarray[0]=x;
		pxyarray[1]=y-radius;
		pxyarray[2]=x+radius;
		pxyarray[3]=y;
		pxyarray[4]=x;
		pxyarray[5]=y+radius;
		pxyarray[6]=x-radius;
		pxyarray[7]=y;

		v_fillarea(handle,4,pxyarray);
		}
	}
		




int start_gem(void)
	{
	gem_on=1;
	appl_init();		/* start AES */
	handle=graf_handle(&char_height,&char_width,
					   &cell_height,&cell_width);	/* find AES handle */

	v_opnvwk(work_in,&handle,work_out);				/* open workstation */

/*	CrsrcEnable();*/

	if(!rsrc_load("scales.rsc"))
		{
		fatal_error("Can't load resource file");
/*		form_alert(1,"[3][Can't load resource file][ quit ]");
		exit(1);
*/		}

	/*get text attributes */
	vqt_attributes(handle,text_attributes);

	if(handle)
		{
	/*	v_clrwk(handle);	*/	/* clear workstation */

		Crsrc_gaddr(R_TREE,MENU1,&menu_ptr);
		menu_bar(menu_ptr,1);
		graf_mouse(ARROW,NULL);


		return 1;
		}
	else
		return 0;
	}
	
int stop_gem(void)
	{
	v_clsvwk(handle);				/* close workstation */

	menu_bar(menu_ptr,0);
	Crsrc_free();
	return appl_exit();				/* shutdown AES */
	}


void display_sizes_form(CURRENT_STATUS *cs)
{
/*	OBJECT *dlog;
	char width_buf[4],height_buf[4];
	int result;
	
	Crsrc_gaddr(R_TREE,FDisplay_sizes,&dlog);
	sprintf(width_buf,"%d",frets_length);
	set_tedinfo(dlog,FFret_width,width_buf);
	sprintf(height_buf,"%d",frets_height);
	set_tedinfo(dlog,FFret_height,height_buf);

	result=handle_dialog(dlog,FFret_width,0);
	if (result==FDisplaySizesOK)
	{
		get_tedinfo(dlog,FFret_width,width_buf);
		get_tedinfo(dlog,FFret_height,height_buf);
		result = stcd_i(width_buf,&frets_length);
		result = stcd_i(height_buf,&frets_height);

	}*/
}

void fret_numbers_form(CURRENT_STATUS *cs)
{
/*	OBJECT *dlog;
	char first_buf[4],inc_buf[4];
	int result;
	
	Crsrc_gaddr(R_TREE,FFretsNumbering,&dlog);
	sprintf(first_buf,"%d",first_fret);
	set_tedinfo(dlog,FFirstFretEdit,first_buf);
	sprintf(inc_buf,"%d",fret_inc);
	set_tedinfo(dlog,FIncrementEdit,inc_buf);

	result=handle_dialog(dlog,FFirstFretEdit,0);
	if (result==FNumberingOK)
	{
		get_tedinfo(dlog,FFirstFretEdit,first_buf);
		get_tedinfo(dlog,FIncrementEdit,inc_buf);
		result = stcd_i(first_buf,&first_fret);
		if(first_fret<0)
			first_fret=0;
		if(first_fret>global_frets-1)
			first_fret=global_frets-1;
		result = stcd_i(inc_buf,&fret_inc);
		if(fret_inc<1)
			fret_inc=1;
		if(fret_inc>global_frets-1)
			fret_inc=global_frets-1;
	}*/
}

void display_frets_form(CURRENT_STATUS *cs)
{
/*	OBJECT *dlog;
	char max_buf[4];
	int result;
	
	Crsrc_gaddr(R_TREE,FFretsOnDisplay,&dlog);
	sprintf(max_buf,"%d",global_frets-1);
	set_tedinfo(dlog,FFretsDisplaEdit,max_buf);

	result=handle_dialog(dlog,FFretsDisplaEdit,0);
	if (result==FFretsDisplayOK)
	{
		get_tedinfo(dlog,FFretsDisplaEdit,max_buf);
		result = stcd_i(max_buf,&global_frets);

		global_frets++;
		if(global_frets<2)
			global_frets=2;
		if(global_frets>MAX_FRETS)
			global_frets=MAX_FRETS;
	}*/
}

void display_options_form(CURRENT_STATUS *cs)
{
	OBJECT *dlog;
	int result;
/*	char text_buf[40];*/
	char num_buf[6];
	

	Crsrc_gaddr(R_TREE,FDisplayOptions,&dlog);

	sprintf(num_buf,"%d",frets_length);
	set_tedinfo(dlog,FFret_width,num_buf);
	sprintf(num_buf,"%d",frets_height);
	set_tedinfo(dlog,FFret_height,num_buf);
	sprintf(num_buf,"%d",first_fret);
	set_tedinfo(dlog,FFirstFretEdit,num_buf);
	sprintf(num_buf,"%d",fret_inc);
	set_tedinfo(dlog,FIncrementEdit,num_buf);
	sprintf(num_buf,"%d",global_frets-1);
	set_tedinfo(dlog,FFretsDisplaEdit,num_buf);


	if(odisplay_root)
		set_button( dlog, FMiscButtons, FRootButton );
	else
		unset_normal_button( dlog, FMiscButtons, FRootButton );

	if(odisplay_relative)
		set_normal_button( dlog, FMiscButtons, FRelativeButton );
	else
		unset_normal_button( dlog, FMiscButtons, FRelativeButton );


	if(numbers_flag)
		set_normal_button( dlog, FMiscButtons, FNumbersButton );
	else
		unset_normal_button( dlog, FMiscButtons, FNumbersButton );

	if(scale_name_flag)
		set_normal_button( dlog, FMiscButtons, FScaleNameButton );
	else
		unset_normal_button( dlog, FMiscButtons, FScaleNameButton );

	if(key_name_flag)
		set_normal_button( dlog, FMiscButtons, FKeyNameButton );
	else
		unset_normal_button( dlog, FMiscButtons, FKeyNameButton );


	/* Set Radio Buttons */

	if(text_notes)
		set_button( dlog, FNotesDisplayAs, FTextNotes );
	else
	{
		if(circle_flag)
			set_button( dlog, FNotesDisplayAs, FCircularButton );
		else
			set_button( dlog, FNotesDisplayAs, FSquareNotes );
	}


	result=handle_dialog(dlog,0,0);

	if (result==FOptionsOK)
	{
		get_tedinfo(dlog,FFirstFretEdit,num_buf);
		result = stcd_i(num_buf,&first_fret);
		if(first_fret<0)
			first_fret=0;
		if(first_fret>global_frets-1)
			first_fret=global_frets-1;

		get_tedinfo(dlog,FIncrementEdit,num_buf);
		result = stcd_i(num_buf,&fret_inc);
		if(fret_inc<1)
			fret_inc=1;
		if(fret_inc>global_frets-1)
			fret_inc=global_frets-1;

		get_tedinfo(dlog,FFretsDisplaEdit,num_buf);
		result = stcd_i(num_buf,&global_frets);

		global_frets++;
		if(global_frets<2)
			global_frets=2;
		if(global_frets>MAX_FRETS)
			global_frets=MAX_FRETS;

		get_tedinfo(dlog,FFret_width,num_buf);
		result = stcd_i(num_buf,&frets_length);
		get_tedinfo(dlog,FFret_height,num_buf);
		result = stcd_i(num_buf,&frets_height);

		/* test radio buttons */
		result = get_button( dlog, FNotesDisplayAs );

		circle_flag=0;
		text_notes=0;
		
		if(result==FTextNotes)
			text_notes=1;
		if(result==FCircularButton)
			circle_flag=1;

		result = get_normal_button( dlog, FMiscButtons, FScaleNameButton );
		if(result)
			scale_name_flag=1;
		else
			scale_name_flag=0;

		result = get_normal_button( dlog, FMiscButtons, FKeyNameButton );
		if(result)
			key_name_flag=1;
		else
			key_name_flag=0;

		result = get_normal_button( dlog, FMiscButtons, FNumbersButton );
		if(result)
			numbers_flag=1;
		else
			numbers_flag=0;

		result = get_normal_button( dlog, FMiscButtons, FRootButton );
		if(result)
			odisplay_root=1;
		else
			odisplay_root=0;

		result = get_normal_button( dlog, FMiscButtons, FRelativeButton );
		if(result)
			odisplay_relative=1;
		else
			odisplay_relative=0;

		set_display_frets(cs);

	}

}

short snd_chph[][]={
						/* the lowest a and a# are wrong because they have pitches
							of 4544 (17,192) and 4290 (16,194) but the soundchip
							only does 0 - 4095 !!! */
						{17,16,15,14,14,13,12,11,11,10,9,9},
						{8,8,7,7,7,6,6,5,5,5,4,4},
						{4,4,3,3,3,3,3,2,2,2,2,2},
						{2,2,1,1,1,1,1,1,1,1,1,1},
						{1,1,0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0,0,0},
						};
short snd_chpl[][]={
						{192,194,208,238,23,77,142,217,47,142,247,103},
						{224,97,232,119,11,166,71,236,151,71,251,179},
						{112,48,244,187,133,83,35,246,203,163,125,89},
						{56,24,250,221,194,169,145,123,101,81,62,44},
						{28,12,153,238,225,212,200,189,178,168,159,150},
						{142,134,126,119,112,106,100,94,89,85,79,75},
						{71,67,63,59,56,53,50,47,44,42,39,37}
						};

short chip_volume=15;


static void play_note(int note, int octave)
{
	char temp;
	clock_t note_stime;



	int midiplay_note;


/*start note */

	if(midi_sound_flag)
		{
		midiplay_note=note_numbers[note]->number+24-3+(octave*12);
		midi_note_on(channel,midiplay_note,velocity);
		}
	else
		{
		Giaccess(254,7+128);
		Giaccess(chip_volume,8+128);
		Giaccess(snd_chpl[octave][note],0+128);
		Giaccess(snd_chph[octave][note],1+128);
		}

	note_stime = clock();
	temp=0;

	while(((clock()-note_stime)/CLK_TCK) < note_length && temp ==0)
	{	
		if(Bconstat(2))
			temp=Bconin(2);
	}

	if(midi_sound_flag)
		{
		midiplay_note=note_numbers[note]->number+24-3+(octave*12);
		if(midi_note_off_flag)
			midi_note_off(channel,midiplay_note,velocity);
		else
			midi_note_off_vel(channel,midiplay_note);
		}
	else
		{
		Giaccess(255,7+128);
		}

}

void octave_form(CURRENT_STATUS *cs,int offset,int octaves[])
{
	OBJECT *dlog;
	int result;
	int loop;	
	int buttons[]={FOctavem3,
						FOctavem2,
						FOctavem1,
						FOctave0,
						FOctave1,
						FOctave2,
				 		FOctave3};

	Crsrc_gaddr(R_TREE,FOctave_form,&dlog);

	set_button( dlog, FOctaveBackdrop, buttons[octaves[offset]] );

	result=handle_dialog(dlog,0,0);

	if(result==FOctaveOK)
		{
		result=get_button(dlog,FOctaveBackdrop);

		for(loop=0;loop<7;loop++)
			{
			if(buttons[loop]==result)
				octaves[offset]=loop;
			}
		}

}



void midi_setup_form(void)
{
	OBJECT *dlog;
	int result,result2;
	char num_buf[6];
	

	Crsrc_gaddr(R_TREE,FMidiSetup,&dlog);

	sprintf(num_buf,"%d",channel+1);
	set_tedinfo(dlog,MidiChannelNum,num_buf);
	sprintf(num_buf,"%d",velocity);
	set_tedinfo(dlog,MidiVelocityNum,num_buf);

	if(midi_note_off_flag)
		set_button( dlog, MidiSetupRadios, MidiNoteOffRadio );
	else
		set_button( dlog, MidiSetupRadios, MidiVel0Radio );

	result=handle_dialog(dlog,0,0);
	if (result==MidiSetupOK)
	{

		get_tedinfo(dlog,MidiChannelNum,num_buf);
		result2 = stcd_i(num_buf,&channel);
	
		get_tedinfo(dlog,MidiVelocityNum,num_buf);
		result2 = stcd_i(num_buf,&velocity);

		result2=get_button( dlog, MidiSetupRadios);
		if(result2==MidiNoteOffRadio)
			midi_note_off_flag=1;
		else
			midi_note_off_flag=0;

		if(channel<0)
			channel=1;

		if(channel>16)
			channel=16;

		channel--;

		if(velocity<0)
			velocity=0;

		if(velocity>127)
			velocity=127;

	}

}

void chip_setup_form(void)
{
	OBJECT *dlog;
	int result,result2;
	char num_buf[6];
	
	Crsrc_gaddr(R_TREE,FChipVolume,&dlog);

	result=chip_volume;
	sprintf(num_buf,"%d",result);


	set_tedinfo(dlog,Fchipvol,num_buf);

	result=handle_dialog(dlog,0,0);
	if (result==chip_volumeOK)
	{

		get_tedinfo(dlog,Fchipvol,num_buf);
		result = stcd_i(num_buf,&result2);
		chip_volume=result2;	
	

		if(chip_volume<0)
			chip_volume=1;

		if(chip_volume>15)
			chip_volume=15;

	}

}

void note_length_form(void)
{
	OBJECT *dlog;
	int result,result2;
	char num_buf[6];
	

/*	form_alert(1,"[1][pos 1][ok]");*/
	Crsrc_gaddr(R_TREE,FNoteLength,&dlog);
/*	form_alert(1,"[1][pos 2][ok]");*/

	sprintf(num_buf,"%d",note_length);
/*	form_alert(1,"[1][pos 3][ok]");*/
	set_tedinfo(dlog,NoteLength,num_buf);
/*	form_alert(1,"[1][pos 4][ok]");*/

	result=handle_dialog(dlog,0,0);
	if (result==NoteLengthOK)
	{

		get_tedinfo(dlog,NoteLength,num_buf);
		result2 = stcd_i(num_buf,&note_length);
		

	}

}


NODE *store_tuning_form(CURRENT_STATUS *cs,/*NODE *known_tunings,*/int *notes,int *octaves/*,int *new_tuning*/)
{
	OBJECT *dlog;
	char edit_buf[66],title_buf[25];
	char *tp;
	NODE *np /* ,*name_exists_at,*notes_exist_at */ ;
	tuning *nt;
	int result /*,tuning_name_exists,tuning_notes_exist,store_result*/ ;
	int count=0;

/*
local_restart:
	store_result=0;
	tuning_name_exists=0;
	tuning_notes_exist=0;
	np=known_tunings;
	name_exists_at=NULL;
	notes_exist_at=NULL;	
*/

		Crsrc_gaddr(R_TREE,NAME_SCALE_SMALL,&dlog);
		tp=current_tuning->name;

		while(tp!='\0'&&count<36)
			{
			edit_buf[count]=*tp;
			tp++;
			count++;
			}

		edit_buf[count]='\0';
		set_tedinfo(dlog,FEditName,edit_buf);
		sprintf(title_buf,"%s","STORE TUNING");
		set_tedinfo(dlog,FScaleEntryTitle,title_buf);
		result=handle_dialog(dlog,FSmallScaleName,0);


	if (result==FSmallScaleOK)
	{

		get_tedinfo(dlog,FSmallScaleName,edit_buf);

	/* do clash code here */
	


/*	if(edit_buf[0]!=NULL)
		{

		while(np!=NULL)
			{
			result=compare_scale_names(((TUNING *)np->data)->name,edit_buf);
			if(result==0)
				{
				tuning_name_exists=1;
				name_exists_at=np;
				}
		
			for(loop1=0;loop1<12;loop1++)
				{
				if(((TUNING *)np->data)->notes[loop1]!=desired_notes[loop1])
					loop1=20;
				}

			if(loop1<20)
				{
				tuning_notes_exist=1;
				notes_exist_at=np;
				}

			np=np->right;
			}

		if(tuning_name_exists || tuning_notes_exist)
			{
					result=resolve_clash_form_small(name_exists_at,notes_exist_at);		
			}
		else
			store_result=store_tuning_with_this_name(cs,known_scales,scale_name);

		if(result==ClashRename || result==SmallClashRename)
			goto local_restart;

		if(result==ClashStoreIt || result==SmallClashStoreT)
		{
			if(scale_name_exists)
				store_result=store_tuning(cs,known_scales);
			else
				store_result=store_tuning_with_this_name(cs,known_scales,scale_name);
		}			
		}
*/
		np=known_tunings;

		if(np!=NULL)
			while(np->right!=NULL)
				np=np->right;

		tp=(char *)malloc(strlen(edit_buf)+1);
		nt=(tuning *)malloc(sizeof(tuning));

/*error("before tp!=NULL etc");*/

		if(tp!=NULL && nt!=NULL)
			{

			if(np==NULL)
			{
/*error("np==NULL");
if(known_tunings==NULL)
	error("known tunings null 1");*/

				np=(NODE *)malloc(sizeof(NODE));
				np->right=NULL;
				np->left=NULL;
				known_tunings=np;
	/*			*new_tuning=1;*/
/*if(known_tunings==NULL)
	error("known tunings null 2");*/
			}	
			else
			{
/*error("np!=NULL");*/
				np->right=(NODE *)malloc(sizeof(NODE));
				((NODE *)(np->right))->left=np;
				np=np->right;
				np->right=NULL;
			}

			nt->name=tp;
			strcpy(tp,edit_buf);
			memcpy(nt->notes,notes,6*sizeof(int));
			memcpy(nt->octaves,octaves,6*sizeof(int));

			np->data=(void *)nt;

			return np;
			}



	}

	return NULL;
}

void tunings_form(CURRENT_STATUS *cs)
{
	OBJECT *dlog;
	int open_notes_copy[6],octaves_copy[6];
/*	int flag;*/
	int loop;						
	int new_tuning;
	int result,lresult;
	char filename[FMSIZE];
	char text_buf[80];
/*	char short_buf[43];*/
	char c_buf[6];
	short x,y,w,h;
	int temp_midi_flag=midi_sound_flag;
	NODE *np=NULL;	
		OBJECT *objptr;

/*	char os[]="OCTAVE";
	char ps[]="PLAY";
*/	
	for(result=0; result<6;result++)
		{
		open_notes_copy[result]=current_tuning->notes[result];
		octaves_copy[result]=current_tuning->octaves[result];
		}

	Crsrc_gaddr(R_TREE,FTuning_form,&dlog);

	
	sprintf(c_buf," %s",note_numbers[open_notes_copy[0]]->note_name);
	set_tedinfo(dlog,FString1Name,c_buf);
	sprintf(c_buf," %s",note_numbers[open_notes_copy[1]]->note_name);
	set_tedinfo(dlog,FString2Name,c_buf);
	sprintf(c_buf," %s",note_numbers[open_notes_copy[2]]->note_name);
	set_tedinfo(dlog,FString3Name,c_buf);
	sprintf(c_buf," %s",note_numbers[open_notes_copy[3]]->note_name);
	set_tedinfo(dlog,FString4Name,c_buf);
	sprintf(c_buf," %s",note_numbers[open_notes_copy[4]]->note_name);
	set_tedinfo(dlog,FString5Name,c_buf);
	sprintf(c_buf," %s",note_numbers[open_notes_copy[5]]->note_name);
	set_tedinfo(dlog,FString6Name,c_buf);
	sprintf(text_buf," %s",current_tuning->name);

/*	sprintf(text_buf," %s",((tuning *)np->data)->name);
	set_tedinfo(dlog,FTuningName,text_buf);
*/
	
	set_tedinfo(dlog,FTuningName,text_buf);

/*	set_tedinfo(dlog,FOctaveName,os);
	set_tedinfo(dlog,TunerPlayButton,ps);
*/
	if(midi_sound_flag)
		set_normal_button( dlog, FTuningBackdrop, FTuningMidiRadio );
	else
		set_normal_button( dlog, FTuningBackdrop, FTuningChipRadio );

	form_center(dlog,&x,&y,&w,&h);
	form_dial(FMD_START,0,0,0,0,x,y,w,h);
	objc_draw(dlog,0,10,x,y,w,h);


	do{
		result=form_do(dlog,0);
 		result=result&0x7fff;

		switch(result)
		{
			case FTuningOK:
										break;
			case FTuningCANCEL:
										break;
			case FString1Right:	
					open_notes_copy[0]=(open_notes_copy[0]+1)%12;
					sprintf(c_buf," %s",note_numbers[open_notes_copy[0]]->note_name);
					set_tedinfo(dlog,FString1Name,c_buf);
					objc_draw(dlog,FString1Name,0,x,y,w,h);
					break;
			case FString1Left:	
					open_notes_copy[0]=(open_notes_copy[0]-1)%12;
					if(open_notes_copy[0]<0)open_notes_copy[0]=11;		
					sprintf(c_buf," %s",note_numbers[open_notes_copy[0]]->note_name);
					set_tedinfo(dlog,FString1Name,c_buf);
					objc_draw(dlog,FString1Name,0,x,y,w,h);
					break;

			case FString2Right:	
					open_notes_copy[1]=(open_notes_copy[1]+1)%12;
					sprintf(c_buf," %s",note_numbers[open_notes_copy[1]]->note_name);
					set_tedinfo(dlog,FString2Name,c_buf);
					objc_draw(dlog,FString2Name,0,x,y,w,h);
					break;
			case FString2Left:	
					open_notes_copy[1]=(open_notes_copy[1]-1)%12;
					if(open_notes_copy[1]<0)open_notes_copy[1]=11;		
					sprintf(c_buf," %s",note_numbers[open_notes_copy[1]]->note_name);
					set_tedinfo(dlog,FString2Name,c_buf);
					objc_draw(dlog,FString2Name,0,x,y,w,h);
					break;

			case FString3Right:	
					open_notes_copy[2]=(open_notes_copy[2]+1)%12;
					sprintf(c_buf," %s",note_numbers[open_notes_copy[2]]->note_name);
					set_tedinfo(dlog,FString3Name,c_buf);
					objc_draw(dlog,FString3Name,0,x,y,w,h);
					break;
			case FString3Left:	
					open_notes_copy[2]=(open_notes_copy[2]-1)%12;
					if(open_notes_copy[2]<0)open_notes_copy[2]=11;		
					sprintf(c_buf," %s",note_numbers[open_notes_copy[2]]->note_name);
					set_tedinfo(dlog,FString3Name,c_buf);
					objc_draw(dlog,FString3Name,0,x,y,w,h);
					break;

			case FString4Right:	
					open_notes_copy[3]=(open_notes_copy[3]+1)%12;
					sprintf(c_buf," %s",note_numbers[open_notes_copy[3]]->note_name);
					set_tedinfo(dlog,FString4Name,c_buf);
					objc_draw(dlog,FString4Name,0,x,y,w,h);
					break;
			case FString4Left:	
					open_notes_copy[3]=(open_notes_copy[3]-1)%12;
					if(open_notes_copy[3]<0)open_notes_copy[3]=11;		
					sprintf(c_buf," %s",note_numbers[open_notes_copy[3]]->note_name);
					set_tedinfo(dlog,FString4Name,c_buf);
					objc_draw(dlog,FString4Name,0,x,y,w,h);
					break;

			case FString5Right:	
					open_notes_copy[4]=(open_notes_copy[4]+1)%12;
					sprintf(c_buf," %s",note_numbers[open_notes_copy[4]]->note_name);
					set_tedinfo(dlog,FString5Name,c_buf);
					objc_draw(dlog,FString5Name,0,x,y,w,h);
					break;
			case FString5Left:	
					open_notes_copy[4]=(open_notes_copy[4]-1)%12;
					if(open_notes_copy[4]<0)open_notes_copy[4]=11;		
					sprintf(c_buf," %s",note_numbers[open_notes_copy[4]]->note_name);
					set_tedinfo(dlog,FString5Name,c_buf);
					objc_draw(dlog,FString5Name,0,x,y,w,h);
					break;

			case FString6Right:	
					open_notes_copy[5]=(open_notes_copy[5]+1)%12;
					sprintf(c_buf," %s",note_numbers[open_notes_copy[5]]->note_name);
					set_tedinfo(dlog,FString6Name,c_buf);
					objc_draw(dlog,FString6Name,0,x,y,w,h);
					break;
			case FString6Left:	
					open_notes_copy[5]=(open_notes_copy[5]-1)%12;
					if(open_notes_copy[5]<0)open_notes_copy[5]=11;		
					sprintf(c_buf," %s",note_numbers[open_notes_copy[5]]->note_name);
					set_tedinfo(dlog,FString6Name,c_buf);
					objc_draw(dlog,FString6Name,0,x,y,w,h);
					break;

			case FString1Octave:
					octave_form(cs,0,octaves_copy);
					objc_draw(dlog,0,10,x,y,w,h);
					break;

			case FString2Octave:
					octave_form(cs,1,octaves_copy);
					objc_draw(dlog,0,10,x,y,w,h);
					break;

			case FString3Octave:
					octave_form(cs,2,octaves_copy);
					objc_draw(dlog,0,10,x,y,w,h);
					break;

			case FSrtring4Octave:
					octave_form(cs,3,octaves_copy);
					objc_draw(dlog,0,10,x,y,w,h);
					break;

			case FString5Octave:
					octave_form(cs,4,octaves_copy);
					objc_draw(dlog,0,10,x,y,w,h);
					break;

			case FString6Octave:
					octave_form(cs,5,octaves_copy);
					objc_draw(dlog,0,10,x,y,w,h);
					break;
					
			case FString1Play: 
					lresult = get_normal_button( dlog, FTuningBackdrop, FTuningMidiRadio );
					if(lresult)
						midi_sound_flag=1;
					else
						midi_sound_flag=0;
					play_note(open_notes_copy[0],octaves_copy[0]);
					break;

			case FString2Play: 
					lresult = get_normal_button( dlog, FTuningBackdrop, FTuningMidiRadio );
					if(lresult)
						midi_sound_flag=1;
					else
						midi_sound_flag=0;
					play_note(open_notes_copy[1],octaves_copy[1]);
					break;

			case FString3Play: 
					lresult = get_normal_button( dlog, FTuningBackdrop, FTuningMidiRadio );
					if(lresult)
						midi_sound_flag=1;
					else
						midi_sound_flag=0;
					play_note(open_notes_copy[2],octaves_copy[2]);
					break;

			case FString4Play: 
					lresult = get_normal_button( dlog, FTuningBackdrop, FTuningMidiRadio );
					if(lresult)
						midi_sound_flag=1;
					else
						midi_sound_flag=0;
					play_note(open_notes_copy[3],octaves_copy[3]);
					break;

			case FString5Play: 
					lresult = get_normal_button( dlog, FTuningBackdrop, FTuningMidiRadio );
					if(lresult)
						midi_sound_flag=1;
					else
						midi_sound_flag=0;
					play_note(open_notes_copy[4],octaves_copy[4]);
					break;

			case FString6Play: 
					lresult = get_normal_button( dlog, FTuningBackdrop, FTuningMidiRadio );
					if(lresult)
						midi_sound_flag=1;
					else
						midi_sound_flag=0;
					play_note(open_notes_copy[5],octaves_copy[5]);
					break;

			case MidiSetupButton:
					midi_setup_form();
					objc_draw(dlog,0,10,x,y,w,h);
					break;
		
			case FChipSetup:
					chip_setup_form();
					objc_draw(dlog,0,10,x,y,w,h);
					break;

			case FTuningLOAD:
#ifdef DEMO
			Crsrc_gaddr(R_TREE,NAG_SCREEN,&objptr);
			set_tedinfo(objptr,Please_Wait_Butt," OK ");
			lresult=handle_dialog(objptr,0,0);
			set_tedinfo(objptr,Please_Wait_Butt,"Please Wait");
			objc_draw(dlog,0,10,x,y,w,h);
#else
				lresult=select("TUN",filename,"LOAD TUNINGS");
				if(lresult)
				{
					known_tunings=read_tunings(known_tunings,filename);
					current_tuning=(tuning *)known_tunings->data;

					for(loop=0; loop<6;loop++)
					{
					open_notes_copy[result]=current_tuning->notes[result];
					octaves_copy[result]=current_tuning->octaves[result];
					}

				}
				objc_draw(dlog,0,10,x,y,w,h);
#endif
				break;

			case FTuningSAVE:
#ifdef DEMO
			Crsrc_gaddr(R_TREE,NAG_SCREEN,&objptr);
			set_tedinfo(objptr,Please_Wait_Butt," OK ");
			lresult=handle_dialog(objptr,0,0);
			set_tedinfo(objptr,Please_Wait_Butt,"Please Wait");
			objc_draw(dlog,0,10,x,y,w,h);
#else
				lresult=select("TUN",filename,"SAVE TUNINGS");
				if(lresult)
					save_tunings(known_tunings,filename);
				
				objc_draw(dlog,0,10,x,y,w,h);
#endif
				break;

			case FTuningSTORE:
/*if(known_tunings==NULL)
	error("known tunings==NULL");
*/					new_tuning=0;
					np=store_tuning_form(cs,/*known_tunings,*/open_notes_copy,octaves_copy/*,&new_tuning*/);

					if(new_tuning==1)
						known_tunings=np;

/*if(known_tunings==NULL)
	error("known tunings still NULL");
*/
					sprintf(c_buf," %s",note_numbers[open_notes_copy[0]]->note_name);
					set_tedinfo(dlog,FString1Name,c_buf);
					sprintf(c_buf," %s",note_numbers[open_notes_copy[1]]->note_name);
					set_tedinfo(dlog,FString2Name,c_buf);
					sprintf(c_buf," %s",note_numbers[open_notes_copy[2]]->note_name);
					set_tedinfo(dlog,FString3Name,c_buf);
					sprintf(c_buf," %s",note_numbers[open_notes_copy[3]]->note_name);
					set_tedinfo(dlog,FString4Name,c_buf);
					sprintf(c_buf," %s",note_numbers[open_notes_copy[4]]->note_name);
					set_tedinfo(dlog,FString5Name,c_buf);
					sprintf(c_buf," %s",note_numbers[open_notes_copy[5]]->note_name);
					set_tedinfo(dlog,FString6Name,c_buf);
				if(np==NULL)
				{
					sprintf(text_buf," %s",current_tuning->name);
					set_tedinfo(dlog,FTuningName,text_buf);
				}
				else
				{
					sprintf(text_buf," %s",((tuning *)np->data)->name);
					set_tedinfo(dlog,FTuningName,text_buf);
				}
	/*				sprintf(text_buf,"OCTAVE");
					set_tedinfo(dlog,FOctaveName,text_buf);
					sprintf(text_buf,"PLAY");
					set_tedinfo(dlog,TunerPlayButton,text_buf);
*/
					objc_draw(dlog,0,10,x,y,w,h);
					break;

			case FTuningRESTORE:
					np=select_tuning_window(cs,known_tunings);
					if(np!=NULL)
					{
						

						for(loop=0;loop<6;loop++)
						{
							open_notes_copy[loop]=((tuning *)np->data)->notes[loop];
							octaves_copy[loop]=((tuning *)np->data)->octaves[loop];
						}

					sprintf(c_buf," %s",note_numbers[open_notes_copy[0]]->note_name);
					set_tedinfo(dlog,FString1Name,c_buf);
					sprintf(c_buf," %s",note_numbers[open_notes_copy[1]]->note_name);
					set_tedinfo(dlog,FString2Name,c_buf);
					sprintf(c_buf," %s",note_numbers[open_notes_copy[2]]->note_name);
					set_tedinfo(dlog,FString3Name,c_buf);
					sprintf(c_buf," %s",note_numbers[open_notes_copy[3]]->note_name);
					set_tedinfo(dlog,FString4Name,c_buf);
					sprintf(c_buf," %s",note_numbers[open_notes_copy[4]]->note_name);
					set_tedinfo(dlog,FString5Name,c_buf);
					sprintf(c_buf," %s",note_numbers[open_notes_copy[5]]->note_name);
					set_tedinfo(dlog,FString6Name,c_buf);
					sprintf(text_buf," %s",((tuning *)np->data)->name);
					set_tedinfo(dlog,FTuningName,text_buf);
					sprintf(text_buf,"OCTAVE");
					set_tedinfo(dlog,FOctaveName,text_buf);
					sprintf(text_buf,"PLAY");
					set_tedinfo(dlog,TunerPlayButton,text_buf);

					}
					objc_draw(dlog,0,10,x,y,w,h);
					break;

			case BTuningDelete:
					np=delete_tuning_window(cs,known_tunings);

					if(np!=NULL)
					{
						
					/* first one */
					if(((NODE *)np->left)==NULL)
						{

						known_tunings=(NODE *)np->right;
						if(np->right)
							((NODE *)(np->right))->left=NULL;
	
						}
					else
						{
						/* last one */
						if((NODE *)(np->right)!=NULL)
							((NODE *)(np->right))->left=np->left;

						((NODE *)(np->left))->right=np->right;

						}
						free(np->data);
						free(np);
						
					}
					objc_draw(dlog,0,10,x,y,w,h);
					break;

			case TunerPlayButton:
			case TunnerNoteButton:
					note_length_form();
					objc_draw(dlog,0,10,x,y,w,h);
					break;

		}
		
		dlog[result].ob_state&=~SELECTED;	/* de-select exit button */
		objc_draw(dlog,result,0,x,y,w,h);
		}while(result!=FTuningOK && result !=FTuningCANCEL);

	form_dial(FMD_FINISH,0,0,0,0,x,y,w,h);

	midi_sound_flag=temp_midi_flag;
	if (result==FTuningOK)
	{

		for( result=0; result<6; result++)
		{
		open_notes[result]=open_notes_copy[result];
		octaves[result]=octaves_copy[result];
		}

		if(np!=NULL)
			current_tuning=(tuning *)np->data;
		result = get_normal_button( dlog, FTuningBackdrop, FTuningMidiRadio );
		if(result)
			midi_sound_flag=1;
		else
			midi_sound_flag=0;

		set_frets(cs);
		set_display_frets(cs);

	}

}

void display_text_print_form(CURRENT_STATUS *cs)
{
	OBJECT *dlog,*objptr;
	int loop,result,result2;
	char text_buf[41];
	char num_buf[6];
	int lresult;
	char filename[FMSIZE];
	FILE *fp;
int ret;
	

	Crsrc_gaddr(R_TREE,FPrintAsText,&dlog);



/*	sprintf(num_buf,"%d",tp_columns);
	set_tedinfo(dlog,FColumns,num_buf);*/
	sprintf(num_buf,"%d",tp_start_fret);
	set_tedinfo(dlog,FStartFret,num_buf);
	sprintf(num_buf,"%d",tp_no_frets);
	set_tedinfo(dlog,FFrets,num_buf);
	sprintf(num_buf,"%d",tp_tab);
	set_tedinfo(dlog,FTabs,num_buf);

	set_tedinfo(dlog,FTrailer1,tp_trailer1);
	set_tedinfo(dlog,FTrailer2,tp_trailer2);
	set_tedinfo(dlog,FHeader1,tp_header1);
	set_tedinfo(dlog,FHeader2,tp_header2);



	for(loop=0;loop<40 && current_driver->name[loop]!='\0'; loop++)
	{
		text_buf[loop]=current_driver->name[loop];
	}
		text_buf[loop]='\0';

	set_tedinfo(dlog,PrintDriverName,text_buf);

/*	set_tedinfo(dlog,FRootString,tp_root_string);
	set_tedinfo(dlog,FNormString,tp_norm_string);
	set_tedinfo(dlog,FRelRootString,tp_root_n_rel);
	set_tedinfo(dlog,FRelNormString,tp_norm_n_rel);
	set_tedinfo(dlog,FRelString,tp_relative);
*/

do{

	result=handle_dialog(dlog,FHeader1,0);


	if(result==PrintDriverName)
	{
		load_new_driver();
		for(loop=0;loop<40 && current_driver->name[loop]!='\0'; loop++)
		{
			text_buf[loop]=current_driver->name[loop];
		}
			text_buf[loop]='\0';

		set_tedinfo(dlog,PrintDriverName,text_buf);
	}	


}
while(result==PrintDriverName);

	if (result==FTextPrintPrint || result==FTextPrintToDisk)
	{
		get_tedinfo(dlog,FTrailer1,tp_trailer1);
		get_tedinfo(dlog,FTrailer2,tp_trailer2);
		get_tedinfo(dlog,FHeader1,tp_header1);
		get_tedinfo(dlog,FHeader2,tp_header2);

/*		get_tedinfo(dlog,FRootString,tp_root_string);
		get_tedinfo(dlog,FNormString,tp_norm_string);
		get_tedinfo(dlog,FRelRootString,tp_root_n_rel);
		get_tedinfo(dlog,FRelNormString,tp_norm_n_rel);
		get_tedinfo(dlog,FRelString,tp_relative);
*/

/*		get_tedinfo(dlog,FColumns,num_buf);
		result2 = stcd_i(num_buf,&tp_columns);*/
		get_tedinfo(dlog,FStartFret,num_buf);
		result2 = stcd_i(num_buf,&tp_start_fret);
		get_tedinfo(dlog,FFrets,num_buf);
		result2 = stcd_i(num_buf,&tp_no_frets);
		get_tedinfo(dlog,FTabs,num_buf);
		result2 = stcd_i(num_buf,&tp_tab);

/*		if(tp_columns<MIN_TEXT_COLUMNS)
			tp_columns=MIN_TEXT_COLUMNS;
		if(tp_columns>MAX_TEXT_COLUMNS)
			tp_columns=MAX_TEXT_COLUMNS;
*/

		if(tp_start_fret<MIN_TEXT_START_FRET)
			tp_start_fret=MIN_TEXT_START_FRET;
		if(tp_start_fret>MAX_TEXT_START_FRET)
			tp_start_fret=MAX_TEXT_START_FRET;

		if(tp_no_frets<MIN_TEXT_NO_FRETS)
			tp_no_frets=MIN_TEXT_NO_FRETS;
		if(tp_no_frets>MAX_TEXT_NO_FRETS)
			tp_no_frets=MAX_TEXT_NO_FRETS;

		if(tp_tab<MIN_TEXT_TAB)
			tp_tab=MIN_TEXT_TAB;
		if(tp_tab>MAX_TEXT_TAB)
			tp_tab=MAX_TEXT_TAB;


		if(result==FTextPrintToDisk)
			{
				lresult=select("DPF",filename,"PRINT TO FILE");
				if(lresult)
					{

					if((fp=fopen(filename,"r"))!=NULL)
						{
						ret=form_alert(1,"[1][ The file already exists! ][Overwrite|Cancel]");
						if(ret==2)
							goto end_save;
						fclose(fp);
						}
						

					if((fp=fopen(filename,"w"))==NULL)
						{
							sprintf(Gtemp_string,"error cannot open| %s |as output",filename);
							error(Gtemp_string);
						}

					gprint_to_disk=(pto_disk_data *)malloc(sizeof(pto_disk_data));
					if(gprint_to_disk==NULL)
						{
						error("Could not allocate memory");
						goto end_save;
						}

					gprint_to_disk->filename=filename;
					gprint_to_disk->output_file=fp;

					print_as_text(cs);

					}

end_save:
		fclose(fp);

		free(gprint_to_disk);
		gprint_to_disk=NULL;
			}

#ifdef DEMO
			Crsrc_gaddr(R_TREE,NAG_SCREEN,&objptr);
			set_tedinfo(objptr,Please_Wait_Butt," OK ");
			result=handle_dialog(objptr,0,0);
			set_tedinfo(objptr,Please_Wait_Butt,"Please Wait");
#else
		if(result==FTextPrintPrint)
			print_as_text(cs);
#endif
	}

}
void rename_form(CURRENT_STATUS *cs,NODE *known_scales)
{
	OBJECT *dlog;
	char edit_buf[66],title_buf[25];
	char *tp;
	int result;
	int count=0;

	if(desk_x<330)
		{
		Crsrc_gaddr(R_TREE,NAME_SCALE_SMALL,&dlog);
		tp=cs->current_scale->name;

		while(tp!='\0'&&count<36)
			{
			edit_buf[count]=*tp;
			tp++;
			count++;
			}
		edit_buf[count]='\0';
		set_tedinfo(dlog,FEditName,edit_buf);
		sprintf(title_buf,"%s","RENAME SCALE");
		set_tedinfo(dlog,FScaleEntryTitle,title_buf);
		result=handle_dialog(dlog,FSmallScaleName,0);
	}
else
	{
	Crsrc_gaddr(R_TREE,NAME_SCALE,&dlog);
	sprintf(edit_buf,"%s",cs->current_scale->name);
	set_tedinfo(dlog,FEditName,edit_buf);
	sprintf(title_buf,"%s","RENAME SCALE");
	set_tedinfo(dlog,FScaleEntryTitle,title_buf);
	result=handle_dialog(dlog,FEditName,0);
	}

	if (result==FEditScaleOK || result==FSmallScaleOK)
	{
		if(desk_x>330)
			get_tedinfo(dlog,FEditName,edit_buf);
		else
			get_tedinfo(dlog,FSmallScaleName,edit_buf);
	
		tp=(char *)malloc(strlen(edit_buf)+1);
		if(tp!=NULL)
			{
			free(cs->current_scale->name);
			cs->current_scale->name=tp;
			strcpy(cs->current_scale->name,edit_buf);
			}

	}
}

char *get_name_of_scale_form(CURRENT_STATUS *cs,NODE *known_scales)
{
	OBJECT *dlog;
	char edit_buf[66],title_buf[25];
	char *tp;
	int count=0;
	int result;

	
	if(desk_x<330)
		{
		Crsrc_gaddr(R_TREE,NAME_SCALE_SMALL,&dlog);
		tp=cs->current_scale->name;

		while(tp!='\0'&&count<36)
			{
			edit_buf[count]=*tp;
			tp++;
			count++;
			}
		edit_buf[count]='\0';

		set_tedinfo(dlog,FSmallScaleName,edit_buf);
		sprintf(title_buf,"%s","STORE SCALE AS ..");
		set_tedinfo(dlog,FSmallScaleTitle,title_buf);
		result=handle_dialog(dlog,FSmallScaleName,0);
		}
	else
		{
		Crsrc_gaddr(R_TREE,NAME_SCALE,&dlog);
		sprintf(edit_buf,"%s",cs->current_scale->name);
		set_tedinfo(dlog,FEditName,edit_buf);
		sprintf(title_buf,"%s","STORE SCALE AS ..");
		set_tedinfo(dlog,FScaleEntryTitle,title_buf);
		result=handle_dialog(dlog,FEditName,0);
		}

	if (result==FEditScaleOK || result==FSmallScaleOK)
	{
		if(desk_x>330)
			get_tedinfo(dlog,FEditName,edit_buf);
		else
			get_tedinfo(dlog,FSmallScaleName,edit_buf);
	
		tp=(char *)malloc(strlen(edit_buf)+1);
		if(tp!=NULL)
			strcpy(tp,edit_buf);
		return(tp);
	}
	return(NULL);
}


void note_on_fretboard_form(CURRENT_STATUS *cs,NODE *known_scales,
						  	int fret, int string)
{

	char string_buf[2],fret_buf[3],note_buf[6];
	OBJECT *dlog;
	int result;
	int loop;
	NOTE *np=cs->current_fretboard[string][fret];
	int rb;
	int remloop;
				
	Crsrc_gaddr(R_TREE,FNoteOnFretboard,&dlog);

	sprintf(string_buf,"%d",string+1);

	set_tedinfo(dlog,FNoteOnStringNum,string_buf);



	sprintf(fret_buf,"%d",fret);
	set_tedinfo(dlog,FNoteOnFretNum,fret_buf);



	if(np->note_name_alias[0]==' ')
		sprintf(note_buf,"  %c  ",np->note_name[0]);
	else
		sprintf(note_buf,"%s %s",np->note_name,np->note_name_alias);

	set_tedinfo(dlog,FNoteOnFretName,note_buf);



	for(loop=0;loop<12;loop++)
		{
			if(note_numbers[loop]==np)
				{
				remloop = loop;
				if( cs->current_notes[ loop ] )
					set_button( dlog, ParentRadioNotes, FNoteInScaleTick );
				else
					set_button( dlog, ParentRadioNotes, FNoteInScaleCros );
				}
		}


	result = handle_dialog( dlog, 0, 0 );
	rb 	 = get_button( dlog, ParentRadioNotes );

	if(result!=FNoteOnFretCancl)
		{
		if(rb==FNoteInScaleTick)
			{
				cs->current_notes[ remloop ] = TRUE;
				set_scales_ticks( cs->current_notes );
				current_notes_to_store(	cs->backup_notes,
												cs->current_notes,
												cs->key);
				set_display_frets( cs );
				cs->changed_scale = TRUE;
			}
		else
			{
				cs->current_notes[ remloop ] = FALSE;
				set_scales_ticks( cs->current_notes );
				current_notes_to_store(	cs->backup_notes,
												cs->current_notes,
												cs->key);
				set_display_frets( cs );
				cs->changed_scale = TRUE;
			}
		}
}
	
void mouse_click_on_fret(ALWINDOW *alw, CURRENT_STATUS *cs, NODE *known_scales, int mousex,int mousey)
	{
	int loop1,loop2;
	int cf;
	int curx,cury;


	scale_offsets(alw,&frets_x_offset,&frets_y_offset);
	frets_y_offset=frets_y_offset+big_y_offset;

	wclip(alw);
	for(loop2=0;loop2<global_frets && loop2+start_fret<global_frets;loop2++)
		{
		for(loop1=0;loop1<6;loop1++)
			{
				cf=loop2+start_fret;

				curx=(loop2*frets_length);
				cury=(loop1*frets_height);


				if(mousex>curx+frets_x_offset  &&
				   mousex<curx+frets_length+frets_x_offset)
					{
					if(mousey>cury+frets_y_offset &&
					   mousey<cury+frets_y_offset+frets_height)
							{
							/*form_alert(1,"[1][before call][ok]");*/
							note_on_fretboard_form(cs,known_scales,cf,loop1);

							}
					}
			}

		}
	
	}

void mouse_double_click_on_fret(ALWINDOW *alw, CURRENT_STATUS *cs, NODE *known_scales, int mousex,int mousey)
	{
	int loop1,loop2;
	int cf;
	int curx,cury;


	scale_offsets(alw,&frets_x_offset,&frets_y_offset);
	frets_y_offset=frets_y_offset+big_y_offset;

	wclip(alw);
	for(loop2=0;loop2<global_frets && loop2+start_fret<global_frets;loop2++)
		{
		for(loop1=0;loop1<6;loop1++)
			{
				cf=loop2+start_fret;

				curx=(loop2*frets_length);
				cury=(loop1*frets_height);


				if(mousex>curx+frets_x_offset  &&
				   mousex<curx+frets_length+frets_x_offset)
					{
					if(mousey>cury+frets_y_offset &&
					   mousey<cury+frets_y_offset+frets_height)
							{
							/*form_alert(1,"[1][before call][ok]");*/
							note_on_fretboard_do(cs,known_scales,cf,loop1);

							}
					}
			}

		}
	
	}



void note_on_fretboard_do(CURRENT_STATUS *cs,NODE *known_scales,
						  	int fret, int string)
{

	int loop;
	NOTE *np=cs->current_fretboard[string][fret];
	int remloop;
				
	for(loop=0;loop<12;loop++)
		{
			if(note_numbers[loop]==np)
				{
				remloop = loop;
				}
		}

		cs->current_notes[ remloop ] ^= TRUE;
		set_scales_ticks( cs->current_notes );
		current_notes_to_store(	cs->backup_notes,
										cs->current_notes,
										cs->key);
		set_display_frets( cs );
		specific_note_draw(&scale_window,cs,remloop);
		if(odisplay_relative)
			specific_note_draw(&scale_window,cs,((NOTE *)get_relative(cs,note_numbers[remloop]))->number);
		cs->changed_scale = TRUE;
}


int store_scale(CURRENT_STATUS *cs, NODE *known_scales)
{
	int result;

	result=form_alert(1,"[2][ | | This will overwrite the | current version of the scale | held in memory ][Store|Cancel]");

	
	if(result==1)
		current_notes_to_store(cs->current_scale->notes,cs->current_notes,cs->key);

	return( (result==2) ? 0 : 1 );

}


int store_scale_with_this_name(CURRENT_STATUS *cs,
											NODE *known_scales,
											char *scale_name)
{
	SCALE *sp;
	NODE *np,*new_node;
	
	sp=(SCALE *)allocation(sizeof(SCALE));
	sp->name=(char *)allocation(strlen(scale_name)+1);
	strcpy(sp->name,scale_name);

	current_notes_to_store(sp->notes,
								  cs->current_notes,cs->key);


if(known_scales)
	{
	np=known_scales;
	while(np->right!=NULL)
	np=np->right;
			
	new_node=(NODE *)allocation(sizeof(NODE));
	new_node->left=np;
	new_node->data=sp;
	new_node->right=NULL;
	np->right=new_node;

	total_scales++;
	}
else
	{
	new_node=(NODE *)allocation(sizeof(NODE));
	new_node->left=NULL;
	new_node->data=sp;
	new_node->right=NULL;
	known_scales=new_node;

	total_scales=1;
	}

	if(osort)
		sort_names(known_scales);
	
	cs->current_scale=(SCALE *)sp;
	cs->changed_scale=FALSE;
	copy_scales(cs->current_scale->notes,cs->backup_notes);
	set_frets(cs);
	fill_notes_from_store(cs->backup_notes,cs->current_notes,cs->key);
	set_scales_ticks(cs->current_notes);
	set_display_frets(cs);

	return(1);		
}



int resolve_clash_form_small(NODE *name, NODE *notes)
	{
	OBJECT *dlog;
	char name_buf[40],notes_buf[40];
	char *tp;
	int count=0;
	int result;
	
	Crsrc_gaddr(R_TREE,Clash_Resolve_Sm,&dlog);

if(name==NULL)
	{
	sprintf(name_buf," NO CLASH ");
	}
else
	{
	tp=((SCALE *)(name->data))->name;

	while(tp!='\0'&&count<36)
		{
		name_buf[count]=*tp;
		tp++;
		count++;
		}
	name_buf[count]='\0';
	}

	set_tedinfo(dlog,SmallClashName,name_buf);

if(notes==NULL)
	{
	sprintf(notes_buf," NO CLASH ");
	}
else
	{
	tp=((SCALE *)(notes->data))->name;

	while(tp!='\0'&&count<36)
		{
		notes_buf[count]=*tp;
		tp++;
		count++;
		}
	notes_buf[count]='\0';
	}

	set_tedinfo(dlog,SmallClashNotes,notes_buf);

	result=handle_dialog(dlog,0,0);
	return(result);

}

int resolve_clash_form_normal(NODE *name, NODE *notes)
	{
	OBJECT *dlog;
	char name_buf[66],notes_buf[66];
	int result;
	
	Crsrc_gaddr(R_TREE,Clash_resolve,&dlog);

	if(name==NULL)
		sprintf(name_buf," NO CLASH ");
	else
		sprintf(name_buf,"%s",((SCALE *)(name->data))->name);

	set_tedinfo(dlog,ClashNameField,name_buf);

if(notes==NULL)
	sprintf(notes_buf," NO CLASH ");
else
	sprintf(notes_buf,"%s",((SCALE *)(notes->data))->name);

	
	set_tedinfo(dlog,ClashNoteField,notes_buf);

	result=handle_dialog(dlog,0,0);
	return(result);

}

void store_scale_as(CURRENT_STATUS *cs, NODE *known_scales)
	{
	int store_result=0,result,scale_name_exists=0,scale_notes_exist=0,loop1;
	int desired_notes[12];
	char *scale_name;
/*	char temp_buf[100];*/
	NODE *np=known_scales,*name_exists_at=NULL,*notes_exist_at=NULL;	


local_restart:
	store_result=0;
	scale_name_exists=0;
	scale_notes_exist=0;
	np=known_scales;
	name_exists_at=NULL;
	notes_exist_at=NULL;	

	scale_name=get_name_of_scale_form(cs, known_scales);

 	current_notes_to_store(desired_notes, cs->backup_notes, cs->key);

	if(scale_name!=NULL)
		{
/*		sprintf(temp_buf,"[2][ %s][OK]",scale_name);
		result=form_alert(1,temp_buf);
*/
		while(np!=NULL)
			{
			result=compare_scale_names(((SCALE *)np->data)->name,scale_name);
			if(result==0)
				{
				scale_name_exists=1;
				name_exists_at=np;
				}
		
			for(loop1=0;loop1<12;loop1++)
				{
				if(((SCALE *)np->data)->notes[loop1]!=desired_notes[loop1])
					loop1=20;
				}

			if(loop1<20)
				{
				scale_notes_exist=1;
				notes_exist_at=np;
				}

			np=np->right;
			}

		if(scale_name_exists || scale_notes_exist)
			{
				if(desk_x<=320)
					result=resolve_clash_form_small(name_exists_at,notes_exist_at);		
				else
					result=resolve_clash_form_normal(name_exists_at,notes_exist_at);
			}
		else
			store_result=store_scale_with_this_name(cs,known_scales,scale_name);

		if(result==ClashRename || result==SmallClashRename)
			goto local_restart;

		if(result==ClashStoreIt || result==SmallClashStoreT)
		{
			if(scale_name_exists)
				store_result=store_scale(cs,known_scales);
			else
				store_result=store_scale_with_this_name(cs,known_scales,scale_name);
		}			
		}


	}

void rename_form_small(CURRENT_STATUS *cs,NODE *known_scales)
{
	OBJECT *dlog;
	char edit_buf[40],title_buf[25];
	char *tp;
	int count=0;
	int result;
	
	Crsrc_gaddr(R_TREE,NAME_SCALE_SMALL,&dlog);

	tp=cs->current_scale->name;

	while(tp!='\0'&&count<36)
		{
		edit_buf[count]=*tp;
		tp++;
		count++;
		}
	edit_buf[count]='\0';

	set_tedinfo(dlog,FSmallScaleName,edit_buf);
	sprintf(title_buf,"%s","RENAME SCALE");
	set_tedinfo(dlog,FSmallScaleTitle,title_buf);

	result=handle_dialog(dlog,FSmallScaleName,0);
	if (result==FSmallScaleOK)
	{

		get_tedinfo(dlog,FSmallScaleName,edit_buf);
		tp=(char *)malloc(strlen(edit_buf)+1);
		if(tp!=NULL)
			{
			free(cs->current_scale->name);
			cs->current_scale->name=tp;
			strcpy(cs->current_scale->name,edit_buf);
			}

	}
}



void set_key_from_menu(CURRENT_STATUS *cs,int item)
	{
		int key_names[]={MAkey,MBbkey,MBkey,
						 MCkey,MDbkey,MDkey,MEbkey,	
						 MEkey,MFkey,MGbkey,MGkey,MAbkey,};
		int loop;

		for(loop=0;loop<12;loop++)
			{
				if(item==key_names[loop])
					{
					menu_icheck(menu_ptr,key_names[loop],1);
					cs->key=note_numbers[loop];
					}
				else
					menu_icheck(menu_ptr,key_names[loop],0);
			}

	}

void set_key_tick(int note_number)
	{

		int key_names[]={MAkey,MBbkey,MBkey,
						 MCkey,MDbkey,MDkey,MEbkey,	
						 MEkey,MFkey,MGbkey,MGkey,MAbkey,};
		int loop;


		for(loop=0;loop<12;loop++)
			if(loop==note_number)
				menu_icheck(menu_ptr,key_names[loop],1);
			else
				menu_icheck(menu_ptr,key_names[loop],0);
	}

void set_note_from_menu(CURRENT_STATUS *cs,int item)
	{
		int menu_names[]={MAscale,MBbscale,MBscale,
						  MCscale,MDbscale,MDscale,MEbscale,	
						  MEscale,MFscale,MGbscale,MGscale,MAbscale};
		int loop;

		for(loop=0;loop<12;loop++)
			{
				if(menu_names[loop]==item)
					cs->current_notes[loop]^=1;
			}
	}

void set_scales_ticks(int note_array[])
	{
		int menu_names[]={MAscale,MBbscale,MBscale,
						  MCscale,MDbscale,MDscale,MEbscale,	
						  MEscale,MFscale,MGbscale,MGscale,MAbscale};
		int loop;

		for(loop=0;loop<12;loop++)
			menu_icheck(menu_ptr,menu_names[loop],note_array[loop]);
	}			


int scale_changed(CURRENT_STATUS *cs)
	{
	int ret=0;
	int loop;

	for(loop=0;loop<12;loop++)
		{
		if(cs->current_scale->notes[loop]!=cs->backup_notes[loop])
			ret=1;
		}

	return ret;
	}
	
	
int is_quit()
	{
	int result;

	result=form_alert(1,"[2][ | Are you sure you | want to QUIT ][Quit|Cancel]");

	if(result==1)
		return(1);
	else
		return(0);
	}
	
NODE *handle_menu(int title,int item,CURRENT_STATUS *cs, NODE *known_scales)
{
	int result,loop;
	OBJECT *objptr;
	NODE *ret_scale;
	char filename[FMSIZE];

	long temp_long;
	size_t temp_sizet;
	time_t temp_time;
	struct tm *tmpoint;
	extern int _XMODE;

	int local_scale_nums[]={MAscale,MBbscale,MBscale,MCscale,
									MDbscale,MDscale,MEbscale,MEscale,
									MFscale,MGbscale,MGscale,MAbscale};
	/*char buf[100];*/
				

	switch (item)
	{
		case MAbout:
				time(&temp_time);
				tmpoint=gmtime(&temp_time);

				temp_sizet= strftime(aldate,20,"%x",tmpoint);
				temp_sizet= strftime(altime,20,"%X",tmpoint);

			menu_tnormal(menu_ptr,title,1);
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
			break;
			
		case MLoadScales:
			menu_tnormal(menu_ptr,title,1);
#ifdef DEMO
			Crsrc_gaddr(R_TREE,NAG_SCREEN,&objptr);
			set_tedinfo(objptr,Please_Wait_Butt," OK ");
			result=handle_dialog(objptr,0,0);
			set_tedinfo(objptr,Please_Wait_Butt,"Please Wait");
#else
			result=select("SCL",filename,"LOAD SCALES");
			if(result)
				{
				known_scales=read_scales(known_scales,filename);
				cs->root=&note8;
				cs->key=&note8;
				cs->current_scale=(SCALE *)known_scales->data;
				cs->changed_scale=FALSE;
				copy_scales(cs->current_scale->notes,cs->backup_notes);
				set_frets(cs);
				fill_notes_from_store(cs->backup_notes,cs->current_notes,cs->key);
				set_display_frets(cs);
				set_scales_ticks(cs->current_notes);
				set_key_tick(cs->key->number);
				}
#endif
			break;

		case MSaveScales:
			menu_tnormal(menu_ptr,title,1);
#ifdef DEMO
			Crsrc_gaddr(R_TREE,NAG_SCREEN,&objptr);
			set_tedinfo(objptr,Please_Wait_Butt," OK ");
			result=handle_dialog(objptr,0,0);
			set_tedinfo(objptr,Please_Wait_Butt,"Please Wait");
#else
			result=select("SCL",filename,"SAVE SCALES");
			if(result)
				save_scales(known_scales,filename);
#endif
			break;

		case MSaveDefaults:
#ifdef DEMO
			Crsrc_gaddr(R_TREE,NAG_SCREEN,&objptr);
			set_tedinfo(objptr,Please_Wait_Butt," OK ");
			result=handle_dialog(objptr,0,0);
			set_tedinfo(objptr,Please_Wait_Butt,"Please Wait");
#else
			save_defaults();
#endif
			break;

		case MLoadDefaults:
#ifdef DEMO
			Crsrc_gaddr(R_TREE,NAG_SCREEN,&objptr);
			set_tedinfo(objptr,Please_Wait_Butt," OK ");
			result=handle_dialog(objptr,0,0);
			set_tedinfo(objptr,Please_Wait_Butt,"Please Wait");
#else
			load_defaults(NULL);
			menu_icheck(menu_ptr,MSortNames,osort);
#endif
			break;

		case MPaths:
			menu_tnormal(menu_ptr,title,1);
			paths_form(cs);
			break;

		case MLoadTunings:
			menu_tnormal(menu_ptr,title,1);
			tunings_form(cs);
			break;

		case MQuit:
			menu_tnormal(menu_ptr,title,1);
			if(is_quit())
				finished=1;
			break;

		case MPrintText:
			menu_tnormal(menu_ptr,title,1);
			display_text_print_form(cs);
			break;

/*		case MPrintGraphics:
			menu_tnormal(menu_ptr,title,1);
			windopen(&graphic_save,0);
			wclip(&graphic_save);
			clearw(&graphic_save);
			result=form_alert(1,"[1][ok][ok]");
			windclose(&graphic_save);
			Crsrc_gaddr(R_TREE,MENU1,&menu_ptr);
			menu_bar(menu_ptr,1);
			break;
*/

		case MAscale:
		case MBbscale:
		case MBscale:
		case MCscale:
		case MDbscale:
		case MDscale:
		case MEbscale:
		case MEscale:
		case MFscale:
		case MGbscale:
		case MGscale:
		case MAbscale:

			for(loop=0;local_scale_nums[loop]!=item;loop++);
			set_note_from_menu(cs,item);
			set_scales_ticks(cs->current_notes);
			current_notes_to_store(cs->backup_notes,cs->current_notes,cs->key);
			cs->changed_scale=TRUE;
			set_display_frets(cs);
			specific_note_draw(&scale_window,cs,loop);
			if(odisplay_relative)
				specific_note_draw(&scale_window,cs,((NOTE *)get_relative(cs,note_numbers[loop]))->number);
			break;


		case MAkey:
		case MBbkey:
		case MBkey:
		case MCkey:
		case MDbkey:
		case MDkey:
		case MEbkey:
		case MEkey:
		case MFkey:
		case MGbkey:
		case MGkey:
		case MAbkey:
			set_key_from_menu(cs,item);
			set_key_tick(cs->key->number);
			fill_notes_from_store(cs->backup_notes,cs->current_notes,cs->key);
			set_scales_ticks(cs->current_notes);
			set_display_frets(cs);
			clearw(&scale_window);
			small_scale_draw(&scale_window,cs);
			break;

/*		case MRootoption:
			odisplay_root^=1;
			menu_icheck(menu_ptr,MRootoption,odisplay_root);
			set_display_frets(cs);
			wclip(&scale_window);
				
			specific_note_draw(&scale_window,cs,cs->key->number);
			if(odisplay_relative)
				specific_note_draw(&scale_window,cs,((NOTE *)get_relative(cs,cs->key))->number);

			break;
*/
/*		case MRelativeoption:
			odisplay_relative^=1;
			menu_icheck(menu_ptr,MRelativeoption,odisplay_relative);
			set_display_frets(cs);
			wclip(&scale_window);
			clearw(&scale_window);
			small_scale_draw(&scale_window,cs);
			break;

		case MCircularNotes:
			circle_flag^=1;
			menu_icheck(menu_ptr,MCircularNotes,circle_flag);
			wclip(&scale_window);
			clearw(&scale_window);
			small_scale_draw(&scale_window,cs);
			break;
*/
		case MSortNames:
			osort^=1;
			menu_icheck(menu_ptr,MSortNames,osort);
			break;

		case MDisplayOptions:
			display_options_form(cs);
			set_big_y_offset(0);
			wind_set(scale_window.handle,WF_VSLIDE,1);
			set_start_fret(0);
			wind_set(scale_window.handle,WF_HSLIDE,1);
			break;

/*		case MFretNumbers:
			fret_numbers_form(cs);
			break;
*/
		case MRename:
			if(desk_x<=320)
				rename_form_small(cs,known_scales);
			else
				rename_form(cs,known_scales);
			break;

		case MStoreScale:
			result=store_scale(cs,known_scales);
			break;

		case MStoreAs:
			store_scale_as(cs,known_scales);
			break;

		case MLoadScale:

			result=2;
			if(scale_changed(cs))
					result=form_alert(1,"[1][ | WARNING : | | The scale has changed. | These changes will be lost ][ STORE | LOSE | CANCEL ]");
					
			if(result==1)
				{
				if(!store_scale(cs,known_scales))
					result=3;
				}

			if(result!=3)
				{	
				if(osort)
					sort_names(known_scales);
	
				ret_scale=restore_scale(cs,known_scales);
				if(ret_scale)
					{
					cs->current_scale=(SCALE *)ret_scale->data;
					cs->changed_scale=FALSE;
					copy_scales(cs->current_scale->notes,cs->backup_notes);
					set_frets(cs);
					fill_notes_from_store(cs->backup_notes,cs->current_notes,cs->key);
					set_scales_ticks(cs->current_notes);
					set_display_frets(cs);
					}
				}
			break;
	
/*		case MDisplayFrets:
			display_frets_form(cs);
			break;
*/
		case MDeleteScale:
/*						sprintf(buf,"[1][known %d][ok]",known_scales);
						form_alert(1,buf);
*/
			if(osort)
				sort_names(known_scales);

			ret_scale=delete_scale_window(cs,known_scales);
			if(ret_scale)
				{
					/* first one */
				if((NODE *)(ret_scale->left)==NULL)
					{
/*						sprintf(buf,"[1][known %d][ok]",known_scales);
						form_alert(1,buf);
*/
						known_scales=(NODE *)ret_scale->right;
						if(ret_scale->right)
						((NODE *)(ret_scale->right))->left=NULL;

/*						sprintf(buf,"[1][known %d][ok]",known_scales);
						form_alert(1,buf);
*/					}
				else
					{
						/* last one */
					if((NODE *)(ret_scale->right)!=NULL)
						((NODE *)(ret_scale->right))->left=ret_scale->left;

					((NODE *)(ret_scale->left))->right=ret_scale->right;

					}
					free(ret_scale->data);
					free(ret_scale);
					total_scales--;
				}
			break;
			
	}
	menu_tnormal(menu_ptr,title,1);
	return(known_scales); 
}


void handle_events(CURRENT_STATUS *cs,NODE *known_scales)
{
	GRECT c;
	short mbuff[8];
	int res;
	int scan_code,ascii_code,little_skip=0;
	short new_pos;
	int no_frets_on;
/*	char buf[100];*/

	short mousex,mousey,mouse_button,key_status,key_return,num_clicks;


	key_return=0;

/*	wind_get(scale_window.handle,WF_WXYWH,&c.g_x,&c.g_y,&c.g_w,&c.g_h);
	no_frets_on=c.g_w/frets_length;

	sprintf(buf,"[1][frets %d global %d][ok]",no_frets_on,global_frets);
	form_alert(1,buf);
*/

	for (; !finished;)
	{

		wind_get(scale_window.handle,WF_WXYWH,&c.g_x,&c.g_y,&c.g_w,&c.g_h);
		no_frets_on=c.g_w/frets_length;

		key_return=0;
		res=evnt_multi(	MU_KEYBD|MU_BUTTON|MU_MESAG,2,3,1,
						NULL,NULL,NULL,NULL,NULL,
						NULL,NULL,NULL,NULL,NULL,
						mbuff,
						NULL,NULL,
						&mousex,&mousey,&mouse_button,
						&key_status,&key_return,&num_clicks);
								

		if(alredraw==TRUE && mbuff[0]!=WM_REDRAW)
			{
				wclip(&scale_window);
				clearw(&scale_window);
				small_scale_draw(&scale_window,cs);
				alredraw=FALSE;
			}

	if(res & MU_KEYBD)
		{

			scan_code=key_return ^ 0xFF00;
			ascii_code=key_return ^ 0x00FF;
			scan_code = scan_code >> 8;


		if(scan_code==WindowDownKey)
		{
				/* down line */

			if(key_status==2) /*page down*/
			{
					mbuff[0]=WM_ARROWED;
					mbuff[3]=scale_window.handle;
					mbuff[4]=WA_DNPAGE;

			}
			else
			{				

					mbuff[0]=WM_ARROWED;
					mbuff[3]=scale_window.handle;
					mbuff[4]=WA_DNLINE;
			}

			goto end_if2;	
		}

		if(scan_code==224 && key_status==8)
		{
					mbuff[0]=MN_SELECTED;
					mbuff[3]=scale_window.handle;
					mbuff[4]=MLoadScale;
				goto end_if2;
		}

		if(scan_code==WindowLeftKey)
		{
				/* left line */

			if(key_status==2) /*page left*/
			{
					mbuff[0]=WM_ARROWED;
					mbuff[3]=scale_window.handle;
					mbuff[4]=WA_LFPAGE;

			}
			else
			{				

					mbuff[0]=WM_ARROWED;
					mbuff[3]=scale_window.handle;
					mbuff[4]=WA_LFLINE;
			}

			goto end_if2;	
		}

		if(scan_code==WindowRightKey)
		{
				/* right line */

			if(key_status==2) /*page right*/
			{
					mbuff[0]=WM_ARROWED;
					mbuff[3]=scale_window.handle;
					mbuff[4]=WA_RTPAGE;

			}
			else
			{				

					mbuff[0]=WM_ARROWED;
					mbuff[3]=scale_window.handle;
					mbuff[4]=WA_RTLINE;
			}

			goto end_if2;	
		}

		if(scan_code==HelpKey)
		{

			  /* Help   */
				do_keys(key_status,key_return);
				little_skip=1;
				goto end_if2;
		}

		if(scan_code==WindowUpKey)
		{	/* up line */
	
			if(key_status==2) /*page up*/
			{
					mbuff[0]=WM_ARROWED;
					mbuff[3]=scale_window.handle;
					mbuff[4]=WA_UPPAGE;

			}
			else
			{				
					mbuff[0]=WM_ARROWED;
					mbuff[3]=scale_window.handle;
					mbuff[4]=WA_UPLINE;
			}
			goto end_if2;			
		}
		else
		{
		/* default unknown key*/
		#ifdef ALTESTING
			do_keys(key_status,key_return);
		#endif
			little_skip=1;
		}

end_if2:;

	}

	if(little_skip==1)
		goto skip_loop_end;




		if(res & MU_BUTTON /*mouse_button*/)
			{
			if(num_clicks==2)
				mouse_double_click_on_fret(&scale_window,cs,known_scales,mousex,mousey);
			else
				mouse_click_on_fret(&scale_window,cs,known_scales,mousex,mousey);
			}
		else
			{

		switch(mbuff[0])
			{
			case MN_SELECTED:
				known_scales=handle_menu(mbuff[3],mbuff[4],cs,known_scales);
				break;
			case WM_SIZED:
				if(mbuff[3]==scale_window.handle)
					{
						wind_set(mbuff[3],WF_CXYWH,mbuff[4],mbuff[5],mbuff[6],mbuff[7]);
						refix_start_fret();
						wclip(&scale_window);
						clearw(&scale_window);
						small_scale_draw(&scale_window,cs);
					}
				break;
			case WM_HSLID:
				if(mbuff[3]==scale_window.handle)
					{
					set_start_fret(mbuff[4]);
					wind_set(mbuff[3],WF_HSLIDE,mbuff[4]);
					alredraw=TRUE;
					}
				break;
				case WM_CLOSED:
				if(mbuff[3]==scale_window.handle)
					{
					if(is_quit())
						finished=1;
					}
				break;
				case WM_VSLID:
				if(mbuff[3]==scale_window.handle)
					{
					set_big_y_offset(mbuff[4]);
					wind_set(mbuff[3],WF_VSLIDE,mbuff[4]);
					alredraw=TRUE;
					}
				break;
			case WM_ARROWED:
				if(mbuff[3]==scale_window.handle)
					{
					switch(mbuff[4])
						{
						case WA_DNPAGE:
							new_pos=window_vert_height(&scale_window)+
								window_vert_pos(&scale_window);
		
							if(new_pos>1000)
								new_pos=1000;

							set_big_y_offset(new_pos);
							wind_set(scale_window.handle,WF_VSLIDE,new_pos);

							wclip(&scale_window);
							clearw(&scale_window);
							small_scale_draw(&scale_window,cs);
							break;
						case WA_DNLINE:
							new_pos=window_vert_pos(&scale_window)+
								frets_height;
		
							if(new_pos>1000)
								new_pos=1000;

							set_big_y_offset(new_pos);
							wind_set(scale_window.handle,WF_VSLIDE,new_pos);

							wclip(&scale_window);
							clearw(&scale_window);
							small_scale_draw(&scale_window,cs);
							break;
						case WA_UPPAGE:
							new_pos=window_vert_pos(&scale_window)-
									window_vert_height(&scale_window);
		
							if(new_pos<1)
								new_pos=1;

							set_big_y_offset(new_pos);
							wind_set(scale_window.handle,WF_VSLIDE,new_pos);

							wclip(&scale_window);
							clearw(&scale_window);
							small_scale_draw(&scale_window,cs);
							break;
						case WA_UPLINE:
							new_pos=window_vert_pos(&scale_window)-
									frets_height;
		
							if(new_pos<1)
								new_pos=1;

							set_big_y_offset(new_pos);
							wind_set(scale_window.handle,WF_VSLIDE,new_pos);

							wclip(&scale_window);
							clearw(&scale_window);
							small_scale_draw(&scale_window,cs);
							break;
						case WA_LFPAGE:
						if(no_frets_on<global_frets)
						{
							start_fret-=frets_on();
							if(start_fret<0) start_fret=0;

							new_pos=pos_from_start_fret();
							if(new_pos<1) new_pos=1;

							wind_set(scale_window.handle,WF_HSLIDE,new_pos);

							wclip(&scale_window);
							clearw(&scale_window);
							small_scale_draw(&scale_window,cs);
							break;
						}
						case WA_LFLINE:
						if(no_frets_on<global_frets)
						{
							start_fret--;
							if(start_fret<0) start_fret=0;

							new_pos=pos_from_start_fret();
							if(new_pos<1) new_pos=1;

							wind_set(scale_window.handle,WF_HSLIDE,new_pos);

							wclip(&scale_window);
							clearw(&scale_window);
							small_scale_draw(&scale_window,cs);
							break;
						}
						case WA_RTPAGE:
						if(no_frets_on<global_frets)
						{
							start_fret+=frets_on();
							if(start_fret>(global_frets-frets_on())) start_fret=global_frets-frets_on();

							new_pos=pos_from_start_fret();
							if(new_pos<1) new_pos=1;

							wind_set(mbuff[3],WF_HSLIDE,new_pos);
							wclip(&scale_window);
							clearw(&scale_window);
							small_scale_draw(&scale_window,cs);
							break;
						}
						case WA_RTLINE:
						if(no_frets_on<global_frets)
						{
							start_fret++;
							if(start_fret>(global_frets-frets_on())) start_fret=global_frets-frets_on();

							new_pos=pos_from_start_fret();
							if(new_pos<1) new_pos=1;

							wind_set(mbuff[3],WF_HSLIDE,new_pos);
							wclip(&scale_window);
							clearw(&scale_window);
							small_scale_draw(&scale_window,cs);
							break;
						}
						}
					}
				break;
/*			case WM_MOVED:
				if(mbuff[3]==scale_window.handle)
					{
						wind_set(mbuff[3],WF_CXYWH,mbuff[4],mbuff[5],mbuff[6],mbuff[7]);

					}
				break;*/
			case WM_REDRAW:
				if(mbuff[3]==scale_window.handle)
					{
						wclip(&scale_window);
						clearw(&scale_window);
						small_scale_draw(&scale_window,cs);
						alredraw=FALSE;
					}
				break;
/*			case WM_FULLED:
				fullw(&scale_window);
				break;
*/
			default:
			break;
						
			}
			}
	

skip_loop_end: little_skip=0;

		if(alredraw==TRUE)
			{
				wclip(&scale_window);
				clearw(&scale_window);
				small_scale_draw(&scale_window,cs);
				alredraw=FALSE;
			}




	}
}

void help_key( void )
{
	OBJECT *dlog;
	int result;
	GRECT xywh;
	short x,y,w,h;
	int circle_radius;
	short tatts[10];	/* text attributes */

		Crsrc_gaddr(R_TREE,Help_Key,&dlog);
		form_center(dlog,&x,&y,&w,&h);
		form_dial(FMD_START,0,0,0,0,x,y,w,h);
		objc_draw(dlog,0,10,x,y,w,h);

		result=objc_xywh(dlog,NormalNoteBox,&xywh);

	if(frets_length<xywh.g_w && frets_height<xywh.g_h)
	{
	if(frets_length>frets_height)
		circle_radius=(frets_height/2)-1-2;
	else
		circle_radius=(frets_length/2)-1-2;
	}
	else
	{
		circle_radius= (xywh.g_h > xywh.g_w) ? ((xywh.g_h-2)/2) : ((xywh.g_w-2)/2);
	}

vqt_attributes(handle,tatts);

/* draw the normal note */
if(text_notes==1)
	{
		vst_effects(handle,0);

		v_gtext(	handle,
					xywh.g_x+(xywh.g_w/2)-(tatts[6]/2),
					xywh.g_y+(xywh.g_h/2)+(tatts[7]/2),
					"E");

	}
else
	{
		vsf_color(handle,BLACK); 
		vsf_interior(handle,2);
		vsf_style(handle,2);

		alcircle(handle,
					xywh.g_x+(xywh.g_w/2),
					xywh.g_y+(xywh.g_h/2),
					circle_radius); 

		vsf_color(handle,BLACK);
		vsf_interior(handle,8);
		vsf_style(handle,2);
	}
/* norm and relative */

		result=objc_xywh(dlog,NormRelBox,&xywh);

if(text_notes==1)
	{
		vst_effects(handle,0|UNDERLINED);

		v_gtext(	handle,
					xywh.g_x+(xywh.g_w/2)-(tatts[6]/2),
					xywh.g_y+(xywh.g_h/2)+(tatts[7]/2),
					"E");

	}
else
	{
		vsf_color(handle,BLACK);
		vsf_interior(handle,FIS_HOLLOW);
		vsf_style(handle,0);

		alcircle(handle, 
					xywh.g_x+(xywh.g_w/2),
					xywh.g_y+(xywh.g_h/2),
					circle_radius+2);
 		
		vsf_color(handle,BLACK);					
		vsf_interior(handle,8);
		vsf_style(handle,2);

		vsf_color(handle,BLACK); 
		vsf_interior(handle,2);
		vsf_style(handle,2);

		alcircle(handle,
					xywh.g_x+(xywh.g_w/2),
					xywh.g_y+(xywh.g_h/2),
					circle_radius); 

		vsf_color(handle,BLACK);
		vsf_interior(handle,8);
		vsf_style(handle,2);
	}
/* root note */

		result=objc_xywh(dlog,RootNoteBox,&xywh);


	if(odisplay_root)
	{
		if(text_notes==1)
			{
				vst_effects(handle,OUTLINE);
		
				v_gtext(	handle,
							xywh.g_x+(xywh.g_w/2)-(tatts[6]/2),
							xywh.g_y+(xywh.g_h/2)+(tatts[7]/2),
							"E");

			}
		else
			{
				vsf_color(handle,BLACK);
				vsf_interior(handle,FIS_SOLID);
				vsf_style(handle,2);
			
				alcircle(handle, 
							xywh.g_x+(xywh.g_w/2),
							xywh.g_y+(xywh.g_h/2),
							circle_radius);
		 		
				vsf_color(handle,BLACK);					
				vsf_interior(handle,8);
				vsf_style(handle,2);

			}
	}
	else
	{
		if(text_notes==1)
			{
				vst_effects(handle,0);
		
				v_gtext(	handle,
							xywh.g_x+(xywh.g_w/2)-(tatts[6]/2),
							xywh.g_y+(xywh.g_h/2)+(tatts[7]/2),
							"E");
		
			}
		else
			{
				vsf_color(handle,BLACK); 
				vsf_interior(handle,2);
				vsf_style(handle,2);
	
				alcircle(handle,
							xywh.g_x+(xywh.g_w/2),
							xywh.g_y+(xywh.g_h/2),
							circle_radius); 

				vsf_color(handle,BLACK);
				vsf_interior(handle,8);
				vsf_style(handle,2);
			}
	}

/* root & relative */

	result=objc_xywh(dlog,RootRelBox,&xywh);

	if(text_notes==1)
		{
			if(odisplay_root)			
				vst_effects(handle,OUTLINE|UNDERLINED);
			else
				vst_effects(handle,UNDERLINED);
				
				v_gtext(	handle,
						xywh.g_x+(xywh.g_w/2)-(tatts[6]/2),
						xywh.g_y+(xywh.g_h/2)+(tatts[7]/2),
						"E");

		}
	else
	{

	vsf_color(handle,BLACK);
	vsf_interior(handle,FIS_HOLLOW);
	vsf_style(handle,0);

	alcircle(handle, 
				xywh.g_x+(xywh.g_w/2),
				xywh.g_y+(xywh.g_h/2),
				circle_radius+2);
		
	vsf_color(handle,BLACK);					
	vsf_interior(handle,8);
	vsf_style(handle,2);

	if(odisplay_root)
	{
		vsf_color(handle,BLACK);
		vsf_interior(handle,FIS_SOLID);
		vsf_style(handle,2);

		alcircle(handle, 
					xywh.g_x+(xywh.g_w/2),
					xywh.g_y+(xywh.g_h/2),
					circle_radius);
 		
		vsf_color(handle,BLACK);					
		vsf_interior(handle,8);
		vsf_style(handle,2);

	}
	else
	{
		vsf_color(handle,BLACK); 
		vsf_interior(handle,2);
		vsf_style(handle,2);

		alcircle(handle,
					xywh.g_x+(xywh.g_w/2),
					xywh.g_y+(xywh.g_h/2),
					circle_radius); 

		vsf_color(handle,BLACK);
		vsf_interior(handle,8);
		vsf_style(handle,2);
	}
	}
/* relative note */

	result=objc_xywh(dlog,RelativeNoteBox,&xywh);

	if(text_notes==1)
		{
				vst_effects(handle,UNDERLINED);
				
				v_gtext(	handle,
						xywh.g_x+(xywh.g_w/2)-(tatts[6]/2),
						xywh.g_y+(xywh.g_h/2)+(tatts[7]/2),
						"E");

		}
	else
	{
	vsf_color(handle,BLACK);
	vsf_interior(handle,FIS_HOLLOW);
	vsf_style(handle,2);

	alcircle(handle, 
				xywh.g_x+(xywh.g_w/2),
				xywh.g_y+(xywh.g_h/2),
				circle_radius);
 		
	vsf_color(handle,BLACK);					
	vsf_interior(handle,8);
	vsf_style(handle,2);
	}

	vst_effects(handle,0);

/* End the form */
		result=form_do(dlog,0);
		dlog[result].ob_state&=~SELECTED;
 		result=result&0x7fff;

		form_dial(FMD_FINISH,0,0,0,0,x,y,w,h);
		


}

