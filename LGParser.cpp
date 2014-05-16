
#ifdef DFASTAR

		#include "ComGlobal.h"
      #include "LGParser.h"
		#include "LGActions.h"

		#define STKSIZE        1000 // Parser stack size.
		#define ACCEPT_STATE	    47 // Final state.
      #define LOOKAHEADS        2

/*--- Parser Table Constants. -----------------------------------------------*/
                                                                                          
      static int n_terms	=    30; // No. of terminals.                               
      static int n_heads   =    40; // No. of non-terminals.     
      static int n_prods   =    90; // No. of productions.       
      static int n_states  =    48; // No. of states.
      static int eol_symb  =     0; // EOL symbol number            
      static int eof_symb  =     8; // EOF symbol number            
      static int err_used  =     0; // <error> used in grammar      

/*--- Terminal and Nonterminal Symbols of the Grammar. ----------------------*/

   // Terminal symbols of the grammar. 
      char* term_symb[30] = 
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
         "{first_string}",
         "{option_string}"
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
         "['=>']",
         "ArrowProd...",
         "[';']",
         "BarProd...",
         "TailExpr..."
      };

/*--- Production Information. -----------------------------------------------*/

   // Production length - 1 
      char PL[90] = 
      {
              1,     1,     2,     2,     3,    -1,     0,     0,     1,     2,
              1,     1,     1,     0,     0,     0,     2,     2,     2,     3,
              3,     3,     0,     0,     0,     1,     0,     1,     0,     0,
              0,     1,     1,     1,     2,     0,     1,     2,     0,     0,
              0,     1,     1,     1,     0,     0,     0,     0,     0,     0,
              4,     5,     0,     0,     0,     2,     2,     2,     2,     2,
              2,     2,     0,     0,     0,     0,     0,     0,     0,     2,
              3,     0,     0,     0,     1,     0,     0,     0,     0,     1,
             -1,     0,     0,     1,    -1,     0,     0,     1,     0,     1
      };

   // Head symbol for a production 
      unsigned char head_numb[90] = 
      {
              0,     1,     1,     1,     1,     2,     2,     3,     4,     4,
              5,     5,     5,     6,     6,     6,     7,     7,     7,     7,
              7,     7,     8,     9,    10,    11,    12,    13,    14,    15,
             16,    16,    16,    16,    16,    17,    17,    17,    18,    19,
             20,    20,    20,    20,    21,    21,    21,    21,    21,    21,
             22,    22,    23,    23,    23,    23,    23,    23,    23,    23,
             23,    24,    25,    26,    27,    27,    27,    28,    28,    29,
             29,    30,    31,    32,    32,    33,    33,    33,    34,    34,
             35,    35,    36,    36,    37,    37,    38,    38,    39,    39
      };

   // Index to first tail symbol for a production 
      unsigned char f_tail[91] = 
      {
              0,     2,     4,     7,    10,    14,    14,    15,    16,    18,
             21,    23,    25,    27,    28,    29,    30,    33,    36,    39,
             43,    47,    51,    52,    53,    54,    56,    57,    59,    60,
             61,    62,    64,    66,    68,    71,    72,    74,    77,    78,
             79,    80,    82,    84,    86,    87,    88,    89,    90,    91,
             92,    97,   103,   104,   105,   106,   109,   112,   115,   118,
            121,   124,   127,   128,   129,   130,   131,   132,   133,   134,
            137,   141,   142,   143,   144,   146,   147,   148,   149,   150,
            152,   152,   153,   154,   156,   156,   157,   158,   160,   161,
            163
      };

   // Tail symbol numbers (T: >= 0, NT: < 0)
      char tail[163] = 
      {
             -1,     8,    -2,   -32,    -2,   -32,    -3,    -2,   -32,   -34,
             -2,   -32,    -3,   -34,    29,    22,    -5,    -6,    -5,    -6,
             21,    24,   -35,    27,   -35,    28,   -35,     7,     1,     6,
             -8,   -36,   -37,    -9,   -36,   -37,   -10,   -36,   -37,    -8,
            -11,   -38,   -37,    -9,   -11,   -38,   -37,   -10,   -11,   -38,
            -37,    23,    24,    25,   -12,   -15,     9,   -14,   -15,    17,
            -39,   -21,   -21,    13,   -21,    14,   -21,    15,   -19,   -17,
            -20,   -16,   -17,   -16,   -17,   -18,   -16,    17,    11,    12,
             12,    13,    12,    14,    12,    15,     1,     4,     2,     5,
              6,     7,    -8,   -12,    19,   -23,    20,    -8,   -12,    19,
            -23,    20,    21,   -24,   -27,   -28,   -23,    17,   -24,   -23,
             17,   -27,   -23,    17,   -28,   -23,    18,   -24,   -23,    18,
            -27,   -23,    18,   -28,   -26,    16,   -25,   -27,   -27,     5,
              6,     7,     1,     4,   -30,   -12,   -31,   -30,   -12,   -31,
             21,    26,     7,    -4,   -32,    -4,    -7,   -22,   -29,   -33,
            -34,   -33,    10,   -11,   -36,   -11,    21,   -13,   -38,   -13,
            -16,   -39,   -16
      };

