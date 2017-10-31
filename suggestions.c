#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dict.h"
#include "ispell.h"

static int c;
/*word of same length will be tested*/

static int myCompare (const void * a, const void * b)
{
	const char *a1 = *(const char **)a;
	const char *a2 = *(const char **)b;
    return strcmp (toLower(a1), toLower(a2));
}
 
void sort(const char *arr[], int n)
{
    qsort (arr, n, sizeof (const char *), myCompare);
}
void printwords(Dict *d) {
	int j;
    sort((const char**)d->words, d->i);
    printf("how about:");
   	for(j = 0;j < d->i-1;j++)
   		if(strcmp(d->words[j], d->words[j+1]) != 0)
			printf("%s, ", d->words[j]);
	printf("%s\n", d->words[j]);
}


void compareEqualString(Dict *d, char *s, char *str) {
	char *s1 = (char *)malloc(strlen(s));
	char *s2 = (char *)malloc(strlen(str));
	s1 = toLower(s);
	s2 = toLower(str);
	int lower = checkLower(str);
	if(strcmp(s1, s2) == 0){
		d->i = -1;
		// printf("fdjkgf\n");
		return;
	}
	int i = mystrcmp(s2, s1, 1);
	if(i == 1){
		s1 = toUpper(str, s);
		// printf("one%s\n",s1);
		d->words[(d->i)++] = s1;
		// printf("fdasdfs\n");	
	}
}
void compareUnEqualString(Dict *d, char *s, char *str, int flag) {
	char *s1 = (char *)malloc(strlen(s));
	char *s2 = (char *)malloc(strlen(str));
	int c = 0;
	s1 = toLower(s);
	s2 = toLower(str);
	for (int j = 0; s2[j+c]; j++)
	{
		if(s2[j + c] != s1[j]){
			c++;
			j--;
		}
		if(c > 1)
			break;
	}
	if(c <= 1){
		
		if(flag == 1){
			s1 = toUpper(str, s);
			// printf("two%s\n",s1);
			d->words[(d->i)++] = s1;
		
		}
		else if(flag == -1){
			s1 = toUpper(s, str);
			// printf("three%s\n",s1);
			d->words[(d->i)++] = s1;
		}
	}
}
void divideWords(Dict *d, char *s) {
	char *p1, * p2;
	int i = 1;
	while(i < strlen(s) - 1) {
		p1 = strdup(s);
		p2 = strdup(s+i);
		p1[i] = '\0';
		if(Search(d, toLower(p1)) != NULL && Search(d, toLower(p2)) != NULL) {
			p1[i] = ' ';p1[i+1] = '\0';
			// printf("hgfhj\n");
			strcpy(d->words[(d->i)], p1);
			strcat(d->words[(d->i)++], p2);
			p1[i] = '-';
			strcpy(d->words[(d->i)], p1);
			strcat(d->words[(d->i)++], p2);
		}
		free(p1);
		free(p2);
		i++;
	}
}
void fprintwords(Dict *d, int rows) {
	int j, residual;
	rows-= 7;
    sort((const char**)d->words, d->i);
    if(d->i < rows){
    	j = 0;
    if(d->i != 0){
   	for(;j < d->i-1;j++){
			printf("%d)%s\n", j, d->words[j]);
	}
	printf("%d)%s\n",j,  d->words[j]);
	j++;
}
	while(j <= rows){
		printf("\n");
		j++;
	}
   		

	}
	else{
		j = 0;
		residual = d->i - rows;
		while(residual > 0) {
			printf("%d)%s\t\t%d)%s\n", j, d->words[j], j+rows, d->words[j+rows]);
			residual--;
			j++;
		}
		for(;j < rows-1;j++){
   			// if(strcmp(d->words[j], d->words[j+1]) != 0)
				printf("%d)%s\n", j, d->words[j]);
	   	}
		printf("%d)%s\n\n",j,  d->words[j]);
	}

}	
void suggest(Dict *d, char *p) {
	int l1 = strlen(p), l2;
	if(l1 > 30)
		return;
	divideWords(d, p);
	for (int j = 0; j < d->size; ++j) {
		l2 = strlen(d->table[j].a);
		
		if(l1 == l2){
			compareEqualString(d, d->table[j].a, p);
			if(d->i == -1)
				break;
		}
		else if((l1-l2) == 1)
			compareUnEqualString(d, d->table[j].a, p, l1 - l2);
		else if((l1-l2) == -1)
			compareUnEqualString(d, p, d->table[j].a, l1 - l2);
	}
}
void Master(Dict *d, char *str) {
	char *s, *p;
	int l1, l2, i = 0;

	l1 = strlen(str);
	if(l1 == 1){
		printf("ok\n");
		return;
	}
	// if(checkUpper(str) >= 2){
	// 	p = toLower(str);
	// }
	if(Search(d, str) != NULL)
		printf("ok\n");
	
	else{
		p = strtok(str, delim);
		// printf("delim = %s\n", p);
		do{
			if(p == NULL)
				continue;
			l1 = strlen(p);
			if(l1 == 1){
				i = 1;
				printf("ok\n");
				continue;
			}
			suggest(d, p);
			if(d->i > 0){
				i = d->i;
				printwords(d);
				reallocWords(d);	
				// printf("fjkna%d\n", d->i);
			}
			else if(d->i == -1){
				printf("ok\n");
				reallocWords(d);
				i = d->i;
			}
			else
				printf("not found\n");
			
		}while((p = strtok(NULL, delim)) != NULL);	
	}
}

