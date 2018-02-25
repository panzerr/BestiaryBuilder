#include "utilities.h"



char * getname(char * filepath)
{
  int i;
  int start = 0;
  int end = 0;
  int size = 0;
  char * retvalue;
  //find the substring
  while ( filepath[i] != '\0')
    {
      if (filepath[i] == '\\')//on est sous windows...
	{
	  start = i;
	}
      if (filepath[i] == '.')
	{
	  end = i;
	}
      i++;
    }
  //create the new string
  if (i < 5)
    {return NULL;}
  size = end - 1 - start;
  retvalue = malloc(size * sizeof(char));
  // copy
  memcpy(retvalue, (filepath + start + 1), size);
  retvalue[size] = '\0';
  return retvalue;
}

void removelastslash(char * toshorten)
{
  int i = 0;
  int lastslash = 0;
  while(toshorten[i] != '\0')
    {
      if(toshorten[i] == '\\')
	{lastslash = i;}
      i++;
    }
  toshorten[lastslash+1] = '\0';
}

//  generate the path to the bestiary, TODO put in a separate file
char * filedest(char * filename)
{
  char * pathbase = ".\\Bestiary\\";
  char * fileterm = ".html";
  char * ret;
  int size = strlen(filename);
  size += strlen(pathbase);
  size += strlen(fileterm);
  size ++; // null byte
  ret = malloc(size * sizeof(char));
  strcpy(ret,pathbase);
  strcat(ret,filename);
  strcat(ret,fileterm);
  return ret;
  
}




void filecpy(char * file1,char * file2)
{
    FILE *fp1, *fp2;
    char ch;

    if ((fp1 = fopen(file1, "r")) == NULL)
    {
      //      debug_print("\nFile cannot be opened.");
        return;
    }
    else
    {
      //debug_print("\nFile opened for copy...\n ");
    }
    fp2 = fopen(file2, "w+");
    while ((ch = fgetc(fp1)) != EOF)
    {
            // Copying file character by character
        fputc(ch, fp2);
    }
    fclose(fp1);
    fclose(fp2);
}
