
#ifdef DFASTAR

		#include "ComGlobal.h"
      #include "LGParser.h"
		#include "LGActions.h"

		#define STKSIZE        1000 // Parser stack size.
		#define ACCEPT_STATE	    48 // Final state.
      #define LOOKAHEADS        2

/*--- Parser Table Constants. -----------------------------------------------*/
                                                                                          
      static int n_terms	=    29; // No. of terminals.                               
      static int n_heads   =    40; // No. of non-terminals.     
      static int n_prods   =    92; // No. of productions.       
      static int n_states  =    49; // No. of states.
      static int eol_symb  =     0; // EOL symbol number            
      static int eof_symb  =     8; // EOF symbol number            
      static int err_used  =     0; // <error> used in grammar      

/*--- Terminal and Nonterminal Symbols of the Grammar. ----------------------*/

   // Terminal symbols of the grammar. 
      char* term_symb[29] = 
      {
         "<error>",
         "<alpha>",
         "<lexical>",
         "<ignore>",
         "<escape>",
         "<literal>",
         "<string>",
         "<integer>",
         "<eof>",
         "\'->\'",
         "\'=>\'",
         "\'(\'",
         "\')\'",
         "\'+\'",
         "\'*\'",
         "\'?\'",
         "\'..\'",
         "\'|\'",
         "\'-\'",
         "\'{\'",
         "\'}\'",
         "\';\'",
         "\'%%\'",
         "{first_alpha}",
         "{first_lexical}",
         "{first_ignore}",
         "{first_escape}",
         "{first_literal}",
         "{first_string}"
      };                                                 

   // Head symbols of the grammar. 
      char* head_symb[40] = 
      {
         "Goal",
         "Grammar",
         "Options",
         "PP",
         "TokenProd",
         "Token",
         "ReturnValue",
         "ActionCode",
         "NonterminalDef",
         "AlphaSymbol",
         "LexicalSymbol",
         "IgnoreSymbol",
         "ArrowProd",
         "Arrow",
         "BarProd",
         "Bar",
         "TailExprList",
         "TailExpr",
         "List",
         "Or",
         "GroupStart",
         "GroupEnd",
         "TailSymbol",
         "SetDef",
         "SetExpr",
         "Range",
         "RangeEnd",
         "RangeStart",
         "Character",
         "CharacterSetName",
         "EscapeDef",
         "EscapeSymbol",
         "Number",
         "TokenProd...",
         "(NonterminalDef | SetDef | EscapeDef)",
         "(NonterminalDef | SetDef | EscapeDef)...",
         "ArrowProd...",
         "[';']",
         "BarProd...",
         "TailExpr..."
      };

