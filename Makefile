all : main.c belement.h belement.c
	gcc -o bbuilder.exe -Wall main.c belement.c
