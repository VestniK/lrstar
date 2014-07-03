
#ifdef DFASTAR

		#include "ComGlobal.h"
      #include "LGParser.h"
		#include "LGActions.h"

		#define STKSIZE        1000 // Parser stack size.
		#define ACCEPT_STATE	    57 // Final state.
      #define LOOKAHEADS        2

/*--- Parser Table Constants. -----------------------------------------------*/
                                                                                          
      static int n_terms	=    30; // No. of terminals.                               
      static int n_heads   =    35; // No. of non-terminals.     
      static int n_prods   =    96; // No. of productions.       
      static int n_states  =    58; // No. of states.
      static int eol_symb  =     0; // EOL symbol number            
      static int eof_symb  =     7; // EOF symbol number            
      static int err_used  =     0; // <error> used in grammar      

/*--- Terminal and Nonterminal Symbols of the Grammar. ----------------------*/

   // Terminal symbols of the grammar. 
      char* term_symb[30] = 
      {
         "<error>",
         "<alpha>",
         "<lexical>",
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
         "\'[]\'",
         "\'...\'",
         "\'[\'",
         "\']\'",
         "\';\'",
         "{first_alpha}",
         "{first_lexical}",
         "{first_escape}",
         "{first_literal}",
         "{first_string}",
         "{option_string}",
         "\'%%\'"
      };                                                 

   // Head symbols of the grammar. 
      char* head_symb[35] = 
      {
         "Goal",
         "Grammar",
         "Options",
         "PP",
         "TokenProd",
         "Token",
         "IgnoreProd",
         "IgnoreToken",
         "ReturnValue",
         "NonterminalDef",
         "NonterminalSymbol",
         "NonterminalProd",
         "Arrow",
         "TailExprList",
         "TailExpr",
         "List",
         "Or",
         "GroupStart",
         "GroupEnd",
         "OptGroupStart",
         "OptGroupEnd",
         "TailSymbol",
         "SetDef",
         "SetExpr",
         "Range",
         "RangeStart",
         "SetStart",
         "Character",
         "SetName",
         "TokenProd...",
         "(NonterminalDef | SetDef)",
         "(NonterminalDef | SetDef)...",
         "IgnoreProd...",
         "NonterminalProd...",
         "TailExpr..."
      };

/*--- Production Information. -----------------------------------------------*/

   // Production length - 1 
      char PL[96] = 
      {
              1,     1,     2,     2,     3,     2,     3,     3,     4,    -1,
              0,     0,     1,     2,     1,     1,     1,     1,     0,     1,
              1,     1,     1,     1,     1,     0,     0,     0,     1,     2,
              0,     0,     0,     1,     0,     0,     0,     1,     1,     1,
              1,     2,     2,     0,     1,     2,     0,     0,     0,     1,
              1,     1,     1,     0,     0,     1,     0,     0,     0,     0,
              0,     2,     3,     0,     2,     2,     2,     2,     2,     2,
              2,     2,     2,     2,     2,     2,     2,     0,     0,     0,
              0,     0,     0,     0,     0,     1,     0,     0,     0,     1,
              0,     1,     0,     1,     0,     1
      };

   // Head symbol for a production 
      unsigned char head_numb[96] = 
      {
              0,     1,     1,     1,     1,     1,     1,     1,     1,     2,
              2,     3,     4,     4,     5,     5,     5,     5,     6,     6,
              7,     7,     7,     7,     7,     8,     8,     8,     9,     9,
             10,    10,    10,    11,    12,    13,    14,    14,    14,    14,
             14,    14,    14,    15,    15,    15,    16,    17,    18,    18,
             18,    18,    18,    19,    20,    20,    21,    21,    21,    21,
             21,    22,    22,    23,    23,    23,    23,    23,    23,    23,
             23,    23,    23,    23,    23,    23,    24,    25,    26,    26,
             27,    27,    28,    28,    29,    29,    30,    30,    31,    31,
             32,    32,    33,    33,    34,    34
      };

   // Index to first tail symbol for a production 
      unsigned char f_tail[97] = 
      {
              0,     2,     4,     7,    10,    14,    17,    21,    25,    30,
             30,    31,    32,    34,    37,    39,    41,    43,    45,    46,
             48,    50,    52,    54,    56,    58,    59,    60,    61,    63,
             66,    67,    68,    69,    71,    72,    73,    74,    76,    78,
             80,    82,    85,    88,    89,    91,    94,    95,    96,    97,
             99,   101,   103,   105,   106,   107,   109,   110,   111,   112,
            113,   114,   117,   121,   122,   125,   128,   131,   134,   137,
            140,   143,   146,   149,   152,   155,   158,   161,   162,   163,
            164,   165,   166,   167,   168,   169,   171,   172,   173,   174,
            176,   177,   179,   180,   182,   183,   185
      };

   // Tail symbol numbers (T: >= 0, NT: < 0)
      char tail[185] = 
      {
             -1,     7,    -2,   -29,    -2,   -29,   -31,    -2,   -29,   -32,
             -2,   -29,   -32,   -31,    -2,   -29,    -3,    -2,   -29,    -3,
            -31,    -2,   -29,    -3,   -32,    -2,   -29,    -3,   -32,   -31,
             28,    29,    -5,    -8,    -5,    -8,    22,    23,     9,    24,
              9,    26,     9,    27,     9,    -7,    -7,    22,    23,    18,
             24,    18,    26,    18,    27,    18,    25,    18,     6,     1,
              5,   -10,   -33,   -10,   -33,    22,    23,    24,    25,   -12,
            -13,     8,   -34,   -21,   -21,    12,   -21,    13,   -21,    14,
            -21,    19,   -17,   -15,   -18,   -19,   -15,   -20,   -14,   -15,
            -14,   -15,   -16,   -14,    16,    10,    11,    11,    12,    11,
             13,    11,    14,    11,    19,    20,    21,    21,    19,     1,
              2,     3,     4,     6,   -10,   -12,   -23,   -10,   -12,   -23,
             22,   -24,   -26,    16,   -27,   -26,    17,   -27,   -26,    16,
            -24,   -26,    17,   -24,   -26,    16,   -28,   -26,    17,   -28,
            -23,    16,   -27,   -23,    17,   -27,   -23,    16,   -24,   -23,
             17,   -24,   -23,    16,   -28,   -23,    17,   -28,   -25,    15,
            -27,   -27,   -27,   -28,     4,     6,     1,     3,    -4,   -29,
             -4,    -9,   -22,   -30,   -31,   -30,    -6,   -32,    -6,   -11,
            -33,   -11,   -14,   -34,   -14
      };

