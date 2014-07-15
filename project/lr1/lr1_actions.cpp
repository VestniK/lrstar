
/* Original source code with tabs = 3 */

#include "lr1_parser.h"
#include "lr1_actions.h"          

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 
//    error     																				  //

int   lr1_token_action::error (short& t)                     
{
		if (token.end == token.start) 
		{
			token.end++;
		}
		return (0); 
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 
//    lookup    																				  //

int   lr1_token_action::lookup (short& t) // Look for Symbol.
{
		int sti;														// Symbol-table index.
		sti = add_symbol (t, token.start, token.end);	// Add to symbol table. 
		t   = symbol[sti].term;									//	Redefine terminal number (need this for typedef names). 
		return (sti); // Return symbol-table index. 
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 
//    goal_ (not used, for testing only)												  //

short lr1_node_action::goal_ (int n)
{
		return 1; // OK
}


/*--- End of Actions. -------------------------------------------------------*/

























