#include "global.h"


char Gtemp_string[TEMPORARY_STRING_LENGTH];

void save_defaults()
{
	FILE *fp;
	int result;
	int ccnt;
	char filename[FMSIZE];

/* saves out the following global variables */

/*select and open file*/
	result=select("DEF",filename,"SAVE DEFAULTS");
	if(result)
	{
	if((fp=fopen(filename,"w"))==NULL)
		{
			sprintf(Gtemp_string,"error cannot open| %s |as output",filename);
			error(Gtemp_string);
		}


	if(fp)
		{
		ccnt=fwrite((void *)&global_frets,sizeof(int),1,fp);
		ccnt=fwrite((void *)&odisplay_scale,sizeof(int),1,fp);
		ccnt=fwrite((void *)&odisplay_root,sizeof(int),1,fp);
		ccnt=fwrite((void *)&odisplay_relative,sizeof(int),1,fp);
		ccnt=fwrite((void *)&numbers_flag,sizeof(int),1,fp);
		ccnt=fwrite((void *)&key_name_flag,sizeof(int),1,fp);
		ccnt=fwrite((void *)&scale_name_flag,sizeof(int),1,fp);
		ccnt=fwrite((void *)&midi_sound_flag,sizeof(int),1,fp);
		ccnt=fwrite((void *)&channel,sizeof(int),1,fp);
		ccnt=fwrite((void *)&velocity,sizeof(int),1,fp);
		ccnt=fwrite((void *)&note_length,sizeof(int),1,fp);
		ccnt=fwrite((void *)&midi_note_off_flag,sizeof(int),1,fp);
		ccnt=fwrite((void *)&frets_length,sizeof(int),1,fp);
		ccnt=fwrite((void *)&frets_height,sizeof(int),1,fp);
		ccnt=fwrite((void *)&osort,sizeof(int),1,fp);
		ccnt=fwrite((void *)&first_fret,sizeof(int),1,fp);
		ccnt=fwrite((void *)&fret_inc,sizeof(int),1,fp);
		ccnt=fwrite((void *)&chip_volume,sizeof(short),1,fp);
		ccnt=fwrite((void *)&circle_flag,sizeof(int),1,fp);
		ccnt=fwrite((void *)&tp_columns,sizeof(int),1,fp);
		ccnt=fwrite((void *)&tp_start_fret,sizeof(int),1,fp);
		ccnt=fwrite((void *)&tp_tab,sizeof(int),1,fp);
		ccnt=fwrite((void *)&tp_no_frets,sizeof(int),1,fp);
		ccnt=fwrite((char *)&tp_header1,sizeof(char),40,fp);
		ccnt=fwrite((char *)&tp_header2,sizeof(char),40,fp);
		ccnt=fwrite((char *)&tp_trailer1,sizeof(char),40,fp);
		ccnt=fwrite((char *)&tp_trailer2,sizeof(char),40,fp);
		}
	}

	fclose(fp);
}

void load_defaults(char *fn)
{
	FILE *fp;
	int result;
	int ccnt;
	char filename[FMSIZE];

/* saves out the following global variables */

/*select and open file*/
	if(fn==NULL)
		result=select("DEF",filename,"READ DEFAULTS");
	else
		{
		result=1;
		strcpy(filename,fn);
		}

	if(result)
	{
	if((fp=fopen(filename,"r"))==NULL)
		{
			sprintf(Gtemp_string,"cannot open| %s |as input",filename);
			error(Gtemp_string);
		}


	if(fp)
		{
		ccnt=fread((void *)&global_frets,sizeof(int),1,fp);
		ccnt=fread((void *)&odisplay_scale,sizeof(int),1,fp);
		ccnt=fread((void *)&odisplay_root,sizeof(int),1,fp);
		ccnt=fread((void *)&odisplay_relative,sizeof(int),1,fp);
		ccnt=fread((void *)&numbers_flag,sizeof(int),1,fp);
		ccnt=fread((void *)&key_name_flag,sizeof(int),1,fp);
		ccnt=fread((void *)&scale_name_flag,sizeof(int),1,fp);
		ccnt=fread((void *)&midi_sound_flag,sizeof(int),1,fp);
		ccnt=fread((void *)&channel,sizeof(int),1,fp);
		ccnt=fread((void *)&velocity,sizeof(int),1,fp);
		ccnt=fread((void *)&note_length,sizeof(int),1,fp);
		ccnt=fread((void *)&midi_note_off_flag,sizeof(int),1,fp);
		ccnt=fread((void *)&frets_length,sizeof(int),1,fp);
		ccnt=fread((void *)&frets_height,sizeof(int),1,fp);
		ccnt=fread((void *)&osort,sizeof(int),1,fp);
		ccnt=fread((void *)&first_fret,sizeof(int),1,fp);
		ccnt=fread((void *)&fret_inc,sizeof(int),1,fp);
		ccnt=fread((void *)&chip_volume,sizeof(short),1,fp);
		ccnt=fread((void *)&circle_flag,sizeof(int),1,fp);
		ccnt=fread((void *)&tp_columns,sizeof(int),1,fp);
		ccnt=fread((void *)&tp_start_fret,sizeof(int),1,fp);
		ccnt=fread((void *)&tp_tab,sizeof(int),1,fp);
		ccnt=fread((void *)&tp_no_frets,sizeof(int),1,fp);
		ccnt=fread((char *)&tp_header1,sizeof(char),40,fp);
		ccnt=fread((char *)&tp_header2,sizeof(char),40,fp);
		ccnt=fread((char *)&tp_trailer1,sizeof(char),40,fp);
		ccnt=fread((char *)&tp_trailer2,sizeof(char),40,fp);


		}
	}

	fclose(fp);
}
