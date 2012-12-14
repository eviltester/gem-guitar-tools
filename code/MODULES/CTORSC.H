#ifndef _CTORSC_H_
#define _CTORSC_H_

#include <stdio.h>
#include <gemlib.h>
#include <string.h>

extern int CrsrcFlag;

extern short *CrsrcImgData;
extern TEDINFO *CrsrcTedinfo;
extern ICONBLK *CrsrcIconblk;
extern BITBLK *CrsrcBitblk;
extern char **CrsrcFreeString;
extern BITBLK **CrsrcFreeImage;
extern OBJECT *CrsrcObject;
extern OBJECT **CrsrcTree;

extern void CrsrcEnable(void);
extern int Crsrc_load(char *);
extern int Crsrc_free(void);
extern int Crsrc_obfix(OBJECT *, int );
extern int Crsrc_gaddr(int , int , void *);
extern int Crsrc_saddr(int , int , void *);




#endif
