
/* Source code with tabs = 3 */

#include "calc1_parser.h"
#include "calc1_actions.h"

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 
//    error     																				  //

int   calc1_token_action::error (int& t)                    
{
		if (token.end == token.start) // Illegal character?
		{
			token.end++; // Bypass it. 
		}
		return (0); // Return symbol-table index = 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 
//    lookup    																				  //

int   calc1_token_action::lookup (int& t) // Look for Symbol.
{
		int sti;														// Symbol-table index.
		sti = add_symbol (t, token.start, token.end);	// Add to symbol table. 
		t   = symbol[sti].term;									//	Redefine terminal number (need this for typedef names). 
		return (sti); // Return symbol-table index. 
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 
//    node_   																				     //

int calc1_parse_action::Start (int p) 
{
		char* sym;
		int i, x, sti, line;
	   i = pact_arg[p];									// Get first argument index.
		if (i >= 0)											// If an argument was specified in grammar.
		{
			x = arg_numb[i] - 1;							// Get parse-stack position. 
			sti = PS[x].sti;								// Get symbol-table index from parse stack.
			if (sti < 0) sym = term_symb[-sti];		// Get symbol from terminal symbols list.
			else         sym = symbol[sti].name;	// Get symbol from symbol table.
			line = PS[x].line;							// Get line number in input file.
			/*	
			** Code to do something goes here
			*/
		}																
      return 1; // OK
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 
//		node_																						  //

int calc1_node_action::emit (int n) 
{
      int a;
   // Get string for this status (0 = TOP_DOWN, 1 = PASS_OVER, 2 = BOTTOM_UP).
      a = nact_arg [node[n].prod] + status;	// Has to be a valid number, always have 3 arguments. 
      if (arg_numb[a] >= 0)						// If current arg_numb not -1 (empty).
      {
			emitstr (n, arg_text [arg_numb[a]]);
      }
		return 1; // OK
}

/*--- End of Actions. -------------------------------------------------------*/

