/*--- Production Information. -----------------------------------------------*/

   // Production length - 1 
      char PL[92] = 
      {
              1,     0,     1,     1,     2,     1,     2,     2,     3,     0,
              0,     1,     1,     0,     0,     0,     0,     0,     0,     0,
              2,     2,     2,     3,     3,     3,     0,     0,     0,     1,
              0,     1,     0,     0,     0,     1,     1,     1,     2,     0,
              1,     2,     0,     0,     0,     1,     1,     1,     0,     0,
              0,     0,     0,     0,     4,     5,     0,     0,     0,     2,
              2,     2,     2,     2,     2,     2,     0,     0,     0,     0,
              0,     0,     0,     2,     3,     0,     0,     0,     1,     0,
              0,     0,     0,     1,     0,     1,    -1,     0,     0,     1,
              0,     1
      };

   // Head symbol for a production 
      unsigned char head_numb[92] = 
      {
              0,     1,     1,     1,     1,     1,     1,     1,     1,     2,
              3,     4,     4,     5,     5,     5,     6,     6,     6,     7,
              8,     8,     8,     8,     8,     8,     9,    10,    11,    12,
             13,    14,    15,    16,    17,    17,    17,    17,    17,    18,
             18,    18,    19,    20,    21,    21,    21,    21,    22,    22,
             22,    22,    22,    22,    23,    23,    24,    24,    24,    24,
             24,    24,    24,    24,    24,    25,    26,    27,    28,    28,
             28,    29,    29,    30,    30,    31,    32,    33,    33,    34,
             34,    34,    35,    35,    36,    36,    37,    37,    38,    38,
             39,    39
      };

   // Index to first tail symbol for a production 
      unsigned char f_tail[93] = 
      {
              0,     2,     3,     5,     7,    10,    12,    15,    18,    22,
             23,    24,    26,    28,    29,    30,    31,    32,    33,    34,
             35,    38,    41,    44,    48,    52,    56,    57,    58,    59,
             61,    62,    64,    65,    66,    67,    69,    71,    73,    76,
             77,    79,    82,    83,    84,    85,    87,    89,    91,    92,
             93,    94,    95,    96,    97,   102,   108,   109,   110,   111,
            114,   117,   120,   123,   126,   129,   132,   133,   134,   135,
            136,   137,   138,   139,   142,   146,   147,   148,   149,   151,
            152,   153,   154,   155,   157,   158,   160,   160,   161,   162,
            164,   165,   167
      };

   // Tail symbol numbers (T: >= 0, NT: < 0)
      char tail[167] = 
      {
             -1,     8,   -33,   -33,    -3,   -33,   -35,   -33,    -3,   -35,
             -2,   -33,    -2,   -33,    -3,    -2,   -33,   -35,    -2,   -33,
             -3,   -35,    28,    22,    -5,    -6,    -5,    -7,    24,    27,
             28,     7,     1,     6,    19,    -9,   -36,   -37,   -10,   -36,
            -37,   -11,   -36,   -37,    -9,   -12,   -38,   -37,   -10,   -12,
            -38,   -37,   -11,   -12,   -38,   -37,    23,    24,    25,   -13,
            -16,     9,   -15,   -16,    17,   -39,   -22,   -22,    13,   -22,
             14,   -22,    15,   -20,   -18,   -21,   -17,   -18,   -17,   -18,
            -19,   -17,    17,    11,    12,    12,    13,    12,    14,    12,
             15,     1,     4,     2,     5,     6,     7,    -9,   -13,    19,
            -24,    20,    -9,   -13,    19,   -24,    20,    21,   -25,   -28,
            -29,   -24,    17,   -25,   -24,    17,   -28,   -24,    17,   -29,
            -24,    18,   -25,   -24,    18,   -28,   -24,    18,   -29,   -27,
             16,   -26,   -28,   -28,     5,     6,     7,     1,     4,   -31,
            -13,   -32,   -31,   -13,   -32,    21,    26,     7,    -4,   -33,
             -4,    -8,   -23,   -30,   -34,   -35,   -34,   -12,   -36,   -12,
             21,   -14,   -38,   -14,   -17,   -39,   -17
      };

/*--- token Action Variables ------------------------------------------------*/

   // token Action number. 
      char tact_numb[29] = 
      {
              0,     1,     1,     1,     1,     1,     1,    -1,    -1,     2,
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1
      };

   // token Action Addresses.
      int (*tact_func[3]) (int, int) = 
      {
         LGActions::error,
         LGActions::firstsym,
         LGActions::arrow
      };

   // token Action argument index. 
      char tact_arg[29] = 
      {
              0,     1,     2,     3,     4,     5,     6,    -1,    -1,    -1,
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1
      };

/*--- Bit Matrix and Access Vectors. -----------------------------------------*/

   // B_matrix.
      unsigned char Bm [44] =
      {
          0,    0,    0,   12,  128,    0,    0,   98,   64,    0,    0,   15,    0,    0,    6,    0,    1,    0,   16,    0,
        126,   64,    0,  129,    0,  126,    0,  144,    0,    4,    0,  122,    0,  128,    0,  126,   18,    0,   48,    0,
          8,  112,    0,    0
      };

   // B_matrix row.
      unsigned char Br [49] =
      {
          1,    4,    1,    7,    9,    0,    9,   12,   15,   15,   15,   15,   12,    0,   12,   12,   12,   17,   20,   22,
         17,   25,   22,   17,   22,    8,   12,   25,   26,   25,   28,   25,   31,   26,   26,   32,   35,   37,   39,    0,
         25,   28,   31,   31,   32,   41,    0,    0,    0
      };

   // B_matrix column.
      unsigned char Bc [29] =
      {
          0,    0,    0,    0,    0,    0,    0,    0,    0,    1,    0,    0,    1,    1,    1,    1,    1,    1,    1,    1,
          1,    1,    2,    2,    2,    2,    2,    2,    2
      };

   // B_matrix mask.
      unsigned char Bmask [29] =
      {
          1,    2,    4,    1,    8,   16,   32,   64,  128,    1,    1,    4,    2,    4,    4,    4,    8,   16,   32,   64,
         32,  128,    1,    2,    4,    2,    2,    8,    8
      };