/*--- token Action Variables ------------------------------------------------*/

   // token Action number. 
      char tact_numb[30] = 
      {
              0,     1,     1,     1,     2,     3,    -1,    -1,     4,    -1,
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
              0,     1,     2,     3,     4,     5,    -1,    -1,    -1,    -1,
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1
      };

/*--- Bit Matrix and Access Vectors. -----------------------------------------*/

   // B_matrix.
      unsigned char Bm [58] =
      {
          0,    0,    0,   64,  128,    0,    0,   40,   98,    0,    0,  184,    0,    2,    0,    0,    4,    0,    0,   56,
          0,    1,    0,    0,   24,    0,  130,    0,  128,    0,   94,    0,    0,    1,    4,    0,    8,    1,    0,   96,
          4,    0,   16,    0,   96,    0,   94,   36,    0,   94,   32,    2,   90,    0,    0,   80,    0,    0
      };

   // B_matrix row.
      unsigned char Br [58] =
      {
          1,    4,    5,    8,    9,   12,   12,   12,   12,   14,   17,   14,   20,   22,   17,   25,   25,   27,   25,   25,
         22,   17,   27,   27,   27,   27,   30,   32,   22,   22,   30,   30,   35,   38,   41,   43,    0,   30,    0,    0,
          0,    0,   30,   46,   49,   52,   52,   55,   52,   52,   30,   35,   31,    0,    0,    0,    0,    0
      };

   // B_matrix column.
      unsigned char Bc [30] =
      {
          0,    0,    0,    0,    0,    0,    0,    0,    1,    1,    0,    1,    1,    1,    1,    1,    1,    1,    1,    2,
          0,    2,    2,    2,    2,    2,    2,    2,    2,    2
      };

   // B_matrix mask.
      unsigned char Bmask [30] =
      {
          1,    2,    4,    8,   16,   32,   64,  128,    1,    2,    4,    4,    8,    8,    8,   16,   32,   64,  128,    1,
          4,    2,    4,    8,    8,   16,   32,   32,   64,  128
      };

