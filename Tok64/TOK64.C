// MPi: Testing debug command lines so I don't forget
///6510+ /stomp /totxt C:\CCS64\MyOldE114Software\MusicEditorCutDownVersion\MUSEDIT1.S.PRG
///6510+ /stomp /totxt C:\CCS64\MyOldE114Software\COMMANDS.S.PRG
/*
   tok64.c

   3/2/96 Cris Berneburg

   This converts ascii text file representations of a Commodore
   basic program listings into a program images suitable to run
   on a C64.  Also converts basic programs into ascii listings.

   6/9/96. Added function key and space special char/keyword replacement.
   Also enabled special char replacement for DATA mode.

   6/14/96. Merged basic extensions from what would have been v2.0
   with v1.3 to make v1.4. GUI interface will have to wait.

	21 October 2007 Martin Piper - Added 6510+ assembler extension keywords and general tidying to get it compiling with Visual Studio 6
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLINE 402

#include "getprog.h"
#include "getline.h"
#include "basename.h"
#include "rtc.h"

#undef VERSION
#undef VDATE
#define PROGNAME "TOK64"
#define VERSION "1.6"
#define VDATE "21 October 2007"
#define VCOMM "Basic extensions. More special keywords."
#define COPYRIGHT "1996"
#define AUTHOR "Cris Berneburg with tweaks by Martin Piper"
#define EMAIL "<crisb@bigfoot.com> <martin.piper@gmail.com>"
#define WEB "https://github.com/martinpiper/C64/tree/master/Tok64"

#define TRUE (-1)
#define FALSE 0
#define EXITOK 0
#define EXITBAD 1
#define MAXPATH 130
#define MAXEXT 5
#define TXT_EXT ".txt"
#define PRG_EXT ".prg"
#define START_PROC "start tok64"
#define STOP_PROC "stop tok64"
#define MAXKWLIST 3
#define PAGE_BREAK 18

#define TOPRG 0
#define TOTXT 1
#define MULT 2
#define NOMULT 3
#define LOWER 4
#define UPPER 5
#define LOWCASE 6
#define CASE 7
#define COL 8
#define NOCOL 9
#define STOMP 10
#define NOSTOMP 11
#define FINCART3 12
#define GRAPH52 13
#define C64 14
#define LIST 15
#define HELP 16
#define KEYWORDS 17
#define SPECIAL 18
#define EXTENSIONS 19
#define ASM6510PLUS 20

#define MAXTOKENLINE 200
#define LOBYTE 0
#define HIBYTE 1
#define START_ADDY 0x801
#define MAXDECIM 4
#define COMT_TOKEN 0x8F
#define COMT_WORD "rem"
#define CLEAR_WORD "{clear}"
#define SPACE 0x20
#define SPACE_RPT 4


typedef union addy_union
{
	unsigned short whole_addy;
	unsigned char byte_addy [2];
} Addy_type;


typedef struct basrec_struct
{
	Addy_type next_rec;
	Addy_type line_num;
	char tokenline [MAXTOKENLINE];
} Basrec_type;


typedef struct keyword_struct
{
	char *keyword;
	unsigned char token;
} Keyword_type;


typedef struct command_struct
{
	char *command;
	char *desc;
} Command_type;


#define BASREC_NULL ((Basrec_type *) 0)
#define ADDY_NULL ((Addy_type *) 0)
#define CH_NULL ((char *) 0)
#define KW_NULL ((Keyword_type *) 0)


Keyword_type *kw_list [MAXKWLIST];


Keyword_type keywords [] =
{
	"*", 0xAC,
	"+", 0xAA,
	"-", 0xAB,
	"/", 0xAD,
	"<", 0xB3,
	"=", 0xB2,
	">", 0xB1,
	"^", 0xAE,
	"abs", 0xB6,
	"and", 0xAF,
	"asc", 0xC6,
	"atn", 0xC1,
	"chr$", 0xC7,
	"close", 0xA0,
	"clr", 0x9C,
	"cmd", 0x9D,
	"cont", 0x9A,
	"cos", 0xBE,
	"data", 0x83,
	"def", 0x96,
	"dim", 0x86,
	"end", 0x80,
	"exp", 0xBD,
	"fn", 0xA5,
	"for", 0x81,
	"fre", 0xB8,
	"get", 0xA1,
	"gosub", 0x8D, /* must appear before "go" */
	"goto", 0x89, /* must appear before "go" */
	"go", 0xCB,
	"if", 0x8B,
	"input#", 0x84, /* must appear before "input" */
	"input", 0x85,
	"int", 0xB5,
	"left$", 0xC8,
	"len", 0xC3,
	"let", 0x88,
	"list", 0x9B,
	"load", 0x93,
	"log", 0xBC,
	"mid$", 0xCA,
	"new", 0xA2,
	"next", 0x82,
	"not", 0xA8,
	"on", 0x91,
	"open", 0x9F,
	"or", 0xB0,
	"peek", 0xC2,
	"{pi}", 0xFF,
	"poke", 0x97,
	"pos", 0xB9,
	"print#", 0x98, /* must appear before "print" */
	"print", 0x99,
	"read", 0x87,
	COMT_WORD, COMT_TOKEN, /* rem */
	"restore", 0x8C,
	"return", 0x8E,
	"right$", 0xC9,
	"rnd", 0xBB,
	"run", 0x8A,
	"save", 0x94,
	"sgn", 0xB4,
	"sin", 0xBF,
	"spc(", 0xA6,
	"sqr", 0xBA,
	"step", 0xA9,
	"stop", 0x90,
	"str$", 0xC4,
	"sys", 0x9E,
	"tab(", 0xA3,
	"tan", 0xC0,
	"then", 0xA7,
	"to", 0xA4,
	"usr", 0xB7,
	"val", 0xC5,
	"verify", 0x95,
	"wait", 0x92,
	"", 0
};


