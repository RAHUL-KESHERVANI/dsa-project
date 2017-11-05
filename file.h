typedef struct file {
	char filename[50];
	char *fileline[2];
	char *line;
	int repl;
	char *str;
	int i;
	int start;
	int end;
	int br;
}file;

void initf(file *f, int rows);
void printfile(file *f, Dict *d);
void Option_R(file *f);
