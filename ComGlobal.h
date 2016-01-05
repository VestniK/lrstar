
#pragma once

		#ifndef WINDOWS
  		#define UNIX
		#endif

      #ifdef MAIN
      #define EXTERN
      #else
      #define EXTERN extern
      #endif

/////////////////////////////////////////////////////////////////////////////////////////////////////
//																																	//
//		Includes

      #include "sys/types.h"
      #include "sys/stat.h"
      #include "fcntl.h"
      #include "ctype.h"
      #include "stdio.h"

      #include <stdlib.h>
      #include <string.h>
      #include <time.h>
      #include <stdarg.h>

      #ifdef WINDOWS
      #include "wtypes.h"
      #include <io.h>
      #include "malloc.h"
      #endif

      #ifdef UNIX
      #include <unistd.h>
      #endif

/////////////////////////////////////////////////////////////////////////////////////////////////////
//																																	//
//		Defines

		enum options 
		{
			#ifdef LRSTAR
         PG_ANALYZEONLY   ,
         PG_ASTCONST      ,
         PG_BACKSLASH     ,
			PG_BOOLMATRIX    ,
         PG_CLR_PARSER    ,
         PG_CONFLICTS     ,
         PG_CONFLICTTRACE ,
		   PG_DEBUG         ,
		   PG_DEFAULTRED	  ,
         PG_EOFINCLUDED   ,
         PG_ERRORCOUNT    ,
			PG_EXPECTING     ,
         PG_GRAMMAR       ,
         PG_HTML          ,
         PG_KEYWORDCHECK  ,
			PG_LALR_PARSER   ,
			PG_LR_PARSER     ,
         PG_MINIMIZE      ,
         PG_NODEARGS      ,
         PG_NONDETER      ,
         PG_OPTIMIZE      ,
			PG_OPTION_X		  ,
			PG_OPTION_Y		  ,
			PG_OPTION_Z		  ,
         PG_PARSETREE     ,
         PG_QUIET         ,
         PG_REDUCEONLY    ,
         PG_SHIFTREDUCE   ,
         PG_STATELIST     ,
         PG_SYMBOLS       ,
         PG_TAB           ,
         PG_TRANSITIONS   ,
         PG_VERBOSE       ,
         PG_WARNINGS      ,
			#endif

			#ifdef DFASTAR
			LG_ANALYZEONLY,    
			LG_BACKSLASH,      
			LG_BLANKGOTOS,
			LG_COLNUMB,      
			LG_CONFLICTS,      
			LG_DEBUG,          
			LG_DIRECTCODE,     
			LG_ERRORCOUNT,     
			LG_GRAMMAR,        
			LG_INSENSITIVE,
			LG_KEYWORDIDENT,    
			LG_LINENUMB,       
			LG_MINIMIZE,       
			LG_NUMBCHAR,       
			LG_OPTIMIZE,
			LG_QUIET,          
			LG_REDUCEONLY,     
			LG_REMOVEDUP,      
			LG_SORTACTIONS,
			LG_STATELIST,      
			LG_STATELISTOPT,      
			LG_TAB, 
			LG_TABLES,
			LG_TABL_SMALL,      
			LG_TABL_MEDIUM,
			LG_TABL_LARGE,         
			LG_TRANSITIONS,    
			LG_VERBOSE,        
			LG_WARNINGS,
			#endif

			MAX_SYM,
			MAX_PRO,
			MAX_TAIL,
			MAX_EBNF,
			MAX_STA,
			MAX_FIN,
			MAX_KER,
			MAX_NTT,
			MAX_TT,
			MAX_TTA,
			MAX_LA,
			MAX_LB,
			MAX_INC,
			MAX_CH,
			MAX_ND,

			N_OPTIONS
		};

      #ifdef WINDOWS
      #define read   _read
      #define write  _write
      #define open   _open
      #define close  _close
      #define chmod  _chmod
      #define unlink _unlink
      #else // WINDOWS
      #define strlwr mystrlwr
      #define strupr mystrupr
      #define _filelength filelength
      #endif

      #define cchar     const char
      #define uchar     unsigned char
      #define uint      unsigned int
      #define ushort    unsigned short
      #define ulong     unsigned long

      #define YES                   1 // Yes value.
      #define NO                    0 // No value.

		#undef  MAX_PATH
      #define MAX_PATH            260 // Maximum path length.
      #define MAX_DIR             160 // Maximum directory name length, 159.
      #define MAX_FILENAME         64 // Maximum file name length, 63.
      #define MAX_FILETYPE         32 // Maximum file type length, 31.

		#undef  UINT_MAX
		#define UINT_MAX     0xFFFFFFFF // 32-bit integer value for hasing.
		#define MAX_INT      2147483647 // Maximum integer.
      #define EOF_CHAR             26
      #define EOL_CHAR             10

      #define UPPERCASE		   1
      #define LOWERCASE       2
      #define UNDERSCORE	   4
      #define DIGIT			   8
      #define QUOTE		     16

   // Function-call defines ...
      #ifdef _DEBUG
		#define ALLOC(x,norg)				alloc (#x, (char*&)x, sizeof(*x), norg)
      #else
		#define ALLOC(x,norg)				alloc ((char*&)x, sizeof(*x), norg)
      #endif

      #define REALLOC(x,norg,nnew)		ralloc((char*&)x, sizeof(*x), norg, nnew)
      #define FREE(x,n)						frea  ((char*&)x, sizeof(*x), n)

      #define FASTCMP(a,b,n)  			fastcmp ((int*)a, (int*)b, n)
      #define FASTCPY(a,b,n)  			fastcpy ((int*)a, (int*)b, n)
      #define FASTINI(a,b,n)  			fastini (      a, (int*)b, n)
      #define FASTMRG(a,b,n)  			fastmrg ((int*)a, (int*)b, n)
      #define FASTOR(a,b,n)   			fastor  ((int*)a, (int*)b, n)

	// USINGBITS (and the following defines) uses less memory 
	//	but runs about the same speed, better for huge grammars. 
		#define USINGBITS						
      #define ifbit1(matrix,x)      	if (matrix[x>>3]    & (0x80>>(x&0x0007)))
      #define ifbit2(matrix,x,y)    	if (matrix[x][y>>3] & (0x80>>(y&0x0007)))
      #define setbit1(matrix,x)			matrix[x>>3]       |= (0x80>>(x&0x0007))
      #define setbit2(matrix,x,y)    	matrix[x][y>>3]    |= (0x80>>(y&0x0007))
	  