Keyword_type special [] =
{
	CLEAR_WORD, 0x93,     "{home}", 0x13,
	"{right}", 0x1D,      "{left}", 0x9D,
	"{up}", 0x91,         "{down}", 0x11,
	"{reverse on}", 0x12, "{reverse off}", 0x92,
	"{black}", 0x90,      "{white}", 0x05,
	"{red}", 0x1C,        "{cyan}", 0x9F,
	"{purple}", 0x9C,     "{green}", 0x1E,
	"{blue}", 0x1F,       "{yellow}", 0x9E,
	"{orange}", 0x81,     "{brown}", 0x95,
	"{pink}", 0x96,       "{dark gray}", 0x97,
	"{gray}", 0x98,       "{light green}", 0x99,
	"{light blue}", 0x9A, "{light gray}", 0x9B,
	"{f1}", 0x85,         "{f2}", 0x89,
	"{f3}", 0x86,         "{f4}", 0x8A,
	"{f5}", 0x87,         "{f6}", 0x8B,
	"{f7}", 0x88,         "{f8}", 0x8C,
	"{space}", SPACE,
	"", 0
};


/*
THE FINAL CARTRIDGE 3 BASIC EXTENSION (C64)
*/
Keyword_type final_cart_3 [] =
{
	"append", 0xD6,
	"array", 0xE0,
	"auto", 0xCD,
	"bar", 0xDD,
	"dappend", 0xD7,
	"del", 0xCE,
	"desktop", 0xDE,
	"dload", 0xD3,
	"dos", 0xD8,
	"dsave", 0xD5,
	"dump", 0xDF,
	"dverify", 0xD4,
	"find", 0xD1,
	"help", 0xD0,
	"kill", 0xD9,
	"mem", 0xE1,
	"mon", 0xDA,
	"mread", 0xE7,
	"mwrite", 0xE8,
	"off", 0xCC,
	"old", 0xD2,
	"order", 0xE4,
	"pack", 0xE5,
	"pdir", 0xDB,
	"plist", 0xDC,
	"renum", 0xCF,
	"replace", 0xE3,
	"trace", 0xE2,
	"unpack", 0xE6,
	"", 0
};


/*
GRAPHICS52 BASIC EXTENSION (C64)
*/
Keyword_type graphics_52 [] =
{
	"adsr", 0xE7,
	"bit(", 0xFC,
	"block", 0xDC,
	"bottom", 0xDD,
	"brd&bkg", 0xF5,
	"char", 0xD3,
	"chrsv", 0xE4,
	"clear", 0xD0,
	"color", 0xD5,
	"copy", 0xF1,
	"cprog", 0xD8,
	"detect", 0xEC,
	"draw", 0xCF,
	"erase", 0xD2,
	"flip", 0xDA,
	"joy(", 0xFB,
	"loc(", 0xFD,
	"lochr", 0xE2,
	"lomem", 0xF3,
	"loscr", 0xE0,
	"lospr", 0xE1,
	"memsv", 0xF2,
	"move", 0xEF,
	"multi", 0xF8,
	"padl(", 0xFA,
	"pen", 0xD9,
	"pitch", 0xE9,
	"place", 0xF0,
	"plot", 0xCE,
	"point(", 0xFE,
	"pulse", 0xEB,
	"put", 0xEE,
	"screen", 0xCC,
	"scrsv", 0xDF,
	"sdp", 0xDE,
	"set", 0xED,
	"shift", 0xE8,
	"shrink", 0xF9,
	"smooth", 0xE5,
	"smove", 0xD4,
	"sprcl", 0xCD,
	"sprite", 0xD6,
	"sprog", 0xD7,
	"sprsv", 0xE3,
	"swap", 0xF4,
	"switch", 0xF6,
	"togl", 0xD1,
	"transfer", 0xDB,
	"unless", 0xF7,
	"volume", 0xE6,
	"wave", 0xEA,
	"", 0
};

/*
Extra 6510+ assembler tokens
*/
Keyword_type asm6510plus[]=
{
	"append",0xe5,
	"assemble", 0xcc,
	"auto", 0xcf,
	"change",0xe2,
	"delete", 0xd1,
	"device",0xe3,
	"disassemble", 0xcd,
	"dump",0xde,
	"find",0xd2,
	"format", 0xd0,
	"help",0xe1,
	"keyload",0xd8,	// Must be before "key"
	"keysave",0xd9,	// Must be before "key"
	"keyon",0xda,	// Must be before "key"
	"keyoff",0xdb,	// Must be before "key"
	"key",0xdc,
	"load",0xe7,
	"merge",0xdf,
	"monitor",0xd6,
	"number", 0xce,
	"old",0xd5,
	"poke",0xe9,
	"renumber",0xd3,
	"save",0xe4,
	"set",0xe8,
	"symsave",0xe6,
	"sys",0xe0,
	"table",0xd4,
	"type",0xdd,
	"verify",0xd7,
	"", 0
};


