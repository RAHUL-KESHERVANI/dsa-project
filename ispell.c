
/**************************************************************************
 *   ispell.c  --  This file is a part of DSA miniproject.                *
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
         /*OPTIONS FOR ISPELL IN FILE MODE*/ 
         /*6 options are there available R for replace*/
         /*number for replaing the suggested words*/
         /*A for accept the word for the rest of session*/
         /*space for ignore*/
         /*I for adding the word to the dictionary*/
         /*U for adding the word to the dictionary in lowercase*/
         /*Q for quiting*/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <sys/ioctl.h>
#include "dict.h"
#include "ispell.h"
#include "file.h"
#define CTRL_KEY(k) ((k) & 0x1f)
struct termios orig_termios;

void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
int getWindowSize(int *rows, int *cols) {
  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    return -1;
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}

void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode);
  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
void clrscr() {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
}
void initf(file *f, int columns){
  f->fileline[0] = (char *)malloc(columns+2);
  f->fileline[1] = (char *)malloc(columns+2);
  f->line = (char *)malloc(columns+2);
  strcpy(f->fileline[0], "\n");
  strcpy(f->fileline[1], "\0");
  f->i = 0;
  f->repl = 0;
  f->start = 0;
  f->end = 0;
}
void updateLine(file *f, int w) {
  int l = strlen(f->line);
  int ls = strlen(f->str);
  int extra, i;
  strcpy(f->fileline[1], "\0");
  if(l < w){
    f->br = f->i - ls;
    f->end = l;
  }
  else {
    if(f->i < w){
      f->br = f->i - ls;
      f->end = w;
    }
    else {
      
      if(ls == w - 34)
        extra = 34;
      else if(f->i == l)
        extra = w - ls -34;
      else
        extra = w-ls;
      
      f->start = f->i - ls - extra;
      f->end = f->i;
      f->br = f->i - ls - f->start;
    }
  }
  for (i = f->start; i < f->end; ++i)
  {
    f->fileline[1][i - f->start] = f->line[i];
  }
  f->fileline[1][i-f->start] = '\0';
}
int getfline(file *f, FILE *fpr, int columns) {
  int j;
  long int n;
  j = getline(&(f->line), &n, fpr);
  f->i = 0;
  f->start = 0;
  f->end = 0;
  f->br = 0;
  return j;
}
void DestroyFile(file *f) {
  free(f->fileline[0]);
  free(f->fileline[1]);
  free(f->line);
}
void OptionsR(file *f) {
  char str[160], *p, c, *q;
  int i = 0, j;
  p = (char*)malloc(173);
  q = strdup(f->line);
  printf("Replace With:");
  
  disableRawMode();
  while((c = getchar()) != '\n')
    str[i++] = c;
  enableRawMode();
  
  str[i] = '\0';
  
  for (j = 0; j < f->i - strlen(f->str); ++j)
    p[j] = f->line[j];
  p[j] = '\0';
  
  strcpy(f->line, p);
  strcat(f->line, str);
  strcat(f->line, q+f->i);
  str[i] = '\0';
  f->end = f->end - strlen(str) + strlen(f->str);
  f->i = f->i - strlen(f->str);
  strcpy(f->str, str);

  for (i = f->start; i < f->end; ++i)
    f->fileline[1][i - f->start] = f->line[i];
  
  f->fileline[1][i-f->start] = '\0';
  f->repl = replaced;
  if(f->i < 0)
    f->i = 0;
  
  free(p);
  free(q);
}

#define KRED  "\x1B[31m"
#define KNRM  "\x1B[0m"