/*--- token Action Variables ------------------------------------------------*/

   // token Action number. 
      char tact_numb[30] = 
      {
              0,     1,     1,     1,     1,     2,     3,    -1,    -1,     4,
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1
      };

   // token Action Addresses.
      int (*tact_func[5]) (int, int) = 
      {
         LGActions::error,
         LGActions::firstsym,
         LGActions::checkcase,
         LGActions::checkstring,
         LGActions::arrow
      };

   // token Action argument index. 
      char tact_arg[30] = 
      {
              0,     1,     2,     3,     4,     5,     6,    -1,    -1,    -1,
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1
      };

/*--- Bit Matrix and Access Vectors. -----------------------------------------*/

   // B_matrix.
      unsigned char Bm [48] =
      {
          0,    0,    0,   32,  128,    0,    0,   24,   98,    0,    0,   30,    0,    2,    0,    0,    1,    0,    0,   12,
        126,  128,    0,    1,    1,  126,    0,   64,    0,    0,   32,    1,    0,    8,    0,  122,    0,    0,  126,   36,
          0,   96,    0,   16,    0,  112,    0,    0
      };

   // B_matrix row.
      unsigned char Br [48] =
      {
          1,    4,    5,    8,    9,   12,   12,   12,   14,   17,   15,   15,   15,   15,   17,   12,   17,    2,   20,   22,
          2,   25,   22,    2,   22,   27,   25,   29,   25,   32,   25,   35,   29,   29,   14,   38,   40,   42,    0,   25,
         32,   35,   35,   14,   45,    0,    0,    0
      };

   // B_matrix column.
      unsigned char Bc [30] =
      {
          0,    0,    0,    0,    0,    0,    0,    0,    0,    1,    1,    0,    1,    1,    1,    1,    1,    1,    1,    1,
          1,    2,    2,    2,    2,    2,    2,    2,    2,    2
      };

   // B_matrix mask.
      unsigned char Bmask [30] =
      {
          1,    2,    4,    1,    8,   16,   32,   64,  128,    1,    2,    4,    4,    8,    8,    8,   16,   32,   64,  128,
         64,    1,    2,    4,    8,    4,    4,   16,   16,   32
      };