Command_type cmds [] =
{
	"/toprg", "Tokenize *.TXT text file to Commodore Basic program (*)",
	"/totxt", "Detokenize *.PRG basic file to ascii text",
	"/mult", "Process multiple listings within a single text file",
	"/nomult", "Process text file as a single listing (*)",
	"/lower", "Render keywords as lower case when de-tokenizing (*)",
	"/upper", "Render keywords as upper case when de-tokenizing",
	"/lowcase", "Lower case quoted strings & comments when tokenizing",
	"/case", "Quoted string & comment tokenizing is case-sensitive (*)",
	"/col", "Column sensitivity enabled while tokenizing",
	"/nocol", "Column sensitivity disabled (*)",
	"/stomp", "Overwrite existing files without confirmation",
	"/nostomp", "Ask before overwriting existing files (*)",
	"/fincart3", "The Final Cartridge 3 Basic Extension",
	"/graph52", "Graphics52 Basic Extension",
	"/64", "C64 Basic V2 (*)",
	"/list", "Display a list of command arguments",
	"/help", "The help screen",
	"/keywords", "Display a list of Commodore Basic keywords",
	"/special", "Display a list of special substitutions",
	"/ext", "Display a list of Basic extension keywords",
	"/6510+", "Use extension keywords from 6510+",
	"{file name}", "Name of file to process",
	"", ""
};


void outprg (FILE *f, Basrec_type *rec)
{
	fputc (rec -> next_rec.byte_addy [LOBYTE], f);
	fputc (rec -> next_rec.byte_addy [HIBYTE], f);
	fputc (rec -> line_num.byte_addy [LOBYTE], f);
	fputc (rec -> line_num.byte_addy [HIBYTE], f);
	fprintf (f, "%s", rec -> tokenline);
	fputc (0, f);
}


void start_prg (FILE *f)
{
	Addy_type start;

	start.whole_addy = START_ADDY;
	fputc (start.byte_addy [LOBYTE], f);
	fputc (start.byte_addy [HIBYTE], f);
}


void end_prg (FILE *f)
{
	fputc (0, f); fputc (0, f);
}


void set_addy (Basrec_type *node)
{
	static unsigned short next_addy = START_ADDY;

	if (node == BASREC_NULL)
	{
		next_addy = START_ADDY;
		return;
	}

	next_addy += 5 + strlen (node -> tokenline);
	node -> next_rec.whole_addy = next_addy;
}


unsigned char get_deci_byte (char *string, int start, int max)
{
	int n, a, num;
	char anum [MAXDECIM];
	unsigned char byte;

	for (n = 0, a = start;
		n < MAXDECIM && a < max && isdigit (string [a]);
		n ++, a ++) anum [n] = string [a];

	anum [n] = 0;

	if (strlen (anum) < MAXDECIM - 1) num = 0;
	else num = atoi (anum);

	if (num < 1 || num > 255) byte = 0;
	else if (string [a] != '}' && string [a] != '*') byte = 0;
	else byte = num;

	return byte;
}


int get_times (char *string, int start, int max, int *stop)
{
	int n, a, num, times;
	char anum [MAXDECIM];

	if (string [start] == '}')
	{
		*stop = start + 1;
		return 1;
	}

	times = (string [start] == '*' ? TRUE : FALSE);
	if (! times && ! isdigit (string [start])) return 0;

	for (n = 0, a = start + (times ? 1 : 0);
		n < MAXDECIM - 1 && a < max && isdigit (string [a]);
		n ++, a ++) anum [n] = string [a];

	anum [n] = 0;
	if (times && strlen (anum) == 0) return 0;
	if (strlen (anum) == 0) num = 1;
	else num = atoi (anum);

	if (num < 1 || num > 255) return 0;
	if (string [a] != '}') return 0;
	*stop = a + 1;
	return num;
}


void exit_illeg_subs (line_num, col)
{
	fprintf (stderr, "\n%s: Illegal quoted substitution line %u col %d.\n",
		program, line_num, col + 1);
	exit (EXITBAD);
}


int tok_quantity (unsigned char byte, char *text, int *tex, int offset,
	char *tokens, int *tok)
{
	int quantity, q, next;

	quantity = get_times (text, *tex + offset, MAXLINE - 1, &next);

	if (quantity)
	{
		for (q = *tok;
			q < *tok + quantity && q < MAXTOKENLINE - 1;
			q ++) tokens [q] = byte;

		*tok = q - 1;
		*tex = next - 1;
		return TRUE;
	}
	else return FALSE;
}