/////////////////////////////////////////////////////////////////////////////////////////////////////
//																																	//
//		Typedefs

		struct OPTION
		{
			const char* name;
			const char* desc;
			int   numb;
			int   defvalue;
		};

      typedef struct	ITEM
      {
         int symb;
         int prod;
         int dot;
      }
      ITEM;

      typedef struct	CLOSURE
      {
         int item;
         int next;
      }
      CLOSURE;

      typedef struct	SYMLIST
      {
         int symb;
         int start;
         int end;
      }
      SYMLIST;

      typedef struct	CHILD
      {
         int numb;
         int link;
      }
      CHILD;

		typedef struct	CODETABLE
		{
			char* keyword;
			char* operators;
			int   number;
			char* description;
		}
		CODETABLE;

		#define MAXTOP 10
		typedef struct	STAKTYPE
		{
			char*	groupstart;
			int	skipcode;
		}
		STAKTYPE;

/////////////////////////////////////////////////////////////////////////////////////////////////////
//																																	//
//		Classes

		class Token
		{
			public:
			char* start;	   // Start of symbol.
			char* end;		   // End of symbol.
			int   sti;			// Symbol table index.
			int   line;	      // Input line number.
			int   column;		// Input column number.
		};

		class Symbol             
		{
		   public:
			char*  name;	 // Pointer to symbol name (allocated).   							      4	 4	
			char*  start;	 // Pointer to symbol start address (in source code).					   4	 8	
			int    length;	 // Length  of symbol.		      												2  12
         int    type;	 // Type (e.g. integer, float, double, char, ...)							2  16
			int    line;	 // Line of first encounter in grammar.	                           4  20
			int    numb;	 // Number assigned to symbol (sequence number).		               4  24 
			int    value;	 // Value of symbol (for constants and integers).		               4  28 
		};

      class Symtab 
		{
		   public:
			static Symbol*  symbol;			   // Symbols.
			static int      n_symbols;		   // Number of symbols.
			static short    length;			   // Current length of symbol name.
			static int      cell;			   // Current hash cell number.
			static int      sti;				   // Current symbol-table index.
			static uint     hashdiv;		   // Hash divisor. 
			static int*     hashvec;		   // Hash vector.
			static int      max_cells;       // Maximum number of cells in the hash vector = 2*max_symbols. 
			static int      max_symbols;	   // Maximum number of symbols.

      // Functions ...
			static void     init_symtab  (int);	                // Initialize the symbol table.
			static void     term_symtab  ();		                // Terminate the symbol table.
			static char*    symname      (int);                 // Get symbol name.
			static int      add_symbol   (short, char*, const char*, int, int); // Add symbol to symbol table.
			static int      get_symbol   (const char*, const char*, int);        // Get symbol number.
			static void     print_symtab (char**);              // Print the symbol table. 
		};

		class Node                
		{
		   public:
			int    id;		 // Node id number.	               				4	 4	
			int    numb;	 // Node number.					                  4	 8	
			int    prod;	 // Production number.					            4	12	
			int    sti;     // Symbol-table index (can be negative).   		4	16
			int    line;    // Line number.              		            4	20
         char*  start;   // Start of token in source line.              4  24
         char*  end;     // End of token in source line.                4  28
			Node*  next;	 // Next node.												4  32
			Node*  prev;	 // Previous node.			   						4  36
			Node*  child;   // Child node.                            		4  40
			Node*  parent;  // Parent node.                            		4  44
		}; 
  
      class PStack // Parser stack.
      {
         public:
         int    state;     // Parser state.                 4	 4	
         char*  start;     // Token start address.          4	 8	
         char*  end;       // Token end address.            4	12	
         int    line;      // Input line number.            4	16	
         int    sti;       // Symbol table index.           4	20	
			int    sym;       // Symbol stacked, terminal (positive) or nonterminal (negative). 
         Node*  node;      // Node pointer.                 4	24	
         Node*  last;      // Last pointer.                 4	28	bytes 
      };

      class RStack // Restore Stack.
      {
         public:
         PStack* ptr;		// Parse stack pointer.				4	 4	
         int     state;    // State.								4	 8	bytes.
      };

      class Stack
      {
         public:
         int   id;      // Node id.                      2	 2	
         int   counter; // Counter of node accesses.     2	 4	
      };

      class AST : public Symtab
		{
			public:	
			static char     indent[256];	   // Indentation for printing current node.
         static int*     counter;		   // Node counter array.
         static short    stacki;		      // AST stack index.
         static Stack*   stack;			   // AST stack array.
			static Node*    node;			   // AST node array.
			static Node*    root;			   // Root node pointer.
			static Node*    currnode;		   // Current node pointer.
			static int      n_nodes;		   // Number of nodes created.
			static int      n_nodenames;     // Number of node names.
			static int      n_nodeactions;	// Number of node actions.
			static short    pass;		      // Pass number when traversing the AST.
			static short    status;		      // Status (TOP_DOWN | PASS_OVER | BOTTOM_UP).
			static char*    node_name[];	   // Node names array.
			static char**   term_symb;	      // Terminal symbols.
			static int      nact_numb[];     // Node action numbers.
  		  	static char*    nact_name[];		// Node action names.

		// Functions ...
			static void     init_ast	 ();
			static void     term_ast	 ();
			static void     print_ast	 (char**);
			static void     print_ast	 (Node* np);
			static void     print_node  (char *indent, Node* np);
         static void     traverse	 ();
         static void     traverse	 (Node* np);
			static void     traverse	 (char *indent, Node* np);
			static short    emitstr	    (Node* np, char* str);
			static int    (*nact_func[])(Node*);	
		};

