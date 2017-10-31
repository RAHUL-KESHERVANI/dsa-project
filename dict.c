#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dict.h"
#include "ispell.h"
#define MULTIPLIER (37)

long int hash(char *s)
{
    long int h;
    unsigned const char *us;

    /* cast s to unsigned const char * */
    /* this ensures that elements of s will be treated as having values >= 0 */
    us = (unsigned const char *) s;

    h = 0;
    while(*us != '\0') {
        h = h * MULTIPLIER + *us;
        us++;
    } 

    return h;
}

void init(Dict *d, int size){
	d->table = (ele*)malloc(size * sizeof(ele));
	d->size = size;
	d->i = 0;
	d->words = (char **)malloc(sizeof(char *) * 100);
	for (int i = 0; i < 100; ++i)
		d->words[i] = (char *)malloc(30);
}

void Load(Dict *d, char*str){
	long int h;
	static int i = 0;
	h = hash(str);
	d->table[i].len = strlen(str);
	strcpy(d->table[i].a, str);
	d->table[i].h = h;
	i++;
}

void PrintD(Dict *d){
	for(int i = 0;i< d->size;i++)
	printf("%s=%ld\n",(d->table[i]).a, d->table[i].h);
}

char * Search(Dict *d, char*str) {
	long int h = hash(str);
	for (int i = 0; i < d->size; ++i)
	{
		if(d->table[i].h == h){
			return d->table[i].a;
		}
	}
	return NULL;
}

void reallocWords(Dict *d) {
	for (int i = 0; i < 100; ++i)
		free(d->words[i]);
	free(d->words);
	// printf("1\n");
	char **p = (char **)malloc(sizeof(char *) * 100);
	for (int i = 0; i < 100; ++i)
		p[i] = (char *)malloc(30);	
	d->words = p;
	d->i = 0;
}

void DestroyTbl(Dict *d) {
	free(d->table);
	for (int i = 0; i < 100; ++i)
		free(d->words[i]);
	free(d->words);
}