/**************************************************************************
 *   suggestions.c  --  This file is a part of DSA miniproject.                *
 *                                                                        *
 *   Copyright 2017   (C)   Rahul Keshervani                            *
 *                                                                        *
 *   ispell is free software: you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published    *
 *   by the Free Software Foundation, either version 3 of the License,    *
 *   or (at your option) any later version.                               *
 *                                                                        *
 *   ispell is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty          *
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.              *
 *   See the GNU General Public License for more details.                 *
 *                                                                        *
 *   You should have received a copy of the GNU General Public License    *
 *   along with this program.  If not, see http://www.gnu.org/licenses/.  *
 *                                                                        *
 **************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dict.h"
#include "ispell.h"
static int c;

static int myCompare (const void * a, const void * b)
{
	const char *a1 = *(const char **)a;
	const char *a2 = *(const char **)b;
    return strcmp (toLower(a1), toLower(a2));
}
 
int sort(const char *arr[], int n) {
    qsort (arr, n, sizeof (const char *), myCompare);
}

void printwords(Dict *d) {
	int c = 0, i, j;
	sort((const char**)d->words, d->i);
    
    for (i = 0; i < d->i - c; ++i)
    {
    	if(strcmp(d->words[i], d->words[i + 1]) == 0){
    		for(j = i; j < d->i - c;j++)
    			strcpy(d->words[j], d->words[j+1]);
    		c++;
    	}
    }
    d->i -= c; 

    printf("how about:");
   	for(j = 0;j < d->i - 1;j++)
			printf("%s, ", d->words[j]);
	printf("%s\n", d->words[j]);
}

/*prints the suggested words in the file interface*/
void fprintwords(Dict *d, int rows) {
	int j, residual, i, c = 0;
	rows-= 7;
	int count = 0;
    sort((const char**)d->words, d->i);
    for (i = 0; i < d->i - c; ++i)
    {
    	if(strcmp(d->words[i], d->words[i + 1]) == 0){
    		for(j = i; j < d->i - c;j++)
    			strcpy(d->words[j], d->words[j+1]);
    		c++;
    	}
    }
    d->i -= c; 
    
    if(d->i > 10){
    	count = 1;
    }
    if(d->i < rows){
    	j = 0;
	    if(d->i != 0){
		   	for(;j < d->i-1;j++) {
		   		if(count && j < 10)
		   			printf("0");
				printf("%d)%s\n", j, d->words[j]);
			}
			if(count && j < 10)
				printf("0");	
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
			if(count && j < 10)
   				printf("0");
			printf("%d)%s\t\t%d)%s\n", j, d->words[j], j+rows, d->words[j+rows]);
			residual--;
			j++;
		}
		for(;j < rows-1;j++){
   			if(count && j < 10)
   			printf("0");
				printf("%d)%s\n", j, d->words[j]);
	   	}
		printf("%d)%s\n\n",j,  d->words[j]);
	}

}	

