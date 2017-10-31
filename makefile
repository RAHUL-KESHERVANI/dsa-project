ispell: ispell.o suggestions.o dict.o
	cc ispell.o suggestions.o dict.o -o project
ispell.o: ispell.c ispell.h dict.h file.h
	cc -c ispell.c
suggestions.o: suggestions.c ispell.h dict.h
	cc -c suggestions.c	
dict.o: ispell.c dict.h
	cc -c dict.c