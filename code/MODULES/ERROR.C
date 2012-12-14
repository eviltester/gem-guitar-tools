#include "error.h"

void alerror(int fatal, char *mess)
	{

	if(gem_on)
		{
		if(fatal)
			{
			sprintf(Gtemp_string,"[1][FATAL ERROR:|%s][ EXIT ]",mess);
			form_alert(1,Gtemp_string);
			fcloseall();
			exit(1);
			}
		else
			{
			sprintf(Gtemp_string,"[1][ERROR:|%s][ OK ]",mess);
			form_alert(1,Gtemp_string);
			}
		}
	else
		{
		if(fatal)
			{
			sprintf(Gtemp_string,"FATAL ERROR\n%s\n EXIT ",mess);
			printf(Gtemp_string);
			getchar();
			exit(1);
			}
		else
			{
			sprintf(Gtemp_string,"ERROR\n%s\n OK ",mess);
			printf(Gtemp_string);
			getchar();
			}
		}
	}
