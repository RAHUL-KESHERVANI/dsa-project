#define TBLSIZE 1024
typedef struct ele
{
	char a[20];
	long int h;
	int len;
}ele;

typedef struct{
	ele *table;
	int size;
}Dict;

void init(Dict *d, int size);
char * Search(Dict *d, char*str) ;
void PrintD(Dict *d);
void Load(Dict *d, char *str);
long int hash(char *str);

int hashcompare(long int h1, long int h2);