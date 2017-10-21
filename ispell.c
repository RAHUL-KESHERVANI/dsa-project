#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dict.h"
#include "ispell.h"
#define SUBFILES 2
#define FILES 2
/*please take care of the dictionary root path*/
FILE *filechange() {
	static int i = 0, j =0;
	char a[3][10] = {"altamer.", "american.", "english."}, s[2];
	FILE *fp;
	char str[100] = "/home/rahul/dsa-project-ispell/dictonary/english/";
	strcat(str, a[i]);
	sprintf(s, "%d", j);
	strcat(str, s);
	if(j <= SUBFILES)
		j++;
	else{
		i++;
		j = 0;
	}
	fp = fopen(str, "r");
	return fp;
}
int main(){
	Dict d;
	char buf[512];
	int i;
	init(&d, 157);
	FILE *fp = filechange();
   	while(fscanf(fp, "%s", buf) != -1){
		Load(&d, buf);
	}
	while(scanf("%s", buf) != -1){
		while((fp=filechange()) != NULL){
			i = Master(&d, buf);
			if(i == -1)
				continue;
			if(i == 0){
				printf("ok\n");
				break;
			}
		} 
		if(i != 0){
			// printf("dhere%d\n", i);
			if(fp == NULL && i == -1)
				printf("not found\n");
			// else
			// 	printwords();
		} 
	}

}