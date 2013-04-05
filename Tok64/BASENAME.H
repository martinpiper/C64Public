int basename (char *path, char *base, int max)
{
   int i, stop;

   stop = strcspn (path, ".");
   if (stop > max) stop = max;
   for (i = 0; i < stop; i ++) base [i] = path [i];
   base [i] = 0;
   return strcmp (path, base);
}


int extension (char *path, char *ext, int max)
{
   char *start;

   if ((start = strchr (path, '.')) == (char *) NULL) ext [0] = 0;
   else
   {
      strncpy (ext, start, max);
      ext [max] = 0;
   }

   return strlen (ext);
}


int bare_name (char *path, int maxp, char *fname, int maxf)
{
   int i, k, start, stop;

/* find the end */
   stop = strcspn (path, ".");
   if (stop > maxp) stop = maxp;

/* find the beginning */
   for (i = stop; i >= 0 && path [i] != '/'; i --);
   start = i + 1;

/* copy file name without path and extension */
   for (k = 0, i = start; k < maxf && i < stop; k ++, i ++)
      fname [k] = path [i];
   fname [k] = 0;

   return strlen (fname);
}
