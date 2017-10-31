typedef struct file {
	char filename[50];
	char *fileline[2];
	char *str;
	int i;
	int flag;
	int rows;
	int columns;
}file;

void initf(file *f, int rows);
void printfile(file *f, Dict *d);
void Options(file *f, char **w);
void Option_R(file *f);
