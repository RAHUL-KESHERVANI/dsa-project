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
  f->fileline[0] = (char *)malloc(columns);
  f->fileline[1] = (char *)malloc(columns);
  strcpy(f->fileline[0], "\0");
  // strcpy(f->fileline[1], "\0");
  f->i = 0;
  f->flag = 0;
}
int getfline(file *f, FILE *fpr, int columns) {
  int  i= 0, extra, j;
  char c;
  f->i = 0;
  int l = strlen(f->fileline[0]);
   if(l == columns){
    if((c = fgetc(fpr)) != EOF && isalpha(c) && isalpha(f->fileline[0][l - 1])){
      // printf("fdasdsf\n");
      f->flag = 1;
    }
    else{
    f->fileline[1][i] = c;
    i++;
  }
  }
  if(f->flag)
  {
    extra = 25 - strlen(f->str);
    j = extra;
    while(j >= 0){
      f->fileline[1][i++] = f->fileline[0][l - j];
      j--;
    }
    strcat(f->fileline[1], f->str);
    i += strlen(f->str);
    f->fileline[1][i++] = c;
    f->flag = 0;
  }
 
  while(i < columns && ((c = fgetc(fpr)) != EOF) && c != '\n') {  
    f->fileline[1][i] = c;  
    i++;
  }
  f->fileline[1][i] = '\0';
  // printf("%s\n", f->fileline[1]);
  if(!i)
    return EOF;

  return 1;
}

void OptionsR(file *f) {
  char str[160], *p, c;
  int i = 0;
  // if(f->i != strlen(f->fileline[1]))
  p = strdup(f->fileline[1]+f->i);
 
printf("Replace With:");
  disableRawMode();
  while((c = getchar()) != '\n')
    str[i++] = c;
enableRawMode();
str[i] = '\0';
  f->fileline[1][f->i - strlen(f->str)] = '\0';
  strcat(f->fileline[1], str);
  strcat(f->fileline[1], p);
 
  f->i = f->i - strlen(f->str);
  strcpy(f->str, str);
}
#define KRED  "\x1B[31m"
#define KNRM  "\x1B[0m"

void printfile(file *f, Dict *d) {
  clrscr();
  int r, c;
  char *p;
  p = strdup(f->fileline[1]);
  p[f->i -strlen(f->str)] = '\0';
  getWindowSize(&r,&c);
  printf("\t%s\t\t%s\n", f->str, f->filename);
  printf("%s\n", f->fileline[0]);
  printf("%s",p);
  printf("%s%s", KRED, f->str); 
  printf("%s%s", KNRM,f->fileline[1]+f->i);
  printf("\n\n");
  // if(d->i >0)
  fprintwords(d, r);
  printf("[SP] <number> R)epl A)ccept I)nsert U)ncap Q)uit\n");
  free(p);
}
char *mystrtok(file *f, int w) {
  int i, j, flag = 1, len;
  char *p = malloc(w);
  int pointer;
  for (i = f->i;i <= strlen(f->fileline[1]); ++i)
  {
    for (j = 0; j < strlen(delim); ++j)
    {
      if(f->fileline[1][i] == delim[j] || f->fileline[1][i] == '\0' || f->fileline[1][i] == ' ')
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
    len = f->i - pointer;
    if(len > w - 20){
      f->i = f->i - len + w - 20;
      len = len - w + 20;
    }
    for (int i = 0; i < len; ++i)
    {
      p[i] = f->fileline[1][i+pointer];
    }
    f->str = p;
    return p;
  }
  return NULL;

}
void cpyfile(char *file) {
  char c;
  FILE *fp1 = fopen(file, "w");
  FILE *fp2 =fopen("myfile", "r");
  while((c = getc(fp2)) != EOF) {
    putc(c, fp1);
  }
}
void sugg(file *f, Dict *d, int i) {
  char c;
  char *p = strdup(f->fileline[1]+ f->i), *str;
   if(d->i > 9){
    while(read(STDIN_FILENO, &c, 1) == 1 && !isdigit(c));
    i= i *10 + c - '0';
  }
  f->fileline[1][f->i - strlen(f->str)] = '\0';
  
  strcat(f->fileline[1], d->words[i]);
  strcat(f->fileline[1], p);
 
  f->i = f->i - strlen(f->str);
  strcpy(f->str, d->words[i]);
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
  char buf[512], fbuf[512], **rejected;
  int i, len, j, rej, flag;
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
    clrscr();
    enableRawMode();
    char c, *p;
    int i =0, w, l, temp1, temp2;
    getWindowSize(&l, &w);
    file f;
    initf(&f, w);
     strcpy(f.filename, argv[1]);
    while((getfline(&f, fpr, w)) != EOF) {
      printf("%s\n", f.fileline[1]);
      str = mystrtok(&f, w);
      flag = 0;
      while(str != NULL) {
        i = FMaster(&d, str);
        if(i != CORRECT){
          printfile(&f, &d);
         while (read(STDIN_FILENO, &c, 1) == 1 && c != 'R' && c != ' ' && c != 'A' && c != 'Q' && c!='I' && c!='U'&& !isdigit(c));
         switch(c){
         case 'R':
          OptionsR(&f);
          break;
          case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
          sugg(&f, &d, c - '0');
          case ' ':
          clrscr();
          break;
          case 'A':
            Load(&d, str);
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
        fwrite(f.fileline[1], 1 , strlen(f.fileline[1]), fpw);
        fwrite("\n", 1, 1, fpw);
        strcpy(f.fileline[0], f.fileline[1]);
        strcpy(f.fileline[1], "\0");
    }
    fclose(fpr);
     fclose(fpw);
    cpyfile(argv[1]);
  }
  DestroyTbl(&d);
  fclose(fp);
  
 
}