int tokenize (Basrec_type *node, char *text, int casemode, int colmode)
{
	int tex = 0, j, k, eol, len, found, quantity, curr_list = 0, search;
	char casetext [MAXLINE];
	unsigned char byte;
	static unsigned int prev_line = 0, line_num = 0;
	static int cont = FALSE, comment = FALSE, quote = FALSE;
	static int tok = 0;
	Keyword_type *kw;

	if (node == BASREC_NULL)
	{
		prev_line = 0;
		tok = 0; quote = FALSE; comment = FALSE;
		return FALSE;
	}

	eol = strlen (text);

	if (colmode)
		if (isdigit (text [0]))
		{
			cont = FALSE;

			if (quote)
				fprintf (stderr, "\n%s: missing close quote on line %u\n",
				program, line_num);
		}
		else
		{
			cont = TRUE;
			if (tex < eol) tex ++;

			if (text [0] && ! isspace (text [0]))
				fprintf (stderr, "\n%s: invalid char in col 1 on line %u\n",
				program, line_num);
		}

		if (cont)
		{
			cont = FALSE;

			if (colmode && tok < MAXTOKENLINE - 1)
				node -> tokenline [tok ++] = ' ';
		}
		else
		{
			if (sscanf (text, "%d", &line_num) != 1)
				return FALSE;

			if (prev_line && line_num <= prev_line)
			{
				fprintf (stderr, "\n%s: line numbers not sorted.", program);
				fprintf (stderr, " Previous = %u, Current = %u.\n", prev_line,
					line_num);
				fprintf (stderr, "Do you need the \"%s\" or the \"%s\" option?\n",
					cmds [MULT].command, cmds [COL].command);
				exit (EXITBAD);
			}

			tok = 0; quote = FALSE; comment = FALSE;
			prev_line = line_num;
			node -> line_num.whole_addy = line_num;
			while (tex < eol && isspace (text [tex])) tex ++;
			while (tex < eol && isdigit (text [tex])) tex ++;
		}

		if (colmode) cont = TRUE;

		if (text [eol - 1] == '\\')
		{
			cont = TRUE; eol --;
		}

		while (tex < eol && isspace (text [tex])) tex ++;
		for (j = tex; j < eol; j++) casetext [j] = tolower (text [j]);
		casetext [j] = 0;

		while (tex < eol && tok < MAXTOKENLINE - 1)
		{
			if (comment)
			{
				if (! casemode && isupper (text [tex]))
					node -> tokenline [tok] = text [tex];
				else if (islower (text [tex]))
					node -> tokenline [tok] = toupper (text [tex]);
				else if (isupper (text [tex]))
					node -> tokenline [tok] = tolower (text [tex]);
				else
					node -> tokenline [tok] = text [tex];
				tex ++;
			}
			else /* not comment mode */
			{
				curr_list = 0;
				if (quote) kw = special; /* display code replacement */
				else kw = kw_list [curr_list]; /* basic keywords */
				k = 0;
				found = FALSE;
				quantity = FALSE;
				search = TRUE;

				while (search) /* search thru lists of keyword lists */
				{
					/* search thru current keyword list */
					while (! found && kw [k].keyword [0])
						if (eol - tex + 1 >= (len = strlen (kw [k].keyword))
							&& ! strncmp (&(casetext [tex]), kw [k].keyword, len))
							found = TRUE;
						else k ++;

						if (quote || found || kw_list [curr_list + 1] == KW_NULL)
							search = FALSE;
						else
						{
							curr_list ++;
							kw = kw_list [curr_list];
						}
				}

				if (quote && ! found)
				{
					k = 0;

					/* look for partial keywords that may be followed by quantity */
					while (! quantity && kw [k].keyword [0])
						if (eol - tex + 1 >= (len = strlen (kw [k].keyword) - 1)
							&& ! strncmp (& (casetext [tex]), kw [k].keyword, len))
							quantity = TRUE;
						else k ++;
				}

				if (found)
				{
					if (! strcmp (kw [k].keyword, COMT_WORD)) comment = TRUE;
					node -> tokenline [tok] = kw [k].token;
					tex += len;
				}
				else if (quantity)
				{
					if (! tok_quantity (kw [k].token, text, &tex, len,
						node -> tokenline, &tok)) exit_illeg_subs (line_num, tex);
					tex ++;
				}
				else /* keyword not found */
				{
					if (text [tex] == '\"') quote = (quote ? FALSE : TRUE);

					if (quote)
					{
						if (! casemode && isupper (text [tex]))
							node -> tokenline [tok] = text [tex];
						else if (islower (text [tex]))
							node -> tokenline [tok] = toupper (text [tex]);
						else if (isupper (text [tex]))
							node -> tokenline [tok] = tolower (text [tex]);
						else if (text [tex] == '{')
						{
							byte = get_deci_byte (text, tex + 1, MAXLINE - 1);

							if (! byte || ! tok_quantity (byte, text, &tex,
								MAXDECIM, node -> tokenline, &tok))
								exit_illeg_subs (line_num, tex);
						}
						else
							node -> tokenline [tok] = text [tex];
					}
					else /* not quote mode */
					{
						if (islower (text [tex]))
							node -> tokenline [tok] = toupper (text [tex]);
						else
							node -> tokenline [tok] = text [tex];
					}

					tex ++;
				}
			} /* end not comment mode */

			tok++;
		} /* end while not eol */

		if (tok < MAXTOKENLINE) node -> tokenline [tok] = 0;
		else
		{
			node -> tokenline [MAXTOKENLINE - 1] = 0;
			fprintf (stderr, "\n%s: line %u is too long.\n", program, line_num);
		}

		if (cont) return FALSE;
		else if (quote)
		{
			fprintf (stderr, "\n%s: missing close quote on line %u\n",
				program, node -> line_num.whole_addy);
			return FALSE;
		}
		else return TRUE;
} /* end tokenize */


void txt2prg (FILE *in, FILE *out, int casemode, int colmode)
{
	Basrec_type rec;
	int first = TRUE;

	set_addy (BASREC_NULL);
	tokenize (BASREC_NULL, CH_NULL, TRUE, FALSE);
	start_prg (out);

	while (getline (line, MAXLINE, in) != EOF)
	{
		if (colmode)
		{
			if (isdigit (line [0]) && ! first)
			{
				set_addy (&rec);
				outprg (out, &rec);
			}

			if (first) first = FALSE;
			tokenize (&rec, line, casemode, colmode);
		}
		else if (tokenize (&rec, line, casemode, colmode))
		{
			set_addy (&rec);
			outprg (out, &rec);
		}
	}

	if (colmode)
	{
		set_addy (&rec);
		outprg (out, &rec);
	}

	end_prg (out);
	return;
}


int exit_exists (int overwrite, FILE *in, char *file_out)
{
	FILE *out;
	int yn, status = FALSE;

	if (! overwrite && (out = fopen (file_out, "r")) != NULL)
	{
		fclose (out);
		fprintf (stderr, "%s: %s already exists. %s",
			program, file_out, "Overwrite it? (Y/N/Q): ");
		yn = getchar ();

		if (yn == 'y' || yn == 'Y') status = FALSE;
		else if (yn == 'n' || yn == 'N')
		{
			fprintf (stderr, "Skipping %s.\n", file_out);
			status = TRUE;
		}
		else
		{
			fprintf (stderr, "Cancelling.\n");
			fclose (in);
			exit (EXITBAD);
		}

		fflush (stdin);
	}

	return status;
}


