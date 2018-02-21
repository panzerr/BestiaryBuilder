#include "belement.h"


void belt_print(belement toprint)
{
  int i = 0;
  printf("%s : %s level : %d \n",toprint.name,toprint.path,toprint.level);
  for (i = 0 ; i < toprint.subelements_size ; i++)
    {
      printf("|");
      belt_print(toprint.subelements[i]);
    }
  
}

belement belt_create(char * name, char * path)
{
  belement ret;
  ret.name = name;
  ret.path = path;
  ret.subelements = NULL;
  ret.subelements_size = 0;
  ret.level = 0;
  return ret;
}



int belt_add_aux(belement * root,belement to_add,int is_cr)
{
  belement * new;
  belement * prev;
  int i,j,already_added;
  int ret;
  ret = 0;
  i = 0; j = 0;
  already_added = -1;
  if (root == NULL)
    exit(1);
  prev = root->subelements; // we will need to free this after
  new = malloc((root->subelements_size+1) * sizeof(belement));
  for ( i = 0 ; i < root->subelements_size ; i ++)
    {
      if(is_cr != 0 && already_added != 0 && strcmp(to_add.name,prev[i].name) < 0)
	{
	  new[j] = to_add;
	  ret = j;
	  j++;
	  new[j] = prev[i];
	  j++;
	  already_added = 0;
	}
      else if (is_cr == 0 && already_added != 0 && atoi(to_add.name) < atoi(prev[i].name) )
	{
	  new[j] = to_add;
	  ret = j;
	  j++;
	  new[j] = prev[i];
	  j++;
	  already_added = 0;	  
	}
       else
	{
	  new[j] = prev[i];
	  j++;
	}
      // printf("i : %d, j : %d \n",i,j);
    }
  if (already_added != 0)
    {
      new[j] = to_add;
      ret = j;
    }
  root->subelements = new;
  free(prev);
  root->subelements_size ++;
  return ret;
}

int belt_add(belement * root, belement to_add)
{
  return belt_add_aux(root,to_add,-1);
}


void belt_add_at_cr(belement * root,belement to_add ,int cr)
{
  int i;
  belement new;
  char * crstring;
  // if the CR already exists then add to the existing CR value
  for (i = 0 ; i < root->subelements_size ; i++)
    {
      if( cr == atoi(root->subelements[i].name))
	{
	  belt_add(root->subelements+i,to_add);
	  return;
	}
    }
  //else we need to create the cr first
  crstring = malloc (4*sizeof(char));
  sprintf(crstring,"%d",cr);
  new = belt_create(crstring,"no_path");
  belt_add(&new,to_add);
  belt_add_aux(root,new,0);
}


//Todo : put in a utilities file
int belt_get_max_level(belement * table,int size)
{
  int i;
  int max = 0;
  for (i = 0 ; i < size ; i++)
    {
      if (table[i].level > max)
	{max = table[i].level;}
    }
  return max;
}


void belt_set_level(belement * root)
{
  int i;
  if(root->subelements_size == 0)
    {
      root->level = 0;
    }
  else
    {
      for ( i = 0 ; i < root->subelements_size; i++)
	{
	  belt_set_level(root->subelements+i);
	  root->level = belt_get_max_level(root->subelements,root->subelements_size)+1;
	  printf("level of %s is %d \n",root->name,root->level);
	}
    }

}


