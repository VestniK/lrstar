
/* Original source code by Paul Mann with tabs = 3 */

		#include "ielr_main.h"
		#include "ielr_parser.h"
  		#include "ielr_actions.h"          

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 
//    error     																				  //

int   ielr_token_action::error (int t)                     
{
	// Illegal character?
		if (token.end == token.start) 
		{
			token.end++;
			return (t); // Return zero (t should be zero).
		}
	// Do a symbol table lookup.
	// Could be a valid keyword or constant from an ambiguous grammar.
		return (lookup (t)); 
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 
//    lookup    																				  //

int   ielr_token_action::lookup (int t) // Look for Symbol.
{
//		int a = tact_arg[t];
		token.sti = add_symbol (t, token.start, token.end); 
		return (symbol[token.sti].term); // This works for {typedef}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 
//    goal_ (not used, for testing only)												  //

int ielr_ast_action::goal_ (int n)
{
		return (0);
}

/*--- End of Actions. -------------------------------------------------------*/

























