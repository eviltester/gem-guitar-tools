#include "tuning.h"

/*temporary tunings */

char def_tuning_name[]="DEFAULT: Standard E Tuning";

tuning def_tuning={def_tuning_name,{3,3,2,2,2,1},{7,2,10,5,0,7}};

NODE known={NULL,&def_tuning,NULL};

NODE *known_tunings=&known;

tuning *current_tuning=&def_tuning;

