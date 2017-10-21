#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dict.h"
#include "ispell.h"
#include <math.h>
static int i = 0; 
/*word of same length will be tested*/
// char *delim = {	};
// void printwords() {
// 	for(int j = 0;j < i-1;j++)
// 		printf("%s, ", words[i]);
// 	printf("\n");
// }
void nearest_hash(Dict *d, char *str, int miss) {
	long int h1, h2, h = hash(str);
	int l = strlen(str);
	for (int j = 0; j < d->size; ++j) {
			if(d->table[j].len != l || str[0] != d->table[j].a[0])
				if(!mystrcmp(str, d->table[j].a, 1))
				continue;
			h1 = d->table[j].h;
			if(h-h1 <= miss * 26 * (pow(10,strlen(str)/3)) ){
				printf("%s\n", d->table[j].a);
				i++;
			}
	}
}
int Master(Dict *d, char *str) {
	char *s;
	words = (char**)malloc(20 * sizeof(char*));
	s = Search(d, str);
	if(s != NULL)
	return i;
	nearest_hash(d, str, 2);
	// removeletter(str);
	// while(s = removeletter(str) != NULL)
	// nearest_hash(d, s, 2);
	if(i == 0)
		return -1;
}
// int in_sqnce(){

// }
char *removeletter(char *str){
	char *s = (char*)malloc(strlen(str));
	static int j = 0;
	int flag = 0;
	if(j == strlen(str) - 1)
		return NULL;
	for (int k = 0; k < strlen(str); ++k)
	{
		if(k == j){
			flag = 1;
			continue;
		}
		s[k-flag] = str[k];

	}
	j++;
	return s;
}
int mystrcmp(char *s1, char *s2, int miss){
	int l1, l2;
	l1 = strlen(s1);
	l2 = strlen(s2);
	if(l1 != l2)
		return 0;
	int a[20][26]={0};
	for(int j = 0;j < l1;j++)
		a[j][(int)s1[j] - 65]++;
	for(int j = 0;j < l2;j++)
		a[j][(int)s2[j] - 65]--;
	for (int k = 0; k < 20; ++k)
	{
		for(int j = 0;j < 26;j++)
			if(a[k][j] != 0)
				miss--;
	}
	if(miss >= 0)
		return 1;
	else
		return 0;
}

// char *variance(char *str) {

// }

// char* toLower(char *s);
// char *toUpper(char *s);