void process_mult (FILE *in, int overwrite, int casemode, int colmode)
{
	Basrec_type rec;
	int processing = FALSE, i, j, eol, len;
	char file_out [MAXPATH], casetext [MAXLINE], fext [MAXEXT];
	FILE *out;

	while (getline (line, MAXLINE, in) != EOF)
	{
		i = 0;
		eol = strlen (line);
		while (i < eol && isspace (line [i])) i ++;
		for (j = i; j < eol; j++) casetext [j] = tolower (line [j]);
		casetext [j] = 0;
		len = strlen (START_PROC);

		if (! strncmp (& (casetext [i]), START_PROC, len))
		{
			if (processing)
			{
				end_prg (out);
				fclose (out);
				processing = FALSE;
				fprintf (stderr, " done.\n");
			}

			i += len;

			if (sscanf (& (line [i]), "%s", file_out) == 1)
			{
				if (! extension (file_out, fext, MAXEXT -1))
					strncat (file_out, PRG_EXT, MAXPATH - 1);
				else if (! strcmp (fext, TXT_EXT))
				{
					fclose (in);
					fprintf (stderr, "%s: not expecting \"%s\" as output.\n",
						program, file_out);
					exit (EXITBAD);
				}

				if (! exit_exists (overwrite, in, file_out))
					if ((out = fopen (file_out, "wb")) == NULL)
					{
						fprintf (stderr, "%s: Can't open %s for output\n",
							program, file_out);
						fclose (in);
						exit (EXITBAD);
					}
					else
					{
						fprintf (stderr, "==> %s ...", file_out);
						processing = TRUE;
						set_addy (BASREC_NULL);
						tokenize (BASREC_NULL, CH_NULL, TRUE, TRUE);
						start_prg (out);
					}
			}
			else
			{
				fprintf (stderr, "%s: no output filename given\n",
					program);
				fclose (in);
				exit (EXITBAD);
			}
		}
		else if (! strncmp (& (casetext [i]), STOP_PROC, strlen (STOP_PROC)))
		{
			if (processing)
			{
				end_prg (out);
				fclose (out);
				processing = FALSE;
				fprintf (stderr, " done.\n");
			}
		}
		else if (processing)
		{
			if (tokenize (&rec, line, casemode, colmode))
			{
				set_addy (&rec);
				outprg (out, &rec);
			}
		}
	}

	return;
}


int get_addy (FILE *in, Addy_type *addy)
{
	int byte;

	if ((byte = fgetc (in)) == EOF) return EOF;
	addy -> byte_addy [LOBYTE] = byte;

	if ((byte = fgetc (in)) == EOF) return EOF;
	addy -> byte_addy [HIBYTE] = byte;

	return 0;
}


void exit_wrong_fmt (char *name)
{
	fprintf (stderr, "\n%s: %s is in the wrong format.\n", program, name);
	exit (EXITBAD);
}


int get_rec_len (Addy_type *addy)
{
	static unsigned short next_addy = START_ADDY;
	int length;

	if (addy == ADDY_NULL)
	{
		next_addy = START_ADDY;
		return 0;
	}

	length = addy -> whole_addy - next_addy - 5;
	next_addy = addy -> whole_addy;
	addy -> whole_addy = length;
	if (length < 1 || length > MAXTOKENLINE) return EOF;
	else return 0;
}


int get_rec (FILE *fin, Basrec_type *record)
{
	int i, c;
	Addy_type addy;

	/* get next record address */
	if (get_addy (fin, &addy) == EOF) return EOF;

	/* if next record address is zero then we're done */
	if (! addy.whole_addy) return 1;

	/* change next record address into record length */
	if (get_rec_len (&addy) == EOF) return EOF;
	record -> next_rec.whole_addy = addy.whole_addy;

	/* get record line number */
	if (get_addy (fin, &addy) == EOF) return EOF;
	record -> line_num.whole_addy = addy.whole_addy;

	/* get tokenized record */
	for (i = 0, c = 0; i < record -> next_rec.whole_addy && c != EOF; i ++)
		record -> tokenline [i] = c = fgetc (fin);

	if (c == EOF) return EOF;
	if (fgetc (fin)) return EOF;
	record -> tokenline [i] = 0;
	return 0;
}