/*--- Terminal Transition Matrix and Access Vectors. ------------------------*/

   // T_matrix.
      char Tm [86] =
      {
        -66,    0,  -68,    0,    0,  -72,    0,  -64,    0,  -67,    0,  -65,  -51,   41,   42,    0,   43,  -70,    0,  -22,
        -23,  -24,  -71,  -49,  -46,  -45,   40,    0,    0,    0,  -47,  -38,  -44,    0,  -48,    0,    0,    0,    0,    0,
          0,    0,    0,  -39,  -13,   47,  -81,  -26,  -31,  -32,  -33,   44,  -28,  -14,    0,  -15,   -9,    0,    0,    5,
          0,    0,    6,    7,   -6,  -49,  -46,  -45,  -26,  -41,  -42,  -43,  -47,  -28,  -44,   31,  -48,  -85,   -7,  -22,
         15,  -24,  -71,    6,    7,  -39
      };

   // T_matrix row.
      unsigned char Tr [48] =
      {
         44,   44,   44,   44,   65,   44,   44,   44,   44,    5,   44,   44,   44,   44,    5,   44,    5,   44,   65,   65,
         44,   65,   65,   44,   65,    5,   65,   65,   65,   44,   65,    0,   65,   65,    5,   23,    5,   44,   44,   65,
         65,    0,    0,    0,    0,   44,   44,   44
      };

   // T_matrix column.
      unsigned char Tc [30] =
      {
          9,    9,    1,    9,    2,    7,   11,    0,    1,    3,    2,   20,    3,    4,    5,    6,    7,    8,    9,   10,
         11,   12,   13,   14,   15,   16,   17,   18,   19,   20
      };

/*--- Nonterminal Transition Matrix and Access Vectors. ---------------------*/

   // N_matrix.
      char Nm [153] =
      {
          0,    0,    0,    0,  -83,   21,    0,  -87,   26,  -87,   26,  -83,   21,    0,    0,    0,  -37,  -83,   21,   28,
          0,   29,  -36,    0,   39,   28,  -34,   29,    0,    0,  -18,  -20,    0,  -21,    0,    0,    0,  -17,   25,  -87,
         26,    0,  -89,  -16,    0,   28,    0,   29,    0,    0,    0,  -61,    0,  -62,    0,  -75,   10,   11,   12,   23,
         21,  -86,   26,  -19,  -35,   35,    0,   28,    0,   29,  -76,    0,  -58,    0,   37,   46,  -60,  -77,   13,    0,
          0,  -79,    0,  -12,   24,    0,   33,  -75,   10,   11,   12,   20,   21,  -86,   26,  -27,  -88,    0,    9,   28,
        -74,   29,  -76,    0,  -55,    3,   37,   45,  -57,  -77,   13,    0,    0,  -78,   14,  -11,   22,    0,   32,   30,
        -75,   10,   11,   12,   17,   18,  -86,   26,  -25,  -88,    2,    1,   28,  -73,   29,  -76,   36,  -52,    3,   37,
         38,  -54,  -77,   13,   34,    4,  -78,   16,  -10,   19,    8,   27,   30
      };

   // N_matrix row.
      unsigned char Nr [48] =
      {
        120,  120,  120,  120,   87,  120,   87,   55,  120,  120,  120,   87,   55,   33,   55,  120,   55,  120,  120,   13,
         87,  120,    7,   55,    0,  120,   87,   33,   55,  120,   33,  120,    1,    3,  120,   13,  120,  120,  120,    7,
        120,   87,   55,  120,   33,  120,  120,  120
      };

   // N_matrix column.
      unsigned char Nc [90] =
      {
         11,   11,   11,   11,   11,   10,   10,   11,   13,   13,   18,   18,   18,   30,   30,   30,    0,    0,    0,    0,
          0,    0,    1,    2,    3,    4,    5,    6,    7,    8,    9,    9,    9,    9,    9,   10,   10,   10,   11,   12,
         13,   13,   13,   13,   14,   14,   14,   14,   14,   14,   15,   15,   16,   16,   16,   16,   16,   16,   16,   16,
         16,   17,   18,   19,   20,   20,   20,   21,   21,   22,   22,   23,   24,   25,   25,   26,   26,   26,   27,   27,
         28,   28,   29,   29,   30,   30,   31,   31,   32,   32
      };