/*--- Terminal Transition Matrix and Access Vectors. ------------------------*/

   // T_matrix.
      char Tm [66] =
      {
        -70,  -71,    0,    0,  -68,    0,  -69,    0,    0,    0,  -72,  -26,  -27,  -28,  -75,  -76,    0,    0,    0,    0,
         42,   43,    0,   44,  -55,  -10,  -26,   13,  -28,  -75,  -14,  -15,  -16,  -17,  -36,  -37,   45,  -32,  -18,  -19,
         48,  -87,  -30,    0,  -13,    0,  -35,  -14,    5,  -53,  -48,  -46,  -47,  -51,  -42,  -52,   32,  -50,  -74,  -49,
        -43,  -13,   41,  -45,  -14,  -15
      };

   // T_matrix row.
      unsigned char Tr [49] =
      {
         32,   32,   49,   32,   15,   32,   15,    0,   32,   32,   32,   32,    0,   32,    0,    0,    0,   32,   49,   32,
         32,   49,   32,   32,   32,   15,    0,   49,   32,   49,   32,   49,    0,   32,   32,   49,   49,   15,   32,   32,
         49,   49,    0,    0,   15,    0,   32,   32,   32
      };

   // T_matrix column.
      unsigned char Tc [29] =
      {
          1,    1,    8,    1,   10,    4,    6,    0,    8,   10,    1,   11,   13,   14,    2,    3,    4,    5,    6,    7,
          8,    9,   10,   11,   12,   13,   14,   15,   16
      };

/*--- Nonterminal Transition Matrix and Access Vectors. ---------------------*/

   // N_matrix.
      char Nm [197] =
      {
          0,    0,    0,    0,    0,  -85,   21,    0,  -89,   27,  -89,   27,  -85,   21,    0,    0,    0,  -41,    0,    0,
         29,    0,   30,    0,    0,    0,    0,    0,    0,    0,  -22,  -24,    0,  -25,    0,    0,    0,  -21,  -79,    8,
          9,   10,  -85,   21,    0,    0,    0,  -40,    0,   40,   29,  -38,   30,  -80,    0,    0,    0,    0,    0,    0,
        -81,   11,    0,    0,  -82,   26,    0,  -20,    3,  -79,    8,    9,   10,    0,   25,  -89,   27,    0,  -91,    0,
         14,   29,  -78,   30,  -80,    0,    0,  -65,    0,  -66,    0,  -81,   11,    0,    0,  -82,   15,    0,  -23,    3,
        -79,    8,    9,   10,   20,   21,  -88,   27,  -31,  -90,    0,    0,   29,  -77,   30,  -80,    0,  -59,    0,   38,
         46,  -61,  -81,   11,    0,    6,  -83,    0,   22,    0,   33,   31,    3,  -79,    8,    9,   10,   23,   21,  -88,
         27,    0,  -39,   36,    7,   29,  -78,   30,  -80,    0,  -62,    0,   38,   47,  -64,  -81,   11,    0,    0,  -82,
         12,   24,    0,   34,    3,  -79,    8,    9,   10,   17,   18,  -88,   27,  -29,  -90,    2,    1,   29,  -77,   30,
        -80,   37,  -56,  -11,   38,   39,  -58,  -81,   11,   35,    4,  -82,   16,   19,  -12,   28,   31
      };

   // N_matrix row.
      unsigned char Nr [49] =
      {
        164,  164,   99,  164,  132,  164,   68,  164,  164,   99,  132,   68,   99,  164,   37,   99,   99,  164,  164,   37,
         99,  164,    7,  132,    0,  164,   99,   99,   68,  132,  164,   68,  164,    1,    3,  164,   37,  164,  164,  164,
          7,  164,   99,  132,  164,   68,  164,  164,  164
      };

   // N_matrix column.
      unsigned char Nc [92] =
      {
         12,   12,   12,   12,   12,   12,   12,   12,   12,   11,   12,   14,   14,    0,    0,    0,   19,   19,   19,   30,
          1,    1,    1,    1,    1,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   10,   10,   10,   10,   11,
         11,   11,   12,   13,   14,   14,   14,   14,   15,   15,   15,   15,   15,   15,   16,   16,   17,   17,   17,   17,
         17,   17,   17,   17,   17,   18,   19,   20,   21,   21,   21,   22,   22,   23,   23,   24,   25,   26,   26,   27,
         27,   27,   28,   28,   29,   29,   30,   30,   31,   31,   32,   32
      };

