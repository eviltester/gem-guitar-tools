#ifndef SCALEH
#define SCALEH
#include <stdio.h>

#ifdef PCVER
#include "b:\note.h"
#endif
#ifdef STVER
#include "note.h"
#endif

typedef struct{
				char *name;
				int notes[12];

		}SCALE;

static void list_notes(SCALE *scale)
	{
		int loop;

		for(loop=0;loop<12;loop++)
			if(scale->notes[loop])
				printf("%s ",note_numbers[loop]->note_name);
		printf("\n");
	}

static void fill_notes_from_scale(SCALE *current_scale, int current_notes[],NOTE *key)
	{
		int pos,loop,diff=(Enote.number)-(key->number);
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

static void fill_notes_from_store(int backup_notes[], int current_notes[],NOTE *key)
	{
		int pos,loop,diff=(Enote.number)-(key->number);
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
		int pos,loop,diff=(key->number)-(Enote.number);
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

#endif