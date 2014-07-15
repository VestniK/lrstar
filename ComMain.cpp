
		#ifdef LRSTAR
      char* program = "LRSTAR";
		#endif
		#ifdef DFASTAR
      char* program = "DFASTAR";
		#endif
      char* version = "6.3.007";
      char* copywrt = "Copyright 2014 Paul B Mann";

		#ifdef  _DEBUG
  		#include "conio.h"
		#endif																						 

      #define   MAIN
		#include "ComGlobal.h"
		#include "ComGenerate.h"

		#ifdef LRSTAR
		#include "PGGlobal.h"

      OPTION PGOption[]= // Parser Generator Options
		{
			"a",			"Analyze the grammar only",									PG_ANALYZEONLY,      0,
			"ast",		"Parser will build an AST",									PG_ASTCONST,			1,
		//	"bm",		   "Boolean matrix (0=none,1=bytes,2=bits)",					PG_BOOLMATRIX,			2,
			"c",			"Conflict report file",											PG_CONFLICTS,			0,
		  	"clr",		"Canonical LR(1) parser",										PG_CLR_PARSER,			0,
			"ct",			"Conflict traceback information",							PG_CONFLICTTRACE,	   0,
			"d",			"Debug option for generated parser",						PG_DEBUG,			   0,
		// "dr",			"Default reductions",											PG_DEFAULTRED,		   1,
			"e",			"Error count limit for grammar errors",					PG_ERRORCOUNT,	      500,
			"eof",		"End-of-file character stops parsing",						PG_EOFINCLUDED,		1,
			"exp",		"Expecting list for syntax errors (1,2)",					PG_EXPECTING,			0,
			"g",			"Grammar listing (1,2)",										PG_GRAMMAR,				0,
		//	"h",			"HTML grammar listing",											PG_HTML,					0,
			"k",			"Keyword checking in grammar", 								PG_KEYWORDCHECK,		0,
		  	"lalr",		"LALR(1) parser",													PG_LALR_PARSER,		0,
		  	"lr",		   "Minimal LR(1) parser",											PG_LR_PARSER,			1,
			"m",			"Minimize parser-table size",									PG_MINIMIZE,			0,
			"na",			"Number of arguments for node actions",					PG_NODEARGS,			0,
			"nd",			"Nondeterministic LR(k) parsing (1,2)",					PG_NONDETER,			0,
			"o",			"Optimize parser speed",										PG_OPTIMIZE,			0,
			"q",			"Quiet mode, minimal screen display",						PG_QUIET,				0,
			"rr",			"Reduce-reduce conflicts only",								PG_REDUCEONLY,			0,
			"s",			"State machine listing (1, 2)",								PG_STATELIST,			0,
		//	"sra",		"Shift-Reduce Actions in parser",							PG_SHIFTREDUCE,		1,
			"tab",		"Tab setting in grammar (2,3,4)",							PG_TAB,					3,
			"v",			"Verbose mode, output more information",					PG_VERBOSE,				1,
			"w",			"Warnings listing",												PG_WARNINGS,			0,
			"x",			"Defined by user (x=?)",										PG_OPTION_X,			0,
			"y",			"Defined by user (y=?)",										PG_OPTION_Y,			0,
			"z",			"Defined by user (z=?)",										PG_OPTION_Z,			0,
			"",			"",																	0,				         0
		};
		#endif

		#ifdef DFASTAR
		#include "LGGlobal.h"

      OPTION LGOption[]= // Lexer Generator Options
		{
			"a",			"Analyze the grammar only",	   							LG_ANALYZEONLY,		0,
			"col",		"Column number counting in lexer",							LG_COLNUMB,			   0,
			"d",			"Debug option for generated lexer",							LG_DEBUG,			   0,
			"e",			"Error count limit for grammar errors",					LG_ERRORCOUNT,	      256,
			"g",     	"Grammar listing",  												LG_GRAMMAR,          0,
			"i",			"Case insensitive lexer ('a'='A')",							LG_INSENSITIVE,		0,
			"line",  	"Line number counting in lexer",								LG_LINENUMB,         1,
			"m",			"Minimize lexer-table size",									LG_MINIMIZE,			1,
			"ki",     	"Keyword and <identifier> recognition",					LG_KEYWORDIDENT,	   1,
			"q",			"Quiet mode, minimal screen display",						LG_QUIET,				0,   
			"r",    		"Remove duplicate states",										LG_REMOVEDUP,			1,
			"s",    		"State machine for conflicts report",		   			LG_STATELIST,			0,
			"so",   		"Optimized state machine",										LG_STATELISTOPT,		0,
			"tab",		"Tab setting in grammar (2,3,4)",							LG_TAB,					3,
			"ts",    	"Table-driven small lexers",									LG_TABL_SMALL,		   0,      
			"tm",    	"Table-driven medium lexers (10% faster)",				LG_TABL_MEDIUM,		1,   
			"v",     	"Verbose mode, output more information",					LG_VERBOSE,		      1,
			"w",     	"Warnings listing",												LG_WARNINGS,			0,  
			"",			"",																	0,				         0
		};
		#endif

      OPTION MAOption[]= // Memory Allocation Options
		{
			"/sym",		"Symbols",															MAX_SYM,			  20000,
			"/pro",		"Productions",														MAX_PRO,		     50000,
			"/tail",		"Tail symbols",													MAX_TAIL,		 200000,
			"/ebnf", 	"EBNF symbol space",												MAX_EBNF,	     10000,
			"/sta",		"States",															MAX_STA,			 300000,
			"/fin",		"Final items",														MAX_FIN,			 300000,
			"/ker",		"Kernel items",													MAX_KER,		  10000000,
			"/ntt",		"Nonterminal transitions",										MAX_NTT,		    200000,
			"/tt",		"Terminal transitions",											MAX_TT,		   2000000,
			"/tta",		"Terminal transitions added",									MAX_TTA,		   5000000,
			"/la",		"Lookaheads",														MAX_LA,		  10000000,
			"/lb",		"Lookbacks",														MAX_LB,		   2000000,
			"/inc",		"Includes",															MAX_INC,		    500000,
			"/ch",		"Child nodes in graph",											MAX_CH,		    500000,
			"/nd",		"Nondeterministic actions",									MAX_ND,		    100000,
			"",			"",																	0,				         0
      };

		void   Options ();
		void   InitOptions ();

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   main (int na, char *arg[])
{
		printf ("\n%s %s %s.\n", program, version, copywrt);
		if (na == 1) // No arguments after program name?
		{
			Options();
			quit (0);
		}

		if (get_fid (arg[1], gdn, gfn, gft)) 
		{
			int i;
			char dn[256], fn[256], ft[256];
			char GFT [MAX_FILETYPE]; 

			#ifdef LRSTAR
			if (*gft == 0) strcpy (gft, ".grm"); // Default filetype.
			strcpy (grmfid, gdn);
			strcat (grmfid, gfn);
			strcat (grmfid, gft);
			strcpy (GFT, gft);
			strlwr (GFT);
			if (strcmp (GFT, ".grm") != 0)
			{
				n_errors++;
				printf ("\nInput file '%s' does not have a filetype of '.grm'\n\n", grmfid);
				quit (0);
			}
			#endif

			#ifdef DFASTAR
			if (*gft == 0) strcpy (gft, ".lgr"); // Default filetype.
			strcpy (grmfid, gdn);
			strcat (grmfid, gfn);
			strcat (grmfid, gft);
			strcpy (GFT, gft);
			strlwr (GFT);
			if (strcmp (GFT, ".lgr") != 0)
			{
				n_errors++;
				printf ("\nInput file '%s' does not have a filetype of '.lgr'\n\n", grmfid);
				quit (0);
			}
			#endif

			filedesc = open (grmfid, 0); 
			if (filedesc < 0)
			{
				n_errors++;
				printf ("\nCannot find input file: '%s'\n\n", grmfid);
				quit (0);
			} 

			close (filedesc);
 			open_log (grmfid);
			open_con (grmfid);
			open_grm (grmfid);
			open_sta (grmfid);
			open_lst (grmfid);

		  	prt_logonly ("\n%s %s %s.\n", program, version, copywrt);
			prt_log     ("\nInput     %s ", grmfid);

			InitOptions ();
	      PRT_OPTNS (na, arg);
			if (SET_OPTNS (na, arg) == 0) 
			{
				Terminate (0);
			}

			for (i = 2; i < na; )
			{
				#ifdef WINDOWS
				if (arg[i][0] == '/') break;  // Options start.
				#endif
				#ifdef UNIX
				if (arg[i][0] == '-') break;  // Options start.
				#endif
				GetSkeletonFilename (arg[i], i++, na); 
				GetOutputFilename   (arg[i], i++, na); 
			}

			if (get_fid (arg[0], dn, fn, ft) == 0) 
			{
				Terminate (0);
			}
			if (GetMaxValues (dn, "lrstar.txt") == 0) 
			{
				Terminate (0);
			}

			#ifdef LRSTAR
			int verbose = optn[PG_VERBOSE];
			if (PG::Main (na, arg))
			#endif

			#ifdef DFASTAR
			int verbose = optn[LG_VERBOSE];
    		if (LG::Main (na, arg))
			#endif
			{
    			int i = 2; 
				int n = 0;
				int r = 1; 
				while (r > 0 && i < na)
				{
					#ifdef WINDOWS
					if (arg[i][0] == '/') break;  // Options start.
					#endif
					#ifdef UNIX
					if (arg[i][0] == '-') break;  // Options start.
					#endif
					if (r = GetSkeletonFilename (arg[i], i, na))
					{
						i++;
						if (r = GetOutputFilename (arg[i], i, na))
						{
							i++; n++;
							Generate::GenerateCode (sklfid, outfid, verbose); 
						}
					}
				}
				if (n)
				{
					if (verbose > 1) prt_log ("\n");
					else             prt_logonly ("\n");
				}
			}
		}
		Terminate (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

#ifdef LRSTAR
void  Options ()
{
		int i;
      char  programdesc[] =                  
			"|\n"
			"|   LR(k) PARSER GENERATOR\n"
			"|\n"
			#ifdef WINDOWS
			"|   lrstar <grammar> [<skeleton> <output>]... [/<option>...]\n"
			#endif
			#ifdef UNIX
			"|   lrstar <grammar> [<skeleton> <output>]... [-<option>...]\n"
			#endif
      ;
		printf ("%s|\n", programdesc);
		printf ("|   OPTION  DEFAULT  DESCRIPTION\n");
		for (i = 0; *PGOption[i].name != 0; i++)
		{
			printf ("|   %-6s  %5d    %s\n", PGOption[i].name, PGOption[i].defvalue, PGOption[i].desc);
		}
		printf ("|_\n");
}
#endif

#ifdef DFASTAR
void  Options ()
{
		int i;
      char  programdesc[] =                  
			"|\n"
			"|   DFA LEXER GENERATOR\n"
			"|\n"
			#ifdef WINDOWS
			"|   dfastar <grammar> [<skeleton> <output>]... [/<option>...]\n"
			#endif
			#ifdef UNIX
			"|   dfastar <grammar> [<skeleton> <output>]... [-<option>...]\n"
			#endif
      ;
		printf ("%s|\n", programdesc);
		printf ("|   OPTION  DEFAULT  DESCRIPTION\n");
		for (i = 0; *LGOption[i].name != 0; i++)
		{
			printf ("|   %-6s  %5d    %s\n", LGOption[i].name, LGOption[i].defvalue, LGOption[i].desc);
		}
		printf ("|_\n");
}
#endif

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  InitOptions ()
{
		int i;
		for (i = 0; i < N_OPTIONS; i++) 
		{
			optn[i] = 0;
		}
		#ifdef LRSTAR
		for (i = 0; * PGOption[i].name != 0; i++)
		{
			optn[PGOption[i].numb] = PGOption[i].defvalue;
		}
		#endif
		#ifdef DFASTAR
		for (i = 0; * LGOption[i].name != 0; i++)
		{
			optn[LGOption[i].numb] = LGOption[i].defvalue;
		}
		#endif
		for (i = 0; * MAOption[i].name != 0; i++)
		{
			optn[MAOption[i].numb] = MAOption[i].defvalue;
		}

		#ifdef LRSTAR
	// Define invisible PG options ...
      optn [PG_BACKSLASH]    	= 0;
      optn [PG_SYMBOLS]     	= 0;
      optn [PG_TRANSITIONS] 	= 1;
      optn [PG_BOOLMATRIX] 	= 2;
      optn [PG_DEFAULTRED] 	= 1;
      optn [PG_HTML] 			= 0;
      optn [PG_SHIFTREDUCE] 	= 1;
		#endif

		#ifdef DFASTAR
	// Define invisible LG options ...
		optn [LG_TABLES]        = 1;
      optn [LG_BACKSLASH]    	= 0;   
      optn [LG_CONFLICTS]    	= 1;   
	   optn [LG_REDUCEONLY]    = 1; // Should be "1" else user will be confused by all the SR conflicts.  
      optn [LG_TABL_LARGE]   	= 0;   
      optn [LG_TABL_EXTRA]    = 0;   
      optn [LG_TRANSITIONS] 	= 1;   
		#endif
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  quit (int rc)
{
		#ifdef _DEBUG
	  	printf ("Press any key to continue ...\n");
	  	while (!_kbhit());
		#endif
		exit (rc);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  InternalError (int n)
{
		n_errors++;
		if (n_errors == 1) printf ("\n");
      printf ("INTERNAL ERROR %d\n\n", n);
      Terminate (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
      
void  MemCrash (char* msg)
{
      prt_log ("\n%s\n", msg);
      Terminate (1);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  MemCrash (char* value, int n)
{
      n_errors++;
		if (n_errors == 1) prt_log ("\n");
      prt_log ("%s(%04d) : %s exceeds the limit of %d.\n", exefid, 1, value, n);
		prt_log ("%s(%04d) : %s\n", exefid, 1, "Please increase the appropriate maximum value in this file:");
  		prt_log ("%s(%04d) : %s\n", exefid, 1, "<-- double click here.");
      Terminate (1);
}

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
