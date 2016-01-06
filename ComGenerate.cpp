#include <algorithm>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include "ComGlobal.h"
#include "ComGenerate.h"

#if defined(LRSTAR)
#   include "PGGenerate.h"
#elif defined(DFASTAR)
#   include "LGGenerate.h"
#endif

int    	 Generate::num_char;
int    	 Generate::num_uchar;
int    	 Generate::num_short;
int    	 Generate::num_ushort;
int    	 Generate::num_int;
int    	 Generate::num_uint;
int    	 Generate::num_charp;
STAKTYPE  Generate::STAK[MAXTOP];
int   	 Generate::staktop;
int   	 Generate::maxtop;
char* 	 Generate::group_start;
int   	 Generate::skip_code;
int   	 Generate::g_size;
char  	 Generate::in_group;
char  	 Generate::format[50];
int   	 Generate::width;
int   	 Generate::bytes_out;
int   	 Generate::first_err;
int   	 Generate::last_sep_l;
int   	 Generate::linenumb;
int   	 Generate::fd;
char  	 Generate::skl_fid [MAX_PATH];
char  	 Generate::out_fid [MAX_PATH];
int   	 Generate::count;
int   	 Generate::mult;
int   	 Generate::plus;
char  	 Generate::middle[2000];
char  	 Generate::sep[2000];
char  	 Generate::end[2000];
char  	 Generate::arg4[30];
char  	 Generate::arg5[30];
char  	 Generate::arg6[30];
int   	 Generate::middle_l;
int   	 Generate::sep_l;
int   	 Generate::end_l;
int   	 Generate::n_out;
char* 	 Generate::skel;
char* 	 Generate::skelbeg;
char* 	 Generate::skelend;
char* 	 Generate::buffer;
char* 	 Generate::buffptr;
char* 	 Generate::buffend;
int   	 Generate::operation_flag;
int   	 Generate::n_origlines;
int   	 Generate::n_addedlines;
int   	 Generate::max_outbuff;

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

		int N_targs;
		int N_pargs;
		int N_nargs;

		#ifdef LRSTAR
		enum DefCon
		{
			ACCP_STA,			// Accept state (final state).
			ARG_NUMB,   		// Argument numbers.
			ARG_TEXT,   		// Argument text strings.
			BMAT_COL,   		// B matrix column index.
			BMAT_MASK,  		// B matrix mask. 
			BMAT_NUMB,  		// B matrix (0's and 1's). 
			BMAT_ROW,   		// B matrix row index.
			DEF_CONS,   		// Defined constants. 
			EOF_NUMB,   		// EOF terminal number.
			EOL_NUMB,   		// EOL terminal number.
			ERR_USED,   		// <error> symbol used in grammar (0 or 1).
			GRM_FILE,   		// Grammar filename (e.g. C.grm).
			GRM_NAME,   		// Grammar name (e.g. C).
			GRM_TEXT,   		// Grammar text (complete file contents). 
			HEAD_PROD,  		// Head production (first production).
			HEAD_SYMB,  		// Head symbols.
			KEY_NUMB,   		// <keyword> symbol number.
			MAKE_AST,   		// Make AST switch (0 or 1).
			NACT_ARG,   		// Node action arguments. 
			NACT_FUNC,  		// Node fuction pointers.
			NACT_NUMB,  		// Node action number for each production.
			ND_ACTION,  		// Nondeterministic action.
			ND_START,   		// Nondeterministic starting point in list (n+1 of these).
			ND_TERM,    		// Nondeterministic terminal.
			NMAT_COL,   		// N matrix column index.
			NMAT_NUMB,  		// N matrix numbers.
			NMAT_ROW,   		// N matrix row index.
			NODE_NAME,  		// Node names.
			NODE_NUMB,  		// Node numbers for productions. 
			NUMB_HEAD,  		// Number of head symbols (nonterminals)
			NUMB_NACT,  		// Number of node actions.
			NUMB_NODE,  		// Number of nodes.
			NUMB_PACT,  		// Number of parse actions (tact + ract).
			NUMB_PROD,  		// Number of productions.
			NUMB_STA,   		// Number of states.
			NUMB_TACT,  		// Number of token actions.
			NUMB_TERM,  		// Number of terminals.
			OPTN_AST, 			// Abstract-syntax tree (0,1).
			OPTN_BM, 			// Boolean matrix (1,2),
			OPTN_CLR, 			// Canonical LR(1) parser tables (0,1),
			OPTN_DEBUG, 		// Debug parser option (0,1,2,3),
			OPTN_DR, 			// Default reductions (0,1),
			OPTN_EXP,   		// Expecting list option (0,1,2),
			OPTN_LR, 			// Minimal LR(1) parser tables (0,1),
			OPTN_NA, 			// Number of arguments required for a node (0,1,2,3).
			OPTN_ND, 			// Nondeterministic parsing in effect (0,1).
			OPTN_SRA,   		// Shift-reduce actions (0,1).
			OPTN_X, 				// x - defined by user (?).								
			OPTN_Y,  			// y - defined by user (?).								
			OPTN_Z, 	   		// z - defined by user (?).								
			OUT_FILE,   		// Output filename. 
			PACT_ARG,   		// Parsing-action argument pointer (first argument also used by make-node code). 
			PACT_FUNC,  		// Parsing-action function names. 
			PACT_NUMB,  		// Parsing-action numbers (one for each production). 
			PROD_HEAD,  		// Production head symbol.
			PROD_LENG,  		// Production length
			PROD_REVS,  		// Production reverse code (0 or 1). 
			PROD_TAIL,  		// Production tail (first tail number).
			PROGRAM,    		// Program name (LRSTAR).
			RESTORE,    		// Restore parser stack code.
			RMAT_COL,   		// R matrix column index.
			RMAT_NUMB,  		// R matrix numbers.
			RMAT_ROW,   		// R matrix row index.
			SKL_FILE,   		// Skeleton filename.
			STA_ACCS,   		// State accessor.
			STA_DOT,    		// State dot.
			STA_ITEM,   		// State items.
			STA_PROD,   		// State productions.
			TACT_ARG,   		// Token action argument numbers. 
			TACT_FUNC,  		// Token action function names. 
			TACT_NUMB,  		// Token action numbers. 
			TAIL_NUMB,  		// Tail numbers. 
			TERM_SYMB,  		// Terminal symbols (all terminals)
			TMAT_COL,   		// T matrix column index.
			TMAT_NUMB,  		// T matrix numbers.
			TMAT_ROW,   		// T matrix row index.
			VERSION,    		// Version of program.
		};
		#endif

		#ifdef DFASTAR
		enum DefCon
		{
			LG_ACTION_CODE,    // Action code. 
			LG_BMAT_COL,   	// B matrix column index.
			LG_BMAT_NUMB,  	// B matrix (0's and 1's). 
			LG_BMAT_ROW,   	// B matrix row index.
			LG_DIRECT_CODE, 	// Direct code. 
			LG_DEF_CONS,   	// Defined constants. 
			LG_ERR_TOKEN,  	// Error token.
			LG_GRM_FILE,   	// Grammar filename.
			LG_GRM_NAME,   	// Grammar name.
			LG_GRM_TEXT,   	// Grammar text (complete file contents). 
			LG_NUMB_STA,   	// Number of states.
			LG_NUMB_TERM,  	// Number of terminals.
			LG_OPTN_CODE,		// Direct-code lexer. 
			LG_OPTN_COL,   	// Column number in lexer.
			LG_OPTN_DEBUG, 	// Debug option.
			LG_OPTN_LARGE, 	// Large table-driven lexer.
			LG_OPTN_LINE,  	// Line number in lexer.
			LG_OPTN_MEDIUM,	// Medium table-driven lexer.
			LG_OPTN_SMALL, 	// Small table-driven lexer.
			LG_OUT_FILE,   	// Output filename. 
			LG_PROGRAM,    	// Program name (DFASTAR).
			LG_SKL_FILE,   	// Skeleton filename.
			LG_STRINGS,    	// String return values.
			LG_TERM_NUMB,  	// Terminal number for lexer tables.
			LG_TMAT_COL,   	// T matrix column index.
			LG_TMAT_NUMB,  	// T matrix numbers.
			LG_TMAT_ROW,   	// T matrix row index.
			LG_VERSION     	// Version of program.
		};
		#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

		CODETABLE* code_table[200]; 

		#ifdef LRSTAR
		CODETABLE PGCodeTable[] =
		{
			"accp_sta",   	"dt",	ACCP_STA,   		"Accept (final) state number",
			"arg_numb",   	"dt",	ARG_NUMB,   		"Argument numbers",
			"arg_text",   	"dt",	ARG_TEXT,   		"Argument text strings",
			"bmat_col",   	"dt",	BMAT_COL,   		"B matrix column index",
			"bmat_mask",  	"dt",	BMAT_MASK,  		"B matrix mask", 
			"bmat_numb",  	"dt",	BMAT_NUMB,  		"B matrix numbers", 
			"bmat_row",   	"dt",	BMAT_ROW,   		"B matrix row index",
			"def_cons",   	"dt",	DEF_CONS,   		"Defined constants", 
			"eof_numb",   	"dt",	EOF_NUMB,   		"End of file terminal number",
			"eol_numb",   	"dt",	EOL_NUMB,   		"End of line terminal number",
			"err_used",   	"dt",	ERR_USED,   		"<error> symbol used in grammar (0 or 1)",
			"grm",   		"s ",	GRM_NAME,   		"Grammar name (same as grm_name)",
			"grm_file",   	"s ",	GRM_FILE,   		"Grammar filename",
			"grm_name",   	"s ",	GRM_NAME,   		"Grammar name",
			"grm_text",   	"s ",	GRM_TEXT,   		"Grammar text (whole file contents)", 
			"head_prod",  	"dt",	HEAD_PROD,  		"Head symbol first production (rule)",
			"head_symb",  	"dt",	HEAD_SYMB,  		"Head symbol for a production (rule)",
			"key_numb",   	"dt",	KEY_NUMB,   		"<keyword> symbol number",
			"make_ast",   	"dt",	MAKE_AST,   		"Make AST switch (0 or 1)",
			"nact_arg",   	"dt",	NACT_ARG,   		"Node-action arguments index", 
			"nact_func",  	"dt",	NACT_FUNC,  		"Node-action fuction names",
			"nact_numb",  	"dt",	NACT_NUMB,  		"Node-action number for a production",
			"nd_action",  	"dt",	ND_ACTION,  		"Nondeterministic action list (goto or reduce)",
			"nd_start",   	"dt",	ND_START,   		"Nondeterministic start point in action list",
			"nd_term",    	"dt",	ND_TERM,    		"Nondeterministic terminal symbol list",
			"nmat_col",   	"dt",	NMAT_COL,   		"N matrix column index",
			"nmat_numb",  	"dt",	NMAT_NUMB,  		"N matrix numbers",
			"nmat_row",   	"dt",	NMAT_ROW,   		"N matrix row index",
			"node_name",  	"dt",	NODE_NAME,  		"Node names",
			"node_numb",  	"dt",	NODE_NUMB,  		"Node number for a production", 
			"numb_head",  	"dt",	NUMB_HEAD,  		"Number of head symbols (nonterminals)",
			"numb_nact",  	"dt",	NUMB_NACT,  		"Number of node actions",
			"numb_node",  	"dt",	NUMB_NODE,  		"Number of nodes",
			"numb_pact",  	"dt",	NUMB_PACT,  		"Number of parse actions",
			"numb_prod",  	"dt",	NUMB_PROD,  		"Number of productions (rules)",
			"numb_sta",   	"dt",	NUMB_STA,   		"Number of states",
			"numb_tact",  	"dt",	NUMB_TACT,  		"Number of token actions",
			"numb_term",  	"dt",	NUMB_TERM,  		"Number of terminal symbols",

			"optn_ast",		"dt",	OPTN_AST, 			"Abstract-Syntax Tree construction (0,1)",
			"optn_bm",		"dt",	OPTN_BM, 			"Boolean matrix (1=bytes,2=bits)",
			"optn_clr",		"dt",	OPTN_CLR, 			"Canonical LR(1) parser tables (0,1)",
			"optn_debug", 	"dt",	OPTN_DEBUG, 		"Debug parser option (0,1,2,3)",
			"optn_dr",		"dt",	OPTN_DR, 			"Default reductions (0,1)",
			"optn_exp",   	"dt",	OPTN_EXP,   		"Expecting list option (0,1,2)",
			"optn_lr",		"dt",	OPTN_LR, 			"Minimal LR(1) parser tables (0,1)",
			"optn_na",		"dt",	OPTN_NA, 			"Number of arguments required for a node (0,1,2,3)",
			"optn_nd",		"dt",	OPTN_ND, 			"Nondeterministic parsing in effect (0,1)",
			"optn_sra",		"dt",	OPTN_SRA, 			"Shirt-reduce actions in parser (0,1)",
			"optn_x",		"dt",	OPTN_X, 				"x - defined by user (?)",								
			"optn_y",		"dt",	OPTN_Y,  			"y - defined by user (?)",								
			"optn_z",		"dt",	OPTN_Z, 	   		"z - defined by user (?)",								

			"out_file",    "s ",	OUT_FILE,   		"Output filename (e.g. parser.cpp)", 
			"pact_arg",    "dt",	PACT_ARG,   		"Parsing-action arguments index", 
			"pact_func",   "dt",	PACT_FUNC,  		"Parsing-action function names", 
			"pact_numb",   "dt",	PACT_NUMB,  		"Parsing-action numbers (one for each production)", 
			"prod_head",   "dt",	PROD_HEAD,  		"Production head symbol (one for each rule)",
			"prod_leng",   "dt",	PROD_LENG,  		"Production length",
			"prod_revs",   "dt",	PROD_REVS,  		"Production reverse code (0 or 1)", 
			"prod_tail",   "dt",	PROD_TAIL,  		"Production tail (first tail number)",
			"program",     "s ",	PROGRAM,    		"Program name (LRSTAR)",
			"restore",     "dt",	RESTORE,    		"Restore parser stack code (0 or 1)",
			"rmat_col",    "dt",	RMAT_COL,   		"R matrix column index",
			"rmat_numb",   "dt",	RMAT_NUMB,  		"R matrix numbers",
			"rmat_row",    "dt",	RMAT_ROW,   		"R matrix row index",
			"skl_file",    "s ",	SKL_FILE,   		"Skeleton filename (e.g. parser.cpp.skl)",
			"sta_accs",    "dt",	STA_ACCS,   		"State accessor symbols",
			"sta_dot",     "dt",	STA_DOT,    		"State dot (in production)",
			"sta_item",    "dt",	STA_ITEM,   		"State items",
			"sta_prod",    "dt",	STA_PROD,   		"State productions",
			"tact_arg",    "dt",	TACT_ARG,   		"Terminal action index into argument numbers list", 
			"tact_func",   "dt",	TACT_FUNC,  		"Terminal action function names", 
			"tact_numb",   "dt",	TACT_NUMB,  		"Terminal action numbers (one for each terminal)", 
			"tail_numb",   "dt",	TAIL_NUMB,  		"Tail numbers", 
			"term_symb",   "dt",	TERM_SYMB,  		"Terminal symbols (all terminals)",
			"tmat_col",    "dt",	TMAT_COL,   		"T matrix column index",
			"tmat_numb",   "dt",	TMAT_NUMB,  		"T matrix numbers",
			"tmat_row",    "dt",	TMAT_ROW,   		"T matrix row index",
			"version",     "s ",	VERSION,    		"Version of program (e.g. 3.0.100)",
			"~",           "",	0,					   "Terminates this list"
		};
		#endif

		#ifdef DFASTAR
		CODETABLE LGCodeTable[] =
		{
			"action_code", "dt",	LG_ACTION_CODE,   "Action code",
			"bmat_col",    "dt",	LG_BMAT_COL,    	"B matrix column index",
			"bmat_numb",   "dt",	LG_BMAT_NUMB,   	"B matrix (0's and 1's)", 
			"bmat_row",    "dt",	LG_BMAT_ROW,    	"B matrix row index",
			"def_cons",    "dt",	LG_DEF_CONS,    	"Defined constants", 
			"direct_code", "s",	LG_DIRECT_CODE,   "Direct code output", 
			"err_token",   "dt",	LG_ERR_TOKEN,   	"Error token",
			"grm",         "s ",	LG_GRM_NAME,    	"Grammar name (same as grm_name)",
			"grm_file",    "s ",	LG_GRM_FILE,    	"Grammar filename",
			"grm_name",    "s ",	LG_GRM_NAME,    	"Grammar name",
			"grm_text",    "s ",	LG_GRM_TEXT,    	"Grammar text (complete file contents)", 
			"numb_sta",    "dt",	LG_NUMB_STA,    	"Number of states",
			"numb_term",   "dt",	LG_NUMB_TERM,   	"Number of terminals/characters(256)",
			"optn_code",   "dt",	LG_OPTN_CODE,    	"Direct code lexers",
			"optn_col",    "dt",	LG_OPTN_COL,    	"Column number in lexer",
			"optn_debug",  "dt",	LG_OPTN_DEBUG,  	"Debug option",
			"optn_large",  "dt",	LG_OPTN_LARGE,  	"Large table-driven lexer",
			"optn_line",   "dt",	LG_OPTN_LINE,   	"Line number in lexer",
			"optn_medium", "dt",	LG_OPTN_MEDIUM, 	"Medium table-driven lexer",
			"optn_small",  "dt",	LG_OPTN_SMALL,  	"Small table-driven lexer",
			"out_file",    "s ",	LG_OUT_FILE,    	"Output filename", 
			"program",     "s ",	LG_PROGRAM,     	"Program name (DFASTAR)",
			"skl_file",    "s ",	LG_SKL_FILE,    	"Skeleton filename",
			"strings",     "dt",	LG_STRINGS,     	"String return values",
			"term_numb",   "dt",	LG_TERM_NUMB,   	"Terminal number for lexer tables",
			"tmat_col",    "dt",	LG_TMAT_COL,    	"T matrix column index",
			"tmat_numb",   "dt",	LG_TMAT_NUMB,   	"T matrix numbers",
			"tmat_row",    "dt",	LG_TMAT_ROW,    	"T matrix row index",
			"version",     "s ",	LG_VERSION,     	"Version of program",
			"~",           "",	0,						"Terminates this list"
		};
		#endif

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

