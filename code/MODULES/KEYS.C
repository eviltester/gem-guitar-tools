#include "keys.h"

int WindowLeftKey=180;
int WindowRightKey=178;
int WindowUpKey=183;
int WindowDownKey=175;
int UndoKey=158;
int HelpKey=157;
int ReturnKey=227;
int EscapeKey=254;

void do_keys(int shift_status, int key_code)
{

int scan_code=key_code ^ 0xFF00;
int ascii_code=key_code ^ 0x00FF;



	scan_code = scan_code >> 8;


	switch(scan_code)
	{

	case 157:
		help_key();
		break;
	
	default:
/*		sprintf(Gtemp_string,"[3][shift %d, scan %d, ascii %d][OK]",
					shift_status,scan_code,ascii_code);

		form_alert(1,Gtemp_string);*/
		break;
	}
}