#ifndef _NOTE_H
#define _NOTE_H

typedef struct{
				int number;     /*position in note_numbers */
				char *note_name;
				char *note_name_alias;
			   }NOTE;

extern NOTE note1;
extern NOTE note2;
extern NOTE note3;
extern NOTE note4;
extern NOTE note5;
extern NOTE note6;
extern NOTE note7;
extern NOTE note8;
extern NOTE note9;
extern NOTE note10;
extern NOTE note11;
extern NOTE note12;

#define		Enote	note8


extern NOTE *note_numbers[];

/* from note in E tuning into supplied key */
extern NOTE *transpose_note(NOTE *, NOTE *);

#endif