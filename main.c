#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "belement.h"
#include <unistd.h>

#define DEBUG 1

#define debug_print(fmt, ...) \
            do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)


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
  debug_print( "%d %d \n", start,size);
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
  debug_print("%s\n",ret);
  return ret;
  
}




void filecpy(char * file1,char * file2)
{
    FILE *fp1, *fp2;
    char ch;
    int pos;

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
  printf("currently adding %s\n",to_add.name);
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

//get the full path of added files
char * * rebuild_paths(char * * source,int size)
{
  int i;
  char * * retval;
  retval = malloc(size * sizeof(char*));
  for ( i = 0 ; i < size ; i++ )
    {
      retval[i] = malloc((MAX_PATH+1)*sizeof(char));
      GetFullPathName(source[i],MAX_PATH+1,retval[i],NULL);
    }
  return retval;
}


int main(int argc,char * * argv)
{
  int lol,i;
  char * name;
  char * copypath;
  belement index;
  belement new;
  char ** files;
  char programpath[MAX_PATH+1];
    if (argc > 1)
    {
      files = rebuild_paths(argv+1,argc-1);
      GetModuleFileName(NULL,programpath,MAX_PATH+1);
      removelastslash(programpath);
      SetCurrentDirectory(programpath);      
      //init
      index = belt_init();
      for (i = 0 ; i < argc-1 ; i++)
	{
	  //build the bestiary element for the new file
	  name = getname(files[i]);
	  copypath = filedest(name);
	  new = belt_create(name,copypath);
	  //add the element
	  add_at_end(&index,new);
	  //copy the file to the bestiary
	  filecpy(files[i],copypath);
	  //set the levels in the tree
	  belt_set_level(&index);
	}
    }
    else
      {
	index = belt_init();
      }
    //regenerate the html tree
    belt_write(index);
    // save the files
    belt_save(index);
      
    
    //scanf("%d",&lol);
    return 0;
}

