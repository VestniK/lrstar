
#ifdef DFASTAR

		#include "ComGlobal.h"
		#include "LGGlobal.h"
      #include "LGActions.h"
		#include "LGGlobal.h"

		char**  	LGActions::Defcon_term;
		char**  	LGActions::head_name;
		int*    	LGActions::head_sym;
		int*    	LGActions::head_line; 
		int*    	LGActions::term_line; 
		int*    	LGActions::prod_len;
		int*    	LGActions::prod_line;
		int*    	LGActions::nullable;
		int     	LGActions::n_nulls;
		int     	LGActions::n_constants;
		int     	LGActions::N_prods;
		int     	LGActions::n_prods;
		int     	LGActions::N_tails;
		int     	LGActions::n_tails;
		int     	LGActions::amt_space;
		int*    	LGActions::Tail;
      int*    	LGActions::F_tail;
		char**  	LGActions::term_name;
		int*    	LGActions::term_type;

		char**   LGActions::str_start;
		char**   LGActions::code_start;
	   int*     LGActions::defcon_value;
		char**   LGActions::defcon_name;
	   int*		LGActions::ret_value;
		char**   LGActions::ret_name;
		char**   LGActions::ret_term;
		int*     LGActions::ret_numb;
		int*     LGActions::prod_type;
		int    	LGActions::max_trans;
		int    	LGActions::max_errs;      
		int    	LGActions::max_final;
		int    	LGActions::max_include;
		int    	LGActions::max_kernel;
		int    	LGActions::max_tt;
		int    	LGActions::max_ntt;
		int    	LGActions::max_lookback;
		int    	LGActions::max_prods;
		int    	LGActions::max_ebnf;
		int    	LGActions::max_states;
		int    	LGActions::max_symbs;
		int    	LGActions::max_tails;
		int    	LGActions::max_space;
		int    	LGActions::max_lev;
		int    	LGActions::max_code;
		int    	LGActions::max_ttas;
		int    	LGActions::max_n_prods;

		char  	LGActions::errormsg[128];
		short 	LGActions::arrow_col = 0;
		short 	LGActions::prev_token;
		int*  	LGActions::tail;
		char* 	LGActions::pool_ptr;
		char* 	LGActions::pool_end;
		int   	LGActions::n_tokens;
		int*		LGActions::f_prod;
		int*		LGActions::l_prod;
		int*		LGActions::f_tail;
		int*		LGActions::l_tail;
		int		LGActions::n_symbs;
		int		LGActions::n_heads;
		int		LGActions::n_keywords;
		int		LGActions::n_strings;
		int		LGActions::n_actioncodes;
		int		LGActions::n_terms;
		int		LGActions::n_lex_terms;

		char*  	LGActions::tokenstartstr; 
		char*  	LGActions::tokenendstr; 
		char*  	LGActions::tokenlinestr; 
		char*  	LGActions::linenumbstr; 
		char*  	LGActions::colnumbstr; 
		char*  	LGActions::tabstopstr;
		int    	LGActions::anl_stat_memory;
		int    	LGActions::amt_code;
		int*   	LGActions::camefrom2;
		int    	LGActions::end_symb;	// $end	(128 | 256)
		int*   	LGActions::final2;
		int    	LGActions::final_state;
		int    	LGActions::gen_tabl_memory;
		int    	LGActions::get_memory_data;
		int*   	LGActions::head_type;
		int*   	LGActions::head_level;
		int    	LGActions::illegal_char_state;
		int    	LGActions::n_genliterals;
		int    	LGActions::n_original_prods;
		int    	LGActions::n_outputfiles;
		int    	LGActions::n_added;
		int    	LGActions::n_levels;
		int    	LGActions::n_bytes;
		int    	LGActions::NDLeft;
		int    	LGActions::n_nonttran;
		int    	LGActions::n_oprods;
		int    	LGActions::n_termtran;
		int    	LGActions::n_words;
		int*   	LGActions::ntt_goto2;
		int*   	LGActions::ntt_symb2;
		int    	LGActions::n_totalterm; 
		int    	LGActions::n_codelines;
		int    	LGActions::opt_stat_memory;
		int    	LGActions::parser_memory;
		int*   	LGActions::possibil;   
		int    	LGActions::rr_con;
		int    	LGActions::SLA_expired;
		int    	LGActions::sr_con;
		int*   	LGActions::st_type;
		int*   	LGActions::tt_goto2;

		#define  TAIL_SYMBOL 0
		#define  HEAD_SYMBOL 1

		enum re_operator
		{
			ZERO,
			GBEG,    // (
			GEND,    // )                
			OBEG,    // [              
			OEND,    // ]
			ORSYM,   // |
			PLUS,		// +    
			ASTER,	// *    
			QUEST,   // ?
			DOTS,	   // ...    
			NOTHING    
		};

		static int    n_cells;
		static uint   hash_divide;
		static char*  done;
		static int*   onstack;
		static int    n_onstack;
		static int    n_setnames;
      static char** sym_start;
      static int*   sym_leng;
      static int*   sym_numb;
      static int*   sym_line;
      static int*   sym_prod1;
      static int*   sym_prod2;
      static int*   sym_type;
		static int*   sym_indx;
      static int*   sym_vect;
      static int*   L_tail;
      static char*  ebnfspace;
      static char*  ebnfspace_start;

      static int    c;
      static int    warning_off = 0;
      static uchar  char_set [256];
      static int    range_start;
      static int    range_end;
      static char*  ebnfspace_end;
      static int**  ebnflevel;
      static int**  ebnflevel_start;
      static int**  ebnflevel_end;
      static int*   ebnfcode;
      static int*   ebnfcode_start;
      static int*   ebnfcode_end;
      static int    curr_symb;
      static int    curr_head;
      static int    curr_terminal;
      static int    goal_symb;

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    error                                                                  //

