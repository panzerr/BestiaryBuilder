#ifndef _UTILITIES
#define _UTILITIES
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>


char * getname(char * filepath);

void removelastslash(char * toshorten);

char * filedest(char * filename);

void filecpy(char * src, char * dest);


#endif
