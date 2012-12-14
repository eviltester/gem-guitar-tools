#ifndef _ALMIDI_H_
#define _ALMIDI_H_

#include <stdio.h>
#include <osbind.h>

char hexify( char );
void send_to_midi( char * , int);
void midi_note_off_vel(int, int);

void midi_play( char * );
void midi_note_on( int , int , int );
void midi_note_off( int , int , int );

#endif