int detoken (Basrec_type *rec, char *text, int max, int lowercase)
{
	int tok = 0, j, k, tex = 0, eot, len, found, look, count;
	unsigned int token;
	int comment = FALSE, quote = FALSE, curr_list = 0, search;
	char deci_byte [MAXDECIM];
	Keyword_type *kw;
	unsigned char *tokens;

	tokens = rec -> tokenline;
	eot = strlen (tokens);

	while (tok < eot && tex < max - 1)
	{
		if (comment)
		{
			if (islower (tokens [tok]))
				text [tex] = toupper (tokens [tok]);
			else if (isupper (tokens [tok]))
				text [tex] = tolower (tokens [tok]);
			else
				text [tex] = tokens [tok];
		}
		else /* not comment mode */
		{
			curr_list = 0;
			if (quote) kw = special; /* display code replacement */
			else kw = kw_list [curr_list]; /* basic keywords */
			k = 0;
			found = FALSE;
			search = TRUE;

			while (search) /* search thru lists of keyword lists */
			{
				/* search thru current keyword list */
				while (! found && kw [k].token)
					if (tokens [tok] == kw [k].token) found = TRUE;
					else k ++;

					if (quote || found || kw_list [curr_list + 1] == KW_NULL)
						search = FALSE;
					else
					{
						curr_list ++;
						kw = kw_list [curr_list];
						k = 0;	// MPi: Bug fix - Start processing the start of the next list
					}
			}

			/* replace space char with keyword w/repeat only if N number of them */
			if (quote && found && tokens [tok] == SPACE)
			{
				for (look = tok + 1, count = 0;
					look < eot && tokens [look] == SPACE && count < SPACE_RPT;
					look ++, count ++);

				if (count < SPACE_RPT) found = FALSE;
			}

			if (found)
			{
				if (kw [k].token == COMT_TOKEN) comment = TRUE;
				len = strlen (kw [k].keyword);

				for (j = 0; tex < max - 1 && j < len; j ++, tex ++)
					if (lowercase)
						text [tex] = tolower (kw [k].keyword [j]);
					else
						text [tex] = toupper (kw [k].keyword [j]);

				if (quote)
				{
					token = tokens [tok];

					for (look = tok + 1, count = 0;
						look < eot && tokens [look] == token;
						look ++, count ++);

					if (count)
					{
						tex --; text [tex] = '*'; tex ++;
						sprintf (deci_byte, "%d", count + 1);

						for (j = 0;
							tex < max - 1 && j < (int)strlen (deci_byte);
							j ++, tex ++) text [tex] = deci_byte [j];

						tok += count;
						text [tex] = '}';
						tex ++;
					}
				}

				tex --;
			}
			else /* keyword not found */
			{
				if (tokens [tok] == '\"') quote = (quote ? FALSE : TRUE);

				if (quote)
				{
					if (islower (tokens [tok]))
						text [tex] = toupper (tokens [tok]);
					else if (isupper (tokens [tok]))
						text [tex] = tolower (tokens [tok]);
					else if (tokens [tok] < 0x20 || tokens [tok] > 0x7E)
					{
						text [tex] = '{'; tex ++;
						token = tokens [tok];
						sprintf (deci_byte, "%03d", token);

						for (j = 0;
							tex < max - 1 && j < MAXDECIM - 1;
							j ++, tex ++)
							text [tex] = deci_byte [j];

						for (look = tok + 1, count = 0;
							look < eot && tokens [look] == token;
							look ++, count ++);

						if (count)
						{
							text [tex] = '*'; tex ++;
							sprintf (deci_byte, "%d", count + 1);

							for (j = 0;
								tex < max - 1 && j < (int)strlen (deci_byte);
								j ++, tex ++) text [tex] = deci_byte [j];

							tok += count;
						}

						text [tex] = '}';
					}
					else
						text [tex] = tokens [tok];
				}
				else /* not quote mode */
				{
					if (lowercase)
						text [tex] = tolower (tokens [tok]);
					else
						text [tex] = toupper (tokens [tok]);
				}

			} /* end keyword not found */
		} /* end not comment mode */

		tok ++; tex ++;
	} /* end while not eot */

	text [tex] = 0;

	if (quote)
	{
		fprintf (stderr, "\n%s: missing close quote on line %u\n",
			program, rec -> line_num.whole_addy);
		return FALSE;
	}
	else return TRUE;
}


void prg2txt (FILE *in, FILE *out, char *fname, int lowercase)
{
	Addy_type start_addy;
	Basrec_type rec;
	char oline [MAXLINE];
	int x;

	if (get_addy (in, &start_addy) == EOF
		|| start_addy.whole_addy != START_ADDY)
		exit_wrong_fmt (fname);

	get_rec_len (ADDY_NULL);

	while (! (x = get_rec (in, &rec)))
		if (detoken (&rec, oline, MAXLINE, lowercase))
			fprintf (out, "%u %s\n", rec.line_num.whole_addy, oline);

	if (x == EOF) exit_wrong_fmt (fname);
	return;
}


void usage ()
{
	fprintf (stderr, "\nUsage: %s [option | file [...]]\n", program);
	fprintf (stderr, "   or: %s %s\n", program, cmds [HELP].command);
	exit (EXITBAD);
}


void list_cmds ()
{
	int i = 0;

	fprintf (stdout, "\n%s %s:\n\n",
		"These are the valid command line arguments for", program);

	while (cmds [i].command [0])
	{
		fprintf (stdout, "%-15s %s\n", cmds [i].command, cmds [i].desc);
		i ++;
		if (! (i % PAGE_BREAK)) rtc (stderr);
	}
}


void disp_list (char *title, Keyword_type *list, char *format, int limit)
{
	int i = 0, lines = 0;

	fprintf (stdout, "\n%s %s:\n\n", title, program);

	while (list [i].keyword [0])
	{
		fprintf (stdout, format, list [i].keyword);
		i ++;

		if (i && ! (i % limit))
		{
			fprintf (stdout, "\n");
			lines ++;

			if (! (lines % PAGE_BREAK)) rtc (stderr);
		}
	}

	if (i % limit) fprintf (stdout, "\n");
}



void list_keywords ()
{
	char *desc = "These are the valid Commodore Basic keywords for";

	disp_list (desc, keywords, "%-15s", 4);
}


void list_special ()
{
	char *desc = "These are the valid special substitions for";

	disp_list (desc, special, "%-20s", 3);
}


void list_extensions ()
{
	disp_list ("Final Cartridge 3 Basic Extension keywords for",
		final_cart_3, "%-15s", 4);
	rtc (stderr);
	disp_list ("Graphics52 Basic Extension keywords for",
		graphics_52, "%-15s", 4);
	rtc (stderr);
	disp_list ("6510+ Assembler Extension keywords for",
		asm6510plus, "%-15s", 4);
}


