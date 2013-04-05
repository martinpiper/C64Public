#define MAXFNAME 13
#define CHNULL ((char *) 0)

char program [MAXFNAME];

void get_prog_name (char *path)
{
	char *p, *q;

	p = (char *) strrchr (path, '\\');
	if (p != CHNULL) p ++;
	else p = path;
	strncpy (program, p, MAXFNAME - 1);
	q = (char *) strchr (program, '.');
	if (q != CHNULL) *q = '\0';
	return;
}
