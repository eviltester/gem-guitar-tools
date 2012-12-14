#include "fsel.h"

char last_filenames[10][FNSIZE];	/* space for filenames with 10 extensions */
char *stored_paths[26];			/* the last paths on each drive */
int last_drive;					/* the number of the last drive */



/*	extract_extension
	given the empty string expansion (minimum of 4 chars, 3 + NULL)
	copy the last three chars of from into it
*/
void extract_extension(char *extension, char *from)
	{
		while(*from!='.')
			from++;					/*	skip until fullstop */

		from++;						/* skip fullstop */
		strcpy(extension,from);	/* copy data */
	}


/*	init_paths
	initialise the last_filenames and stored paths to NULL
	store the current path and current drive
*/

void init_paths(void)
	{
		char *cp;
		int loop;
		int curr_drive;
		int error;

		curr_drive=getdsk();

		for(loop=0; loop<26; loop++)
			{
				if(loop==curr_drive)
					{
					cp=(char *)malloc(FMSIZE);
					
					error=getcd(curr_drive+1,cp);

					if(error!=0)
						exit(1);
				
					strcat(cp,"\\");					
					stored_paths[loop]=cp;
					}
				else
					stored_paths[loop]=NULL;
			}

		for(loop=0; loop<10; loop++)
			last_filenames[loop][0]='\0';

		last_drive=curr_drive;
	}

int select(char *file_extension, char *returned_path, char *fseltitle)
	{
		char local_path[FMSIZE];
		char local_filename[FNSIZE];
		char local_extension[4];
		char *cp;
	/*	char tempbuf[200];*/
		int loop, finloop, curr_drive, count,ret;
		short button;
		finloop=0;

		ret=0;

		for(loop=0; loop<10 && 
						last_filenames[loop][0]!='\0' &&
						finloop==0;
			 loop++)
			{
				extract_extension(local_extension, last_filenames[loop]);

				if(!strcmp(local_extension, file_extension))
					finloop=1;
			}

		if(finloop)
			loop--;
		else
			{
			strcpy(last_filenames[loop],"DEFAULT.");
			strcat(last_filenames[loop],file_extension);
			}

		strcpy(local_filename,last_filenames[loop]);

		curr_drive=last_drive;

		strcpy(local_path, stored_paths[curr_drive]);

		count=strlen(local_path);
		while(local_path[count] != '\\')	
			count--;
		local_path[count+1]='*';
		local_path[count+2]='.';
		strcpy(&(local_path[count+3]),file_extension);

		fsel_exinput(local_path, local_filename, &button, fseltitle);

		count=strlen(local_path);
		while(local_path[count] != '\\')	
			count--;
		local_path[count+1]='\0';

		if(strlen(local_filename) == 0)
			button=0;

/*		sprintf(tempbuf,"[1][button = %d ok][ok]",button);
		form_alert(1,tempbuf);
*/
		if(button)
			{
				curr_drive=local_path[0]-'A';

				if(stored_paths[curr_drive] == NULL)
					{
						cp=(char *)malloc(FMSIZE);

						strcpy(cp, local_path);
						stored_paths[curr_drive]=cp;
					}
				else
					{
						strcpy(stored_paths[curr_drive],local_path);
					}

						last_drive=curr_drive;

						strcpy(last_filenames[loop],local_filename);

						strcpy(returned_path, local_path);
						strcat(returned_path, local_filename);

			}

/*		sprintf(tempbuf,"[1][the returned path is | %s| ok][ok]",returned_path);
		form_alert(1,tempbuf);*/
		ret=button;
		return(ret);
	}			