/*--- Terminal Transition Matrix and Access Vectors. ------------------------*/

   // T_matrix.
      char Tm [85] =
      {
          0,    0,    0,    0,  -16,  -82,    0,  -81,  -83,  -17,  -22,    0,  -80,   45,   46,  -23,    0,    0,  -62,  -30,
        -31,  -32,  -26,  -27,  -25,   57,  -14,  -37,  -39,   47,   48,   49,  -20,  -40,  -38,  -13,    5,    6,    0,    7,
          8,  -10,  -34,   38,  -57,   41,   39,  -15,  -49,  -51,   40,    0,  -47,  -21,  -52,  -50,  -19,   15,   16,   17,
         18,   19,  -53,  -11,  -56,  -57,  -60,  -58,  -16,   51,    0,  -59,  -46,  -47,  -24,  -55,   52,  -29,   22,   23,
         17,   24,   25,  -53,  -34
      };

   // T_matrix row.
      unsigned char Tr [58] =
      {
         22,   22,   22,   22,   43,   22,   43,   64,    5,   22,   64,   43,   22,    5,   64,   22,   43,   64,    0,    5,
          5,   64,   22,   43,    0,    5,   43,   64,    5,    5,   64,   64,   22,    5,   22,   22,   22,   64,   22,   22,
         22,   22,   64,   64,   64,    5,    5,    5,    5,    5,   64,   43,   64,   22,   22,   22,   22,   22
      };

   // T_matrix column.
      unsigned char Tc [30] =
      {
          0,    0,    1,    3,    7,    1,    2,    3,   20,    4,    9,    5,    5,   12,    6,    7,    8,    9,   10,   11,
         19,   12,   13,   14,   15,   16,   17,   18,   19,   20
      };

/*--- Nonterminal Transition Matrix and Access Vectors. ---------------------*/

   // N_matrix.
      char Nm [149] =
      {
          0,    0,    0,    0,    0,    0,    0,    0,  -45,    0,    0,   30,    0,   31,    0,   32,    0,    0,  -67,   34,
          0,   56,  -69,  -91,   11,    0,  -86,   12,    0,    0,    0,  -95,    0,    0,   30,    0,   31,    0,   32,  -87,
          0,    0,    0,    0,  -76,  -86,   12,  -88,   28,    0,  -43,   44,    0,   30,    0,   31,    0,   32,  -87,    0,
        -73,   34,    0,   54,  -75,    0,  -89,  -91,   11,    0,  -86,   12,    0,    0,    0,  -44,    0,   50,   30,  -41,
         31,  -42,   32,  -87,    0,  -66,   34,    0,   55,  -68,    0,  -88,   29,  -90,   11,    0,  -86,   12,  -93,   42,
          0,  -43,   43,   10,   30,  -85,   31,    3,   32,  -87,    0,  -72,   34,    0,   53,  -74,    0,  -88,   13,   14,
        -90,   11,    9,  -86,   12,  -92,   26,  -33,  -94,    2,    1,   30,  -84,   31,    3,   32,  -87,   33,  -63,   34,
         35,   36,  -79,    4,  -88,   20,   21,   27,   37
      };

   // N_matrix row.
      unsigned char Nr [58] =
      {
        120,  120,  120,  120,   93,  120,  120,  120,  120,  120,  120,  120,  120,   42,   23,  120,  120,  120,  120,  120,
         42,   67,  120,  120,  120,  120,  120,   93,   42,   42,   93,   42,  120,  120,  120,  120,  120,   23,  120,  120,
        120,  120,  120,   67,   67,   93,   42,   23,   67,    0,    0,  120,  120,  120,  120,  120,  120,  120
      };

   // N_matrix column.
      unsigned char Nc [96] =
      {
         10,   10,   10,   10,   10,   10,   10,   10,   10,    9,    9,   10,   12,   12,   14,   14,   14,   14,    0,    0,
          1,    1,    1,    1,    1,    2,    2,    2,    3,    3,    4,    4,    4,    5,    6,    7,    8,    8,    8,    8,
          8,    8,    8,    9,    9,    9,   10,   11,   12,   12,   12,   12,   12,   13,   14,   14,   15,   15,   15,   15,
         15,   16,   16,   17,   17,   17,   17,   17,   17,   17,   17,   17,   17,   17,   17,   17,   18,   19,   20,   20,
         21,   21,   22,   22,   23,   23,   24,   24,   25,   25,   26,   26,   27,   27,   28,   28
      };

/*--- Reduction Matrix and Access Vectors. ----------------------------------*/

   // R_matrix.
      unsigned char Rm [47] =
      {
          0,    0,    0,    0,    0,   77,   78,    0,   56,   56,    0,   82,    0,   58,   58,    0,   83,    0,   59,   59,
         80,   80,    0,   60,   60,   81,   81,    0,    0,   70,   77,   70,    0,    0,   71,   77,   71,    0,    0,   64,
         77,   64,    0,    0,   65,   77,   65
      };

   // R_matrix row.
      char Rr [58] =
      {
          9,    0,    0,    0,    1,    0,    0,    0,    0,   12,    5,   18,    0,    2,    3,   30,   31,   32,    0,    0,
          6,    7,   30,   31,    0,    0,    0,   28,    4,    8,    0,    0,   36,   61,    0,    0,   -2,   35,   -7,  -12,
        -17,  -22,    0,    0,    0,    0,    0,    0,    0,    0,    0,   48,   54,  -27,  -32,  -37,  -42,    0
      };

   // R_matrix column.
      unsigned char Rc [30] =
      {
          0,    1,    1,    1,    1,    0,    1,    2,    1,    0,    1,    0,    1,    1,    1,    3,    4,    4,    0,    1,
          1,    0,    2,    2,    2,    2,    0,    0,    0,    0
      };