int   LGActions::error (int t, int a) // New version                   
{
		if (token.end == token.start) token.end++;
      return (t); // t = 0 
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 
//    firstsym()   																			  //

int   LGActions::firstsym (int t, int a) // New version
{
		if (line_pos == 1)
		{
			if (token.column < 6)										
			{
				prev_token = HEAD_SYMBOL;
				return (arg_numb[a]);   // 1st arg.
			}
			if (token.column < 7) 
			{
				prt_error ("'%s' is out of place, must start before column 6", token.start, token.end, token.line);
				LG::Terminate (0);
				return (0);
			}
		}
		prev_token = TAIL_SYMBOL;
		return t;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    arrow                                                                  //

int   LGActions::arrow (int t, int a) // New version                                   
{
		arrow_col = token.column;
		if (arrow_col < 4)
		{
			prt_error ("'%s' cannot start before column 4", token.start, token.end, token.line);
			LG::Terminate (0);
			return (0);	
		}
		if (line_pos == 1) return (t);
		if (line_pos == 2)
		{
			if (prev_token == HEAD_SYMBOL) return (t);
		}
		prt_error ("'%s' is out of place, must be the first symbol on the next line", token.start, token.end, token.line);
		LG::Terminate (0);
		return 0;
}										 

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    EOGI: End Of Generated Input (.lex file, generated by PG).

int   LGActions::EOGI (int t, int a)                                   
{
		if (lexer_input == 0) // %% found in .lex file?
		{
			prt_log ("%s.lgr file: reading ...\n\n", gfn);
			strcpy (gft, ".lgr");
			strcpy (grmfid, gdn);
			strcat (grmfid, gfn);
			strcat (grmfid, gft);
			if (!inputi ("")) return 0;
			init_lexer ();
		}
		else // %% found in .lgr file!
		{
			/* nothing */
		}
		return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//    Process Options 

int   LGActions::PRO_OPTNS (int prod, int a) 
{
		int  rc = 0, x, OK;
		char *p, *opt, c;
		x = arg_numb [pact_arg[prod]];
		p = T_start;             
		p++;                                   // bypass "
Top:	while (*p == ' ' || *p == '\t' || *p == ',') p++;   // bypass spaces, tabs & commas.
		opt = p;
		while (*p != '\n' && *p != 26 && *p != '"' && *p != ',') p++;
		c = *p;
		*p = 0;
		OK = SET_OPTN (opt, grmfid, PS[x-1].line);
		if (!OK) rc = 1;  // fatal error
		*p++ = c;
		if (c == '\"') goto Done;
		goto Top;

Done:	if (rc) return (rc);
		rc = CheckOptions ();
		if (rc == 0) return 1;	// Error.
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
      
int	LGActions::CheckOptions ()
{
		static int first_time = 1;
		if (optn [LG_TAB] < 2)	
		{
         n_errors++;
         printf ("%s(%04d) : Tab setting cannot be less than 2.\n", grmfid, 1);
         return (0);
		}
		if (optn [LG_TAB] > 4)	
		{
         n_errors++;
         printf ("%s(%04d) : Tab setting cannot be greater than 4.\n", grmfid, 1);
         return (0);
		}
		tab = optn[LG_TAB]; // Set tab value in lexer !!!
		if (optn[LG_ERRORCOUNT] == 0)	        optn[LG_ERRORCOUNT] = 1000; 
   	else if (optn[LG_ERRORCOUNT] > 10000) optn[LG_ERRORCOUNT] = 1000; 

		if (first_time)
		{
			if (optn [LG_CONFLICTS] == 1) // 'c' specified?
			{
				optn [LG_REDUCEONLY]  = 0; 
			}
			else
			{
				optn [LG_CONFLICTS]   = 1;   
				optn [LG_REDUCEONLY]  = 1; // Should be "1" else user will be confused by all the SR conflicts.  
			}
		}

		int n_outputs = 0;
		if (optn[LG_TABL_SMALL])	n_outputs++;  
		if (optn[LG_TABL_MEDIUM])	n_outputs++;
		if (optn[LG_TABL_LARGE])	n_outputs++;
		if (optn[LG_DIRECTCODE])	n_outputs++;
		if (n_outputs == 0) optn[LG_TABL_MEDIUM] = 1;
		if (n_outputs >  1)
		{
         n_errors++;
			printf ("Use only one of the following options: ts, tm, tl, dc.\n\n");
		}

		if (optn[LG_QUIET]) optn[LG_VERBOSE] = 0;

		if (optn[LG_STATELIST] && optn[LG_STATELISTOPT])
		{
         n_errors++;
         printf ("State machine option should be 'S' or 'SO', but not both.\n\n");
		}

		if (first_time)
		{
			if (optn[LG_NUMBCHAR] ==   0) 
			{
				optn[LG_NUMBCHAR] = 256; // If undefined.
			}
			else if (optn[LG_NUMBCHAR] != 128 && optn[LG_NUMBCHAR] != 256)
			{
				n_errors++;
				printf ("Option 'nc' must be 128 or 256.\n\n");
			}
			max_char_set = optn[LG_NUMBCHAR];
		}

		max_errors      = optn[LG_ERRORCOUNT];
		option_grammar  = optn[LG_GRAMMAR];
		option_warnings = optn[LG_WARNINGS];

		first_time = 0;
		if (n_errors > 0) return 0;
      return (1);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::init ()
{
      int i;

      n_symbs      =   0;
      n_constants  =   0;
      n_strings    =   1;  // Start with one, because cannot return a zero. 
		n_actioncodes=   1;	// Start with one, because cannot return a zero. 
      n_tails      =   0;
      n_terms      =   0;
      n_heads      =   0;
      n_tokens     =   0;
      n_keywords   =   0;
      n_prods      =  -1;
      n_errors     =   0;
		pool_ptr     =   0;
		pool_end     =   0;

      max_symbs = optn[MAX_SYM ];
      max_prods = optn[MAX_PRO ];
      max_tails = optn[MAX_TAIL];
      max_space = optn[MAX_EBNF];
      max_code  = 1000; // maximum ebnf codes per production.
      max_lev   = 1000; // maximum ebnd nesting levels per production.

		n_cells     = 2*max_symbs;
		hash_divide  = UINT_MAX / n_cells + 1;

      ALLOC (sym_start, max_symbs);
      ALLOC (sym_leng,  max_symbs);
      ALLOC (sym_line,  max_symbs);
      ALLOC (sym_type,  max_symbs);
      ALLOC (sym_numb,  max_symbs);
      ALLOC (sym_indx,  max_symbs);
      ALLOC (sym_prod1, max_symbs);	// First production for NT.
      ALLOC (sym_prod2, max_symbs); // Last  production for NT.
      ALLOC (sym_vect,  n_cells);

      ALLOC (ret_numb,  max_prods);
      ALLOC (prod_line, max_prods);
      ALLOC (prod_type, max_prods);
      ALLOC (F_tail,    max_prods+1);
      ALLOC (Tail,      max_tails);

      ALLOC (ebnfspace_start, max_space);
      ALLOC (ebnflevel_start, max_lev);
      ALLOC (ebnfcode_start,  max_code);

      FASTINI ( 0, sym_line,  max_symbs);
      FASTINI ( 0, sym_type,  max_symbs);
      FASTINI (-1, sym_numb,  max_symbs);
      FASTINI ( 0, sym_prod1, max_symbs); 
      FASTINI ( 0, sym_prod2, max_symbs); 
      FASTINI (-1, sym_vect,  n_cells);
      FASTINI ( 0, ret_numb,  max_prods);

      for (i = 0; i < max_char_set; i++) char_set[i] = 0; 

      ebnfspace     = ebnfspace_start;
      ebnfspace_end = ebnfspace_start + max_space;

      ebnfcode      = ebnfcode_start;
      ebnfcode_end  = ebnfcode_start + sizeof(ebnfcode)*(max_code);

      ebnflevel     = ebnflevel_start;
      ebnflevel_end = ebnflevel_start + sizeof(ebnflevel)*(max_lev);

      curr_head     =  0;
      curr_terminal =  0;

      MAKE_TERMINALS ();
		START_GRM ();
      return (0);		
}

////////////////////////////////////////////////////////////////////////////////
//																										//
  
int   LGActions::MAKE_TERMINALS ()
{
      int i; 
      char token[16];
      for (i = 0; i < max_char_set; i++) 
      {
         if (i >= 32 && i <= 126) 
         {
				token[0] = '\'';
				token[1] = i;
				token[2] = '\'';
				T_start = token;
				T_end = token + 3;
         }
         else 
         {
				sprintf (token, "%d\0", i);
				T_start = token;
				if (i <    10) T_end = T_start + 1;
				else if (i <   100) T_end = T_start + 2;
				else if (i <  1000) T_end = T_start + 3;
				else if (i < 10000) T_end = T_start + 4;
				else           T_end = T_start + 5;
         }    
         ADD_TERMINAL();
      }
		T_start = "$end";
		T_end   = T_start + 4;
		ADD_TERMINAL ();
      return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::ADD_SYMBOL (char* name, int& symnumb) 
{
		T_start  = name;
		T_end    = T_start + strlen(name);
      symnumb = SYMNUMB ();
		return 0;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::ADD_TERMINAL () 
{
		int rc = 0;
      curr_terminal = SYMNUMB ();
		sym_type[curr_terminal] = TOKEN; 
		sym_line[curr_terminal] = 0;
		sym_numb[curr_terminal] = n_tokens++;
      return (rc);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::ADD_NOTERM (int p, int a) 
{
      ret_numb[n_prods] = MAX_INT; // Ignore this symbol (<spaces>, <comment>, etc.)
      return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

      static int mode = 0;
      #define TERMNO 1
      #define DEFCON 2

int   LGActions::ADD_TERMNO (int p, int a) 
{
		int rc = 0, n;
      if (mode != 0 && mode != TERMNO)
      {
			prt_error ("Cannot use numeric return values after defined constants were used", T_start, T_end, T_line);
         return (rc);
      }
      mode = TERMNO; // Establish terminal number mode.
		n = atoi (T_start);
		if (n == 0) 
		{
			prt_error ("Cannot use 0, which is reserved for the <error> symbol", T_start, T_end, T_line);
         return (rc);
		}
      for (p = 1; p < n_prods; p++)
      {
         if (ret_numb[p] > 0 && sym_numb[ret_numb[p]] == n) 
         {
				prt_error ("'%s' has already been used for another token", T_start, T_end, T_line);
            return (rc);
         }
      }
      ret_numb[n_prods] = n;     
      return (rc);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::ADD_DEFCON (int p, int a) 
{
		int s, rc = 0;
      if (mode != 0 && mode != DEFCON)
      {
			prt_error ("Cannot use defined constant return values after numeric values were used", T_start, T_end, T_line);
         return (rc);
      }
      mode = DEFCON; // Establish defined constant mode.
		s = SYMNUMB ();  
		if (sym_line[s] == 0) // New constant?
		{
			ret_numb[n_prods] = -s;    // Set to negative symbol number.
			sym_numb[s]  = n_prods;    // Set to production #
			sym_prod1[s] = curr_symb;  // Assign symbol number of current token.
			sym_line[s]  = T_line;
			sym_type[s] |= CONSTANT;
			n_constants++;
		}
		else // Old constant!
		{
			prt_error ("'%s' has already been used for another token", T_start, T_end, T_line);
		}
      return (rc);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::ADD_STRING (int p, int a) 
{
		int s, rc = 0;
		s = ADDSYM (T_start, T_end-T_start);  
		if (sym_line[s] == 0)				// First encounter?
		{
			sym_line[s] = T_line;			// Save first encounter of string.
			sym_type[s] |= STRING;
			sym_numb[s] = n_strings++;
		}
		ret_numb[n_prods] = sym_numb[s]; // Set to negative symbol number.
      return (rc);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::ADD_CODE (int prod, int a) 
{
		int   s;
		int   level = 0;
		char* p = token.start;
		int   codeblock = 1;
		int   codeblockline = line_numb;

Scan: while (*p != '{' && *p != '}' && *p != '\n' && *p != 26 && *p != '/' && *p != '\'' && *p != '"') p++;
		switch (*p)
		{
			case '{':
			{
				p++;
				level++;
				goto Scan;
			}
			case '}':
			{
				p++;
				if (--level) goto Scan;
				token.end = p;
				codeblock = 0;
				goto Ret;
			}
			case '\n':
			{
				p++;
				line_numb++;
				goto Scan;
			}
			case  26:
			{
				token.end = p;
				goto Ret;
			}
			case  '/':
			{
				p++;
				if (*p == '/')
				{
					p++;
					while (*p != '\n') p++;
					p++;
					line_numb++;
					goto Scan;
				}
				if (*p == '*')
				{
					p++;
Scan2:			while (*p != '\n' && *p != 26 && *p != '*') p++;
					switch (*p)
					{
						case '\n': 
						{
							p++;
							line_numb++;
							goto Scan2;
						}
						case 26:
						{
							token.end = p;
							goto Ret;
						}
						case '*':
						{
							p++;
							if (*p == '/')
							{
								p++;
								goto Scan;
							}
							goto Scan2;
						}
					}
				}
				goto Scan;
			}
			case  '\'':
			{
				p++;
Scan3:		while (*p != '\n' && *p != 26 && *p != '\\' && *p != '\'') p++;
				switch (*p)
				{
					case '\n':
					{
						token.end = p;
						return 0;
					}
					case 26:
					{
						token.end = p;
						goto Ret;
					}
					case '\\':
					{	
						p += 2;
						goto Scan3;
					}
					case '\'':
					{	
						p++;
						goto Scan;
					}
				}
			}
			case  '"':
			{
				p++;
Scan4:		while (*p != '\n' && *p != 26 && *p != '\\' && *p != '"') p++;
				switch (*p)
				{
					case '\n':
					{
						token.end = p;
						goto Ret;
					}
					case 26:
					{
						token.end = p;
						goto Ret;
					}
					case '\\':
					{	
						p += 2;
						goto Scan4;
					}
					case '"':
					{	
						p++;
						goto Scan;
					}
				}
			}
		}

Ret:	T_end = token.end;
		s = ADDSYM (T_start, T_end-T_start);  
		if (sym_line[s] == 0)				// First encounter?
		{
			sym_line[s] = T_line;			// Save first encounter of string.
			sym_type[s] |= ACTIONCODE;
			sym_numb[s] = n_actioncodes++;
		}
	//	printf ("symb_numb = %d\n", sym_numb[s]);
		ret_numb[n_prods] = sym_numb[s]; // Set to negative symbol number.
		return 0;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::START_GRM () 
{
		int rc = 0;
		short p = 0, a = 0;
		T_start = "$start";
		T_end   = T_start + 6;
		ADD_GOAL(p, a);								// Nonterminal 0
		ADD_PROD(p, a);

		T_start = "$token";
		T_end   = T_start + 6;
		ADD_TAIL(p, a);

		T_start = "$end";
		T_end   = T_start + 4;
		ADD_TAIL(p, a);								// Terminal 128 or 256
		end_symb = curr_symb;

		T_start = "$token";
		T_end   = T_start + 6;
		ADD_HEAD(p, a);								// Nonterminal 1
      return (rc);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::ADD_IGNORE (int p, int a) 
{
		ADD_TOKEN (p, a);
      ret_numb[n_prods] = MAX_INT; // Ignore this symbol (<spaces>, <comment>, etc.)
		return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::ADD_TOKEN (int p, int a) 
{
		ADD_PROD(p, a);
		ADD_TAIL(p, a);
		sym_type[curr_symb] |= OUTPUTSYM;			
		return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::ADD_TOKEN2 (int p, int a) 
{
		if (optn [LG_INSENSITIVE])
		{
			for (char* p = T_start+1; p < T_end-1; p++)
			{
				*p = lower[*p]; // Set each character to lower case and later add upper case. 
			}
		}
		ADD_PROD(p, a);
		ADD_TAIL(p, a);
		sym_type[curr_symb] |= OUTPUTSYM;			
		return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::ADD_KEYWORD (int p, int a) // Not used anymore, June 5 2013.
{
		n_keywords++;
		ADD_PROD(p, a);
		ADD_TAIL(p, a);
      return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::ADD_GOAL (int p, int a)
{
		int rc = 0;
      goal_symb = curr_head = SYMNUMB ();
      sym_line [goal_symb] = T_line;
      sym_type [goal_symb] = NONTERMINAL;
      sym_numb [goal_symb] = n_heads;
		sym_indx [n_heads]   = goal_symb;
      sym_prod1[goal_symb] = n_prods+1;
      n_heads++; 
      return (rc);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::ADD_HEADLEX (int p, int a)
{
		int rc = ADD_HEAD(p,a);
		sym_type [curr_head] |= LEXICON;
//		printf ("%s [%d] type = %d\n", sym_start[curr_head], curr_head, sym_type[curr_head]);
		return (rc);
}

int   LGActions::ADD_HEADIGNORE (int p, int a)
{
		int rc = ADD_HEAD(p,a);
		sym_type [curr_head] |= IGNORESYM;
//		printf ("%s [%d] type = %d\n", sym_start[curr_head], curr_head, sym_type[curr_head]);
		return (rc);
}

int   LGActions::ADD_HEADSET (int p, int a)
{
		int rc = ADD_HEAD(p,a);
		sym_type [curr_head] |= SETNAME;
		return (rc);
}

int   LGActions::ADD_HEAD (int p, int a)
{
		int rc = 0;
		static int first = 1;

      sym_prod2 [curr_head] = n_prods + 1;

	/* Added Sep 11 2013	...	
		if (first)
		{
			if (n_prods > 0)
			{
				first = 0;
				N_prods = n_prods;	
				N_tails = n_tails;
				if (n_prods < max_prods/2)
				{
					n_prods = max_prods/2;
					n_tails = max_tails/2;
				}
				else
				{
					prt_error ("Number of productions may exceed the maximum, please double the maximum number", T_start, T_end, T_line);
					LG::Terminate (0);
				}
			}
		}	
	// End of addition.
	*/

      curr_head = SYMNUMB ();
		sym_line[curr_head] = T_line;
		if (sym_type[curr_head] & CONSTANT) // Defined constant?
		{
         prt_error ("at '%s', defined constants cannot be used in grammar rules", T_start, T_end, T_line);
		}
      else if (sym_type[curr_head] & NONTERMINAL)
      {
			prt_error ("Nonterminal symbol '%s' was previously defined", T_start, T_end, T_line);
      }
      sym_type [curr_head] |= NONTERMINAL; // Could have been an argument before!
      sym_numb [curr_head] = n_heads;
		sym_indx [n_heads]   = curr_head;
      sym_prod1[curr_head] = n_prods + 1;
      n_heads++; 
      return (rc);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::CHECK_PROD (int p, int a)
{
	// Check to see if this head symbol can be a set name.
      return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::ADD_PROD (int p, int a)
{
      if (++n_prods >= max_prods) 
			MemCrash ("Number of productions", max_prods); 
      F_tail   [n_prods] = n_tails;
		prod_line[n_prods] = line_numb;
		prod_type[n_prods] = PRODARROW;
      return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::ADD_PRODADD ()
{
      if (++n_prods >= max_prods) 
			MemCrash ("Number of productions", max_prods); 
      F_tail   [n_prods] = n_tails;
		prod_line[n_prods] = line_numb;
		prod_type[n_prods] = PRODADD;
      return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::ADD_PRODSUB ()
{
      if (++n_prods >= max_prods) 
			MemCrash ("Number of productions", max_prods); 
      F_tail   [n_prods] = n_tails;
		prod_line[n_prods] = line_numb;
		prod_type[n_prods] = PRODSUB;
      return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::ADD_PRODTO ()
{
      if (++n_prods >= max_prods) 
			MemCrash ("Number of productions", max_prods); 
      F_tail   [n_prods] = n_tails;
		prod_line[n_prods] = line_numb;
		prod_type[n_prods] = PRODTO;
      return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::ADD_TAIL (int p, int a)
{
		int rc = 0;
      DEF_TAIL();
		Tail[n_tails++] = curr_symb;
		if (n_tails >= max_tails) 
			MemCrash ("Number of tail symbols", max_tails);
      return (rc);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::DEF_TAIL ()
{
		int rc = 0;
      curr_symb = SYMNUMB ();
	//	printf ("curr_symb = %s\n", sym_start[curr_symb]);
		if (sym_line[curr_symb] == 0) sym_line[curr_symb] = T_line;
		if (gft[3] == 'x') sym_type[curr_symb] |= LEXFILE; // We are in .lex file!
      sym_type[curr_symb] |= TERMINAL;	  
		if (sym_type[curr_symb] & CONSTANT) // Defined constant?
		{
			prt_error ("at '%s', defined constants cannot be used in grammar rules", T_start, T_end, T_line);
		}
      return (rc);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::ADD_SETNAME (int p, int a)
{
		int rc = 0;
      sym_prod2 [curr_head] = n_prods + 1;
      curr_head = SYMNUMB ();
		sym_line[curr_head] = T_line;
		if (sym_type[curr_head] & CONSTANT) // Defined constant?
		{
         prt_error ("at '%s', defined constants cannot be used in grammar rules", T_start, T_end, T_line);
		}
      else if (sym_type[curr_head] & NONTERMINAL)
      {
			prt_error ("Character set '%s' was previously defined", T_start, T_end, T_line);
      }
      sym_type [curr_head] |= (SETNAME|NONTERMINAL); 
      sym_numb [curr_head] = n_heads;
		sym_indx [n_heads] = curr_head;
      sym_prod1[curr_head] = n_prods + 1;
      n_heads++; 
      return (rc);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   LGActions::FIRST_CHAR(int p, int a)
{
	   int ch;
		ch = get_char();
		if (ch == -1) return (1);
		Tail[n_tails++] = ch;
      sym_type [curr_head] |= SETNAME; 
      return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   LGActions::ADD_CHAR(int p, int a)
{
	   int ch;
		ch = get_char();
		if (ch == -1) return (1);
		ADD_PRODADD ();
		Tail[n_tails++] = ch;
      return (0);
}

int   LGActions::SUB_CHAR(int p, int a)
{
	   int ch;
		ch = get_char();
		if (ch == -1) return (1);
		ADD_PRODSUB ();
		Tail[n_tails++] = ch;
      return (0);
}

int   LGActions::RANGE_START(int p, int a)
{
	   int ch;
		ch = get_char();
		if (ch == -1) return (1);
	   range_start = ch;
      sym_type [curr_head] |= SETNAME; 
      return (0);
}

int   LGActions::RANGE_END(int p, int a)
{
	   int ch;
		ch = get_char();
		if (ch == -1) return (1);
		range_end = ch;
      return (0);
}

int   LGActions::RANGE_ALL(int p, int a) // * operator
{
		Tail[n_tails++] = 0;
		ADD_PRODTO();
		Tail[n_tails++] = max_char_set;
      return (0);
}

int   LGActions::RANGE_TOP(int p, int a) // ? operator
{
		ADD_PRODTO();
		Tail[n_tails++] = max_char_set;
      return (0);
}

int   LGActions::get_char()
{
      if (*T_start == '\'' || *T_start == '"') 
		{
			if (T_end - T_start > 3)
			{	
				prt_error ("at %s, literal is more than one character in length", T_start, T_end, T_line);
				return (-1);
			}
			return (*(T_start+1));
		}
		else
		{
			char*p = T_start;
			int number = atoi (p);
			if (number >= max_char_set)
			{
				sprintf (string, "Symbol '%%s' is outside the allowable range of 0..%d", max_char_set-1);
				prt_error (string, T_start, T_end, T_line);
				return (-1);
			}
			return (number);
      }
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::DEF_SET (int p, int a)
{
	  	sym_type [curr_head] |= SETNAME;
      return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::FIRST_RANGE(int p, int a)
{
		Tail[n_tails++] = range_start;
		ADD_PRODTO();
		Tail[n_tails++] = range_end;
      return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::ADD_RANGE(int p, int a)
{
		ADD_PRODADD();
		Tail[n_tails++] = range_start;
		ADD_PRODTO();
		Tail[n_tails++] = range_end;
      return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::FIRST_SET(int p, int a)
{
      int rc = 0;
      curr_symb = SYMNUMB ();
	   sym_type[curr_symb] |= SETNAMEREF; 
		if (sym_line[curr_symb] == 0) 
		{
			sym_line[curr_symb] = T_line;
		}
		else if (sym_type[curr_symb] & CONSTANT) // Defined constant?
		{
         prt_error ("at '%s', defined constants cannot be used in grammar rules", T_start, T_end, T_line);
			rc = 1;
		}
		Tail [n_tails++] = curr_symb;
		return (rc);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::ADD_SET(int p, int a)
{
      int rc = 0;
      curr_symb = SYMNUMB ();
      sym_type [curr_symb] |= SETNAMEREF; 
		if (sym_line[curr_symb] == 0) 
		{
			sym_line[curr_symb] = T_line;
		}
		else if (sym_type[curr_symb] & CONSTANT) // Defined constant?
		{
         prt_error ("at '%s', defined constants cannot be used in grammar rules", T_start, T_end, T_line);
			rc = 1;
		}
		ADD_PRODADD ();
		Tail [n_tails++] = curr_symb;
		return (rc);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::SUB_SET(int p, int a)
{
      int rc = 0;
      curr_symb = SYMNUMB ();
      sym_type [curr_symb] |= SETNAMEREF; 
		if (sym_line[curr_symb] == 0) 
		{
			sym_line[curr_symb] = T_line;
		}
		else if (sym_type[curr_symb] & CONSTANT) // Defined constant?
		{
         prt_error ("at '%s', defined constants cannot be used in grammar rules", T_start, T_end, T_line);
			rc = 1;
		}
		ADD_PRODSUB ();
		Tail [n_tails++] = curr_symb;
      return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::SUB_RANGE(int p, int a)
{
		ADD_PRODSUB();
		Tail[n_tails++] = range_start;
		ADD_PRODTO();
		Tail[n_tails++] = range_end;
      return (0);
}

static char*	set_start;
static char*	set_end;

int   LGActions::SET_START (int p, int a)
{
		set_start = T_start;
      return (0);
}

int   LGActions::SET_END (int p, int a)
{
		set_end = T_end;
      return (0);
}

int   LGActions::MAKE_SETNAME (int p, int a)
{
		if (*T_start != '[') 
		{
			T_start = set_start;
			T_end   = set_end;
		}
		PUSH_TAIL (p, a);
      return (0);
}

int   LGActions::PUSH_TAIL (int p, int a)
{
      DEF_TAIL();
      if (ebnfcode > ebnfcode_end) 
			MemCrash ("EBNF code size", max_code);
      *ebnfcode++ = curr_symb;
      return (0);
}

int   LGActions::PUSH_UNDEF (int p, int a)
{
      DEF_TAIL();
      if (ebnfcode > ebnfcode_end) 
			MemCrash ("EBNF code size", max_code);
      *ebnfcode++ = curr_symb;
      return (0);
}

int   LGActions::POP_ALL (int p, int a)
{
      int *sp;
      for (sp = ebnfcode_start; sp < ebnfcode; sp++)
      {
			curr_symb = Tail [n_tails++] = *sp;
         if (n_tails >= max_tails) 
				MemCrash ("Number of tail symbols", max_tails);
      }

		if (ebnfcode-ebnfcode_start > amt_code)
			amt_code = ebnfcode-ebnfcode_start;

		if (ebnflevel-ebnflevel_start > n_levels)
			n_levels = ebnflevel-ebnflevel_start;

      ebnfcode  = ebnfcode_start;
      ebnflevel = ebnflevel_start;
      return (0);
}
/*		NOT USED
int   LGActions::POP_SET (int p, int a)
{
      int *sp;
      for (sp = ebnfcode_start; sp < ebnfcode; sp++)
      {
         ADD_PROD(p,a);
         curr_symb = Tail [n_tails++] = *sp;
      }
      ebnfcode = ebnfcode_start;
      ebnflevel = ebnflevel_start;
      return (0);
}
*/
int   LGActions::PUSH_DOTS (int p, int a)
{
      if (ebnflevel > ebnflevel_end) 
			MemCrash ("EBNF nesting levels", max_lev);
      if (ebnfcode > ebnfcode_end)  
			MemCrash ("EBNF code size", max_code);
      *ebnflevel++ = ebnfcode - 1;
      *ebnfcode++ = -DOTS;
      GEN_SYMB ();
      return (0);
}

int   LGActions::PUSH_PLUS (int p, int a)
{
      if (ebnflevel > ebnflevel_end) 
			MemCrash ("EBNF nesting levels", max_lev);
      if (ebnfcode > ebnfcode_end)  
			MemCrash ("EBNF code size", max_code);
      *ebnflevel++ = ebnfcode - 1;
      *ebnfcode++   = -PLUS;
      GEN_SYMB ();
      return (0);
}

int   LGActions::PUSH_ASTER (int p, int a)
{
      if (ebnflevel > ebnflevel_end) MemCrash ("EBNF nesting levels", max_lev);
      if (ebnfcode  > ebnfcode_end ) MemCrash ("EBNF code size", max_code);
  		*ebnflevel++ = ebnfcode - 1;   // save ptr to last symbol number put on ebnfcode.
		*ebnfcode++ = -PLUS;				 // put -PLUS on ebnfcode.
		GEN_SYMB ();						 // generate symbol* text 

      if (ebnflevel > ebnflevel_end) MemCrash ("EBNF nesting levels", max_lev);
      if (ebnfcode  > ebnfcode_end ) MemCrash ("EBNF code size", max_code);
		*ebnflevel++ = ebnfcode - 1;	 // save ptr to last symbol number put on ebnfcode.
		*ebnfcode++ = -QUEST;
		GEN_SYMB ();
      return (0);
}

int   LGActions::PUSH_QUEST (int p, int a)
{
      if (ebnflevel > ebnflevel_end) 
			MemCrash ("EBNF nesting levels", max_lev);
      if (ebnfcode > ebnfcode_end)  
			MemCrash ("EBNF code size", max_code);
      *ebnflevel++ = ebnfcode - 1;
      *ebnfcode++ = -QUEST;
      GEN_SYMB ();
      return (0);
}

int   LGActions::PUSH_GDOTS (int p, int a)
{
		PUSH_GEND(p,a);
		PUSH_DOTS (p,a);
		return (0);
}

int   LGActions::PUSH_GPLUS (int p, int a)
{
		PUSH_GEND(p,a);
		PUSH_PLUS (p,a);
		return (0);
}

int   LGActions::PUSH_GASTER (int p, int a)
{
		PUSH_GEND(p,a);
		PUSH_ASTER (p,a);
		return (0);
}

int   LGActions::PUSH_GQUEST (int p, int a)
{
		PUSH_GEND(p,a);
		PUSH_QUEST (p,a);
		return (0);
}

int   LGActions::PUSH_OBEG (int p, int a)
{
      if (ebnflevel > ebnflevel_end) 
			MemCrash ("EBNF nesting levels", max_lev);
      if (ebnfcode > ebnfcode_end)  
			MemCrash ("EBNF code size", max_code);
      *ebnflevel++  =  ebnfcode;
      *ebnfcode++ = -OBEG;

      if (ebnflevel > ebnflevel_end) 
			MemCrash ("EBNF nesting levels", max_lev);
      if (ebnfcode > ebnfcode_end)  
			MemCrash ("EBNF code size", max_code);
      *ebnflevel++  =  ebnfcode;
      *ebnfcode++ = -NOTHING;  /* In case this has to become '(' */
      return (0);
}

int   LGActions::PUSH_OEND (int p, int a)
{
		if (*(ebnflevel-1) == ebnfcode-2) /* If not a list? */
		{
			ebnflevel--; // Drop NOTHING. 
			*ebnfcode++ = -OEND;
			GEN_SYMB ();
		}
		else  // List of symbols.
		{
	      if (ebnfcode > ebnfcode_end) 
				MemCrash ("EBNF code size", max_code);
			**(ebnflevel-1) = -GBEG;	// Replace NOTHING with '('
			*ebnfcode++ = -GEND;			// Add ')'
			GEN_SYMB ();

	      if (ebnfcode > ebnfcode_end) 
				MemCrash ("EBNF code size", max_code);
			*ebnfcode++ = -OEND;
			GEN_SYMB ();
		}  
      return (0);
}

int   LGActions::PUSH_ODOTS (int p, int a)
{
      if (*(ebnflevel-1) == ebnfcode-2) /* If not a list? */
      {
			if (ebnfcode > ebnfcode_end) MemCrash ("EBNF code size", max_code);
			 ebnflevel--; /* Drop NOTHING. */
			*ebnflevel++ = ebnfcode - 1;
			*ebnfcode++ = -DOTS;
			GEN_SYMB ();

			if (ebnfcode > ebnfcode_end) MemCrash ("EBNF code size", max_code);
			*ebnfcode++ = -OEND;
			GEN_SYMB ();
      }
      else  // List of symbols.
      {
			if (ebnfcode > ebnfcode_end) MemCrash ("EBNF code size", max_code);
         **(ebnflevel-1) = -GBEG;	// Replace NOTHING with '('
         *ebnfcode++   = -GEND;		// Add ')'
         GEN_SYMB ();

	      if (ebnflevel > ebnflevel_end) MemCrash ("EBNF nesting levels", max_lev);
			if (ebnfcode  > ebnfcode_end ) MemCrash ("EBNF code size", max_code);
         *ebnflevel++  = ebnfcode - 1;
         *ebnfcode++ = -DOTS;
         GEN_SYMB ();

			if (ebnfcode  > ebnfcode_end ) MemCrash ("EBNF code size", max_code);
         *ebnfcode++ = -OEND;
         GEN_SYMB ();
      }  
      return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::PUSH_GBEG (int p, int a)
{
      if (ebnflevel > ebnflevel_end) MemCrash ("EBNF nesting levels", max_lev);
      if (ebnfcode  > ebnfcode_end ) MemCrash ("EBNF code size", max_code);
      *ebnflevel++ =  ebnfcode;
      *ebnfcode++ = -GBEG;
      return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::PUSH_GEND (int p, int a)
{
      if (ebnfcode  > ebnfcode_end ) MemCrash ("EBNF code size", max_code);
      *ebnfcode++ = -GEND;
      GEN_SYMB ();
      return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::PUSH_OR (int p, int a)
{
      if (ebnfcode > ebnfcode_end) MemCrash ("EBNF code size", max_code);
      *ebnfcode++ = -ORSYM;
      return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::GEN_SYMB ()
{
      char *start;
      int  *sp, leng, ns;

      ebnflevel--;
      start = ebnfspace;
      for (sp = *ebnflevel; sp < ebnfcode; sp++)
      {
			if (*sp < 0)
			{
				switch (-(*sp))
				{
					case GBEG:
				      if (ebnfspace+1 > ebnfspace_end) MemCrash ("EBNF symbols space", max_space);
						*ebnfspace++ = '(';
						break;
					case GEND:
				      if (ebnfspace+1 > ebnfspace_end) MemCrash ("EBNF symbols space", max_space);
						ebnfspace--;
						*ebnfspace++ = ')';
						*ebnfspace++ = ' ';
						break;
					case OBEG:
				      if (ebnfspace+1 > ebnfspace_end) MemCrash ("EBNF symbols space", max_space);
						*ebnfspace++ = '[';
						break;
					case OEND:
				      if (ebnfspace+1 > ebnfspace_end) MemCrash ("EBNF symbols space", max_space);
						ebnfspace--;
						*ebnfspace++ = ']';
						*ebnfspace++ = ' ';
						break;
					case ORSYM:
						ebnfspace--;
						*ebnfspace++ = '|';
						break;
					case PLUS:
				      if (ebnfspace+1 > ebnfspace_end) MemCrash ("EBNF symbols space", max_space);
						ebnfspace--;
						*ebnfspace++ = '+';
						*ebnfspace++ = ' ';
						break;
					case ASTER:
				      if (ebnfspace+1 > ebnfspace_end) MemCrash ("EBNF symbols space", max_space);
						ebnfspace--;
						*ebnfspace++ = '*';
						*ebnfspace++ = ' ';
						break;
					case QUEST:
				      if (ebnfspace+1 > ebnfspace_end) MemCrash ("EBNF symbols space", max_space);
						ebnfspace--;
						*ebnfspace++ = '?';
						*ebnfspace++ = ' ';
						break;
					case DOTS:
				      if (ebnfspace+2 > ebnfspace_end) MemCrash ("EBNF symbols space", max_space);
						ebnfspace--;
						*ebnfspace++ = '.';
						*ebnfspace++ = '.';
						*ebnfspace++ = '.';
						*ebnfspace++ = ' ';
						break;
					case NOTHING:
						break;
				}
			}
			else
			{
            leng = sym_leng [*sp];
		      if (ebnfspace+leng+1 >= ebnfspace_end) 
					MemCrash ("EBNF symbols space", max_space);
            memcpy (ebnfspace, sym_start[*sp], leng);
            ebnfspace += leng;
            *ebnfspace++ = ' ';
         }  
		//	z = *ebnfspace; *ebnfspace = 0; printf ("%s\n", start); *ebnfspace = z;
      }

      T_start = start;
      T_end = --ebnfspace;
      *ebnfspace++ = 0;
	//	printf ("%s\n\n", start);

      ns = n_symbs;
      curr_symb = GENSYMNUMB ();
	//	printf("curr_symb = %s\n", sym_start[curr_symb]);
      if (n_symbs > ns) /* New symbol? */
      {
			if (ebnfspace+sizeof(int) > ebnfspace_end) 
				MemCrash ("EBNF symbols space", max_space);
         *(int *)ebnfspace = ebnfcode - *ebnflevel; /* Store count. */
         ebnfspace += sizeof(int);
         for (sp = *ebnflevel; sp < ebnfcode; sp++)
         {
				if (ebnfspace+sizeof(int) > ebnfspace_end) 
					MemCrash ("EBNF symbols space", max_space);
            *(int *)ebnfspace = *sp; /* Store symbol numbers. */
            ebnfspace += sizeof(int);
         }  
      }
      else ebnfspace = start;

      sym_type [curr_symb] |= GENERATED;
		if (ebnfcode > ebnfcode_end)
			MemCrash ("EBNF symbols space", max_code);
      ebnfcode = *ebnflevel;
      *ebnfcode++ = curr_symb;
      return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::term ()
{
      int *offset;
      int s, t, h, p, n;

	   GEN_EBNF ();
		EXP_LITERALS (); 
      n_prods++;

      sym_prod2[curr_head] = n_prods;
      F_tail[n_prods]      = n_tails;

      ALLOC (offset, n_symbs);
      for (s = 0; s < n_symbs; s++)
      {
         if (sym_type [s] & GENERATED)
         offset [s] = sym_start[s] - ebnfspace_start;
      }

      amt_space = ebnfspace - ebnfspace_start;
      REALLOC (ebnfspace_start, max_space, amt_space);

      for (s = 0; s < n_symbs; s++)
      {
         if (sym_type [s] & GENERATED)
         sym_start[s] = ebnfspace_start + offset [s];
      }
      FREE (offset, n_symbs);

   // Count the number of terminals ...
		n_terms = n_tokens;
      for (int s = max_char_set+1; s < n_symbs; s++)
      {  
         if (!(sym_type[s] & (NONTERMINAL|CONSTANT|STRING|ACTIONCODE))) 
         {  
				if (sym_numb[s] == -1) sym_numb[s] = n_terms++;
				else InternalError(11);
         }  
      } 

   /* Check for lexical setnames. This is handled by the grammar now, LG.grm.
      for (h = 0; h < n_heads; h++) 
      {
         s = sym_indx[h];
			if (sym_type[s] & LEXICON)
			{
			//	sym_type[s] &= ~LEXICON; // Remove LEXICON type 
				if (sym_type[s] & SETNAME)
				{
					prt_error ("at %s, a lexical symbol cannot be a character set", sym_start[s], 0, sym_line[s]);
				}
			}
      }  
		if (n_errors) LG::Terminate(93);
	*/  

	  	if (optn[LG_VERBOSE] > 2)
	  	printf ("Expanding sets ...\n");

		EXPAND_SETS ();

	  	if (optn[LG_VERBOSE] > 2)
	  	printf ("Done with expanding sets !!!\n");

   /* Allocate head & term arrays. */
      ALLOC (head_name, n_heads);
      ALLOC (head_line, n_heads);
      ALLOC (head_type, n_heads);
      ALLOC (term_name, n_terms);
      ALLOC (term_line, n_terms);
      ALLOC (term_type, n_terms);

	// Now we can define the defined constants.
      GEN_CON (); 
      GEN_STRINGS (); 

	  	if (optn[LG_VERBOSE] > 2)
	  	printf ("Starting terminal numbering ...\n");

   // Define term_name for all terminals ...
      n_terms = 0;
      for (s = 0; s < n_symbs; s++)
      {  
         if (!(sym_type[s] & (NONTERMINAL|CONSTANT|STRING|ACTIONCODE))) 
         {  
	         term_name[n_terms]   = sym_start[s];
		      term_line[n_terms]   = sym_line [s];
		      term_type[n_terms++] = sym_type [s];
         }  
      } 

	  	if (optn[LG_VERBOSE] > 2)
	  	printf ("Done with terminal numbering !!!\n");

   /* Assign first production and Tail numbers. */
		max_n_prods = n_prods;
      ALLOC (f_prod, n_heads);
      ALLOC (l_prod, n_heads);
      ALLOC (f_tail, max_prods);
      ALLOC (l_tail, max_prods);
      ALLOC (tail,   max_tails);   

	  	if (optn[LG_VERBOSE] > 2)
	  	printf ("Starting nonterminal numbering ...\n");

   // Define nonterminal symbols. 
      n_prods = n_tails = 0;
      for (h = 0; h < n_heads; h++) // Have to do this in nonterminal order.
      {
         s = sym_indx[h];
         f_prod [h]    = n_prods; 
         head_name [h] = sym_start[s];
         head_type [h] = sym_type [s];
         head_line [h] = sym_line [s];

//			printf ("%s [%d] type = %d\n", head_name[h], s, head_type[h]);
         for (p = sym_prod1[s]; p < sym_prod2[s]; p++) 
         {
            f_tail [n_prods] = n_tails;
            for (t = F_tail[p]; t < L_tail[p]; t++)
            {
               tail [n_tails++] = Tail[t];
            }
            l_tail [n_prods++] = n_tails;
         }  
			l_prod[h] = n_prods;
      }  

	  	if (optn[LG_VERBOSE] > 2)
	  	printf ("Done with nonterminal numbering !!!\n");

      FREE (sym_prod1,     max_symbs);
      FREE (sym_prod2,     max_symbs);
   // FREE (Tail,				max_tails);
   // FREE (F_tail,			max_prods+1); 

   /* Assign Tail numbers. */
      for (t = 0; t < n_tails; t++)
      {
         s =  tail [t];
         n =  sym_numb [s];
         if  (sym_type [s] & NONTERMINAL) n = -n;
         tail [t] = n;
      }

	  	if (optn[LG_VERBOSE] > 2)
	  	printf ("Generating constants ...\n");

      FREE (ebnfcode_start,  max_code);
      FREE (ebnflevel_start, max_lev);

      FREE (sym_start,     max_symbs);  
      FREE (sym_numb,      max_symbs);
      FREE (sym_indx,      max_symbs);
      FREE (sym_type,      max_symbs);
      FREE (sym_leng,      max_symbs);
      FREE (sym_vect,    2*max_symbs);		 
	   return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  LGActions::EXPAND_SETS () /* Expand set productions. */
{
		int s, p, t, sym, h;

	// Make L_tail ...
		ALLOC (L_tail, max_prods);
		for (p = 0; p < n_prods; p++)
		{
			L_tail[p] = F_tail[p+1];
		}

	// Count the number of sets ...
		n_setnames = 0;
		for (s = 0; s < n_symbs; s++)
		{
			if (sym_type[s] & SETNAME)
			{
				n_setnames++;
			//	printf ("Setname: %s\n", sym_start[s]);
			}
		}
		if (n_setnames == 0) return;

	//	printf ("\n");
	// Check setnames references ...
		for (s = 0; s < n_symbs; s++)
		{
			if (sym_type[s] & SETNAMEREF)
			{
				if (!(sym_type[s] & SETNAME))
				{
					if (sym_type[s] & NONTERMINAL)
					{
						sprintf (string, "'%s', when used in a character set, must be defined as: %s -> { ... }", sym_start[s], sym_start[s]);
						prt_error (string, sym_start[s], 0, sym_line[s]);
					}
				}
			}
		}
		if (n_errors) LG::Terminate(0);

	//	printf ("\n");
	// Check for cycles in set definitions ...
		n_onstack = 0;
		ALLOC (onstack, n_setnames);
		FASTINI (0, onstack, n_setnames);
		for (s = 0; s < n_symbs; s++)
		{
			if (sym_type[s] & SETNAME)
			{
				CHECK_SET(s);
			}
		}
		FREE (onstack, n_setnames);

	// Make productions & tails for sets ...
		n_onstack = 0;
		ALLOC (done, n_symbs);
		memset (done, 0, n_symbs);
		ALLOC (onstack, n_setnames);
		FASTINI (0, onstack, n_setnames);
		for (s = 0; s < n_symbs; s++)
		{
			if (sym_type[s] & SETNAME)
			{
				if (!done[s]) MAKE_SET (s);
			}
		}
		FREE (onstack, n_setnames);
		FREE (done, n_symbs);
}

void	LGActions::MAKE_SET (int s)
{
		char* hit;
		int   i, p, sym, lastsym, lastcode;

	// Collect characters for this set ...
  	//	printf ("Making: %s\n", sym_start[s]);
		ALLOC (hit, max_char_set);
		memset (hit, 0, max_char_set);
		for (p = sym_prod1[s]; p < sym_prod2[s]; p++) // productions for setname.
		{
			sym = Tail[F_tail[p]];
			switch (prod_type[p])
			{
				case PRODARROW: // ->
				case PRODADD: // |
				if (sym < max_char_set) hit [sym] = 1;
				else ADDSETNAME (sym, hit);
				lastsym = sym;
				lastcode = 1;
				break;

				case PRODSUB: // -
				if (sym < max_char_set) hit [sym] = 0;
				else SUBSETNAME (sym, hit);
				lastsym = sym;
				lastcode = 0;
				break;

				case PRODTO: // ..
				if (optn[LG_INSENSITIVE]) // if case-insensitive, shift to lower case
				{
					for (i = lastsym+1; i <= sym; i++) hit[lower[i]] = lastcode;		
				}
				else
				{
					for (i = lastsym+1; i <= sym; i++) hit[i] = lastcode;		
				}
				break;
			}
			prod_type[p] = 0;
		}

	// Make productions for set ...
	//	printf ("\n%s\n", sym_start[s]);
		sym_prod1[s] = n_prods;
		for (i = 0; i < max_char_set; i++)
		{
			if (hit[i] == 1)
			{
			 	if (n_prods >= max_prods) 
			 		MemCrash ("Number of productions", max_prods);
				if (n_tails >= max_tails) 
					MemCrash ("Number of tail symbols", max_tails);
				prod_type[n_prods] = 0;
				F_tail [n_prods]   = n_tails;
				Tail   [n_tails++] = i;
				L_tail [n_prods++] = n_tails;
			//	printf ("   -> %s\n", sym_start[i]);
			}
		}
		sym_prod2[s] = n_prods;
		FREE (hit, max_char_set);
		done[s] = 1;
}

void  LGActions::ADDSETNAME (int s, char* hit)
{
		int p;
		if (!done[s]) MAKE_SET (s);
		for (p = sym_prod1[s]; p < sym_prod2[s]; p++) 
		{
			hit [Tail[F_tail[p]]] = 1;
		}
}

void  LGActions::SUBSETNAME (int s, char* hit)
{
		int p;
		if (!done[s]) MAKE_SET (s);
		for (p = sym_prod1[s]; p < sym_prod2[s]; p++) 
		{
			hit [Tail[F_tail[p]]] = 0;
		}
}

void  LGActions::CHECK_SET (int s)
{
  		int p, i, j;
		for (i = 0; i < n_onstack; i++)
		{
			if (onstack[i] == s) 
			{
				prt_log ("Cycle detected in set definitions.\n\n");
				for (j = i; j < n_onstack; j++)
				{
					prt_log ("%s -> ", sym_start [onstack[j]]);
				}
				prt_log ("%s\n\n", sym_start [onstack[i]]);
				LG::Terminate(0);
			}
		}
		onstack [n_onstack++] = s;
	//	printf ("Checking: %s\n", sym_start[s]);
		for (p = sym_prod1[s]; p < sym_prod2[s]; p++)
		{
			int sym = Tail[F_tail[p]];
			if (sym_type[sym] & SETNAME)
			{	
			//	printf ("-> %s\n", sym_start[sym]);
				CHECK_SET (sym);
			}
		}
		n_onstack--;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::GEN_EBNF () /* Generate EBNF symbols. */
{
      int *sp, *spend, h, t, s, z;

      t = 0; z = 0;
      for (s = 0; s < n_symbs; s++)
      {
         if (sym_type [s] & GENERATED)
         {
            GEN_HEAD (h = s);
            sp = (int *)(sym_start[s] + sym_leng [s] + 1);
            spend = sp + *sp;
            sp++; /* Keep this line separate from above for Microsoft C. */
            for (; sp <= spend; sp++)
            {
					if (*sp < 0)
					{
						switch (-(*sp))
						{
							case GBEG:
								break;
							case GEND:
								break;
							case OBEG:
								GEN_PROD ();
								break;
							case OEND:
								break;
							case ORSYM:
								GEN_PROD ();
								break;
							case PLUS:
								GEN_PROD ();
								GEN_TAIL (h);
								GEN_TAIL (t);
								break;
							case ASTER:
								n_tails--;
								GEN_PROD ();
								GEN_TAIL (t);
								GEN_PROD ();
								GEN_TAIL (h);
								GEN_TAIL (t);
								break;
							case QUEST:
								n_tails--;
								GEN_PROD ();
								GEN_TAIL (t); 
								break;
							case DOTS:
								GEN_PROD ();
								GEN_TAIL (h);
								GEN_TAIL (t);
								break;
							case NOTHING:
								break;
						}
					}
					else 
					{
						GEN_TAIL (t = *sp);
					}
            }
         }  
      }
      return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::GEN_HEAD (int h)
{
      sym_prod2[curr_head] = n_prods+1;
      sym_type [h] = (NONTERMINAL | GENERATED);
      sym_numb [h] = n_heads;
		sym_indx [n_heads] = h;
      sym_prod1[h] = n_prods+1;
      curr_head = h;
      n_heads++; 
      if (++n_prods >= max_prods) 
		   MemCrash ("Number of productions", max_prods);
      F_tail   [n_prods] = n_tails;
		prod_type[n_prods] = PRODARROW;
      return (0);
}

int   LGActions::GEN_HEAD_LITERAL (int h, char* sym, int L)
{
		int i;
		n_genliterals++;
      sym_prod2[curr_head] = n_prods+1;	 
      sym_type [h] = NONTERMINAL;
      sym_numb [h] = n_heads;
		sym_indx [n_heads] = h;
      sym_prod1[h] = n_prods + 1;
      curr_head    = h;
      n_heads++; 
      if (++n_prods >= max_prods) 
		   MemCrash ("Number of productions", max_prods);
      F_tail   [n_prods] = n_tails;
		prod_type[n_prods] = PRODARROW;
		for (i = 0; i < L; i++)
		{
			GEN_TAIL (sym[i]);
		}
      return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::GEN_PROD ()
{
      if (++n_prods >= max_prods) 
			MemCrash ("Number of productions", max_prods);
      F_tail [n_prods] = n_tails;
      prod_line [n_prods] = 0;
		prod_type [n_prods] = PRODARROW;
      return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::GEN_TAIL (int t)
{
      if (n_tails >= max_tails) 
			MemCrash ("Number of tail symbols", max_tails);
      Tail [n_tails++] = t;
      sym_type[t] |= TERMINAL;
      return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::GEN_CON ()   /* Generate defined constant array. */
{
      int p, s, t, ndc;

      ALLOC (ret_name,     max_prods);
      ALLOC (defcon_name,  n_constants);
      ALLOC (defcon_value, n_constants);
      ALLOC (Defcon_term,  n_constants);

      ndc = 0;
      for (p = 0; p < n_prods; p++)
      {
         if (ret_numb[p] == MAX_INT)
         {
            ret_numb[p] = -1;
	   		ret_name[p] = "UNDEFINED";          // Assign name to defined constant.
            continue;
         }
         if (ret_numb[p] < 0)
         {
            s = -ret_numb[p];                   // Get symbol number.
            t = sym_prod1[s];                   // Get terminal number.
	   		ret_name[p] = sym_start[s];         // Assign name to defined constant.
            ret_numb[p] = sym_numb[s];          // Reset back to original number.
            defcon_name[ndc]  = sym_start[s];   // Make defined constant name.
            defcon_value[ndc] = sym_numb[s];    // Make defined constant value.
            Defcon_term[ndc]  = sym_start[t];   // Assign terminal symbol name.
            ndc++;
         }
         else
         {
	   		ret_name[p] = "UNDEFINED";          // Assign name to defined constant.
         }
      }
      if (ndc != n_constants) 
         InternalError(13);
      return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::GEN_STRINGS ()   
{
      int s, i;

	/* Generate strings array. */
		i = 0;
      ALLOC (str_start, n_strings);
		str_start[i++] = "\'UNDEFINED\'";
      for (s = 0; s < n_symbs; s++)
      {
         if (sym_type[s] & STRING)
         {
				str_start[i++] = sym_start[s];
         }
      }

	/* Generate actincodes array. */
		i = 0;
      ALLOC (code_start, n_actioncodes);
		code_start[i++] = "{ /* UNDEFINED */ }";
      for (s = 0; s < n_symbs; s++)
      {
         if (sym_type[s] & ACTIONCODE)
         {
				code_start[i++] = sym_start[s];
         }
      }
      return (0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

void  LGActions::EXP_LITERALS () // For literals like '==', '<=' ...			 
{
      char* sym;
      int   s, L;
      
		n_genliterals = 0;
		n_original_prods = n_prods;
      for (s = 0; s < n_symbs; s++)
      {
         if (sym_type [s] & STRING) continue;
         if (sym_type [s] & ACTIONCODE) continue;
         if (sym_type [s] & GENERATED) continue;
         if (!(sym_type [s] & NONTERMINAL))	
         {
            sym = sym_start[s];              
            if (sym[0] == '\'') 
            {
					L = sym_leng[s];
               if (L > 3) GEN_HEAD_LITERAL (s, sym+1, L-2);		
            }  
            else if (sym[0] == '"' || sym[0] == '`') 
            {
					L = sym_leng[s];
               GEN_HEAD_LITERAL (s, sym+1, L-2);		
            }  
         }  
      }  
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   length, cell;

int   LGActions::SYMNUMB ()
{
      int x;
		char *p;
      x = EXIST ();
      if (x >= 0) return (x);

	// Add new symbol to table.
      if (n_symbs >= max_symbs) 
			MemCrash ("Number of symbols", max_symbs); 

      sym_vect [cell] = n_symbs;
      sym_leng [n_symbs] = length;
      if (pool_ptr + length + 1 > pool_end)	// No room left in pool?
      {
         ALLOC (pool_ptr, 4000);					// Get more space.
         pool_end = pool_ptr + 4000;			// Reset end pointer.
      }
      sym_start [n_symbs] = pool_ptr;			// Define start of symbol.
		for (p = T_start; p < T_end;)
		{
			*pool_ptr++ = *p++;
		}
		*pool_ptr++ = 0;								// Put ending zero byte here.
      return (n_symbs++);	
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::ADDSYM (char* start, int leng)
{
		char *p;

	// Add symbol ...
      if (n_symbs >= max_symbs) 
			MemCrash ("Number of symbols", max_symbs); 

      sym_leng [n_symbs] = leng;
      if (pool_ptr + leng + 1 > pool_end)		// No room left in pool?
      {
         ALLOC (pool_ptr, 4000);					// Get more space.
         pool_end = pool_ptr + 4000;			// Reset end pointer.
      }
      sym_start [n_symbs] = pool_ptr;			// Define start of symbol.
		for (p = T_start; p < T_end;)
		{
			*pool_ptr++ = *p++;
		}
		*pool_ptr++ = 0;								// Put ending zero byte here.
      return (n_symbs++);	
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

char* LGActions::ADDSYMBOL (char* start, int leng)
{
		char* symaddr;
      if (pool_ptr + leng > pool_end)		// No room left in pool?
      {
         ALLOC (pool_ptr, 4000);				// Get more space.
         pool_end = pool_ptr + 4000;		// Reset end pointer.
      }
      symaddr = pool_ptr;						// Define start of symbol.
      memcpy (pool_ptr, start, leng);		// Move symbol to pool. 
		pool_ptr += leng;							// Add length of symbol name to pool_ptr.
      return (symaddr);							// Return symbol name address.
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::GENSYMNUMB ()
{
      int x;
      x = EXIST ();
      if (x >= 0) return (x);
      if (n_symbs >= max_symbs) 
			MemCrash ("Number of symbols", max_symbs); 
      sym_vect [cell]    = n_symbs;
      sym_start[n_symbs] = T_start;
      sym_leng [n_symbs] = length;
      n_symbs++; 
      return (n_symbs-1);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   LGActions::EXIST ()
{
		int  i;
		uint hash;
      char *p, *q;						

		i = 0;
		p = T_start;					
		q = T_end-1;					
		if (*p == '\"' && *q == '\"')
		{
			*p = '\'';
			*q = '\'';
		}
		hash = length = (T_end-T_start); // Set hash to length. 

      do									      // Assume length != 0
      {
         hash += *p << i;
         i += 3;		                  // 3 is optimum.
         i %= 30; 
      }
      while (++p < T_end);

      cell = hash % n_cells; 				// Get first cell.
		i = sym_vect [cell];					// Get symbol index.

		while (i >= 0)
		{
			if (sym_leng[i] == length)
			{
				p = T_start;
				q = sym_start[i];
				do 
				{
					if (*p++ != *q++) goto Cont;			
				}
				while (p < T_end); 
				return (i);									// Found it.
			}
Cont:    cell = (hash *= 65549) / hash_divide;	// Get new cell number.
			i = sym_vect [cell];							// Get symbol index.
		}
      return (i);												// Return symbol number.
}

void  LGActions::P_PROD (int p)
{
      prt_log ("%d -> ", p);
      for (int t = F_tail [p]; t < F_tail [p+1]; t++)
      {
         prt_log ("%s ", sym_start [Tail[t]]);
      }
      prt_log ("\n\n");
}

#endif

/*--- End of Actions2.CPP ------------------------------------------------------*/

