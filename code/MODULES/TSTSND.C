#include <stdio.h>
#include "time.h"
#include <osbind.h>

main()
{

short snd_chph[][]={
						{8,8,7,14,14,13,12,11,11,10,9,9},
						{4,4,3,7,7,6,6,5,5,5,4,4}
						};
short snd_chpl[][]={
						{224,97,232,238,23,77,142,217,47,142,247,103},
						{112,48,244,119,11,166,71,236,151,71,251,179}
						};
short volume=15;

int octave=0;
int note=7;
int note_length=1;

	char temp;
	clock_t note_stime;



	int midiplay_note;

		Giaccess(254,7+128);
		Giaccess(volume,8+128);
		Giaccess(snd_chpl[octave][note],0+128);
		Giaccess(snd_chph[octave][note],1+128);

	note_stime = clock();
	temp=0;

	while(((clock()-note_stime)/CLK_TCK) < note_length && temp ==0)
	{	
		if(Bconstat(2))
			temp=Bconin(2);
	}

		Giaccess(255,7+128);
}