void belt_write(belement root)
{
  if (root.level == 0)
     return;
  FILE *fp;
  int i,j;
  char actualpath [MAX_PATH+1];
  if((fp = fopen(root.path, "w+")) == NULL)
    {
      perror("error");
      printf("\n cannot open %s",root.path);
    }
  // base
  fprintf(fp,"<!DOCTYPE html>\r\n<head></head>\r\n<body>\r\n ");
  // print all elements links
  if (root.level != 2)
    {
      for ( i = 0 ; i < root.subelements_size ; i++)
	{
	  GetFullPathName(root.subelements[i].path,MAX_PATH+1,actualpath,NULL);
	  fprintf(fp,"<a href=\"file:///%s\">%s</a><br/>\r\n",actualpath,root.subelements[i].name);
	}
    }
  else
    {
      //when we are at cr level
      for ( i = 0 ; i < root.subelements_size ; i++)
	{
	  // print title
	  fprintf(fp,"<h5>CR : %s</h5>\n",root.subelements[i].name);
	  //for each final element print the link 
	  for ( j = 0 ; j < root.subelements[i].subelements_size ; j++)
	    {
	      GetFullPathName(root.subelements[i].subelements[j].path,MAX_PATH+1,actualpath,NULL);
	      fprintf(fp,"<a href=\"file:///%s\">%s</a><br/>\r\n",actualpath,root.subelements[i].subelements[j].name);
	    }
	}
    }
  fprintf(fp,"</body>");
  fclose(fp);
  for (i = 0 ; i < root.subelements_size ; i++)
    {
      belt_write(root.subelements[i]);
    }
}


void space_to_underscore(char * tochange)
{
  int i = 0;
  while ( tochange[i] != '\0')
    {
      if(tochange[i] == ' ')
	{tochange[i] = '_';}
      i++;
    }

}

void underscore_to_space(char * tochange)
{
  int i = 0;
  while ( tochange[i] != '\0')
    {
      if(tochange[i] == '_')
	{tochange[i] = ' ';}
      i++;
    }
}

void belt_save_aux(belement elt,FILE * fp)
{
  int i;
  char * formatname;
  char * formatpath;
  formatname = strdup(elt.name);
  formatpath = strdup(elt.path);
  space_to_underscore(formatname);
  space_to_underscore(formatpath);
  fprintf(fp,"%s : %s : %d\r\n",formatname,formatpath,elt.subelements_size);
  for (i = 0 ; i < elt.subelements_size ; i++)
    {
      belt_save_aux(elt.subelements[i],fp);
    }
  free(formatname);
  free(formatpath);
}

void belt_save(belement root)
{
  FILE * fp;
  fp = fopen(".\\data.save","w+");
  belt_save_aux(root,fp);
  fclose(fp);
}


belement belt_load_aux(FILE * fp)
{
  char name[40];
  char path[100];
  char * allocname;
  char * allocpath;
  int sons;
  int i;
  int sizename = 0,sizepath = 0;
  belement ret;
  fscanf(fp,"%s : %s : %d\r\n",name,path,&sons);
  underscore_to_space(name);
  underscore_to_space(path);
  //  printf("red : %s %s %d \n",name,path,sons);
  sizename = strlen(name)+1;
  sizepath = strlen(path)+1;
  allocname = malloc( sizename * sizeof(char));
  allocpath = malloc( sizepath * sizeof(char));
  memcpy(allocname,name,sizename);
  memcpy(allocpath,path,sizepath);
  ret = belt_create(allocname,allocpath);
  for ( i = 0 ; i < sons ; i++)
    {
      belement to_add;
      to_add = belt_load_aux(fp);
      belt_add(&ret,to_add);
    }
  return ret;
}

belement belt_load()
{
  FILE * fp;
  belement ret;
  fp = fopen(".\\data.save","r");
  ret = belt_load_aux(fp);
  fclose(fp);
  return ret;
}


int file_exists(const char * filename)
{
  FILE * file;
  if ((file = fopen(filename, "r")) == NULL)
    {
        return 1;
    }
  fclose(file);
  return 0;
}




/* create a file if does not exist and then load it */
belement belt_init()
{
  belement ret;
  char * name;
  char * path;
  if ( file_exists(".\\data.save") == 0)
    {
      printf("load file \n");
      ret = belt_load();
      belt_set_level(&ret);
      return ret;
    }
  else
    {
      name = malloc(6 * sizeof(char));
      name = "index";
      path = malloc(14 * sizeof(char));
      path = ".\\index.html";
      ret = belt_create(name,path);
      return ret;
    }
}