/*--- Reduction Matrix and Access Vectors. ----------------------------------*/

   // R_matrix.
      unsigned char Rm [22] =
      {
          0,    0,    0,    0,    0,   80,   23,    0,    0,    0,   63,   53,    0,    0,    0,   63,   56,    0,    0,    0,
         63,   59
      };

   // R_matrix row.
      char Rr [48] =
      {
          5,    0,    0,    0,    1,   80,   80,   80,    8,    2,    0,    0,    0,    0,    3,   -4,    4,   82,    0,   84,
         82,    0,   84,   82,   84,    0,    0,   84,    0,   30,   29,    0,   84,   84,   69,    0,    0,    0,   -7,    0,
         40,    0,    0,   50,    0,  -12,  -17,    0
      };

   // R_matrix column.
      unsigned char Rc [30] =
      {
          0,    1,    0,    0,    0,    0,    1,    1,    0,    2,    0,    0,    0,    0,    0,    0,    3,    4,    4,    0,
          4,    0,    0,    0,    0,    0,    0,    0,    0,    0
      };

/*--- Production Action Variables. ------------------------------------------*/

/* // Reverse production.
      char Reverse[90] = 
      {
              0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
              0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
              0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
              0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
              0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
              0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
              0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
              0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
              0,     0,     0,     0,     0,     0,     0,     0,     0,     0
      }; */

   // First action for a production
      char pact_numb[90] = 
      {
             -1,    -1,    -1,    -1,    -1,     0,     0,     1,    -1,    -1,
              2,     2,     2,     3,     4,     5,    -1,    -1,    -1,    -1,
             -1,    -1,     6,     7,     8,    -1,     9,    -1,     9,    10,
             -1,    11,    12,    13,    -1,    -1,    -1,    -1,    14,    15,
             16,    17,    18,    19,    20,    20,    20,    20,    20,    20,
             21,    21,    22,    23,    24,    25,    26,    27,    28,    29,
             30,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
             -1,    33,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1
      };

   // Production Action Addresses.
      int (*pact_func[34]) (int, int) = 
      {
         LGActions::PRO_OPTNS,
         LGActions::EOGI,
         LGActions::ADD_TOKEN,
         LGActions::ADD_TERMNO,
         LGActions::ADD_DEFCON,
         LGActions::ADD_STRING,
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
      char LGParser::pact_arg[90] = 
      {
             -1,    -1,    -1,    -1,    -1,     8,     9,    10,    -1,    -1,
             11,    12,    13,    14,    15,    16,    -1,    -1,    -1,    -1,
             -1,    -1,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
             -1,    -1,    -1,    -1,    20,    21,    22,    23,    24,    25,
             -1,    -1,    26,    27,    28,    29,    30,    31,    32,    33,
             34,    -1,    35,    36,    -1,    -1,    -1,    -1,    -1,    -1,
             -1,    37,    38,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1
      };

/*--- Argument Variables. ---------------------------------------------------*/

   // Action argument numbers 
		unsigned char LGParser::arg_numb[39] = 
      {
              0,    23,    24,    25,    26,    27,    28,    29,     0,     1,
              0,     1,     1,     1,     1,     1,     1,     1,     1,     1,
              1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
              3,     3,     3,     3,     3,     1,     1,     1,     1
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
      if (!LGParser::parse()) return 0;
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

		LGLexer::init_lexer ();							// Initialize lexer.
      PS = P_stack;										// Set parse stack pointer.   
      n_errors = 0;										// Set number of errors.    
      x = 0;												// Set state to zero.

      RS = R_stack;										// Reset reduction stack pointer.
		top_ptr = PS;										// Save parse stack top pointer.
		top_state = x;										// Save current state.
		t = get_token ();									// Get next token. 
		if (t == 0) goto Err;							// Bad token!
		goto Cont;

Read: RS = R_stack;										// Reset reduction stack pointer.
		top_ptr = PS;										// Save parse stack top pointer.
		top_state = x;										// Save current state.
		t = get_token ();									// Get next token. 
Cont:	if (tact_numb[t] >= 0)							// If input action ...         
      {
			int t1 = t;
         T_start = token.start;						// Set token start.
         T_end   = token.end;							// Set token end.
         t = (*tact_func[tact_numb [t]]) (t, tact_arg[t]);	// Call token action.  
      }
	/*	if (0)
		{
    	  	char ch = *token.end;
  			*token.end = 0;
			printf ("       %-20s %3d, %s\n", token.start, t, term_symb[t]);
  			*token.end = ch; 
		}	*/

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