void printfile(file *f, Dict *d) {
  clrscr();
  int r, c;
  char *p;
  p = strdup(f->fileline[1]);
  getWindowSize(&r,&c);
  if(strtok(f->fileline[1], "\n") != NULL)
    f->fileline[1][strlen(f->fileline[1])] = '\0';

  printf("\t");
  printf("%s", f->str);
  if(strlen(f->str) < c/4)
  printf("\t\t");
  printf(" file :%s\n\n",f->filename);
  printf("%s", f->fileline[0]);
  p[f->br] = '\0';
  printf("%s",p);
  printf("%s%s", KRED, f->str);
  printf("%s%s", KNRM, f->fileline[1]+f->br + strlen(f->str));
  printf("\n");
  fprintwords(d, r);
  printf("[SP] <number> R)epl L)ookup A)ccept I)nsert U)ncap Q)uit\n");
  free(p);
}
char *mystrtok(file *f, int w) {
  int i, j, flag = 1  , len;
  char *p = malloc(w-34);
  int pointer = -1;
  if(f->repl == replaced){
    f->i = f->i + strlen(f->str);
    f->repl = not_repl;
    return f->str;
  }
    for (i = f->i;i <= strlen(f->line); ++i)
  {
    for (j = 0; j < strlen(delim); ++j)
    {
      if(f->line[i] == delim[j] || f->line[i] == '\0' || f->line[i] == ' ' || f->line[i] == '\n')
        break;
    }
    if(j == strlen(delim) && flag) {
      flag = 0;
      pointer = i;
    }
    if((!flag && strlen(delim) != j)){
      f->i = i;
      break;
    }
  }
  if(!flag) {
 if(pointer == -1)
      return NULL;
    len = f->i - pointer;
    if(len > w - 34){
      f->i = f->i - len + w - 34;
      len = w-34;
    }
    for (int i = 0; i < len; i++)
      p[i] = f->line[i+pointer];
    p[len] = '\0';
    f->str = p;
    updateLine(f, w);
    return p;
  }
  return NULL;

}
#define Quit 1
#define Ignore 2
int sugg(file *f, Dict *d, int i) {
  char c;
  char *p = strdup(f->line+ f->i), *str;
  int j;
  if(d->i > 10) {
    if(i > (d->i - 1) /10) {
      while(read(STDIN_FILENO, &c, 1) == 1){
        if(c == 'Q')
          return Quit;
        if(c == ' ')
          return Ignore;
        i = c - '0';
        if(i <= (d->i - 1) /10)
          break;
      }
    }
    while(read(STDIN_FILENO, &c, 1) == 1){
      if(c == 'Q')
          return Quit;
        if(c == ' ')
          return Ignore;
          if(!isdigit(c))
      continue;
      j = i;
      i= i *10 + c - '0';
      if(i >= d->i)
        i = j;
      else
        break;
    }
  }
  else if(i >= d->i){
    while(read(STDIN_FILENO, &c, 1) == 1){
      if(c == 'Q')
        return Quit;
      if(c == ' ')
        return Ignore;
      if(!isdigit(c))
        continue;
      if(c - '0' < d->i){
        i = c - '0';
        break;
      }
    }
  }
  f->line[f->i - strlen(f->str)] = '\0';
  strcat(f->line, d->words[i]);
  strcat(f->line, p);
  f->end = f->end + strlen(d->words[i]) - strlen(f->str);
  f->i = f->i + strlen(d->words[i]) - strlen(f->str);
  for (i = f->start; i < f->end && i < strlen(f->line); ++i)
    f->fileline[1][i - f->start] = f->line[i];
  f->fileline[1][i-f->start] = '\0';
  return 0;

}
void Lookup(Dict *d) {
  char *p, *s, *str, c;
  int i = 0;
  s = (char *)malloc(40);
  str = (char *)malloc(200);
  printf("Lookup string ('*' is wildcard):");
  disableRawMode();
  while((c = getchar()) != '\n')
    str[i++] = c;
    enableRawMode();
  printf("\n");
  str[i] = '\0';
  if(str[0] == '*' && i == 1)
    str[0] = '\0';
  for (int i = 0; i < d->size; ++i)
  {
    strcpy(s, d->table[i].a);
    p = strstr(s, str);
    if(p == s)
      printf("%s\n", s);
  }
  printf("-- Type space to continue --\n");
  while(read(STDIN_FILENO, &c, 1) == 1 && c != ' ');
  free(str);
 
}
int getFileLength(char *f) {
  FILE *fp = fopen(f, "r");
  char c;int count = 0;
  for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') // Increment count if this character is newline
            count = count + 1;
  fclose(fp);
  return count+1;
}
int main(int argc, char*argv[]) {
  Dict d;
  char buf[512], fbuf[512];
  int i, j, k;
  FILE *fp, *fpr, *fpw;
  char c, *str;
  fp = fopen("british-english", "a+");
  if(fp == NULL)
    printf("dictionary could not be opened\n");
  init(&d, getFileLength("british-english"));

  while(fscanf(fp, "%s", fbuf) != -1){
    Load(&d, fbuf);
  }
  if(argc == 1){
    while(scanf("%s", buf) != -1){
      Master(&d, buf);
      printf("\n");
    }
    printf("\n");
  }

  if(argc == 2){
    fpr = fopen(argv[1], "r");
    if(fpr == NULL){
    	perror("");
    	return errno;
    }
    fpw = fopen("myfile", "w");
    if(fpw == NULL)
      printf("error\n");
    clrscr();
    enableRawMode();
    int w, l;
    getWindowSize(&l, &w);
    file f;
    initf(&f, w);
    strcpy(f.filename, argv[1]);
    while((getfline(&f, fpr, w)) != EOF) {
      str = mystrtok(&f, w);
      while(str != NULL) {
        i = FMaster(&d, str);
        if(i != CORRECT){
          printfile(&f, &d);
          again:
         while (read(STDIN_FILENO, &c, 1) == 1 && c != 'R' && c != ' ' && c != 'A' && c != 'L' && c != 'Q' && c!='I' && c!='U'&& (d.i == 0 || !isdigit(c)));
         /*6 options are there available R for replace*/
         /*number for replaing the suggested words*/
         /*A for accept the word for the rest of session*/
         /*space for ignore*/
         /*I for adding the word to the dictionary*/
         /*U for adding the word to the dictionary in lowercase*/
         /*Q for quiting*/
         if(c == 'L'){
          Lookup(&d);
          printfile(&f, &d);
          goto again;
        }
         switch(c){
           case 'R':
            OptionsR(&f);
            break;
            case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
              k = sugg(&f, &d, c - '0');
              if(k == Quit){
                clrscr();
                return 0;
              }
              break;
            case ' ':
              clrscr();
              break;
            case 'A':
              Load(&d, str);
              break;
            case 'I':
              str[strlen(str)] = '\n';
              fwrite(str, 1, strlen(str), fp);
              break;
            case 'U':
              fwrite(toLower(str), 1, strlen(str), fp);
            	break;
           case'Q':
            clrscr();
            return 0;
          }  
       }
       (str = mystrtok(&f, w));
       reallocWords(&d);
      }
      clrscr();
      fwrite(f.line, 1 , strlen(f.line), fpw);
      l = strlen(f.line);
        if(l > w){
          f.line[w] = '\n';
          l = w + 1;
        }
        f.line[l] = '\0';
        strcpy(f.fileline[0], f.line);
        strcpy(f.fileline[1], "\0");
    }
    fclose(fpr);
    fclose(fpw);
    /*copying the edited tempfile into the original file*/
    fpw = fopen(argv[1], "w");
    fpr =fopen("myfile", "r");
    while((c = getc(fpr)) != EOF) {
      putc(c, fpw);
    }
    DestroyFile(&f);
  }
  DestroyTbl(&d);
  fclose(fp);
}
