all : main.c belement.h belement.c utilities.h utilities.c
	gcc -o bbuilder.exe -Wall main.c belement.c utilities.c
