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

int hashcompare(long int h1, long int h2) {
	char str1[512], str2[512];
	sprintf(str1, "%ld", h1);
	sprintf(str2, "%ld", h2);
	int miss = 0,i;
	if(strlen(str1) != strlen(str2))
		return 0;
	for (i = 0; str1[i]; i++)
	{
		if(str1[i] != str2[i])
			miss++;
	}
	if(miss < strlen(str1)/2)
		return 1;
	else
		return 0;
}