/*--- Production Action Variables. ------------------------------------------*/

/* // Reverse production.
      char Reverse[96] = 
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
              0,     0,     0,     0,     0,     0
      }; */

   // First action for a production
      char pact_numb[96] = 
      {
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,
              0,     1,    -1,    -1,     2,     2,     2,     2,    -1,    -1,
              3,     3,     3,     3,     3,     4,     5,     6,    -1,    -1,
              7,     8,     9,    -1,    10,    11,    -1,    12,    13,    14,
             15,    -1,    -1,    -1,    -1,    -1,    16,    17,    18,    19,
             20,    21,    22,    23,    24,    25,    26,    26,    26,    26,
             26,    -1,    -1,    27,    28,    29,    30,    31,    32,    33,
             28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
             -1,    -1,    -1,    -1,    -1,    -1
      };

   // Production Action Addresses.
      int (*pact_func[38]) (int, int) = 
      {
         LGActions::PRO_OPTNS,
         LGActions::EOGI,
         LGActions::ADD_TOKEN,
         LGActions::ADD_IGNORE,
         LGActions::ADD_TERMNO,
         LGActions::ADD_DEFCON,
         LGActions::ADD_STRING,
         LGActions::ADD_HEAD,
         LGActions::ADD_HEADLEX,
         LGActions::ADD_HEADESC,
         LGActions::ADD_PROD,
         LGActions::POP_ALL,
         LGActions::PUSH_PLUS,
         LGActions::PUSH_ASTER,
         LGActions::PUSH_QUEST,
         LGActions::PUSH_DOTS,
         LGActions::PUSH_OR,
         LGActions::PUSH_GBEG,
         LGActions::PUSH_GEND,
         LGActions::PUSH_GPLUS,
         LGActions::PUSH_GASTER,
         LGActions::PUSH_GQUEST,
         LGActions::PUSH_GDOTS,
         LGActions::PUSH_OBEG,
         LGActions::PUSH_OEND,
         LGActions::PUSH_ODOTS,
         LGActions::PUSH_TAIL,
         LGActions::FIRST_RANGE,
         LGActions::ADD_CHAR,
         LGActions::SUB_CHAR,
         LGActions::ADD_RANGE,
         LGActions::SUB_RANGE,
         LGActions::ADD_SET,
         LGActions::SUB_SET,
         LGActions::RANGE_END,
         LGActions::RANGE_START,
         LGActions::FIRST_CHAR,
         LGActions::FIRST_SET
      };

   // Production Action argument index. 
      char LGParser::pact_arg[96] = 
      {
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     7,
              8,     9,    -1,    -1,    10,    11,    12,    13,    -1,    -1,
             14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
             22,    23,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
             -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
             29,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
             37,    38,    -1,    -1,    39,    40,    41,    42,    43,    44,
             -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
             -1,    -1,    -1,    -1,    -1,    -1
      };

/*--- Argument Variables. ---------------------------------------------------*/

   // Defined Constants
      #define T_ERROR                  0 // 
      #define T_ALPHA                  1 // 
      #define T_LEXICAL                2 // 
      #define T_ESCAPE                 3 // 
      #define T_LITERAL1               4 // 
      #define T_LITERAL2               5 // 
      #define T_INTEGER                6 // 
      #define T_EOF                    7 // 
      #define T_ARROW                  8 // 
      #define T_RETURN                 9 // 
      #define T_LEFTP                 10 // 
      #define T_RIGHTP                11 // 
      #define T_PLUS                  12 // 
      #define T_STAR                  13 // 
      #define T_QUEST                 14 // 
      #define T_DOTS                  15 // 
      #define T_BAR                   16 // 
      #define T_DASH                  17 // 
      #define T_IGNORE                18 // 
      #define T_REPEAT                19 // 
      #define T_LEFTB                 20 // 
      #define T_RIGHTB                21 // 
      #define T_SEMI                  22 // 

   // Action argument numbers 
		unsigned char LGParser::arg_numb[45] = 
      {
              0,    23,    24,    25,    26,    27,    28,     0,     1,     0,
              1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
              1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
              1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
              3,     3,     1,     1,     1
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
         prt_log ("at %d %s\n", x,  term_symb[t]);
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
            prt_log ("at %s %s\n", token.start, term_symb[t]);
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
				if (T_list[t] == 0)
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




