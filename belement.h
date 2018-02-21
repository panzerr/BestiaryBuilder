//datatypes for monsters
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<errno.h>
#include<windows.h>

typedef struct _bestiaryelement
{
  char * name;
  char * path;
  struct _bestiaryelement * subelements;
  int subelements_size;
  int level;
} belement;

void belt_print(belement toprint);

belement belt_create(char * name, char * path);

int belt_add(belement * root,belement to_add);

void belt_add_at_cr(belement * root, belement to_add,int cr);

void belt_set_level(belement * root);

void belt_write(belement root);

void belt_save(belement root);

belement belt_load();

belement belt_init();
