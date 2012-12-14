#include "paths.h"
#define DEMO

char *prog_name_path;
char scales_path[FMSIZE];
char defaults_path[FMSIZE];
char tunings_path[FMSIZE];
char printer_path[FMSIZE];
char paths_file[FMSIZE];


/* save paths */
/* paths form */

/* load paths initially */

void load_paths( void )
{
#ifndef DEMO
		int curr_drive;
		int error;

		FILE *fp;

		curr_drive=getdsk();
	
		prog_name_path=(char *)malloc(FMSIZE);
		error=getcd(curr_drive+1,prog_name_path);

		if(error!=0)
				fatal_error("cannot find current program path");
				
		strcat(prog_name_path,"\\");
		strcpy(paths_file,prog_name_path);
		strcat(paths_file,"paths.man");
		
		fp=fopen(paths_file,"r");

		if(!fp)
		{
			error("Cannot find paths, defaults used");
		}
		else
			{
			if(!feof(fp))
			{
			fscanf(fp,"%s",scales_path);
			if(!feof(fp))
			{
			fscanf(fp,"%s",defaults_path);
			if(!feof(fp))
			{
			fscanf(fp,"%s",tunings_path);
			if(!feof(fp))
			{
			fscanf(fp,"%s",printer_path);
			}
			}
			}
			}

			fclose(fp);

	/*		error(scales_path);
			error(defaults_path);
			error(tunings_path);
	*/

			/*	test paths */
			fp=fopen(scales_path,"r");
			if(!fp)
				{
/*				error("Scales path is unavailable");
*/				scales_path[0]='\0';
				}

			fp=fopen(defaults_path,"r");
			if(!fp)
				{
/*				error("Defaults path is unavailable");
*/				defaults_path[0]='\0';
				}

			fp=fopen(tunings_path,"r");
			if(!fp)
				{
/*				error("Tunings path is unavailable");
*/				tunings_path[0]='\0';
				}

			fp=fopen(printer_path,"r");
			if(!fp)
				{
/*				error("Printer path is unavailable");
*/				printer_path[0]='\0';
				}

 		}
#endif
}

void save_paths( void )
{
#ifndef DEMO
	FILE *fp;

	fp=fopen(paths_file,"w");

	if(!fp)
	{
		error("Cannot save paths");
	}
	else
		{
		fprintf(fp,"%s\n",scales_path);
		fprintf(fp,"%s\n",defaults_path);
		fprintf(fp,"%s\n",tunings_path);
		fprintf(fp,"%s\n",printer_path);
		fclose(fp);
		}

#endif
}


void paths_form(CURRENT_STATUS *cs)
{
	OBJECT *dlog;
	int result,lresult;
	int i;

	char scalesp[40];
	char defsp[40];
	char tunsp[40];
	char prnsp[40];

	char backup_scalesp[FMSIZE];
	char backup_defsp[FMSIZE];
	char backup_tunsp[FMSIZE];
	char backup_prnsp[FMSIZE];
	char filename[FMSIZE];

/*form_alert(1,"[1][Paths start][ok]");*/

	strcpy(backup_scalesp,scales_path);
	strcpy(backup_defsp,defaults_path);
	strcpy(backup_tunsp,tunings_path);
	strcpy(backup_prnsp,printer_path);

	
	for(i=0;i<35 && backup_scalesp[i]!='\0';i++)
	{
		scalesp[i]=backup_scalesp[i];
	}
	scalesp[i]='\0';

	for(i=0;i<35 && backup_defsp[i]!='\0';i++)
	{
		defsp[i]=backup_defsp[i];
	}
	defsp[i]='\0';

	for(i=0;i<35 && backup_tunsp[i]!='\0';i++)
	{
		tunsp[i]=backup_tunsp[i];
	}
	tunsp[i]='\0';

	for(i=0;i<35 && backup_prnsp[i]!='\0';i++)
	{
		prnsp[i]=backup_prnsp[i];
	}
	prnsp[i]='\0';

	Crsrc_gaddr(R_TREE,FDefault_paths,&dlog);

	set_tedinfo(dlog,ScalesPathLow,scalesp);
	set_tedinfo(dlog,TuningPathLow,tunsp);
	set_tedinfo(dlog,DefaultsPathLow,defsp);
	set_tedinfo(dlog,PrinterPathLow,prnsp);

	do{
		lresult=0;
		result=handle_dialog(dlog,0,0);

		if(result==ScalesPathLow)
		{
		lresult=select("SCL",filename,"NEW SCALES PATH");
		if(lresult)
			{
			strcpy(backup_scalesp,filename);
			for(i=0;i<35 && backup_scalesp[i]!='\0';i++)
			{
				scalesp[i]=backup_scalesp[i];
			}
			scalesp[i]='\0';
			set_tedinfo(dlog,ScalesPathLow,scalesp);
			}
		}

		if(result==TuningPathLow)
		{
		lresult=select("TUN",filename,"NEW TUNINGS PATH");
		if(lresult)
			{
			strcpy(backup_tunsp,filename);
			for(i=0;i<35 && backup_tunsp[i]!='\0';i++)
			{
				tunsp[i]=backup_tunsp[i];
			}
			tunsp[i]='\0';
			set_tedinfo(dlog,TuningPathLow,tunsp);
			}
		}

		if(result==DefaultsPathLow)
		{
		lresult=select("DEF",filename,"NEW DEFAULTS PATH");
		if(lresult)
			{
			strcpy(backup_defsp,filename);
			for(i=0;i<35 && backup_defsp[i]!='\0';i++)
			{
				defsp[i]=backup_defsp[i];
			}
			defsp[i]='\0';
			set_tedinfo(dlog,DefaultsPathLow,defsp);
			}
		}

		if(result==PrinterPathLow)
		{
		lresult=select("DRV",filename,"NEW PRINTER PATH");
		if(lresult)
			{
			strcpy(backup_prnsp,filename);
			for(i=0;i<35 && backup_prnsp[i]!='\0';i++)
			{
				prnsp[i]=backup_prnsp[i];
			}
			prnsp[i]='\0';
			set_tedinfo(dlog,PrinterPathLow,prnsp);
			}
		}
		
		if(result==PathsSAVE)
			save_paths();

	}while(result!=PathsOK && result!=PathsCANCEL);

	if(result==PathsOK)
	{
	strcpy(scales_path,backup_scalesp);
	strcpy(defaults_path,backup_defsp);
	strcpy(tunings_path,backup_tunsp);
	strcpy(printer_path,backup_prnsp);
	}

}