#include "almidi.h"
#include "global.h"

/*
	looking at this I guess it is meant to change an ascii 
	char into the decimal representation of the hex num.
	But looking at this it doesn't work!!

	if ch >= A then ch=ch-A would mean that A returns 0 instead of 10
	!!!! do I use this ****?
*/
char hexify(char ch)
	{
		if(ch<='9')
		{
			ch=ch-'0';
			return(ch);
		}

		if(ch>='A')
		{
			ch=ch-'A';
			return(ch);
		}
	}

/*
	given a string of chars, and the length of the string.
	send them out to the midi port. Mask out the top byte
	since midi is 1-256, and not 1-64XXX
*/
void send_to_midi(char *ch, int chars)
{
	int duff=0x00FF;

	while(chars!=0)
			{
			duff&=*ch;
			Bconout(3,duff);
			duff=0x00FF;
			ch++;
			chars--;
			}
}	

/* This function doesn't play ****. is it still used??? */
void midi_play(char *str)
	{

/*	Dynamic memory allocation removed 4/10/93

	char *chp=(char *)malloc(strlen(str)),
*/
	char *chp; 
	char *chp2;
	char ch1,ch2;

/************** Replacement to dynamic code */

	if(strlen(str)>TEMPORARY_STRING_LENGTH)
	{
		form_alert(1,"[1][Internal error *midi_play dyn mem][SH*T]");
	}

	chp=Gtemp_string;

/************** End Replacement code */



	chp2=chp;


	while(*str!='\0')
		{
			ch1=hexify(*str);
			str++;
			
			if(*str!='\0')
				{
				ch2=hexify(*str);
				str++;
				}
			else
				ch2=0;

			ch1=ch1<<4;
		
			*chp=ch1|ch2;
			chp++;
		}

		*chp='\0';


/*	send_to_midi(chp2);*/

	}

/* 
	given a channel, a note, and a velocity - turn the note on 
*/
void midi_note_on(int channel,int note, int velocity)
{
	char midi_channels[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
								 0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
	char buf[4];


	char channelch=0;


	channelch=midi_channels[channel];
	channelch|=0x90;

	if(note<0)
		note=0;

	if(note>127)
		note=127;

	if(velocity<0)
		velocity=0;

	if(velocity>127)
		velocity=127;


	sprintf(buf,"%c%c%c",channelch,note,velocity);
	send_to_midi(buf,3);

}

/*
	given a channel and a note, turn the note off by making the 
	note velocity 0. This is the old fashioned way to turn a note off
*/
void midi_note_off_vel(int channel, int note)
{
	char midi_channels[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
								 0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
	char buf[4];


	char channelch=0;


	channelch=midi_channels[channel];
	channelch|=0x80;

	if(note<0)
		note=0;

	if(note>127)
		note=127;


	sprintf(buf,"%c%c%c",channelch,note,0);
	send_to_midi(buf,3);

}

/*
	given a channel, a note and a velocity - turn the note off 
	by sending a note off message.
*/
void midi_note_off(int channel,int note, int velocity)
{
	char midi_channels[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
								 0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
	char buf[4];


	char channelch=0;

	channelch=midi_channels[channel];
	channelch|=0x80;

	if(note<0)
		note=0;

	if(note>127)
		note=127;

	if(velocity<0)
		velocity=0;

	if(velocity>127)
		velocity=127;

	sprintf(buf,"%c%c%c",channelch,note,velocity);
	send_to_midi(buf,3);

}

