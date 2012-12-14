#include <stdio.h>
#include <osbind.h>

static char hexify(char ch)
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

static void send_to_midi(char *ch)
{
	int duff=0x00FF;

	while(*ch!='\0')
			{
			duff&=*ch;
			Bconout(3,duff);
			duff=0x00FF;
			ch++;
			}
}	
static void midi_play(char *str)
	{

/*	char midi_channels[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
								 0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
*/

	char *chp=(char *)malloc(strlen(str)), *chp2;
	char ch1,ch2;
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


	send_to_midi(chp2);

	}

static void midi_off(int channel)
	{

	char midi_channels[]={'0','1','2','3','4','5','6','7','8','9',
								 'A','B','C','D','E','F'};
	char buf[4];

	char channelch;

	channelch=midi_channels[channel];


/*	channelch=midi_channels[channel]|0x80;
*/
	sprintf(buf,"8%c6000",channelch);
	midi_play(buf);
	}


static void midi_note_on(int channel,int note, int velocity)
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
	send_to_midi(buf);

}

static void midi_note_off(int channel,int note, int velocity)
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
	send_to_midi(buf);

}


void main()
{

	/*midi_play("906050");*/
	midi_note_on(0,60,50);
	getchar();
	midi_note_off(0,60,50);
	/*midi_play("806050");*/
	/*midi_off(0);*/

}