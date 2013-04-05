#ifndef MAXLINE
   #define MAXLINE 200
#endif

char line [MAXLINE];

int getline (char *line, int max, FILE *infile)
{
   int i;

   if (fgets (line, max, infile) == NULL) i = EOF;
   else
   {
      i = strlen (line) - 1;
      if (line [i] == '\n') line [i] = '\0';
      else i ++;
   }

   return i;
}
