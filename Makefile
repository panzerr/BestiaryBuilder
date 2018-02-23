all : main.c belement.h belement.c
	gcc -o bbuilder.h -Wall main.c belement.c