/*--- Reduction Matrix and Access Vectors. ----------------------------------*/

   // R_matrix.
      unsigned char Rm [30] =
      {
          0,    0,    0,    0,    0,    0,   15,    0,    0,    0,    9,    0,   13,   27,    0,    0,    0,   67,   57,    0,
          0,    0,   67,   60,    0,    0,    0,   67,   63,    0
      };

   // R_matrix row.
      char Rr [49] =
      {
          0,    0,    0,    0,    1,   -5,    5,    2,    0,    0,    0,    0,    3,  -11,    6,    7,    4,   84,    0,   86,
         84,    0,   86,   84,   86,    0,    8,    0,   86,    0,   34,   33,    0,   86,   86,   73,    0,    0,    0,  -14,
          0,   44,    0,    0,   54,    0,  -19,  -24,    0
      };

   // R_matrix column.
      unsigned char Rc [29] =
      {
          0,    1,    0,    0,    0,    0,    1,    1,    0,    2,    0,    0,    0,    0,    0,    0,    3,    4,    4,    1,
          4,    0,    0,    0,    5,    0,    0,    5,    5
      };

/*--- Production Action Variables. ------------------------------------------*/

/* // Reverse production.
      char Reverse[92] = 
      {
              0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
              0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
              0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
              0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
              0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
              0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
              0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
              0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
              0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
              0,     0
      }; */

   // First action for a production
      char pact_numb[92] = 
      {
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,
              1,    -1,    -1,     2,     3,     3,     4,     5,     6,     7,
             -1,    -1,    -1,    -1,    -1,    -1,     8,     9,    10,    -1,
             11,    -1,    11,    12,    -1,    13,    14,    15,    -1,    -1,
             -1,    -1,    16,    17,    18,    19,    20,    21,    22,    22,
             22,    22,    22,    22,    23,    23,    24,    25,    26,    27,
             28,    29,    30,    31,    32,    -1,    33,    34,    -1,    -1,
             -1,    -1,    -1,    -1,    -1,    35,    25,    -1,    -1,    -1,
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
             -1,    -1
      };

   // Production Action Addresses.
      int (*pact_func[36]) (int, int) = 
      {
         LGActions::PRO_OPTNS,
         LGActions::EOGI,
         LGActions::ADD_TOKEN,
         LGActions::ADD_TOKEN2,
         LGActions::ADD_TERMNO,
         LGActions::ADD_DEFCON,
         LGActions::ADD_STRING,
         LGActions::ADD_CODE,
         LGActions::ADD_HEAD,
         LGActions::ADD_HEADLEX,
         LGActions::ADD_HEADIGNORE,
         LGActions::ADD_PROD,
         LGActions::POP_ALL,
         LGActions::PUSH_PLUS,
         LGActions::PUSH_ASTER,
         LGActions::PUSH_QUEST,
         LGActions::PUSH_OR,
         LGActions::PUSH_GBEG,
         LGActions::PUSH_GEND,
         LGActions::PUSH_GPLUS,
         LGActions::PUSH_GASTER,
         LGActions::PUSH_GQUEST,
         LGActions::PUSH_TAIL,
         LGActions::DEF_SET,
         LGActions::FIRST_RANGE,
         LGActions::FIRST_CHAR,
         LGActions::FIRST_SET,
         LGActions::ADD_RANGE,
         LGActions::ADD_CHAR,
         LGActions::ADD_SET,
         LGActions::SUB_RANGE,
         LGActions::SUB_CHAR,
         LGActions::SUB_SET,
         LGActions::RANGE_END,
         LGActions::RANGE_START,
         LGActions::ADD_HEADSET
      };

   // Production Action argument index. 
      char LGParser::pact_arg[92] = 
      {
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     7,
              8,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
             -1,    -1,    -1,    -1,    -1,    -1,    16,    17,    18,    -1,
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,
             21,    22,    23,    24,    -1,    -1,    25,    26,    27,    28,
             29,    30,    31,    32,    33,    -1,    34,    35,    -1,    -1,
             -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    -1,    -1,
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
             -1,    -1
      };