void compareEqualString(Dict *d, char *s, char *str) {
	char *s2, *s1, *temp;	
	s1 = strdup(s);
	s2 = strdup(str);

	if(allUpper(s2)){
		temp = s1;
		s1 = toUpper(s2, s1);
		free(temp);
	}
	else if(s2[0] >= 'A' && s2[0] <= 'Z')
		s2[0] += 32;
	
	if(strcmp(s1, s2) == 0){
		d->i = -1;
		return;
	}
	int i = mystrcmp(s2, s1, 1);
	free(s1);
	if(i == 1){
		s1 = toUpper(str, s);
		strcpy(d->words[(d->i)++], s1);
		free(s1);
	}
	free(s2);
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
		if(c > 1)				//two variances between the word and dictionary words are allowed
			break;
	}
	free(s1);
	if(c <= 1){
		
		if(flag == 1){
			s1 = toUpper(str, s);
			strcpy(d->words[(d->i)++],s1);		
		}
		else if(flag == -1){
			s1 = toUpper(s, str);
			strcpy(d->words[(d->i)++],s1);
		}
	}
}
void divideWords(Dict *d, char *s) {
	char *p1, * p2, *p3, *p4, *d1, *d2;
	int i = 1, flag1 = 0, flag2 = 0, l, l2, l1;
	while(i < strlen(s) - 1) {
		p1 = strdup(s);
		p2 = strdup(s+i);
		p1[i] = '\0';
		l1 = strlen(p1);
		l2 = strlen(p2);
		p3 = toLower(p1);
		p4 = toLower(p2);
		flag1 = 0;
		flag2 = 0;
		for (int j = 0; j < d->size; ++j) {
			d1 = d->table[j].a;
			l = strlen(d1);
			d2 = toLower(d1);
			//1st part of the string
			if(l == l1 && strcmp(p3, d2) == 0)
				flag1 = 1;
			//2nd part
			if(l == l2 && strcmp(p4, d2) == 0)
				flag2 = 1;
			if(flag1 && flag2)
				break;
		}
		if(flag1 && flag2) {
			p1[i] = ' ';p1[i+1] = '\0';
			p1 = toUpper(p1, p1);
			p2 = toUpper(p2, p2);
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

/*goes through the dictionary and checks with words with length equal , 1 less or 1 more*/
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
		else if((l1-l2) == 1 )
			compareUnEqualString(d, d->table[j].a, p, l1 - l2);
		else if((l1-l2) == -1)
			compareUnEqualString(d, p, d->table[j].a, l1 - l2);
	}
}
/*Master string suggesting function*/
void Master(Dict *d, char *str) {
	char *s, *p;
	int l1, l2, i = 0;

	l1 = strlen(str);
	if(l1 == 1){
		printf("ok\n");
		return;
	}
	if(Search(d, str) != NULL)
		printf("ok\n");
	
	else{
		p = strtok(str, delim);
		do{
			if(p == NULL)
				continue;
			if(p[strlen(p) - 1] == '\'')
				p[strlen(p) - 1] = '\0';
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
/*Master string suggesting function for files*/
int FMaster(Dict *d, char *str) {
	int l1 = strlen(str);
	char *p;
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
/*if the letter are misplaced 'miss' or less times returns 1 else 0 */
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
		if(s1[j] != s2[j])
			miss--;
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
	int i;
	char *new = (char*)malloc(strlen(s) + 1);
	for (i = 0; i < strlen(s); ++i)
	{
		if(s[i] >= 'A' && s[i] <= 'Z'){
			new[i] = s[i] + 'a' - 'A';
		}
		else
			new[i] = s[i];
	}
	new[i] = '\0';
	return new;
}

/*returns 1 if letter of the input word is Capital Alphabet, 0 if not*/
int allUpper(char * p) {
	for (int i = 0; i < strlen(p); ++i)
	{
		if(p[i] >= 'a' && p[i] <= 'z')
			return 0;
	}
	return 1;
}
/*Captializes the first letter in output string s if there are 1 or more Captial letter in input string str*/
/*if all letter are Captial in str thenoutput string does too*/
char *toUpper(char *str, char *s) {
	int i;
	char *s1 = (char *)malloc(strlen(s));
	strcpy(s1, s);
	if(str[0] >= 'A' && str[0] <= 'Z' && (s1[0] < 'A' || s1[0]>'Z'))
		s1[0] -= 32;
	if(allUpper(str))
		for (i = 1; s1[i] != '\0'; ++i)
		{
			if(s1[i] >= 'a' && s1[i] <= 'z'){
				s1[i] -= 32;

			}
		}
	else
		for (i = 1; s1[i] != '\0'; ++i)
		{
			if(s1[i] >= 'A' && s1[i] <= 'Z'){
				s1[i] += 32;

			}
		}
	s1[i] = '\0';
	return s1;
}