int FMaster(Dict *d, char *str) {
	int l1 = strlen(str);
	char *p;
	// reallocWords(d);
	if(Search(d, str) == NULL && l1 != 1){
		p = strdup(str);
		suggest(d, p);

		if(d->i > 0){
			
			return SUGG;
		}
		else if(d->i == 0)
			return INCORRECT;

	}
	return CORRECT;
}

int mystrcmp(char *s1, char *s2, int miss){
	int l1, l2;
	l1 = strlen(s1);
	l2 = strlen(s2);
	if(l1 != l2)
		return 0;
	int *a, *b, variance = 0;
	a = (int *)malloc(sizeof(int) * l1);
	b = (int *)malloc(sizeof(int) * l1);
	for(int j = 0;j < l2;j++){
		if(s1[j] != s2[j]){
			// printf("fjhgfj\n");
			miss--;
		}
		if(s1[j] != s1[j+1] && 	s1[j] == s2[j+1] && s1[j+1] == s2[j])
			variance++;
	}

	if(miss >= 0)
		return 1;
	else if(variance == 1 && miss == -1)
		return 1;
	else
		return 0;
}


char* toLower(const char *s) {
	char *new = (char *)malloc(strlen(s));
	for (int i = 0; i < strlen(s); ++i)
	{
		if(s[i] >= 'A' && s[i] <= 'Z'){
			new[i] = s[i] + 'a' - 'A';
		}
		else
			new[i] = s[i];

	}
	return new;
}
int checkUpper(char *p) {
	int count = 0;
	for (int i = 0; i < strlen(p); ++i)
	{
		if(p[i] >= 'A' && p[i] <= 'Z')
			count++;
	}
	return count;
}
/*returns 1 if first letter of the input word is Capital Alphabet, 0 if not*/
int allUpper(char * p) {
	for (int i = 0; i < strlen(p); ++i)
	{
		if(p[i] >= 'a' && p[i] <= 'z')
			return 0;
	}
	return 1;
}
int checkLower(char *p) {
	for (int i = 1; i < strlen(p); ++i)
	{
		if(p[i] >= 'A' && p[i] <= 'Z')
			return 0;
	}
	return 1;
}
char *toUpper(char *str, char *s) {
	char *s1, *s2;
	s1 = strdup(s);
	s2 =strdup(s);
	if(str[0] >= 'A' && str[0] <= 'Z' && (s1[0] < 'A' || s1[0]>'Z'))
	s1[0] -= 32;
	// printf("%d\n", allUpper(str));
	if(allUpper(str))
	for (int i = 1; s1[i] != '\0'; ++i)
	{
		if(s1[i] >= 'a' && s1[i] <= 'z'){
			s1[i] -= 32;

		}
	}
	// printf("%s\n", s1);

		return s1;
}