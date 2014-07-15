
/* Original source code with tabs = 3 */

#include "c_parser.h"
#include "c_actions.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 // 
//    error     																				                        

int   c_token_action::error (int& t)                    
{
		if (token.end == token.start) // Illegal character?
		{
			token.end++; // Bypass it. 
		}
		return 0; // Return symbol-table index = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 // 
//    lookup    																				                        

int   c_token_action::lookup (int& t) // Lookup in symbol table.
{
		int sti;														// Symbol-table index.
		sti = add_symbol (t, token.start, token.end);	// Add to symbol table. 
		t   = symbol[sti].term;									//	Redefine terminal number (need this for typedef names). 
		return sti; // Return symbol-table index. 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 // 
//    goal_ (not used, for testing only)												                        

int c_parse_action::goal_ (int p) 
{
	   int a = pact_arg[p];				  // Get first argument index.
      return 1; // OK
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 
//    typedefidentifier_   																				  //

int c_parse_action::typedefidentifier_ (int p) 
{
	   int i = pact_arg[p];				   // Get first argument index.
      int sti = PS [arg_numb[i]-1].sti; // Get sti for first argument.
      symbol[sti].term = arg_numb [i+1];  // Set term to second argument.
      return 1; // OK
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 // 
//    goal_ (not used, for testing only)												                        

int c_node_action::goal_ (int n)
{
		return 0;
}

//                                                                                                 // 
/////////////////////////////////////////////////////////////////////////////////////////////////////
