void help ()
{
	fprintf (stdout, "\n%s: Text <==> C64 Basic Converter\n", program);
	fprintf (stdout, "Version %s %s %s\n", VERSION, VDATE, VCOMM);
	fprintf (stdout, "Copyright (c) %s %s %s\n", COPYRIGHT, AUTHOR, EMAIL);
	fprintf (stdout, "Extension 6510+ keywords added by Martin Piper (martin.piper@gmail.com)\n");
	fprintf (stdout, "%s\n", WEB);
	fprintf (stdout, "This software is FREEWARE and is freely distributable.\n");

	if (strcmp (program, PROGNAME)) fprintf (stdout, "\n%s%s%s\n",
		"This program was originally named ",
		PROGNAME,
		".EXE.");

	fprintf (stdout, "\n%s%s%s%s%s%s%s\n",
		program,
		" converts ascii text file representations of Commodore basic",
		"\nprogram listings into program images suitable to run on a",
		" C64.  It \nalso converts basic programs back into ascii text",
		" listings.  ",
		program,
		"\ndoes NOT perform syntax checking.");

	fprintf (stdout, "\n%s%s%s\n",
		"*** Use the \"",
		cmds [LIST].command,
		"\" command line argument for a list of valid commands.");

	fprintf (stdout, "\n%s%s%s\n",
		"You may specify multiple commands and/or",
		" file names on the command \nline in the order you want the",
		" commands to occur.");

	fprintf (stdout, "\n%s%s%s%s%s%s%s%s\n",
		"Default extensions are \"",
		TXT_EXT,
		"\" for ascii text files and \"",
		PRG_EXT,
		"\" for \nCommodore Basic program files.  That means you",
		" may specify extensions \nbut if you don't ",
		program,
		" will look for files with the defaults.");

	rtc (stderr);

	fprintf (stdout, "%s%s%s%s%s%s%s%s%s%s%s\n",
		"An input text file may contain",
		" multiple program listings.  Use the \n\"",
		cmds [MULT].command,
		"\" command to enable this function.  Mark the start of each ",
		"\nlisting with \"",
		START_PROC,
		" {file name}\" and the end with \"",
		STOP_PROC,
		"\".  \nThis feature is only available with the \"",
		cmds [TOPRG].command,
		"\" command.");

	fprintf (stdout, "\n%s%s%s%s%s%s%s%s%s%s%s\n",
		"Keywords, variables, and quoted named identifiers are NOT",
		" case-\nsensitive but quoted strings and comments ARE.",
		" So, if you issue a\n\"POKE 53272,21\" command on your C64 the",
		" quoted strings will appear as\nupper case and graphics.  But",
		" if you do a \"POKE 53272,23\" the strings\nwill be lower case and",
		" upper case.  In other words, ASCII to PETASCII\nconversion is performed"
		" automatically.  To represent special characters\nin quoted strings",
		" use named identifiers, like \"",
		CLEAR_WORD,
		"\", or curly-\nbracketed ascii values, between \"{001}\"",
		" and \"{255}\".  You may also\nspecify quantities like",
		" \"{down*10}\" or \"{001*5}\".");

	fprintf (stdout, "\n%s%s%s%s%s%s%s%s%d%s\n",
		"You may break up the ascii text lines by appending a \"\\\"",
		" continuation\ncharacter to the end of a line.  The next line of",
		" text will be combined\nwith the current line to create one numbered",
		" Basic command line.  So,\nthe next line of text should NOT have a",
		" line number, and any leading\nwhitespace characters will be",
		" ignored.  Do NOT break up a keyword in\ncontinuing a line -- ",
		program,
		" will not parse it correctly.  BTW, the\nmaximum line length is ",
		MAXLINE - 2,
		" characters.");

	rtc (stderr);

	fprintf (stdout, "%s%s%s%s%s%s%s%s%s%s%s\n",
		"If you find that much of a listing will require many continuation",
		" chars \nand each logical (basic) line is hanging-indented, use the ",
		cmds [COL].command,
		" command. \n",
		program,
		" will start a new basic line when a line number starts in column\n",
		"one and will continue the line, ignoring the physical end of line",
		" (cr/lf), \nuntil another number appears in column one or the end of",
		" the file is \nreached. All but one leading spaces are ignored when",
		" continuing a line. \nAny non-numeric chars in the first column are",
		" not allowed.");

	fprintf (stdout, "%s%s%s%s\n",
		"Use TRANS64, Bernhard Schwall",
		" <schwall@athene.informatik.uni-bonn.de>,",
		"\nor some other utility program to transfer your basic programs",
		" between \nyour PC and C64 or emulator disk images.");

	fprintf (stdout, "\n%s%s%s%s%s%s%s\n",
		"I would like to give special thanks to the following people.",
		" Peter \nKarlsson (Fidonet 2:204/145.42) <Peter.T.Karlsson@abc.se>",
		" for \nassisting with the keyword/token list and with testing.",
		" Also to Phil \nHoff <phoff@ecst.csuchico.edu> for heavy-duty",
		" testing and pushing \nfor more features. Ville Muikkula",
		" <vmuikku@yrttis.ratol.fi> for testing \nand suggestions for",
		" special char replacement.");

	rtc (stderr);

	fprintf (stdout, "\n%s%s%s%s%s%s\n",
		"OK, now for the scary part.  This software is provided to you",
		" \"AS-IS\".\nNo other warranties of any kind, express or implied,",
		" are made to you\nas to the software or any medium it may be on,",
		" including but not\nlimited to warranties of merchantability or",
		" fitness for a particular\npurpose.  So use at your own risk.",
		"  Whew!  Are you still there?");

	fprintf (stdout, "\n%s%s%s%s%s%s%s\n",
		"If you like ",
		program,
		" and/or find it useful please consider sending me",
		"\nan email or a couple bucks to encourage me to continue",
		" improving \nit or write other useful software.  Thanks -- ",
		AUTHOR,
		".");
}


