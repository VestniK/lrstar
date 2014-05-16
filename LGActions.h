
		#pragma once
		#include "LGParser.h"

		class LGActions : public LGParser
      {
			public:
			static char**  Defcon_term;
			static char**  head_name;
			static int*    head_sym;
			static int*    head_line; 
			static int*    term_line; 
			static int*    prod_len;
			static int*    prod_line;
			static int*    nullable;
			static int     n_nulls;
			static int     n_constants;
			static int     N_prods;				// Number of token productions (<identifier> => 1).
			static int     n_prods;				// Number of other productions.
			static int     N_tails;				// Number of tails for token productions.
			static int     n_tails;				// Number of tails for other productions.
			static int     amt_space;
			static int*    Tail;
			static int*    F_tail;
			static char**  term_name;
			static int*    term_type;

			static char**  str_start;
	      static int*    defcon_value;
		   static char**  defcon_name;
	      static int*		ret_value;
		   static char**  ret_name;
			static char**  ret_term;
			static int*    ret_numb;
			static int   	n_symbs;
      	static int   	n_strings;
      	static int   	n_terms;
      	static int   	n_lex_terms;
      	static int   	n_heads;
      	static int   	n_tokens;
      	static int   	n_keywords;
			static char* 	pool_ptr;
			static char* 	pool_end;
			static int   	ignore;
			static int*  	f_tail;
			static int*  	l_tail;
			static int*  	f_prod;
			static int*  	l_prod;
			static int*  	tail;	
			static int*    prod_type;
			static int    	max_trans;
			static int    	max_errs;      
			static int    	max_final;
			static int    	max_include;
			static int    	max_kernel;
			static int    	max_tt;
			static int    	max_ntt;
			static int    	max_lookback;
			static int    	max_prods;
			static int    	max_ebnf;
			static int    	max_states;
			static int    	max_symbs;
			static int    	max_tails;
			static int    	max_space;
			static int    	max_lev;
			static int    	max_code;
			static int    	max_ttas;
			static int    	max_n_prods;
			static char*  	tokenstartstr; 
			static char*  	tokenendstr; 
			static char*  	tokenlinestr; 
			static char*  	linenumbstr; 
			static char*  	colnumbstr; 
			static char*  	tabstopstr;
			static int    	anl_stat_memory;
			static int    	amt_code;
			static int*   	camefrom2;
			static int    	end_symb;	// $end	(128 | 256)
			static int*   	final2;
			static int    	final_state;
			static int    	gen_tabl_memory;
			static int    	get_memory_data;
			static int*   	head_type;
			static int*   	head_level;
			static int    	illegal_char_state;
			static int    	n_genliterals;
			static int    	n_original_prods;
			static int    	n_outputfiles;
			static int    	n_added;
			static int    	n_levels;
			static int    	n_bytes;
			static int    	NDLeft;
			static int    	n_nonttran;
			static int    	n_oprods;
			static int    	n_termtran;
			static int    	n_words;
			static int*   	ntt_goto2;
			static int*   	ntt_symb2;
			static int    	n_totalterm; 
			static int    	n_codelines;
			static int    	opt_stat_memory;
			static int    	parser_memory;
			static int*   	possibil;   
			static int    	rr_con;
			static int    	SLA_expired;
			static int    	sr_con;
			static int*   	st_type;
			static int*   	tt_goto2;

      // Token data ...
			static short 	arrow_col;
			static short 	prev_token;
			static char  	errormsg[128];

      // Token actions ...
         static int   	error (int, int);
         static int   	is_head (int, int);
         static int   	firstsym (int, int);
			static int   	arrow (int, int);
         static int   	checkcase (int, int);
         static int   	checkstring (int, int);

      // Functions ...	
			static int	 	CheckOptions ();
			static void	 	Count_terms ();
         static int   	init ();
         static int   	term ();
         static int   	get_char ();
         static int   	GEN_EBNF ();
			static char* 	ADDSYMBOL (char*, int);
         static int   	ADD_TOKEN (int, int);
         static int   	ADD_IGNORE (int, int);
         static int   	ADD_KEYWORD (int, int);
         static void  	EXPAND_SETS ();
			static int   	NUM_PRODS (int s);
         static int   	MAKE_TERMINALS ();
         static int   	FIRST_CHAR (int, int);
         static int   	FIRST_RANGE (int, int);
         static int   	FIRST_SET (int, int);
         static int   	DEF_SET (int, int);
			static void  	CHECK_SET (int s);
         static int   	GEN_SYMB ();
         static int   	GEN_HEAD (int);
         static int   	GEN_HEAD_LITERAL (int, char*, int);
         static int   	GEN_PROD ();
         static int   	GEN_TAIL (int);
         static int   	REMOVE_QUOTES (); 
	   	static int   	CHECK_SYMBOL (int);
         static int   	GEN_CON (); 
         static int   	GEN_STRINGS (); 
         static void  	EXP_LITERALS ();
         static int   	EXIST ();
         static int   	SYMNUMB ();
         static int   	GENSYMNUMB ();
			static int   	ADD_SYMBOL (char* name, int& sym_numb); 

      // Production actions ...
         static int   	EOGI (int, int);
         static int   	PRO_OPTNS (int, int);
         static int   	pro_optns (int, int);
         static int   	ADD_TAIL (int, int);
         static int   	PUSH_UNDEF (int, int);
         static int   	PUSH_TAIL (int, int);
         static int   	SET_START (int, int);
         static int   	SET_END (int, int);
         static int   	MAKE_SETNAME (int, int);
         static int   	ADD_HEAD (int, int);
         static int   	ADD_HEADLEX (int, int);
         static int   	ADD_HEADIGNORE (int, int);
         static int   	ADD_HEADSET (int, int);
         static int   	ADD_PROD (int, int);
         static int   	CHECK_PROD (int, int);
         static int   	DEF_TAIL ();
         static int   	ADD_TERMINAL ();
         static int   	CHK_NUMB (); 
			static void	 	MAKE_SET (int s);
			static void  	P_PROD (int p);
			static void  	ADDSETNAME (int s, char* hit);
			static void  	SUBSETNAME (int s, char* hit);
         static int   	ADD_NOTERM (int, int);
         static int   	ADD_TERMNO (int, int);
         static int   	ADD_DEFCON (int, int);
         static int   	ADD_STRING (int, int);
         static int   	START_GRM ();
         static int   	ADD_GOAL (int, int);
         static int   	ADD_EOF (int, int);
         static int   	ADD_CHAR (int, int);
         static int   	SUB_CHAR (int, int);
         static int   	RANGE_START (int, int);
         static int   	RANGE_END (int, int);
         static int   	RANGE_TOP (int, int);
         static int   	RANGE_ALL (int, int);
         static int   	ADD_RANGE (int, int);
         static int   	SUB_RANGE (int, int);
         static int   	ADD_PRODADD ();
         static int   	ADD_PRODSUB ();
         static int   	ADD_PRODTO ();
         static int   	ADD_SETNAME (int, int);
         static int   	ADD_SET (int, int);
         static int   	SUB_SET (int, int);
         static int   	CHK_SETNAME ();
         static int   	ADD_NULL (int, int);	
         static int   	POP_ALL (int, int);
         static int   	POP_SET (int, int);
         static int   	PUSH_GBEG (int, int);
         static int   	PUSH_GEND (int, int);
         static int   	PUSH_OR (int, int);
         static int   	PUSH_DOTS (int, int);
         static int   	PUSH_PLUS (int, int);
         static int   	PUSH_ASTER (int, int);
         static int   	PUSH_QUEST (int, int);
         static int   	PUSH_GDOTS (int, int);
         static int   	PUSH_GPLUS (int, int);
         static int   	PUSH_GASTER (int, int);
         static int   	PUSH_GQUEST (int, int);
         static int   	PUSH_OBEG (int, int);
         static int   	PUSH_OEND (int, int);
         static int   	PUSH_ODOTS (int, int);

      };