bool Generate::GenerateCode(const char* sklfid, const char* outfid, int verbose)
{
    static bool first = true;
    if (first) {
			first = false;
			#ifdef LRSTAR
  			for (int k = 0;; k++)	
			{
				code_table[k] = &(PGCodeTable[k]);
				if (PGCodeTable[k].keyword[0] == '~') break;
			}
			#endif
			#ifdef DFASTAR
  			for (int k = 0;; k++)	
			{
				code_table[k] = &(LGCodeTable[k]);
				if (LGCodeTable[k].keyword[0] == '~') break;
			}
			#endif
        initialize();
    }

      first_err    = 1;
		n_origlines  = 0;
		n_addedlines = 0;
      staktop      = 0;
		count        = 1;
		mult         = 1;
		plus         = 0;
      maxtop       = MAXTOP;
      group_start  = NULL;
		operation_flag = 0;

		strcpy (skl_fid, sklfid);
		strcpy (out_fid, outfid);

    // Load skeleton file ...
    std::vector<char> skeldata;
    std::ifstream skel_stm(skl_fid, std::ios_base::binary);
    if (!skel_stm)
        return false;
    skel_stm.unsetf(std::ios_base::skipws);
    skel_stm.seekg(0, std::ios_base::end);
    skeldata.reserve(static_cast<size_t>(skel_stm.tellg()));
    skel_stm.seekg(0, std::ios_base::beg);
    std::copy(std::istream_iterator<char>(skel_stm), std::istream_iterator<char>(), std::back_inserter(skeldata));
    if (skeldata.empty()) {
        if (++n_errors == 1)
            prt_log ("\n");
        prt_log ("Skeleton: %s  is empty!\n", skl_fid);
        return false;
    }

    if (verbose > 1)
        prt_log("Skeleton: %s\n", skl_fid);
    else
        prt_logonly("Skeleton: %s\n", skl_fid);

    // TODO: elliminate those globals
    skel = skeldata.data();
    skelend = skel + skeldata.size();

      if (chmod (out_fid, S_IWRITE) == 0) // File can be written ?
      {
         if (unlink (out_fid) != 0) // Delete it?
         {
	         if (++n_errors == 1) prt_log ("\n");
				prt_log ("Output:   %s  cannot be rewritten!\n", out_fid);
            return false;
         }
      }

		#ifdef DFASTAR
		if (optn[LG_DIRECTCODE])
		{
			if (!open_code (out_fid))
				return false;
		}
		else
		#endif
		{
			fd = open (out_fid, O_CREAT | O_TRUNC | O_WRONLY, S_IREAD | S_IWRITE);
			if (fd < 0)
			{
				if (++n_errors == 1) prt_log ("\n");
				prt_log ("Output:   %s  cannot be created!\n", out_fid);
				return false;
			}
		}
    INIT_VARS ();
    EMIT_ALL (verbose);
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

#ifdef LRSTAR
void PGGenerate::initialize()
{
    static bool firsttime = true;
    if (firsttime)
        firsttime = false;
    else
        return;
    if (N_args == 0) {
        N_args = 1;
        Arg_numb[0] = 0;
    }
    if (N_strings == 0) {
        N_strings = 1;
        char* nullstr;
        nullstr = (char*)malloc(4);
        strcpy (nullstr, "\"\"");
        Str_start[0] = nullstr;
    }
    // AST construction turned off ?
    if (optn[PG_ASTCONST] == 0) {
        N_nodes = 0;
        N_nacts = 0;
        for (int i = 0; i < N_prods; i++)
            Node_numb[i] = -1;
    }
    // No token actions ?
    if (N_tacts == 0) {
        N_targs = 1;
        F_targ[0] = 0;
    } else
        N_targs = N_terms;
    // No parse actions and no nodes ?
    if (N_pacts == 0 && N_nodes == 0) {
        N_pargs = 1;
        F_parg[0] = 0;
    } else
        N_pargs = N_prods;
    // No node actions ?
    if (N_nacts == 0) {
        N_nargs = 1;
        F_narg[0] = 0;
    } else
    N_nargs = N_prods;
}
#endif

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void Generate::INIT_VARS()
{
		strcpy (str_char,      "char"          );
		strcpy (str_uchar,     "unsigned char" );
		strcpy (str_short,     "short"         );
		strcpy (str_ushort,    "unsigned short");
		strcpy (str_int,       "int"           );
		strcpy (str_uint,      "unsigned int"  );
		strcpy (str_charp,     "char*" );

		num_char   = 1;
		num_uchar  = 1;
		num_short  = 2;
		num_ushort = 2;
		num_int    = 4;
		num_uint   = 4;
		num_charp  = 4;
}

void  Generate::EMIT_ALL (int verbose)
{
		g_size      = 0;
		in_group    = 0;
      linenumb    = 1;
      bytes_out   = 0;
      skip_code   = 0;
      max_outbuff = 1000000; // 1 MB
      ALLOC (buffer, max_outbuff);
      buffptr = buffer;
      buffend = buffer + max_outbuff - max_outbuff/4; // 25% overflow.
      skelbeg = skel;

		SCAN ();

      DUMP_SKEL (skelbeg, skel, NULL);
      FREE (buffer, max_outbuff);
		char num[12] = "           ";
	  	number (n_origlines+n_addedlines, num);

		#ifdef DFASTAR
		if (optn[LG_DIRECTCODE])
		{
			close_code();
		}
		else
		#endif
		{
			close (fd);
		}
		if (verbose > 1) prt_log     ("Output:   %s (%s lines)\n", out_fid, num);
		else             prt_logonly ("Output:   %s (%s lines)\n", out_fid, num);

      chmod (out_fid, S_IREAD); // Make output file read-only.
}

void Generate::SCAN ()
{
      while (skel < skelend)
      {
         if (*skel == '"') // String ?
			{
				char *p;
Scan:			p = skel++;
				while (*skel != '"' && *skel != '\n' && *skel != '\\' && *skel != '@') skel++;
				if (*skel == '"') // End of string?
				{
					/* Nothing */
				}
            else if (*skel == '\n') // End of line?
				{
					PRT_ERR (p, linenumb);
					prt_log ("%s(%04d) : String has no ending '\"' on this line.\n\n", skl_fid, linenumb);
					Terminate (n_errors);
				}
		  		else if (*skel == '\\') // \ escape.
				{
		  			if (*(skel+1) == '@') // \@ found?
					{
						DUMP_SKEL (skelbeg, skel, skel+1);
					}
					goto Scan;
				}
				else if (*skel == '@')
				{
					if (alpha[*(skel+1)]) // @[a-zA-Z] inside of string?
					{
	               DUMP_SKEL (skelbeg, skel, skel);
		            skelbeg = skel = READ_CODE(skel);
						goto Scan;
					}
					else
					{
						PRT_ERR (skel+1, linenumb);
						prt_log ("%s(%04d) : Expecting <name> after '@'.\n\n", skl_fid, linenumb);
						Terminate (n_errors);
					}
				}
			}
         else if (*skel == '@') // Skeleton code ?
         {
			  	if (*(skel-1) == '\\') // \@ escape.
				{
					DUMP_SKEL (skelbeg, skel-1, skel);
					goto Incr;
				}
			  	if (*(skel+1) == '(') // \@ escape.
				{
					char*p = skel;
					READ_VARS (skel+1);
					DUMP_SKEL (skelbeg, p, skel);
					goto Cont;
				}
            if (*(skel+1) == '/' && *(skel+2) == '/') // @// line comment.
            {
					char*p = skel+3;
               DUMP_SKEL (skelbeg, skel, skel);
               skelbeg = skel = skip_rest_of_line (p);
               goto Cont;
            }
            if (*(skel+1) == '/' && *(skel+2) == '*') // @/* block comment.
            {
					char*p = skel;
               DUMP_SKEL (skelbeg, skel, skel);
               skelbeg = skel = skip_rest_of_comment (p);
               goto Cont;
            }
				if (alpha[*(skel+1)]) // @[a-zA-Z]?
				{
               DUMP_SKEL (skelbeg, skel, skel);
               skelbeg = skel = READ_CODE(skel);
               goto Cont;
				}
            if (*(skel+1) == '@') // '@@' skeleton group terminator.
            {
				//	printf ("%5d @@\n", linenumb);
					if (in_group == 0) // Already outside of group?
					{
						PRT_ERR (skel, linenumb);
						prt_log ("%s(%04d) : Group ending '@@' found outside of a group.\n\n", skl_fid, linenumb);
						Terminate (n_errors);
					}
				  	if (skip_code)
					{
						while (*skel++ != '\n'); // Skip rest of line.
						linenumb++;
						DUMP_SKEL (skelbeg, skel, skel);
					}
				  	else
					{
						DUMP_SKEL (skelbeg, skel, skel);
						while (*skel++ != '\n'); // Skip rest of line.
						linenumb++;
						skelbeg = skel;
					}
					UNSTAKCOND();
					goto Cont;
            }
			  	else // Not recognized, crash.
				{
			      PRT_ERR (skel, linenumb);
					prt_log ("%s(%04d) : After '@' expecting: '//', '/*', <name>, or '@'\n\n", skl_fid, linenumb);
					Terminate (n_errors);
				}
         }
Incr:  	if (*skel++ == '\n') linenumb++;
Cont:    continue;
      }
	//	printf ("in_group = %d\n", in_group);
		if (in_group)
		{
			while (skel > group_start) if (*--skel == '\n') linenumb--;
	      PRT_ERR (group_start, linenumb);
			prt_log ("%s(%04d) : End of file reached, but no group ending '@@' was found for this group.\n\n", skl_fid, linenumb);
			unlink (out_fid); // Delete output file.
			Terminate (n_errors);
		}
}

void	Generate::STAKCOND() // Stack current conditional status.
{
		in_group = 1;
	//	printf ("in_group = %d\n", in_group);
		if (staktop == maxtop)
		{
			PRT_ERR (skel, linenumb);
			prt_log ("%s(%04d) : More than 10 nested groups causes a stack overflow.\n\n", skl_fid, linenumb);
			Terminate (n_errors);
		}
		STAK[staktop].skipcode = skip_code;
		STAK[staktop].groupstart = group_start;
		char* p = skel;
		while (*p != '\n') p--; // Find beginning of line.
		p++;							// p = beginning of line.
		while (*p != '@') p++;
		group_start = p;
		staktop++;

		#ifdef _DEBUG
		char c;
		c = *(group_start+15);
		*(group_start+15) = 0;
		printf ("After STAK %s, line %d\n", group_start, linenumb);
	  	printf ("STAK[%d].skipcode   = %d\n", staktop, skip_code);
	  	printf ("STAK[%d].groupstart = %p\n", staktop, group_start);
		printf ("in_group = %d\n\n", in_group);
		*(group_start+15) = c;
		#endif  
}

void	Generate::UNSTAKCOND() // Unstack conditional status.
{
		if (staktop > 0)
		{
			staktop--;
			skip_code   = STAK[staktop].skipcode;
			group_start = STAK[staktop].groupstart;
			if (staktop == 0)
			{
				in_group = 0; // Out of block now.
			//	printf ("in_group = %d\n", in_group);
			}
		}
		else
		{
			in_group = 0;
		//	printf ("in_group = %d\n", in_group);
			skip_code = 0;
			group_start = NULL;
		}
	/*	#ifdef _DEBUG
		char c;
		if (group_start != NULL) { c = *(group_start+15); *(group_start+15) = 0; }
		printf ("After UNSTAK %s, line %d\n", group_start, linenumb);
	  	printf ("STAK[%d].skipcode   = %d\n", staktop, skip_code);
	  	printf ("STAK[%d].groupstart = %p\n", staktop, group_start);
		printf ("in_group = %d\n\n", in_group);
		if (group_start != NULL) *(group_start+15) = c;
		#endif  
	*/
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

char* Generate::READ_CODE(char* sk) // sk -> @
{
      char *p, c;
		int x, result = 1, oper = 1, val, vx;
		skel = sk;

      p = GETCODENUM (++skel, x); // If skeleton code name is bad, this terminates.
		switch (c = GET_OPER (p, x))
		{
			case '?':
Quest:		val = value(x);
				if (val != 0) val = 1;
				if (oper == 0) result |= val;
				else           result &= val;
				goto Check;

			case '!':
Excl:			val = value(x);
				if (val == 0) val = 1;
				else            val = 0;
				if (oper == 0) result |= val;
				else           result &= val;

Check:		if (*++skel == ';')
				{
Ret:				if (result) ERASE ();
					else        SkipRestOfLineOrBlock (skel+1);
					return (skel);
				}
Or:			if (strncmp (skel, ".or.", 4) == 0)
				{
					oper = 0; // or
					skel += 4;
					if (*skel == '@') goto Name;
					goto Err1;
				}
				else if (strncmp (skel, ".and.", 5) == 0)
				{
					oper = 1; // and
					skel += 5;
					if (*skel == '@') goto Name;
					goto Err1;
				}
				goto Err2;

Name:			p = GETCODENUM (++skel, x); // If skeleton code name is bad, this terminates.
				switch (c = GET_OPER (p, x))
				{
					case '?': goto Quest;
					case '!': goto Excl;
					case '.': goto Eq;
				}
				goto Err3;

			case '.':
Eq:			vx = value(x);
   			if (strncmp (skel, ".eq.", 4) == 0)
				{
					skel += 4;
					char* num = skel;
					while (numeric[*skel]) skel++;
					if (skel == num) goto Err4; // No numeric characters?
					if (vx == atoi(num)) val = 1;
					else                 val = 0;
					if (oper == 0) result |= val;
					else           result &= val;
					if (*skel == '.') goto Or;
					if (*skel == ';') goto Ret;
					goto Err2;
				}
   			else if (strncmp (skel, ".ne.", 4) == 0)
				{
					skel += 4;
					char* num = skel;
					while (numeric[*skel]) skel++;
					if (skel == num) goto Err4; // No numeric characters?
					if (vx != atoi(num)) val = 1;
					else                 val = 0;
					if (oper == 0) result |= val;
					else           result &= val;
					if (*skel == '.') goto Or;
					if (*skel == ';') goto Ret;
					goto Err2;
				}
   			else if (strncmp (skel, ".gt.", 4) == 0)
				{
					skel += 4;
					char* num = skel;
					while (numeric[*skel]) skel++;
					if (skel == num) goto Err4; // No numeric characters?
					if (vx >  atoi(num)) val = 1;
					else                 val = 0;
					if (oper == 0) result |= val;
					else           result &= val;
					if (*skel == '.') goto Or;
					if (*skel == ';') goto Ret;
					goto Err2;
				}
   			else if (strncmp (skel, ".ge.", 4) == 0)
				{
					skel += 4;
					char* num = skel;
					while (numeric[*skel]) skel++;
					if (skel == num) goto Err4; // No numeric characters?
					if (vx >= atoi(num)) val = 1;
					else                 val = 0;
					if (oper == 0) result |= val;
					else           result &= val;
					if (*skel == '.') goto Or;
					if (*skel == ';') goto Ret;
					goto Err2;
				}
   			else if (strncmp (skel, ".lt.", 4) == 0)
				{
					skel += 4;
					char* num = skel;
					while (numeric[*skel]) skel++;
					if (skel == num) goto Err4; // No numeric characters?
					if (vx <  atoi(num)) val = 1;
					else                 val = 0;
					if (oper == 0) result |= val;
					else           result &= val;
					if (*skel == '.') goto Or;
					if (*skel == ';') goto Ret;
					goto Err2;
				}
   			else if (strncmp (skel, ".le.", 4) == 0)
				{
					skel += 4;
					char* num = skel;
					while (numeric[*skel]) skel++;
					if (skel == num) goto Err4; // No numeric characters?
					if (vx <= atoi(num)) val = 1;
					else                 val = 0;
					if (oper == 0) result |= val;
					else           result &= val;
					if (*skel == '.') goto Or;
					if (*skel == ';') goto Ret;
					goto Err2;
				}
				goto Err5;

			case 'd':
				skel++;
				EMIT_NUM (value(x));
				return (skel+1);

			case 's':
				skel++;
				emitStr(x);
				return (skel+1);

			case 't':
				skel++;
				emitType(x);
				if (*format == 0) strcpy (format, "%t");
				SPRINT (format, 0, (int*)string, (int*)string);
				return (skel+1);

			case '|': // operator was a <number>
				skel++;
				GET_STRINGS (skel);
				emit(x);
				return (skel);

			case ';':
				skel++;
				if (code_table[x]->operators[0] == 's')
					emitStr(x);
				else {
					skel--;
					PRT_ERR (skel, linenumb);
					prt_log ("%s(%04d) : Default string type not valid for skeleton name \"%s\"\n\n",
					skl_fid, linenumb, code_table[x]->keyword);
					Terminate (n_errors);
				}
				return (skel);
		}

Err1: PRT_ERR (skel+1, linenumb);
		prt_log ("%s(%04d) : Expecting another skeleton code name starting with '@'\n\n", skl_fid, linenumb);
		Terminate (n_errors);

Err2: PRT_ERR (skel, linenumb);
		prt_log ("%s(%04d) : Expecting '.or.', '.and.', or ';'\n\n", skl_fid, linenumb);
		Terminate (n_errors);

Err3: PRT_ERR (skel, linenumb);
		prt_log ("%s(%04d) : Expecting '?' or '!'\n\n", skl_fid, linenumb);
		Terminate (n_errors);

Err4: PRT_ERR (skel, linenumb);
		prt_log ("%s(%04d) : Expecting an <integer>\n\n", skl_fid, linenumb);
		Terminate (n_errors);

Err5: PRT_ERR (skel, linenumb);
		prt_log ("%s(%04d) : Expecting '.eq.', '.ne.', '.ge.', or '.lt.'\n\n", skl_fid, linenumb);
		Terminate (n_errors);
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

char* Generate::GETCODENUM (char *keyword, int& x)
{
      int  i, a;
		char *p, c;
		for (p = keyword; alpha[*p] != 0 && p < keyword+32; ++p);
		if (p == keyword) // Check for no skeleton code name @;.
		{
			PRT_ERR (keyword, linenumb);
			prt_log ("%s(%04d) : No skeleton code name was specified.\n\n", skl_fid, linenumb);
			Terminate (n_errors);
		}
		c = *p;
		*p = 0;
  		#ifdef _DEBUG  
      printf ("%s\n", keyword);
  		#endif  
		for (i = 0;; i++)
		{
			if (code_table[i]->keyword[0] < keyword[0]) continue;			// Sorted table!
			a = strcmp (code_table[i]->keyword, keyword);
			if (a < 0) continue;
			if (a > 0) goto Err;
			x  = i;	// Set code number.
			*p = c;
		//	printf ("%5d %s\n", linenumb, code_table[i].keyword);
			return (p);
		}

Err:	*p = c;
		PRT_ERR (keyword, linenumb);
		*p = 0;
		prt_log ("%s(%04d) : \"%s\" was not found among skeleton code names.\n\n", skl_fid, linenumb, keyword);
      prt_log ("Expecting one of the following:\n\n");
		for (i = 0;; i++)
		{
			if (code_table[i]->keyword[0] == '~') break;
		  	prt_log ("   %-12s - %s\n", code_table[i]->keyword, code_table[i]->description);
		}
      prt_log ("\n");
		Terminate (n_errors);
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

char  Generate::GET_OPER (char* p, int x)
{
		int n  = 0;
		skel   = p;
		p      = NULL;
      n_out  = 0;
      count  = 1;
      width  = 1;
		format[0] = 0;
      switch (*skel)
      {
			case '.':
				skel++;
				break;
         case '?':
         case '!':
	    		return *skel; // Return operator character.
			case ';':
				return *skel; // Return operator character.
			default:
				*(skel+1) = 0;
				PRT_ERR (skel, linenumb);
				if (*skel == '\n') skel = "\\n";
				else if (*skel == '\t') skel = "\\t";
				else if (*skel ==  26 ) skel = "\\z";
				prt_log ("%s(%04d) : Error at '%s', expecting '.', '?', '!', or ';'\n\n", skl_fid, linenumb, skel);
				Terminate (n_errors);
		}
Top:  switch (*skel)
      {
         case '\0':
         case '\t':
         case ' ':
				skel++;
				goto Top;

         case '0':
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '8':
         case '9':
				p = skel;
				n = GETN (skel);
				goto Top;

         case 'e':			// First part of .eq. ?
         case 'g':			// First part of .ge. ?
         case 'l':			// First part of .lt. ?
         case 'n':			// First part of .ne. ?
				skel--;			// point at '.'
				return *skel;	// return '.'

         case 'd':
         case 's':
         case 't':
				if (code_table[x]->operators[0] == *skel || code_table[x]->operators[1] == *skel);
				else
				{
					PRT_ERR (skel, linenumb);
					prt_log ("%s(%04d) : Operator '%c' not valid for this skeleton name\n\n", skl_fid, linenumb, *skel);
					Terminate (n_errors);
				}
				if (*(skel+1) != ';')
				{
					PRT_ERR (skel+1, linenumb);
					prt_log ("%s(%04d) : Expecting ';' instead of '%c'\n\n", skl_fid, linenumb, *(skel+1));
					Terminate (n_errors);
				}
				width = n;
				if (p != NULL)
				{
					int i = 1;
					format[0] = '%';
					while (p <= skel && i < sizeof(format)-1) format[i++] = *p++;
					format[i] = 0;
				}
         	count = n;
				return *skel; // Return operator character.

         case '|':
         	count = n;
				return *skel; // Return operator character.

			case ';':
				return *skel; // Return operator character.

         case '?':
         case '!':
		      *(skel+1) = 0;
				PRT_ERR (skel-1, linenumb);
				prt_log ("%s(%04d) : Error at '%s', expecting '?' or '!'\n\n",
				skl_fid, linenumb, skel-1);
				Terminate (n_errors);

			default:
		      *(skel+1) = 0;
				PRT_ERR (skel, linenumb);
				if (*skel == '\n') skel = "\\n";
				prt_log ("%s(%04d) : Error at '%s', expecting 'd', 's', 't', '|', 'eq.', 'ge.', 'lt.', ';', or <integer>\n\n",
				skl_fid, linenumb, skel);
				Terminate (n_errors);
      }
		return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void	Generate::GET_STRINGS (char *p)
{
		skel = p;
		middle_l = GET_STRING (middle, sizeof(middle));
		sep_l    = GET_STRING (sep,    sizeof(sep));
		end_l    = GET_STRING (end,    sizeof(end));
		if (*skel != ';')
		{
			PRT_ERR (skel, linenumb);
			prt_log ("%s(%04d) : Characters found after 3rd argument, expecting ';' instead.\n\n", skl_fid, linenumb);
			Terminate (n_errors);
		}
		skel++;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::READ_VARS (char* p)
{
		skel = p;
		if (GET_STR(str_char))								// 1
		{
			if (GET_STR(str_uchar))							// 2
			{
				if (GET_STR(str_short))						// 3
				{
					if (GET_STR(str_ushort))				// 4
					{
						if (GET_STR(str_int))				// 5
						{
							if (GET_STR(str_uint))			// 6
							{
								if (GET_STR(str_charp))		// 7
								{
									while (*skel++ != '\n');
									linenumb++;
								}
							}
						}
					}
				}
			}
		}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

bool Generate::GET_STR(char* string)
{
		int i;
		char *p, *q;
		for (p = skel; *p != '"' && *p != '\n' && *p != ')'; p++);
		if (*p == ')' )
		{
			skel = p+1;
			while (*skel++ != '\n');
			linenumb++;
			return false;
		}
		if (*p == '\n')
		{
         PRT_ERR (skel, linenumb);
         prt_log ("%s(%04d) : End of line reached while looking for ')'\n\n", skl_fid, linenumb);
			Terminate (1);
		/*	skel = p+1;
			linenumb++; */
			return false;
		}
		p++;
		for (q = p; *q != '"' && *q != '\n'; q++);
		if (*q == '\n')
		{
         PRT_ERR (p-1, linenumb);
         prt_log ("%s(%04d) : String starting here has no ending '\"' on this line.\n\n", skl_fid, linenumb);
			Terminate (1);
		}
		i = 0;
		while (*p != '"')
		{
			string[i++] = *p++;
		}
		string[i] = 0;
		skel = q+1;
		return true;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   Generate::GET_NUM (int& num)
{
		char *p;
		int  n;
		for (p = skel; *p == ' ' || *p == '\t'; p++);
		if (*p == '\n') return (0);
		n = 0;
      while (isdigit (*p)) n = 10*n + *p++ - '0';
      num  = n;
		skel = p;
      return (1);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::DEF_TYPEA (int n) // address type (char*).
{
		g_size += num_charp*n;
		strcpy (string, str_charp);
}

void Generate::DEF_TYPEC (int n) // unsigned char.
{
		g_size += num_uchar*n;
		strcpy (string, str_uchar);
}

void Generate::DEF_TYPE (int *x, int n) // signed or unsigned type.
{
		DEF_T (x, n, 0, 0);
}

void Generate::DEF_TYPES (int *x, int n) // signed only
{
		DEF_T (x, n, -1, 0);
}

void  Generate::DEF_T (int *x, int n, int min, int max) // signed or unsigned type.
{
		int i;
		if (x == NULL)
		{
			strcpy (string, "undefined");
			return;
		}
		for (i = 0; i < n; i++)
		{
			if (x[i] > max) max = x[i];
			else if (x[i] < min) min = x[i];
		}
		if (min == 0)
		{
			if      (max <     256) { strcpy (string, str_uchar ); g_size += num_uchar  * n; }
			else if (max <   65536) { strcpy (string, str_ushort); g_size += num_ushort * n; }
			else                    { strcpy (string, str_int   ); g_size += num_int    * n; }
		}
		else if (-min < max) // -(-127) < 128
		{
			if      (max <     128) { strcpy (string, str_char  ); g_size += num_char   * n; }
			else if (max <   32768) { strcpy (string, str_short ); g_size += num_short  * n; }
			else                    { strcpy (string, str_int   ); g_size += num_int    * n; }
		}
		else
		{
			if      (min >=   -128) { strcpy (string, str_char  ); g_size += num_char   * n; }
			else if (min >= -32768) { strcpy (string, str_short ); g_size += num_short  * n; }
			else                    { strcpy (string, str_int   ); g_size += num_int    * n; }
		}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::SkipRestOfLineOrBlock (char *s) /* Skip rest of line, including '\n' OR skip whole block. */
{
      skel = s;
      if (strncmp (skel, "...", 3) == 0) // block start? then skip the whole block.
      {
			STAKCOND();
			skip_code = 1;
			SkipBlock ();
		}
	  	else
		{
			skel = skip_rest_of_line (skel);
		}
		return;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void	Generate::SkipBlock ()
{
		int lineno;
		lineno = linenumb;
		skel += 3;
Skip: while (*skel != '\n')
		{
			if (*skel == '"')
			{
				skel = skip_rest_of_string (skel+1);
			}
			else if (*skel == '\\' && *(skel+1) == '@')
			{
				skel += 2;
			}
			else if (*skel == '@')
			{
				if (*(skel+1) == '@')
				{
					UNSTAKCOND();
					while (*skel++ != '\n'); // skip rest of line.
					linenumb++;
					return;
				}
				else if (*(skel+1) == '(')	// 20120227 pbm
				{
					skel = skip_rest_of_line (skel+3);
				}
				else if (*(skel+1) == '/' && *(skel+2) == '/')
				{
					skel = skip_rest_of_line (skel+3);
				}
			  	else if (*(skel+1) == '/' && *(skel+2) == '*')
				{
					skel = skip_rest_of_comment (skel);
				}
				else if (alpha[*(skel+1)])
				{
					skel = skip_rest_of_code (skel+1);
				}
				else
				{
					PRT_ERR (skel, linenumb);
					prt_log ("%s(%04d) : '@' not followed by '(', '//', '/*', <name> or '@'\n\n", skl_fid, lineno);
					unlink (out_fid); // Delete output file.
					Terminate (1);
				}
			}
			else skel++;
		}
		skel++;
		linenumb++;
	//	printf ("in_group = %d\n", in_group);
		if (in_group)
		{
			if (skel >= skelend)
			{
				while (skel > group_start) if (*--skel == '\n') linenumb--;
				PRT_ERR (group_start, linenumb);
				prt_log ("%s(%04d) : End of file reached while looking for ending '@@' for this group.\n\n", skl_fid, lineno);
				unlink (out_fid); // Delete output file.
				Terminate (1);
			}
			while (*skel == ' ' || *skel == '\t') skel++;	// find first non-blank.
			goto Skip;
		}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

char*	Generate::skip_rest_of_string (char* skel)
{
		char *ls;
		ls = skel-1;
Scan:	while (*skel != '"' && *skel != '\n') skel++;
		if (*skel == '"')
		{
			if (*(skel-1) == '\\')
			{
				skel++;
				goto Scan;
			}
		}
		else if (*skel == '\n')
		{
			PRT_ERR (ls, linenumb);
			prt_log ("%s(%04d) : End of line found in string (not terminated with '\"').\n\n", skl_fid, linenumb);
			Terminate (n_errors);
		}
		return skel+1;
}

char*	Generate::skip_rest_of_code (char* sk)
{
		int  x;
		char c;
      sk = GETCODENUM (sk, x);	// If skeleton code name is bad, this terminates.
		c  = GET_OPER (sk, x);
		switch (c)
		{
			case '?':
			case '!':
			case '.':
			while (*skel != ';' && *skel != '\n') skel++;
			if (*skel == '\n')
			{
				PRT_ERR (sk, linenumb);
				prt_log ("%s(%04d) : No ending ';' found on skeleton expression.\n\n", skl_fid, linenumb);
				Terminate (n_errors);
			}
			skel++;
			if (strncmp (skel, "...", 3) == 0)
			{
				STAKCOND();
				skip_code = 1;
				SkipBlock();
			}
			else
			{
				skel = skip_rest_of_line (skel);
			}
			break;

			case 'd':
			case 's':
			case 't':
			skel++;
			break;

			case '|':
			GET_STRINGS(skel+1);
			break;

			case ';':
			if (code_table[x]->operators[0] == 's');
			else
			{
				PRT_ERR (skel, linenumb);
				prt_log ("%s(%04d) : Default string type not valid for skeleton name \"%s\"\n\n", skl_fid, linenumb, code_table[x]->keyword);
				Terminate (n_errors);
			}
			break;
		}
		return skel;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

char*	Generate::skip_rest_of_line (char* skel)
{
		while (*skel++ != '\n');
		linenumb++;
		return skel;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

char*	Generate::skip_rest_of_comment (char* skel)
{
		char *ls = skel;
		int   lineno = linenumb;
Scan:	while (*skel != '@' && skel < skelend)
		{
			if (*skel++ == '\n') linenumb++;
		}
		if (*skel == '@')
		{
			if (*(skel+1) == '*' && *(skel+2) == '/')
			{
				while (*skel++ != '\n');
				linenumb++;
			   return skel;
			}
			skel++;
			goto Scan;
		}
		PRT_ERR (ls, lineno);
		prt_log ("%s(%04d) : End of file found while looking for '@*/'\n\n", skl_fid, lineno);
		Terminate (n_errors);
		return skel; // Dummy return.
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::ERASE ()
{
      char *p;
      if (strncmp (++skel, "...", 3) == 0)
      {
			STAKCOND();
			skip_code = 0;
         while (*skel++ != '\n');
         linenumb++;
         skelbeg = skel;
      }
		else // Keep spacing (I guess?)
		{
			for (p = skelbeg; p < skel; p++) *p = ' ';
		}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   Generate::GET_STRING (char *string, int size)
{
		int i;
		char *p = skel;
		size--;
      for (i = 0; *skel != '|' && *skel != '\n' && *skel != ';'; skel++)
      {
         if (i >= size)
         {
            PRT_ERR (skel, linenumb);
            prt_log ("%s(%04d) : String field is longer than %d characters.\n\n", skl_fid, linenumb, size);
				Terminate (1);
         }
         if (*skel == '\\')
			{
				skel++;
				if      (*skel == 't' ) string [i++] = '\t';
				else if (*skel == 'n' ) string [i++] = '\n';
				else if (*skel == 'v' ) string [i++] = '\v';
				else if (*skel == 'f' ) string [i++] = '\f';
				else if (*skel == 'r' ) string [i++] = '\r';
				else                    string [i++] = *skel;
			}
         else if (*skel == '@')
			{
				if (strncmp (skel, "@grm;",       5) == 0        
				||  strncmp (skel, "@grm_name;", 10) == 0) 
				{
      			string [i++] = '%';
					string [i++] = 'g';
					string [i++] = '.';
					while (*skel != ';') skel++;
				}
				else string [i++] = *skel;
			}
         else string [i++] = *skel;
      }
      string [i] = 0;
		if (*skel == '\n')
		{
         PRT_ERR (p, linenumb);
         prt_log ("%s(%04d) : End of line reached while looking for '|'\n\n", skl_fid, linenumb);
			Terminate (1);
		}
		if (*skel == ';')
		{
         PRT_ERR (skel, linenumb);
         prt_log ("%s(%04d) : End of string reached while looking for '|'\n\n", skl_fid, linenumb);
			Terminate (1);
		}
		skel++;
      return (i);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int Generate::GETN(const char* p)
{
    int n;
    p = READNUM (p, &n);
    if (p == skel) {
        PRT_ERR(p, linenumb);
        prt_log("%s(%04d) : Non numeric value.\n\n", skl_fid, linenumb);
        Terminate(1);
        return 0;
    }
    skel = const_cast<char*>(p); // TODO
    return n;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

const char* Generate::READNUM(const char *p, int *num)
{
      int n = 0;
      if (isdigit (*p))
      {
         n = *p++ - '0';
         if (isdigit (*p))
         {
            n = 10 * n + *p++ - '0';
            if (isdigit (*p))
            {
               n = 10 * n + *p++ - '0';
            }
         }
      }
      *num = n;
      return p;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::EMIT_NUM (int num)
{
		if (*format == 0) strcpy (format, "%d");
      SPRINT (format, 0, &num, &num);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   Generate::FIRSTNB (char *p) /* Check to see if p points at first nonblank char. */
{
      while (*--p != '\n')
      {
         if (*p != ' ' && *p != '\t') return (0);  // No, so don't change skel.
      }
      skel = p + 1; /* Point at beginning of line. */
      return (1);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

char* Generate::GETFILESPEC ()
{
		char* filespec;
      char* p = ++skel;	// point at character following the '
		filespec = p;     // get start of file specification
      while (*p != '\'' && *p != '\n') p++;
		if (*p == '\n')
		{
         PRT_ERR (filespec-1, linenumb);
         prt_log ("%s(%04d) : End of line reached while looking for ending single quote (').\n\n", skl_fid, linenumb);
			Terminate (1);
	      return (NULL);
		}
		*p = 0; // mark end of filespec
		skel = p+1;
		return (filespec);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::O_ARRAY (int* x, int n)
{
      int i, y [2];
		if (x == NULL) return;

      y [0] = 0;
      y [1] = 0;
      if (n-- > 0)
      {
         for (i = 0;; i++)
         {
            y [0] = x [i];
            if (i == n) { O_DATA (i, y, (char*)y, NULL); return; }
            O_DATA (i, y, (char*)y, end);
         }
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::O_TERM_CONS (int* x, int n)
{
      int i, y [2];
		if (x == NULL) return;

      y [0] = 0;
      y [1] = 0;
      if (n-- > 0)
      {
         for (i = 0;; i++)
         {
            y [0] = x [i];
            if (i == n) { O_DATA (i, y, (char*)y, NULL); return; }
            O_DATA (i, y, (char*)y, end);
         }
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::O_ARRAY_CHAR (char* x, int n)
{
      int i, y [2];
		if (x == NULL) return;

      y [0] = 0;
      y [1] = 0;
      if (n-- > 0)
      {
         for (i = 0;; i++)
         {
            y [0] = (uchar)x [i];
            if (i == n) { O_DATA (i, y, (char*)y, NULL); return; }
            O_DATA (i, y, (char*)y, end);
         }
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::O_NUMTXT (int *x, char **s, int n)
{
      char *t;
      int i, y[2];
		if (x == NULL) return;

		y [0] = 0;
      y [1] = 0;
      if (n-- > 0)
      {
         for (i = 0;; i++)
         {
            t = s[i];
            y [0] = x[i];
            if (i == n) { O_DATA (i, y, t, NULL); return; }
            O_DATA (i, y, t, end);
         }
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::O_DEFCON (int *x, char **s, int n)
{
      char *t;
      int i, y[2];
		if (x == NULL) return;

		y [0] = 0;
      y [1] = 0;
      if (n-- > 0)
      {
         for (i = 0; ; i++)
         {
            t = s[i];
            y[0] = x[i];
            if (i == n) { O_DATA (i, y, t, NULL); return; }
            O_DATA (i, y, t, end);
         }
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::O_NUMTXT2 (int *x, char **s, int n)
{
      char *t;
      int i, y [2];
		if (x == NULL) return;

		y [0] = 0;
      y [1] = 0;
      if (n-- > 0)
      {
         for (i = 0;; i++)
         {
            if (x[i] >  0) t = s[x[i]]; // Different here!
            else if (x[i] == 0) t = "0";
            else           t = "-1";
            y[0] = x[i];
            if (i == n) { O_DATA (i, y, t, NULL); return; }
            O_DATA (i, y, t, end);
         }
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::O_TERM (char **s, int n) // Terminals.
{
      char *t, *p, ch;
      int  i, x, y[2];
		if (s == NULL) return;

      y[0] = 0;
      y[1] = 0;
      if (n-- > 0)
      {
         for (i = 0; i <= n; i++)
         {
            y[0] = i;
            t = s[i];
            if (*t == '\'' || *t == '\"') // If quoted string.
				{
					ch = *t;
					x  = 0;
					string[x++] = '\\';
					string[x++] = ch;
					for (p = t+1; *(p+1) != 0; p++)
					{
						string[x++] = *p;
					}
					string[x++] = '\\';
					string[x++] = ch;
					string[x++] = 0;
		         if (i < n) O_DATA (i, y, string, end);
	            else       O_DATA (i, y, string, NULL);
				}
				else
				{
		         if (i < n) O_DATA (i, y, t, end);
	            else       O_DATA (i, y, t, NULL);
				}
         }
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::O_HEAD (char **s, int n) // Nonterminals.
{
      char *t;
      int  i, y[2];
		if (s == NULL) return;

      y[0] = 0;
      y[1] = 0;
      if (n-- > 0)
      {
         for (i = 0; i <= n; i++)
         {
            y[0] = i;
            t = s[i];
	         if (i < n) O_DATA (i, y, t, end);
            else       O_DATA (i, y, t, NULL);
         }
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::O_ONLY (char **s, int n, char ch)
{
      char *t, *p, cz;
      int i, y[2], last, k = 0;

      y[0] = 0;
      y[1] = 0;
      last = N_ONLY (s, n, ch);
      if (last == 0) return;

      for (i = 0; i < n; i++)
      {
         if (s[i][0] == ch)
         {
            y[0] = i;
            t = s[i] + 1;					// Skip '<' or '{'
				p = t + strlen(t) - 1;		// Find '>' or '}'
				cz = *p;							// Save '>' or '}'
				*p = 0;
            if (++k < last) O_DATA (i, y, t, end);
            else            O_DATA (i, y, t, NULL);
				*p = cz;							// Restore '>' or '}'
         }
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::O_ONLY2 (char **s, int n, char ch) // Output only '...' without alpha chars.
{
      char *t, c;
      int i, x, y[2], last, k = 0;
		if (s == NULL) return;

      y[0] = 0;
      y[1] = 0;
      last = N_ONLY2 (s, n, ch);
      if (last == 0) return;

      for (i = 0; i < n; i++)
      {
         if (s[i][0] == ch)
         {
            char* p = s[i];
            while (*++p != 0)
            {
               if (*p >= 'a' && *p <= 'z') goto Next;
               if (*p >= 'A' && *p <= 'Z') goto Next;
            }
            y[0] = i;
            t = s[i];
            if (*t == '\'' || *t == '\"') // If quoted string.
				{
					x = 0;
					c = *t;
					string[x++] = '\\';
					string[x++] = c;
					for (p = t+1; *(p+1) != 0; p++)
					{
						string[x++] = *p;
					}
					string[x++] = '\\';
					string[x++] = c;
					string[x++] = 0;
					if (++k < last) O_DATA (i, y, string, end);
					else            O_DATA (i, y, string, NULL);
				}
				else
				{
					if (++k < last) O_DATA (i, y, t, end);
					else            O_DATA (i, y, t, NULL);
				}
         }
Next:    continue;
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int	Generate::N_ONLY (char **s, int n, char ch)
{
      int i, j;
		if (s == NULL) return 0;
      for (i = j = 1; i < n; i++) // From 1, skip <error>.
      {
         if (s[i][0] == ch) j++;
      }
      return (j);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int	Generate::N_ONLY2 (char **s, int n, char ch) // Output only '...' without alpha chars.
{
      int i, j;
		if (s == NULL) return 0;
      for (i = j = 0; i < n; i++)
      {
         if (s[i][0] == ch)
         {
            char* p = s[i];
            while (*++p != 0)
            {
               if (*p >= 'a' && *p <= 'z') goto Next;
               if (*p >= 'A' && *p <= 'Z') goto Next;
            }
            j++;
         }
Next:    continue;
      }
      return (j);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::O_KEYWRD (char **s, int n)
{
      char *t, c, ch;
      int i, k, y[2], last, len;
		if (s == NULL) return;

      k    = 0;
      y[0] = 0;
      y[1] = 0;
      last = N_KEYWRD (s, n);
      if (last == 0) return;
      for (i = 0; i < n; i++)
      {
         t = s[i];
         c = s[i][0];
			if (c == '\'' || c == '\"') // First letter is quote ?
			{
			// Check 2nd letter for alpha character.
	         ch = s[i][1];
				if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
				{
					t++;
					len = strlen(t)-1;
					*(t+len) = 0;
					y[0] = i;
					if (++k < last) O_DATA (i, y, t, end);
					else O_DATA (i, y, t, NULL);
					*(t+len) = c;
				}
			}
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   Generate::N_KEYWRD (char **s, int n) // Number of keywords.
{
      int  i, j;
      char c, ch;
		if (s == NULL) return 0;

      for (i = j = 0; i < n; i++)
      {
         c = s[i][0]; // Get 1st letter.
			if (c == '\'' || c == '\"') // First letter is quote ?
			{
				ch = s[i][1]; // Get 2nd letter.
				if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) j++;
			}
      }
      return (j);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::O_TXT (char **s, int n)
{
      char *t;
      int i, y[2];
		if (s == NULL) return;

      y[0] = 0;
      y[1] = 0;
      if (n-- > 0)
      {
         for (i = 0;; i++)
         {
            t = s[i];
            y[0] = i;
            if (i == n) { O_DATA (i, y, t, NULL); return; }
            O_DATA (i, y, t, end);
         }
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::O_TXTN (char **s, int m, int n)
{
      char *t;
      int i, y[2];
		if (n ==    0) return;
		if (s == NULL) return;

      y[0] = 0;
      y[1] = 0;
      if (n-- > 0)
      {
         for (i = m;; i++)
         {
            t = s[i];
            y[0] = i;
            if (i == n) { O_DATA (i, y, t, NULL); return; }
            O_DATA (i, y, t, end);
         }
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::O_TXTSTR (char **s, int n)
{
      char *t;
      int i, y[2], x;
		if (n == 0) return;
		if (s == NULL) return;

      y[0] = 0;
      y[1] = 0;
      if (n-- > 0)
      {
         for (i = 0;; i++)
         {
            t = s[i]+1;
				x = strlen(s[i])-1;
				s[i][x] = 0;
            y[0] = i;
            if (i == n) 
				{ 
					O_DATA (i, y, t, NULL); 
					s[i][x] = '"';
					return; 
				}
            O_DATA (i, y, t, end);
				s[i][x] = '"';
         }
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::O_SORTED (char **s, int n, int *seq)
{
      char *t;
      int i, y[2], j;
		if (s == NULL) return;

      y[0] = 0;
      y[1] = 0;
      if (n-- > 0)
      {
         for (i = 0;; i++)
         {
            y[0] = i;
            j = seq[i];
            t = s[j];
            if (i == n) { O_DATA (j, y, t, NULL); return; }
            O_DATA (j, y, t, end);
         }
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::O_SORTED2(int *s, int n, int *seq)
{
      int i, y[2], j;
		if (s == NULL) return;

      y[0] = 0;
      y[1] = 0;
      if (n-- > 0)
      {
         for (i = 0;; i++)
         {
            j = seq[i];
            y[0] = s[j];
            if (i == n) { O_DATA (i, y, (char*)y, NULL); return; }
            O_DATA (i, y, (char*)y, end);
         }
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//		Output node action for each node. 

void  Generate::O_NA1 (char** s, int* npp, int n, int *seq)
{
      char *t;
      int i, y[2], j;

      y[0] = 0;
      y[1] = 0;
      if (n-- > 0)
      {
         for (i = 0;; i++)
         {
            y[0] = i;
            j = seq[i];
            if (npp[j] == -1) t = "null";
            else t = s[npp[j]];
            if (i == n) { O_DATA (i, y, t, NULL); return; }
            O_DATA (i, y, t, end);
         }
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::O_DATA (int i, int *data, char *text, char *str)
{
      char *p, *q;
      p = middle;
      if ((q = strchr (p, '%')) != NULL)
      {
Loop:    if ((q = strchr (++q, '%')) != NULL)
         {
            *q = 0;
            SPRINT (p, i, data, (int*)text);
            *q = '%';
            p = q;
            goto Loop;
         }
      }
      SPRINT (p, i, data, (int*)text);
      ISEOL (str);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::ISEOL (char *str)
{
      if (str != NULL)
      {
         if (++n_out < count)
         {
            memcpy (buffptr, sep, sep_l);
            buffptr += sep_l;
            last_sep_l = sep_l;
            if (buffptr > buffend) DUMP_BUFFER ();
         }
         else
         {
            n_out = 0;
            memcpy (buffptr, end, end_l);
            buffptr += end_l;
            last_sep_l = end_l;
            if (buffptr > buffend) DUMP_BUFFER ();
         }
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::PUT_FILE (char *buffer, int leng) // If leng maybe > 32767.
{
      if (leng > 0)
      {
         while (leng > 32000)
         {
            OUTPUT (buffer, 32000);
            leng -= 32000;
            buffer += 32000;
         }
         OUTPUT (buffer, leng);
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   Generate::OUTPUT (char* buffer, int leng) // If leng <= 32767.
{
		#ifdef DFASTAR
		if (optn[LG_DIRECTCODE])
		{
			char ch = buffer[leng];
			buffer[leng] = 0;
			prt_code ("%s", buffer);
			buffer[leng] = ch;
		}
		else
		#endif
		{
			if (write (fd, buffer, leng) != leng)
			{
				if (++n_errors == 1) prt_log ("\n");
				prt_log ("\nError writing to file: '%s'\n\n", out_fid);
				Terminate (1);
			}
		}
		for (char* p = buffer; p < buffer+leng; p++)
		{
			if (*p == '\n') n_origlines++;
		}
		return (leng);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void Generate::PRT_ERR(const char *p, int line_num)
{
    if (++n_errors == 1) prt_log ("\n");
    const char* s = p;
    while (*--s != '\n'); // Back up to beginning of line.
    s++;
    prt_line(line_num, s);
    prt_pointer(line_num, s, p);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::DUMP_BUFFER ()
{
      int nb = buffptr - buffer;
		if (nb > 0)
		{
			bytes_out += nb;
			OUTPUT (buffer, nb);
			buffptr = buffer;
		}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  Generate::DUMP_SKEL (char* start, char* end, char* newstart)
{
      int nb;
		DUMP_BUFFER();
		nb = end - start;
		if (nb > 0)
		{
			bytes_out += nb;
			OUTPUT (start, nb);
		}
		skelbeg = skel = newstart;
		return;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int Generate::SPRINT(const char* format, int indx, int* data, int* text)
{
    int   n, leng;
    char *o, s[16];

      o = buffptr;
      for (const char* p = format; *p != 0;)
      {
         if (*p == '%')
         {
            p = READNUM (++p, &n);
            if (n > 80) n = 80;
            switch (*p++)
            {
					case 'd': /* Decimal integer */
					sprintf (s, "%d\0", data[0]);
               data = (int*)s;
               leng = strlen ((char*)data);
               if (n <= leng) // no spaces?
               {
                  memcpy (buffptr, (char*)data, leng);
                  buffptr += leng;
               }
               else if (n > leng)  // spaces before number.
               {
                  memset (buffptr, ' ', n-leng);
                  memcpy (buffptr+n-leng, data, leng);
                  buffptr += n;
               }
               break;

					case 'i': /* Index value */
					sprintf (s, "%d\0", indx);
               data = (int*)s;
					leng = strlen ((char*)data);
               if (n <= leng) // no spaces?
               {
                  memcpy (buffptr, (char*)data, leng);
                  buffptr += leng;
               }
               else if (n > leng)  // spaces before number.
               {
                  memset (buffptr, ' ', n-leng);
                  memcpy (buffptr+n-leng, data, leng);
                  buffptr += n;
               }
               break;

               case 'g': // Grammar name. 
					if (*p == '.') p++; // Skip over '.' character.
               leng = strlen ((char*)gfn);
               if (n <= leng) // no spaces?
               {
                  memcpy (buffptr, (char*)gfn, leng);
                  buffptr += leng;
               }
               else if (n > leng)  // spaces after string.
               {
                  memcpy (buffptr, (char*)gfn, leng);
                  memset (buffptr+leng, ' ', n-leng);
                  buffptr += n;
               }
               break;
  
               case 'l': /* String translate to lower. */
                  strcpy (string,(char*)text);
                  mystrlwr (string);
                  text = (int*)string;
                  goto String;

               case 'u': /* String translate to upper. */
                  strcpy (string,(char*)text);
                  mystrupr (string);
                  text = (int*)string;
                  goto String;

               case ';': /* String */
               case 's': /* String */
               case 't': /* Type   */
String:        leng = strlen ((char*)text);
               if (n <= leng) // no spaces?
               {
                  memcpy (buffptr, (char*)text, leng);
                  buffptr += leng;
               }
               else if (n > leng)  // spaces after string.
               {
                  memcpy (buffptr, (char*)text, leng);
                  memset (buffptr+leng, ' ', n-leng);
                  buffptr += n;
               }
               break;

/*             case 'p': // Production.
					if (indx < N_prods)
					{
						strcpy (string, head_name [head_sym [indx]]);
						strcat (string, " -> ");
						for (t = F_tail [indx]; t < F_tail [indx+1]; t++)
						{
							if (Tail[t] >= 0) sym = term_name[ Tail[t]];
							else              sym = head_name[-Tail[t]];
							strcat (string, sym);
							strcat (string, " ");
						}
						leng = strlen (string);
						if (n <= leng) // no spaces?
						{
							memcpy (buffptr, string, leng);
							buffptr += leng;
						}
						else if (n > leng)  // spaces after string.
						{
							memcpy (buffptr, string, leng);
							memset (buffptr+leng, ' ', n-leng);
							buffptr += n;
						}
					}
               break;
*/
/*             case 'T': // Token (special case) 
				   if (Defcon_term != NULL && Defcon_term[indx] != NULL)
					{
						leng = strlen (Defcon_term[indx]);
					// Patch here:
						if (Defcon_term[indx][0]      != '\''
						&&  Defcon_term[indx][leng-1] == '\'')
						{
							Defcon_term[indx]--;
							leng++;
						}
						if (n <= leng) // no spaces?
						{
							memcpy (buffptr, Defcon_term[indx], leng);
							buffptr += leng;
						}
						else if (n > leng)  // spaces after string.
						{
							memcpy (buffptr, Defcon_term[indx], leng);
							memset (buffptr+leng, ' ', n-leng);
							buffptr += n;
						}
					}
               break;
*/
            }
         }
         else *buffptr++ = *p++;
      }
	   if (buffptr > buffend) DUMP_BUFFER ();
      return (buffptr - o);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

const char* Generate::prt_line(int numb, const char* line)
{
    std::string to_print;
    to_print.reserve(256);
    const char* p = line;
    for (; *p != 26 && *p != '\n'; ++p) {
        if (*p == '\t')
            to_print.push_back(' ');
        else
            to_print.push_back(*p);
    }
    if (p != line)
        prt_log("%s(%04d) : %s\n", skl_fid, numb, to_print.c_str());
    return p + 1;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void Generate::prt_pointer(int numb, const char *line, const char *object) /* Print pointer (-------------^) */
{
    int col = object - line;
    char string[256];
    memset(string, '-', col);
    string[col] = 0;
    prt_log("%s(%04d) : %s^\n", skl_fid, numb, string);
}

#ifdef LRSTAR
void PGGenerate::emit(int i)
{
      switch (code_table[i]->number)
      {
         /* Terminal Symbol Data. */
			case TERM_SYMB: O_TERM (term_name,   N_terms); break;

         /* Nonterminal Symbol Data. */
         case HEAD_SYMB: O_HEAD (head_name,   N_heads); break;
         case HEAD_PROD: O_ARRAY (F_prod,     N_heads+1); break;

         /* Production Data. */
         case PROD_LENG: O_ARRAY (prod_len,   N_prods); break;
         case PROD_HEAD: O_ARRAY (head_sym,   N_prods); break;
         case PROD_TAIL: O_ARRAY (F_tail,     N_prods+1); break;  
         case PROD_REVS: O_ARRAY (Reverse,    N_prods); break;
         case TAIL_NUMB: O_ARRAY (Tail,       N_tails); break;

         /* Error Recovery Items. */
         case STA_ITEM:  O_ARRAY (f_eri,      n_states+1); break;
         case STA_PROD:  O_ARRAY (eri_prod,   n_eris); break;
         case STA_DOT:   O_ARRAY (eri_dot,    n_eris); break;

         /* Terminal Symbol Actions (=>). */
         case TACT_NUMB: O_ARRAY (Tact_numb,  N_terms); break;
         case TACT_FUNC: O_TXT   (Tact_start, N_tacts); break;
         case TACT_ARG:  O_ARRAY (F_targ,     N_targs); break;

         /* Production Stuff. */
         case PACT_NUMB: O_ARRAY (Pact_numb,  N_prods); break;
         case PACT_FUNC: O_TXT   (Pact_start, N_pacts); break;
         case PACT_ARG:  O_ARRAY (F_parg,     N_pargs); break;
         case NACT_ARG:  O_ARRAY (F_narg,     N_nargs); break;
         case NODE_NUMB: O_ARRAY (Node_numb,  N_prods); break;

         /* Action Arguments. */
         case ARG_NUMB:  O_ARRAY  (Arg_numb,     N_args); break; 
         case ARG_TEXT:  O_TXTSTR (Str_start,    N_strings); break;
         case DEF_CONS:  O_NUMTXT (Defcon_value, Defcon_name, n_constants); break;

         /* Node Names & Processors. */
         case NODE_NAME: O_TXT   (Node_start,    N_nodes); break; 
         case NACT_NUMB: O_ARRAY (Nact_numb,     N_prods); break; 
         case NACT_FUNC: O_TXT   (Nact_start,    N_nacts); break; 

         /* Boolean Matrix. */
         case BMAT_NUMB: O_ARRAY_CHAR (B_matrix, B_size); break;
         case BMAT_MASK: O_ARRAY (B_mask,        N_terms); break;
         case BMAT_ROW:  O_ARRAY (B_row,         n_states); break;
         case BMAT_COL:  O_ARRAY (B_col,         N_terms); break;

         /* Terminal Matrix. */
         case TMAT_NUMB: O_ARRAY (T_matrix,   T_size); break;
         case TMAT_ROW:  O_ARRAY (T_row,      tt_states); break;
         case TMAT_COL:  O_ARRAY (T_col,      N_terms); break;

         /* N_matrix data. */
         case NMAT_NUMB: O_ARRAY (N_matrix,   N_size); break;
         case NMAT_ROW:  O_ARRAY (N_row,      ntt_states); break;
         case NMAT_COL:  O_ARRAY (N_col,      N_prods); break;
      
         /* R_matrix data. */
         case RMAT_NUMB: O_ARRAY (R_matrix,   R_size); break;
         case RMAT_ROW:  O_ARRAY (R_row,      n_states); break;
         case RMAT_COL:  O_ARRAY (R_col,      N_terms); break;

         /* Nondeterministic Stuff. */
			case ND_START:	 O_ARRAY (nd_start,   n_states+1); break;
			case ND_TERM:	 O_ARRAY (nd_term,    n_nditems); break;
			case ND_ACTION: O_ARRAY (nd_action,  n_nditems); break;

         /* State accessor symbol. */
         case STA_ACCS:  O_ARRAY (accessor,   n_states); break;
      }  
}
#endif

#ifdef DFASTAR
///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

void  LGGenerate::emit(int i)
{
      switch (code_table[i]->number)
      {
         /* Boolean Matrix. */
			case LG_BMAT_NUMB: O_ARRAY_CHAR (B_matrix, B_size); break;
         case LG_BMAT_ROW:  O_ARRAY (B_row, n_states); break;
         case LG_BMAT_COL:  O_ARRAY (B_col, n_terms); break;

         /* Terminal Matrix. */
         case LG_TMAT_NUMB: O_ARRAY (T_matrix, T_size); break;
         case LG_TMAT_ROW:  O_ARRAY (T_row, tt_states); break;
         case LG_TMAT_COL:  O_ARRAY (T_col, n_terms); break;

         /* Terminals returned from lexer. */
         case LG_TERM_NUMB:	O_NUMTXT2(D_red, ret_name, n_states); break;
         case LG_DEF_CONS:		O_NUMTXT (defcon_value, defcon_name, n_constants); break;
         case LG_STRINGS:		O_TXTN   (str_start, 1, n_strings); break;
         case LG_ACTION_CODE:  O_TXTN   (code_start, 1, n_actioncodes); break;
      }  
}
#endif

#ifdef LRSTAR
///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

void  PGGenerate::emitType(int i)
{
      switch (code_table[i]->number)
      {
         /* Terminal Symbol Data. */
         case TERM_SYMB: DEF_TYPEA (N_terms); break;

         /* Nonterminal Symbol Data. */
         case HEAD_SYMB: DEF_TYPEA (N_heads); break;
         case HEAD_PROD: DEF_TYPE  (F_prod, N_heads+1); break;

         /* Production Data. */
         case PROD_LENG: DEF_TYPES(prod_len, N_prods); break; // signed type
         case PROD_HEAD: DEF_TYPE (head_sym, N_prods); break;
         case PROD_TAIL: DEF_TYPE (F_tail, N_prods+1); break; 
         case PROD_REVS: DEF_TYPE (Reverse, N_prods); break;
         case TAIL_NUMB: DEF_TYPE (Tail, N_tails); break;

         /* Error Recovery Items. */
         case STA_ITEM:  DEF_TYPE (f_eri, n_states+1); break;
         case STA_PROD:  DEF_TYPE (eri_prod, n_eris); break;
         case STA_DOT:   DEF_TYPE (eri_dot, n_eris); break;

         /* Terminal Symbol Actions (=>). */
         case TACT_NUMB: DEF_TYPE  (Tact_numb, N_terms); break;
         case TACT_FUNC: DEF_TYPEA (N_terms); break;
         case TACT_ARG:  DEF_TYPE  (F_targ, N_terms); break;

         /* Production Actions. */
         case PACT_NUMB: DEF_TYPE  (Pact_numb, N_prods); break;
         case PACT_FUNC: DEF_TYPEA (N_prods); break;
         case PACT_ARG:  DEF_TYPE  (F_parg, N_prods); break;
			case NODE_NUMB: DEF_TYPE  (Node_numb, N_prods); break;

         /* Action Arguments. */
         case ARG_NUMB:  DEF_TYPE  (Arg_numb, N_args); break; 
         case ARG_TEXT:  DEF_TYPEA (N_strings); break;
         case DEF_CONS:  DEF_TYPE  (Defcon_value, n_constants); break;

         /* Node Names & Processors. */
         case NODE_NAME: DEF_TYPEA (N_nodes); break; 
         case NACT_NUMB: DEF_TYPE  (Nact_numb, N_prods); break; 
         case NACT_FUNC: DEF_TYPEA (N_nacts); break; 
         case NACT_ARG:  DEF_TYPE  (F_narg, N_prods); break;

         /* Boolean Matrix. */
         case BMAT_NUMB: DEF_TYPEC(B_size); break;
         case BMAT_MASK: DEF_TYPEC(N_terms); break;
         case BMAT_ROW:  DEF_TYPE (B_row, n_states);  break;
         case BMAT_COL:  DEF_TYPE (B_col, N_terms);   break;

         /* Terminal Matrix. */
         case TMAT_NUMB: DEF_TYPE (T_matrix, T_size); break;
         case TMAT_ROW:  DEF_TYPE (T_row, tt_states); break;
         case TMAT_COL:  DEF_TYPE (T_col, N_terms); break;

         /* N_matrix data. */
         case NMAT_NUMB: DEF_TYPE (N_matrix, N_size); break;
         case NMAT_ROW:  DEF_TYPE (N_row, ntt_states); break;
         case NMAT_COL:  DEF_TYPE (N_col, N_prods); break;
      
         /* R_matrix data. */
         case RMAT_NUMB: DEF_TYPE (R_matrix, R_size); break;
         case RMAT_ROW:  DEF_TYPE (R_row, n_states); break;
         case RMAT_COL:  DEF_TYPE (R_col, N_terms); break;

         /* Nondeterministic Stuff. */
			case ND_START:  DEF_TYPE (nd_start, n_states+1); break;
			case ND_TERM:	 DEF_TYPE (nd_term,  n_nditems); break;
			case ND_ACTION: DEF_TYPE (nd_action, n_nditems); break;

         /* State accessor symbol. */
         case STA_ACCS:  DEF_TYPE (accessor, n_states); break;

			/* Default (erroroneous code) */
			default: 
		      PRT_ERR (skel-2, linenumb);
			   prt_log ("%s(%04d) : 't' is an invalid operator for this Skeleton Code Name.\n", skl_fid, linenumb);
				Terminate (n_errors);
      }  
}
#endif

#ifdef DFASTAR
///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

void  LGGenerate::emitType(int i)
{
      switch (code_table[i]->number)
      {
         /* Boolean Matrix. */
         case LG_BMAT_NUMB: DEF_TYPEC(B_size); break;
         case LG_BMAT_ROW:  DEF_TYPE (B_row, n_states);  break;
         case LG_BMAT_COL:  DEF_TYPE (B_col, n_terms);   break;

         /* Terminal Matrix. */
         case LG_TMAT_NUMB: DEF_TYPE (T_matrix, T_size); break;
         case LG_TMAT_ROW:  DEF_TYPE (T_row, tt_states); break;
         case LG_TMAT_COL:  DEF_TYPE (T_col, n_terms); break;

         /* Terminals returned from lexer. */
         case LG_TERM_NUMB:	DEF_TYPE (D_red, n_states); break;
         case LG_DEF_CONS:		DEF_TYPEA (n_constants); break;
			case LG_STRINGS:		DEF_TYPEA (n_strings); break;
			case LG_ACTION_CODE:  DEF_TYPEA (n_actioncodes); break;

			/* Default (erroroneous code) */
			default: 
		      PRT_ERR (skel-2, linenumb);
			   prt_log ("%s(%04d) : 't' is an invalid operator for this Skeleton Code Name.\n", skl_fid, linenumb);
				Terminate (n_errors);
      }  
}
#endif

#ifdef LRSTAR
int PGGenerate::value(int i)
{
      int n;
      switch (code_table[i]->number)
      {
         /* Terminal Symbol Data. */
         case TERM_SYMB:	n = N_terms; break;

         /* Nonterminal Symbol Data. */
         case HEAD_SYMB:	n = N_heads; break;
         case HEAD_PROD:	n = N_heads+1; break;

         /* Production Data. */
         case PROD_LENG:	n = N_prods; break;
         case PROD_HEAD:	n = N_prods; break;
         case PROD_TAIL:	n = N_prods+1; break;
         case PROD_REVS:	n = N_prods; break;
         case TAIL_NUMB:	n = N_tails; break;

         /* Error Recovery Items. */
         case STA_ITEM:		n = n_states+1; break;
         case STA_PROD:		n = n_eris; break;
         case STA_DOT:		n = n_eris; break;

         /* Terminal Symbol Actions (=>). */
         case TACT_NUMB:	n = N_terms; break;
         case TACT_FUNC:	n = N_tacts; break;
         case TACT_ARG:		n = N_targs; break;

         /* Production Actions (=>). */
         case PACT_NUMB:	n = N_prods; break;
         case PACT_FUNC:	n = N_pacts; break;
         case PACT_ARG:		n = N_pargs; break;

         /* Nodes (+>). */
         case NODE_NUMB:	n = N_prods; break;
         case NODE_NAME:	n = N_nodes; break;				  

         /* Node Actions (*>). */
         case NACT_NUMB:	n = N_prods; break;
         case NACT_FUNC:	n = N_nacts; break;
         case NACT_ARG:		n = N_nargs; break;

         /* Action Arguments. */
         case ARG_NUMB:		n = N_args;      break;
         case ARG_TEXT:		n = N_strings;   break;
         case DEF_CONS:		n = n_constants; break;

         /* Boolean Matrix. */
         case BMAT_NUMB:	n = B_size; break;
         case BMAT_MASK:	n = N_terms; break;
         case BMAT_ROW:		n = n_states; break;
         case BMAT_COL:		n = N_terms; break;
                
         /* Terminal Matrix. */
         case TMAT_NUMB:	n = T_size; break;
         case TMAT_ROW:		n = tt_states; break;
         case TMAT_COL:		n = N_terms; break;

         /* N_matrix data. */
         case NMAT_NUMB:	n = N_size; break;
         case NMAT_ROW:		n = ntt_states; break;
         case NMAT_COL:		n = N_prods; break;

         /* R_matrix data. */
         case RMAT_NUMB:	n = R_size; break;
         case RMAT_ROW:		n = n_states; break;
         case RMAT_COL:		n = N_terms; break;

         /* Nondeterministic Stuff. */
			case ND_START:		n = n_states+1; break;
			case ND_TERM:		n = n_nditems; break;
			case ND_ACTION:	n = n_nditems; break;

         /* Accessors_list. */
         case STA_ACCS:		n = n_states; break;     

         /* Numbers of stuff. */
         case NUMB_STA:		n = n_states; break; 
         case NUMB_TERM:	n = N_terms; break;
         case NUMB_HEAD:	n = N_heads; break;
         case NUMB_PROD:	n = N_prods; break;
         case NUMB_TACT:	n = N_tacts; break;
         case NUMB_PACT:	n = N_pacts; break;
         case NUMB_NODE:	n = N_nodes; break;
         case NUMB_NACT:	n = N_nacts; break;
                
         /* Miscelaneous Values. */
         case KEY_NUMB:		n = keyword_sym; break;
         case EOL_NUMB:		n = eol_term; break;
         case EOF_NUMB:		n = eof_term; break;
         case ERR_USED:		n = error_sym_used; break;
         case RESTORE :		if (error_sym_used || optn[PG_EXPECTING]) n = 1; else n = 0; break;
         case ACCP_STA:		n = accept_state; break; 
         case MAKE_AST:		n = make_ast; break; 

         /* Options. */
			case OPTN_AST: 	n = optn[PG_ASTCONST];		break; 
			case OPTN_BM: 		n = optn[PG_BOOLMATRIX];	break; 
			case OPTN_CLR: 	n = optn[PG_CLR_PARSER];	break; 
         case OPTN_DEBUG:	n = optn[PG_DEBUG];			break; 
			case OPTN_DR: 	   n = optn[PG_DEFAULTRED];	break; 
         case OPTN_EXP:		n = optn[PG_EXPECTING];		break; 
			case OPTN_LR: 	   n = optn[PG_LR_PARSER];	   break; 
			case OPTN_NA: 	   n = optn[PG_NODEARGS];		break; 
			case OPTN_ND: 	   n = n_nditems; if (n > 0) n = 1; break;
         case OPTN_SRA:		n = optn[PG_SHIFTREDUCE];	break; 
			case OPTN_X: 	   n = optn[PG_OPTION_X];		break; 
			case OPTN_Y:  		n = optn[PG_OPTION_Y];		break; 
			case OPTN_Z: 		n = optn[PG_OPTION_Z];		break; 

			/* Default (erroroneous code) */
			default: 
		      PRT_ERR (skel-2, linenumb);
			   prt_log ("%s(%04d) : 'd' is an invalid operator for this Skeleton Code Name.\n", skl_fid, linenumb);
				Terminate (n_errors);
      }
      return (n);
}
#endif

#ifdef DFASTAR
int LGGenerate::value(int i)
{
      int n;
      switch (code_table[i]->number)
      {
         /* Boolean Matrix. */
         case LG_BMAT_NUMB: n = B_size; break;
         case LG_BMAT_ROW:  n = n_states; break;
         case LG_BMAT_COL:  n = n_terms; break;
                
         /* Terminal Matrix. */
         case LG_TMAT_NUMB: n = T_size; break;
         case LG_TMAT_ROW:  n = tt_states; break;
         case LG_TMAT_COL:  n = n_terms; break;

         /* Terminals returned from lexer. */
         case LG_TERM_NUMB:	n = n_states; break; 
         case LG_DEF_CONS:		n = n_constants; break; 
         case LG_STRINGS:		n = n_strings-1; break; 
         case LG_ACTION_CODE:  n = n_actioncodes-1; break; 

         /* Numbers of stuff. */
         case LG_NUMB_STA:  n = n_states; break; 
         case LG_NUMB_TERM: n = n_terms; break;
         case LG_ERR_TOKEN: n = err_ret_numb; break;
                
         /* Options. */
         case LG_OPTN_CODE:	n = optn[LG_DIRECTCODE]; break;
         case LG_OPTN_COL:		n = optn[LG_COLNUMB]; break; 
         case LG_OPTN_DEBUG:	n = optn[LG_DEBUG]; break; 
         case LG_OPTN_LINE:	n = optn[LG_LINENUMB]; break; 
         case LG_OPTN_SMALL:	n = optn[LG_TABL_SMALL]; break; 
         case LG_OPTN_MEDIUM:	n = optn[LG_TABL_MEDIUM]; break; 
         case LG_OPTN_LARGE:	n = optn[LG_TABL_LARGE]; break; 

			/* Default (erroroneous code) */
			default: 
		      PRT_ERR (skel-2, linenumb);
			   prt_log ("%s(%04d) : 'd' is an invalid operator for this Skeleton Code Name.\n", skl_fid, linenumb);
				Terminate (n_errors);
      }
      return (n);
}
#endif

#ifdef LRSTAR
void PGGenerate::emitStr(int i)
{
      static char charstring[256];
      charstring[0] = 0;
      switch (code_table[i]->number)
      {
         case GRM_NAME: // Grammar name. 
         {
            strcpy (charstring, gfn);
            break;
         }
         case GRM_FILE: // Grammar file name. 
         {
            strcpy (charstring, grmfid);
            break;
         }
         case GRM_TEXT: // Input grammar file.
         {
            break;
         }
         case SKL_FILE: // Skeleton file name. 
         {
            strcpy (charstring, skl_fid);
            break;
         }
         case OUT_FILE: // Output file name.
         {
            strcpy (charstring, out_fid);
            break;
         }
         case PROGRAM: // Program name.
         {
            strcpy (charstring, program);
            break;
         }
         case VERSION: // Version No.
         {
            strcpy (charstring, version);
            break;
         }
			default:
			{ 
		      PRT_ERR (skel, linenumb);
			   prt_log ("%s(%04d) : this is an invalid operator for this Skeleton Code Name.\n", skl_fid, linenumb);
				Terminate (n_errors);
            break;
			}
      }
		if (*format == 0) strcpy (format, "%s");
      SPRINT (format, 0, (int*)charstring, (int*)charstring);
}
#endif

#ifdef DFASTAR
///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

void LGGenerate::emitStr(int i)
{
      static char charstring[256];
      charstring[0] = 0;
      switch (code_table[i]->number)
      {
         case LG_GRM_NAME: // Grammar name. 
         {
            strcpy (charstring, gfn);
            break;
         }
         case LG_GRM_FILE: // Grammar file name. 
         {
            strcpy (charstring, grmfid);
            break;
         }
         case LG_GRM_TEXT: // Input grammar file.
         {
				char *p, *ls;
				ls = input_start+1;
				for (p = ls; p < input_end; p++)
				{
					if (*p == '\n') // EOL ?
					{
						PUT_FILE ("// ", 3);
						PUT_FILE (ls, p - ls);
						PUT_FILE ("\n", 1);
						ls = p+1;
					}
				}
            return;
         }
         case LG_SKL_FILE: // Skeleton file name. 
         {
            strcpy (charstring, skl_fid);
            break;
         }
         case LG_OUT_FILE: // Output file name.
         {
            strcpy (charstring, out_fid);
            break;
         }
         case LG_PROGRAM: // Program name.
         {
            strcpy (charstring, program);
            break;
         }
         case LG_VERSION: // Version
         {
            strcpy (charstring, version);
            break;
         }
         case LG_DIRECT_CODE: // Version
         {
            n_addedlines = OUTPUT_CODE ();
				return; // return not break!
         }
			default:
			{ 
		      PRT_ERR (skel, linenumb);
			   prt_log ("%s(%04d) : this is an invalid operator for this Skeleton Code Name.\n", skl_fid, linenumb);
				Terminate (n_errors);
            break;
			}
      }
		if (*format == 0) strcpy (format, "%s");
      SPRINT (format, 0, (int*)charstring, (int*)charstring);
}
#endif
