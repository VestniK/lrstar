
/* Original source code with tabs = 3 */

		#include "typedef_parser.h"
  		#include "typedef_actions.h"

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 
//    error     																				  //

int   typedef_token_action::error (int& t)                    
{
		if (token.end == token.start) 
		{
			token.end++;
		}
		return 0; 
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 
//    lookup    																				  //

int   typedef_token_action::lookup (int& t) // Look for Symbol.
{
		int sti;														// Symbol-table index.
		sti = add_symbol (t, token.start, token.end);	// Add to symbol table. 
		t   = symbol[sti].term;									//	Redefine terminal number (need this for typedef names). 
		return (sti); // Return symbol-table index. 
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 
//    Typedef Identifier   													   		  //

int   typedef_parse_action::typedefidentifier_ (int p) 
{
	   int i = pact_arg[p];				   // Get first argument index.
      int sti = PS [arg_numb[i]-1].sti; // Get sti for first argument.
      symbol[sti].term = arg_numb [i+1];  // Set term to second argument.
      return 1; // OK
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 
//    goal_ (not used, for testing only)												  //

int   typedef_node_action::goal_ (int n)
{
      return 1; // OK
}

/*--- End of Actions. -------------------------------------------------------*/

