/////////////////////////////////////////////////////////////////////////////////////////////////////
//																																	//
//		Global Variables 

extern const char* program;
extern const char* version;

		extern uchar  charcode[256]; 
		extern uchar  numeric[256]; 
		extern uchar  alpha[256];
		extern uchar  upper[256];
		extern uchar  lower[256];

      EXTERN int    n_errors;
      EXTERN int    n_warnings;

      EXTERN char   confid [MAX_PATH];
      EXTERN char   dir [MAX_DIR];   
	   EXTERN char   exefid [MAX_PATH];
      EXTERN char   gdn [MAX_DIR];      
      EXTERN char   gfn [MAX_FILENAME]; 
      EXTERN char   gft [MAX_FILETYPE]; 
	   EXTERN char   grmfid [MAX_PATH];  
	   EXTERN char   logfid [MAX_PATH];
	   EXTERN char   lstfid [MAX_PATH];

      EXTERN int    optn[N_OPTIONS];
      EXTERN int    optncount[N_OPTIONS];
      EXTERN char   odn [MAX_DIR];
      EXTERN char   ofn [MAX_FILENAME];
      EXTERN char   oft [MAX_FILETYPE];
      EXTERN char   outfid [MAX_PATH];
      EXTERN char   sdn [MAX_DIR];
      EXTERN char   sfn [MAX_FILENAME];
      EXTERN char   sft [MAX_FILETYPE];
	   EXTERN char   sklfid [MAX_PATH];
      EXTERN char   stafid [MAX_PATH];

      EXTERN FILE*  logfp;
      EXTERN FILE*  errfp;
      EXTERN FILE*  lstfp;
      EXTERN FILE*  confp;
      EXTERN FILE*  grmfp;
      EXTERN FILE*  stafp;

      EXTERN int    time1;
      EXTERN int    time2;
		EXTERN char   string [10000];

      EXTERN int    filesize;          
		EXTERN int	  filedesc;

      EXTERN char*  input_start;			/* First byte of input area.              */
      EXTERN char*  input_end;			/* Byte after input.                      */
      EXTERN char*  lex_input_start;   /* First byte of input area.              */
      EXTERN char*  lex_input_end;     /* Byte after input.                      */

      EXTERN int    n_lines;

      EXTERN char   spaces [256] 
      #ifdef MAIN
      = "                                                                " //  64
        "                                                                "	// 128
        "                                                                "	// 192
        "                                                               "	// 255
      #endif
      ;
      extern OPTION PGOption[];
      extern OPTION LGOption[];
      extern OPTION MAOption[];

      EXTERN char** line_ptr;
      EXTERN char** lex_line_ptr;

		EXTERN int    memory_max;
		EXTERN int    memory_usage;
      EXTERN int    max_child;			// Option setting (rename this).
		EXTERN int    max_child_usage;	// Highest number found during processing.
      EXTERN int    max_lookah;
      EXTERN int    max_terml;
      EXTERN int    max_headl;
      EXTERN int    max_symbl;
      EXTERN int    max_errors;

		EXTERN int	  option_warnings;
		EXTERN int    option_grammar;

      EXTERN char*  lexer_input;
      EXTERN int    lexer_linenumb;

      EXTERN CHILD* child;
      EXTERN int*   f_child;
      EXTERN int    n_childs;

