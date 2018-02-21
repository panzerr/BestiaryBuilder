#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "belement.h"
#include <unistd.h>

#define DEBUG


void debugmess(char * mess)
{
#ifdef DEBUG
  printf("%s\n",mess);
#endif
  return;
}

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
  printf( "%d %d \n", start,size);
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
  debugmess(ret);
  return ret;
  
}




void filecpy(char * file1,char * file2)
{
    FILE *fp1, *fp2;
    char ch;
    int pos;

    if ((fp1 = fopen(file1, "r")) == NULL)
    {
        printf("\nFile cannot be opened.");
        return;
    }
    else
    {
        printf("\nFile opened for copy...\n ");
    }
    fp2 = fopen(file2, "w+");
    fseek(fp1, 0L, SEEK_END);   // File pointer at end of file
    pos = ftell(fp1);
    fseek(fp1, 0L, SEEK_SET);   // File pointer set at start
    while (pos--)
    {
        ch = fgetc(fp1);    // Copying file character by character
        fputc(ch, fp2);
    }
    fclose(fp1);
    fclose(fp2);
}

// honestly this should not be here
void add_at_end(belement * index,belement to_add)
{
  int userinput = 0;
  char name[40];
  char * allocname;
  belement * new;
  int size;
  int i;
  int tmp;
  do
    {
      if(index->level == 2)
	{
	  new = malloc(sizeof(belement));
	  printf("CR ?\n");
	  scanf("%d",&tmp);
	  belt_add_at_cr(index,to_add,tmp);
	  return;
	}
      printf(" 0 : confirm\n");
      for ( i = 0 ; i < index->subelements_size ; i++)
	{
	  printf(" %d : %s \n", i+1, index->subelements[i].name);
	}
      i++;
      printf("%d : new\n",i);
      scanf("%d",&userinput);
      if(userinput == 0)
	{
	  new = malloc(sizeof(belement));
	  printf("CR ?\n");
	  scanf("%d",&tmp);
	  belt_add_at_cr(index,to_add,tmp);
	  return;
	}
      if(userinput < i)
	{
	  add_at_end(index->subelements+userinput-1,to_add);
	  return;
	}
      if(userinput == i)
	{
	  new = malloc(sizeof(belement));
	  printf("name ?\n");
	  fflush(stdin);
	  fgets(name,40,stdin);
	  fflush(stdin);
	  size = strlen(name);
	  allocname = malloc(size * sizeof(char));
	  memcpy(allocname,name,size);
	  allocname[size-1] = '\0';
	  printf("created : %s\n",allocname);
	  *new = belt_create(allocname,filedest(allocname));
	  tmp = belt_add(index,*new);
	  add_at_end(index->subelements+tmp,to_add);
	  return;
	}
      printf("wrong option \n");
    }
  while(1);
}





int main(int argc,char * * argv)
{
  int lol;
  char * name;
  char * copypath;
  belement index;
  belement new;
  char file[MAX_PATH+1];
  char programpath[MAX_PATH+1];
  
    if (argc > 1)
    {
      //save the path of the dragged file and move to the program directory
      GetFullPathName(argv[1],MAX_PATH+1,file,NULL);
      GetModuleFileName(NULL,programpath,MAX_PATH+1);
      removelastslash(programpath);
      SetCurrentDirectory(programpath);
      //init
      index = belt_init();
      //build the bestiary element for the new file
      name = getname(file);
      copypath = filedest(name);
      new = belt_create(name,copypath);
      //add the element
      add_at_end(&index,new);
      //copy the file to the bestiary
      filecpy(file,copypath);
      //set the levels in the tree
      belt_set_level(&index);
      //regenerate the html tree
      belt_write(index);
      // save the files
      belt_save(index);

    }
    
      scanf("%d",&lol);
  return 0;
}