void main (int argc, char *argv [])
{
	FILE *in, *out;
	int toprg = TRUE, mult = FALSE, lower = TRUE, stomp = FALSE;
	int filegiven = FALSE, i, k, casemode = TRUE, colmode = FALSE;
	char fin [MAXPATH], fout [MAXPATH], inmode [3], outmode [3];
	char in_ext [MAXEXT], out_ext [MAXEXT], fext [MAXEXT];

	kw_list [0] = keywords;
	kw_list [1] = KW_NULL;
	get_prog_name (argv [0]);

	if (argc == 1) usage ();
	else
	{
		for (i = 1; i < argc; i++)
		{
			for (k = 0; k < (int)strlen (argv [i]); k++)
				argv [i][k] = tolower (argv [i][k]);

			if (argv [i][0] == '/' || argv [i][0] == '-')
			{
				if (! strcmp (argv [i], cmds [TOTXT].command))
					toprg = FALSE;
				else if (! strcmp (argv [i], cmds [TOPRG].command))
					toprg = TRUE;
				else if (! strcmp (argv [i], cmds [MULT].command))
					mult = TRUE;
				else if (! strcmp (argv [i], cmds [NOMULT].command))
					mult = FALSE;
				else if (! (strcmp (argv [i], cmds [HELP].command)
					&& strcmp (argv [i], "/h")
					&& strcmp (argv [i], "/?")
					&& strcmp (argv [i], "-h")))
					help ();
				else if (! strcmp (argv [i], cmds [LIST].command))
					list_cmds ();
				else if (! strcmp (argv [i], cmds [KEYWORDS].command))
					list_keywords ();
				else if (! strcmp (argv [i], cmds [SPECIAL].command))
					list_special ();
				else if (! strcmp (argv [i], cmds [EXTENSIONS].command))
					list_extensions ();
				else if (! strcmp (argv [i], cmds [LOWER].command))
					lower = TRUE;
				else if (! strcmp (argv [i], cmds [UPPER].command))
					lower = FALSE;
				else if (! strcmp (argv [i], cmds [LOWCASE].command))
					casemode = FALSE;
				else if (! strcmp (argv [i], cmds [CASE].command))
					casemode = TRUE;
				else if (! strcmp (argv [i], cmds [COL].command))
					colmode = TRUE;
				else if (! strcmp (argv [i], cmds [NOCOL].command))
					colmode = FALSE;
				else if (! strcmp (argv [i], cmds [STOMP].command))
					stomp = TRUE;
				else if (! strcmp (argv [i], cmds [NOSTOMP].command))
					stomp = FALSE;
				else if (! strcmp (argv [i], cmds [FINCART3].command))
				{
					kw_list [0] = final_cart_3;
					kw_list [1] = keywords;
					kw_list [2] = KW_NULL;
				}
				else if (! strcmp (argv [i], cmds [GRAPH52].command))
				{
					kw_list [0] = graphics_52;
					kw_list [1] = keywords;
					kw_list [2] = KW_NULL;
				}
				else if (! strcmp (argv [i], cmds [ASM6510PLUS].command))
				{
					kw_list [0] = asm6510plus;
					kw_list [1] = keywords;
					kw_list [2] = KW_NULL;
				}
				else if (! strcmp (argv [i], cmds [C64].command))
				{
					kw_list [0] = keywords;
					kw_list [1] = KW_NULL;
				}
				else
				{
					fprintf (stderr, "%s: invalid command \"%s\".\n",
						program, argv [i]);
					exit (EXITBAD);
				}
			}
			else
			{
				filegiven = TRUE;

				if (toprg)
				{
					strcpy (in_ext, TXT_EXT);
					strcpy (out_ext, PRG_EXT);
					strcpy (inmode, "rt");
					strcpy (outmode, "wb");
				}
				else
				{
					strcpy (in_ext, PRG_EXT);
					strcpy (out_ext, TXT_EXT);
					strcpy (inmode, "rb");
					strcpy (outmode, "wt");
				}

				/* get base name and check if there's an extension */
				if (! basename (argv [i], fout, MAXPATH - 1))
				{
					/* nope, no extension specified */
					strcpy (fin, fout);
					strncat (fin, in_ext, MAXPATH - 1);
				}
				else
				{
					strncpy (fin, argv [i], MAXPATH - 1);
					extension (fin, fext, MAXEXT - 1);

					if (! strcmp (fext, out_ext))
					{
						fprintf (stderr, "%s: not expecting \"%s\" as input.\n",
							program, argv [i]);
						exit (EXITBAD);
					}
				}

				strncat (fout, out_ext, MAXPATH - 1);

				if ((in = fopen (fin, inmode)) == NULL)
				{
					fprintf (stderr, "%s: Can't open %s for input.\n",
						program, fin);
					exit (EXITBAD);
				}
				else if (mult)
				{
					fprintf (stderr, "processing %s ...\n", fin);
					process_mult (in, stomp, casemode, colmode);
					fprintf (stderr, "processing of %s complete.\n", fin);
				}
				else
				{
					if (! exit_exists (stomp, in, fout))
					{
						if ((out = fopen (fout, outmode)) == NULL)
						{
							fprintf (stderr, "%s: Can't open %s for output\n",
								program, fout);
							exit (EXITBAD);
						}
						else
						{
							fprintf (stderr, "%s ==> %s ...", fin, fout);
							if (toprg) txt2prg (in, out, casemode, colmode);
							else prg2txt (in, out, fin, lower);
							fclose (out);
							fprintf (stderr, " done.\n");
						}
					}

					fclose (in);
				} /* end can open input file */
			} /* end command line arg check */
		} /* end for loop thru args */
	} /* end if check for args */

	if (! filegiven) usage ();
	exit (EXITOK);
} /* end main */