/////////////////////////////////////////////////////////////////////////////////////////////////////
//																																	//
//		Generate Output Variables

		EXTERN int    num_char;
		EXTERN int    num_uchar;
		EXTERN int    num_short;
		EXTERN int    num_ushort;
		EXTERN int    num_int;
		EXTERN int    num_uint;
		EXTERN int    num_charp;

		EXTERN char   str_char  [32];
		EXTERN char   str_uchar [32];
		EXTERN char   str_short [32];
		EXTERN char   str_ushort[32];
		EXTERN char   str_int   [32];
		EXTERN char   str_uint  [32];
		EXTERN char   str_charp [32];

		EXTERN int    parser_defined;
		EXTERN int    lexer_defined;

/////////////////////////////////////////////////////////////////////////////////////////////////////
//																																	//
//		Global Functions

		extern void   MTSL ();
		extern void   C_CAMEFROM (int n_states, int* tt_start, int* tt_action, int* ntt_start, int* ntt_action, int*& f_camefrom, int*& camefrom); 
		extern void   C_FIRST (int n_heads, int n_terms, int n_prods, int* f_prod, int* f_tail, int* tail, char**& FIRST, int* nullable, int* head_sym); 
		extern void   C_FOLLOW(int n_heads, int n_terms, int n_prods, int* f_prod, int* f_tail, int* tail, char**  FIRST, char**& FOLLOW, int* nullable, int* head_sym);
      extern void   C_FIRSTNT ();
		extern void   P_FIRST  (int n_heads, int n_terms, char** FOLLOW, char** term_name, char** head_name); 
		extern void   P_FOLLOW (int n_heads, int n_terms, char** FOLLOW, char** term_name, char** head_name); 

		extern int    close_con ();
		extern int    close_grm ();
		extern int    close_log ();
		extern int    close_sta ();
		extern int    close_lst ();

      #ifdef UNIX
		extern long   _filelength (int fd);
      #endif

      extern int    fastcmp (int*, int*, int);
      extern void   fastcpy (int*, int*, int);
      extern void   fastini (int, int *, int);
      extern int    fastmrg (int*, int*, int);
      extern void   fastor  (int*, int*, int);
      extern void   fix_backslash (char*in);
      extern void   frea (char*& x, int size, int n);

      extern int    get_fid (char*arg, char*dir, char*fn, char*ft);
		extern int 	  GetSkeletonFilename (char* arg, int i, int na);
		extern int 	  GetOutputFilename (char* arg, int i, int na);
		extern int    GetMaxValues  (const char* dn, const char* fn);
		extern void   SaveMaxValues (const char* dn, const char* fn);

		extern int	  itsakeyword (char* terminal);
      extern int    inputi (char*);
		extern void   InternalError (int n);

		extern void   MemCrash (char* msg);
		extern void   MemCrash (char* value, int n);
		 
		extern char*  mystrlwr (char* s);
		extern char*  mystrupr (char* s);
      extern void   memcrash (char*, int);

		extern void   number (int x, char* num);

    extern int open_log(const char*);
		extern int    open_grm (char*);
		extern int    open_con (char*);
		extern int    open_sta (char*);
		extern int    open_lst (char*);

      extern void   prt_error   (char* msg, char* start, char* end, int linenumb);
		extern void   prt_warning (char* msg, char* start, char* end, int linenumb);
		extern void   prt_message (char* msgtype, char* msg, char* start, char* end, int linenumb);
		extern void   prt_line_with_pointer (char* tokenstart, char* tokenend, int tokenlinenumb);
      extern void   prt_lst     (char*format,...);
    extern void prt_log(const char* format, ...);
    extern void prt_logonly(const char*format,...);
      extern int    prt_grm     (char*format,...);
      extern void   prt_con     (char*format,...);
      extern void   prt_sta     (char*format,...);
		extern void   PRT_OPTNS   (int na, char** arg);
      extern void   prt_path    (int itm, int state);
      extern void   prt_pointer (int numb, char*line, char*object);
      extern void   prt_prod    (int p, int dot);
    extern void prt_num(const char* desc, int n, const char* name, int max);
		extern int	  prt_sym     (int s, char *sp, char** term_name, char** head_name);


		extern void   quit (int);

      extern void   ralloc (char*& x, int size, int n1, int n2);

		extern void   SORT (int *start, int *end);
      extern void   SORT2 (int* first, int* second, int n);
      extern void   SORT3 (int* first, int* second, int* third, int n);
		extern void   SORTNUMBS (int* numb, int n, int* seq);
      extern int    symlen (int s, char** term_name, char** head_name);

		extern char*  strchr (char*p, char c);
		extern char*  strrchr (char* str, char c);
		extern int    SET_OPTN (char* opt, char* fid, int linenumb);
		extern int    set_optn (OPTION* option, char* opt, char* fid, int linenumb);
		extern int    SET_OPTNS (int na, char** arg);
		extern void   SORTNAMES (char** start, int n, int* seq);
		extern void   SORTNAMES2 (char** start, int n, int* seq, int* pos);

		extern void   TRAVERSE (int x);
		extern void   Terminate (int n);

		#ifdef _DEBUG
		extern char*  alloc (char *s, char*& x, int size, int n);
		#else
		extern char*  alloc (char*& x, int size, int n);
		#endif		  

		extern void   ralloc (char*& x, int size, int n1, int n2);
		extern void   frea  (char*& x, int size, int n);
		extern int    fastcmp (int *a, int *b, int n);
		extern void   fastcpy (int *a, int *b, int n);
		extern void   fastini (int v, int *b, int n);
		extern int    fastmrg (int *a, int *b, int n);
		extern void   fastor (int *a, int *b, int n);

		extern int    ATTACH   (int x, int y);
		extern int    ATTACHED (int x, int y);
		extern void   T_GRAPH  (char **graph, int nr, int nc);
		extern void   P_GRAPH  (char** graph, int nr, int nc);
		extern void   TRAVERSE (int x);
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
