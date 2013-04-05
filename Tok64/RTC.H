void rtc (FILE *screen)
{
   fflush (stdin);
   fprintf (screen, "\nHit <RETURN> to continue: ");
   getchar ();
   fflush (stdin);
   fprintf (screen, "\n");
}

