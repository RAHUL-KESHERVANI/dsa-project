#define CORRECT 200
#define INCORRECT -1
#define SUGG 0
#define delim "0123456789/%!@#$^&*()_+{}[]=\n;:\".,.><?-\\ \0"
void Master(Dict *d, char *str);

int mystrcmp(char *s1, char * s2, int miss);
void printwords(Dict *d);
void fprintwords(Dict *d, int r);
int checkRejected(char *s, char **r, int i);
void compareEqualString(Dict *d, char *s, char *str);
char* toLower(const  char *s);
int checkLower(char *p) ;
int checkUpper(char * p);
char* toUpper(char *s, char *str);
int FMaster(Dict *d, char *str);