/*--- Argument Variables. ---------------------------------------------------*/

   // Action argument numbers 
		unsigned char LGParser::arg_numb[38] = 
      {
              0,    23,    24,    25,    26,    27,    28,     1,     0,     1,
              1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
              1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
              3,     3,     3,     3,     1,     1,     1,     1
      };

   // Text for string arguments 
      char* arg_text[1] = 
      {
         ""
      };

/*--- End of Parser Tables. ----------------------------------------------------*/

  		int      top_state;
  		PStack*  top_ptr;

      PStack*  LGParser::PS;                  
      PStack   LGParser::P_stack[STKSIZE]; 

      RStack*	LGParser::RS; 
      RStack	LGParser::R_stack[STKSIZE]; 

		char*    LGParser::T_list;
		char*    LGParser::H_list;
		int*     LGParser::P_list;

		char*    LGParser::T_start;
		char*    LGParser::T_end; 
		int      LGParser::T_line;        

		static char*	prt_line   (char* ls, int ln); 

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 
//    LGParse                                                                  //

int   LGParser::Parse ()                            
{
		init_lexer ();										// Initialize lexer.
		if (LGActions::CheckOptions() == 0) return 0;
		LGActions::init();								// Initialize everything else.

      if (LGParser::parse() == 0) return 0;
		LGActions::term ();
		if (n_errors) return 0;
      return (1);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 
//    term                                                                   //
/*
int   LGParser::term ()
{
		delete [] node;
      LGLexer::term();
      return (0);
}
*/
///////////////////////////////////////////////////////////////////////////////
//                                                                           // 
//    parse                                                                  //

int   LGParser::parse ()                        // LR Parser.               
{
      int t;												// Terminal. 
      int p;												// Production.  
      int x;												// State.  

      PS = P_stack;										// Set parse stack pointer.   
      n_errors = 0;										// Set number of errors.    
      x = 0;												// Set state to zero.

      RS = R_stack;										// Reset reduction stack pointer.
		top_ptr = PS;										// Save parse stack top pointer.
		top_state = x;										// Save current state.
		t = get_token ();									// Get token. 
		if (t == 0) goto Err;							// Bad token!
		goto Cont;

Read: RS = R_stack;										// Reset reduction stack pointer.
		top_ptr = PS;										// Save parse stack top pointer.
		top_state = x;										// Save current state.
		t = get_token ();									// Get next token. 
Cont:	if (tact_numb[t] >= 0)							// If input action ...         
      {
			int t1 = t;
      // T_start = token.start;						// Set token start.
      // T_end   = token.end;							// Set token end.
         t = (*tact_func[tact_numb [t]]) (t, tact_arg[t]);	// Call token action.  
      }
    	/*	For debugging ...
			char ch = *token.end;
  			*token.end = 0;
			printf ("       %-20s %3d, %s\n", token.start, t, term_symb[t]);
  			*token.end = ch; 
		*/

Shft: if (Bm [Br[x] + Bc[t]] & Bmask[t])			// Accept this terminal symbol?
      {
         (++PS)->state = x;                     // Stack state.   
         PS->start = token.start;					// Stack token start pointer.
         PS->end   = token.end;						// Stack token end pointer.
         PS->line  = token.line;						// Stack token line number.
         p = -Tm [Tr[x] + Tc[t]];					// Get reduction or next state.
         while (p >= 0)							      // While shift-reduce action.
         {
            PS -= PL[p];                        // Reduce parse stack pointer. 
				if (apply (p) != 0) return (0);		// Apply reduction, quit parsing?
            p = -Nm [Nr[PS->state] + Nc[p]];		// Get reduction or next state.
         }
			x = -p;											// Set next state.
         goto Read;										// Go read next token.
      }
      if ((p = Rr[x]) > 0 || (p = Rm [Rc[t] - p]) > 0) // Get reduction?
      {
         if (PL[p] == -1)                       // Null production?
         {
     		   (++RS)->state = (++PS)->state;		// Save state before destroying it.
            PS->state  = x;							// Stack state.
				RS->ptr = PS;						   // Save parse-stack pointer.
            PS->start  = NULL;					   // Clear start.
            PS->end    = NULL;					   // Clear end.
				goto Apply;
         }
         do 
         {
            PS -= PL[p];                        // Reduce parse stack ptr by production length. 
Apply:		if (apply (p) != 0) return (0);		// Apply reduction, quit parsing?
            p = -Nm [Nr[PS->state] + Nc[p]];		// Get reduction or next state.
         }
         while (p > 0);	
         x = -p;											// Set new state.
         goto Shft;										// Try to shift from this new state.
      }

      if (x == ACCEPT_STATE) 
      {
         PS -= PL[0];                             
         line_numb--;
			if (n_errors > 0) return (0);
         return (1);
      }

//		Error ...
Err:  syntax_error (t); 
		expecting ();
      return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 
//    apply                                                                  //

int   LGParser::apply (int p)
{
      int i, n, r, a;
      if (pact_numb[p] >= 0)				      // Call function?  
      {
			a = pact_arg[p];							// Get first arg index.
         i = arg_numb [a] - 1;					// Get parse stack position.
			if (i >= 0)									// Valid parse stack position?	
			{
				T_start = PS[i].start;           // Define T_start working pointer.
				T_end   = PS[i].end;             // Define T_end working pointer.
				T_line  = PS[i].line;            // Define T_line.
			}
			else											// Use last token accepted.
			{
				T_start = token.start;           // Define T_start working pointer.
				T_end   = token.end;             // Define T_end working pointer.
				T_line  = token.line;            // Define T_line.
			}
         r = (*pact_func [pact_numb[p]]) (p, a);   // Call production action.               
			if (r != 0) return (r);
      }  
      return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 

void  LGParser::syntax_error (int t)
{
      ushort ln, i, j;																																 
      char *ls, *le, *p, c;

		if (++n_errors == 1) prt_log ("\n");

		ls = line_start;
		ln = line_numb;
		if (*token.start == 26 || *ls == 26)  // At <eof> ?
		{
			do // Backup to last line start.
			{
				for (ls -= 2; *ls != '\n'; ls--);
				ls++;
				ln--;
			}
			while (token.start < ls);
		}
		else if (token.start < ls) // If token starts before current line start.
		{
			do 
			{
				for (ls--; *ls != '\n'; ls--);
			}
			while (token.start < ls);
			ls++;
		}
		le = prt_line (ls, ln);
		prt_pointer (ls, ln, token.start);
      if (*token.start <= 32) 
      {
         short x = *token.start;
         if (x < 0) x += 256;
         prt_log ("at (%d) %s\n", x,  term_symb[t]);
      }
      else
      {
         if (token.end == token.start) token.end++;
         if (token.end > le) 
         {
            if (*token.end == 26) line_numb--;
            prt_log ("starts here\n");
				prt_line (line_start, line_numb);
				prt_pointer (line_start, line_numb, token.end);
				prt_log ("ends here.\n");
         }
         else
         {
            c = *token.end; 
            *token.end = 0;
				int x = *(token.start+1);
				if (x < 0) x += 256;
            prt_log ("at %s (%d) %s\n", token.start, x, term_symb[t]);
            *token.end = c;
         }
      }	
      prt_log ("%s(%04d) : Syntax error.\n\n", grmfid, line_numb);
}

char* LGParser::prt_line (char* ls, int ln) 
{
		char *p;
	// Replace tabs with spaces. 
      for (p = ls; *p != '\n'; p++)
      {
         if (*p == '\t') *p = ' ';
      }
	// Remove ending spaces.
      while (*(--p) == ' '); 
      *(++p) = 0;
	// Print the line.
      prt_log ("%s(%04d) : %s\n", grmfid, ln, ls);
		*p = '\n';
		return (p);
}

void	LGParser::prt_pointer (char* ls, int ln, char* token) 
{
      int  i = 0;
		char *p, string[10000];
      for (p = ls; p < token; p++)   
      {
			string[i++] = '-';
			if (i == 9999) break;
      }
      string[i++] = 0;
      prt_log ("%s(%04d) : %s^ ", grmfid, ln, string);
}

////////////////////////////////////////////////////////////////////////////////
//

void  LGParser::expecting ()
{
		int t;

		T_list = (char*)malloc (sizeof(*T_list)*n_terms);
		H_list = (char*)malloc (sizeof(*H_list)*n_heads);
		P_list = (int* )malloc (sizeof(*P_list)*n_prods);

		for (t = 0; t < n_terms; t++) T_list[t] = 0;

		prt_log ("Expecting one of the following:\n\n");

		restore ();
	  	collect (top_state);	

		prt_log ("\n");

		free (P_list);
		free (H_list);
		free (T_list);
}

////////////////////////////////////////////////////////////////////////////////
//

void  LGParser::restore () // Restore parse stack.    
{
		while (RS > R_stack) // For all null reductions.                   
		{  
			(RS->ptr)->state = RS->state;	// Replace state with the saved one.
			RS--;
		}
		PS = top_ptr;
}

////////////////////////////////////////////////////////////////////////////////
//

void	LGParser::collect (int x) // Collect terminals that cause a transition or reduction. 
{
		int t, y, p, h, i, la, offset, n_red;
	//	printf ("Collecting terminals in state %d\n", x);
		for (h = 0; h < n_heads; h++) H_list[h] = -1;
     	for (t = 0; t < n_terms; t++)							// For all terminals.
		{
  			if (Bm [Br[x] + Bc[t]] & Bmask[t])				// Accept this terminal?
			{
				if (T_list[t] == 0 && t != T_PERCENT)
				{
					T_list[t] = 1;										// Mark it done, in case of SR conflict.
					y = Tm [Tr[x] + Tc[t]];							// Get action?
					if (y > 0)											// Shift & goto y action?
					{
			    		if ((p = Rr[y]) > 0)							// Default reduction in state y?
						{
							if (PL[p] == 0) H_list [head_numb[p]] = t;				
						}
						else 
						{
							offset = -p;	
     						for (la = 0; la < n_terms; la++)			// For all lookaheads.
							{
								if ((p = Rm [Rc[la] + offset]) > 0)	// Got a reduction on this terminal?
								{
									if (PL[p] == 0) H_list [head_numb[p]] = t; // Yes t and not la.				
								}
							}
						}
					}
					else 													// Shift & reduce action!
					{
						p = -y;											// Get production.
						if (PL[p] == 0) H_list [head_numb[p]] = t;				
					}
					prt_log ("\t%-32s", term_symb[t]);
					char first = 1;
					char blank[21] = "                    ";
					for (h = 0; h < n_heads; h++)
					{
						if (H_list[h] == t)
						{
							if (first)
							{
								prt_log (" => %s\n", head_symb[h]);	
								first = 0;
							}
							else
							{
								prt_log ("\t%-32s => %s\n", blank, head_symb[h]);
							}
						}
					}
					if (first)
					{
						prt_log ("\n");
					}
				}
			}
		}
    	if ((p = Rr[x]) > 0)										// Default reduction?
		{
			reduce (p, x);
		}
		else															// Reductions based on lookaheads!    		
		{
			n_red  = 0;
			offset = -p;	
			for (la = 0; la < n_terms; la++)			
			{
				if ((p = Rm [Rc[la] + offset]) > 0)			// Got a reduction on this terminal?
				{	
			  		for (i = 0; i < n_red; i++)				// For all reductions in the list.
					{
						if (P_list[i] == p) break;				// Already in this list?
					}
					if (i == n_red) P_list [n_red++] = p;	// Add this production to list.
				}
			}
		  	for (i = 0; i < n_red; i++)
			{
				reduce (P_list[i], x);
			}	  
		}
}

////////////////////////////////////////////////////////////////////////////////
//

void	LGParser::reduce (int p, int x)			// Reduce production. 
{
      if (PL[p] == -1)                       // Null production?
      {
			PS++;											// Increment stack pointer.
         PS->state = x;								// Stack state.
			goto Cont;
      }
		do
		{
			PS -= PL[p];								// Reduce parse stack pointer. 
Cont:		p = -Nm [Nr[PS->state] + Nc[p]];		// Get reduction or next state.
		}
		while (p > 0);
		x = -p;											// Set new state.
		collect (x);									// Go collect terminals.	
}

#endif

///// End of Parser ////////////////////////////////////////////////////////////




