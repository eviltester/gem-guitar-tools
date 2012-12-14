#include <stdio.h>
#include "note.h"


NOTE note1={0,"A"," "};
NOTE note2={1,"A#","Bb"};
NOTE note3={2,"B"," "};
NOTE note4={3,"C"," "};
NOTE note5={4,"C#","Db"};
NOTE note6={5,"D"," "};
NOTE note7={6,"D#","Eb"};
NOTE note8={7,"E"," "};
NOTE note9={8,"F"," "};
NOTE note10={9,"F#","Gb"};
NOTE note11={10,"G"," "};
NOTE note12={11,"G#","Ab"};



NOTE *note_numbers[]={&note1,&note2,&note3,&note4,&note5,&note6,
				  &note7,&note8,&note9,&note10,&note11,&note12,
				  &note1,&note2,&note3,&note4,&note5,&note6,
				  &note7,&note8,&note9,&note10,&note11,&note12,
				  &note1,&note2,&note3,&note4,&note5,&note6,
				  &note7,&note8,&note9,&note10,&note11,&note12};

/* from note in E tuning into supplied key */
NOTE *transpose_note(NOTE *from, NOTE *key)
	{
		int diff=(note8.number) - (key->number);

		return note_numbers[from->number+12-diff];
	}

