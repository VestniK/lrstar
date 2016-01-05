
/* Source code with tabs = 3 */

#include "calc_parser.h"
#include "calc_actions.h"

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 
//    error     																				  //

int   calc_token_action::error (int& t)                    
{
		if (token.end == token.start) // Illegal character?
		{
			token.end++;
		}
      return (0); 
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 
//    lookup    																				  //

int   calc_token_action::lookup (int& t) // Look for Symbol.
{
		int sti;														// Symbol-table index.
		sti = add_symbol (t, token.start, token.end);	// Add to symbol table. 
		t   = symbol[sti].term;									//	Redefine terminal number (need this for typedef names). 
		return (sti); // Return symbol-table index. 
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 
//    program_   																				  //

int calc_parse_action::pgmname_ (int p) 
{																						  
	   int i = pact_arg[p];						// Get first argument index.
      int sti = PS [arg_numb[i]-1].sti;		// Get sti for first argument.
      symbol[sti].term = arg_numb[i+1];		// Set term to second argument.
//	  	if (arg_numb[i+2] >= 0)		  				// If symbol type specified ...
//		{
//	  		symbol[sti].type |= arg_numb[i+2];	// Set the symbol's type.
//		}
      return 1; // OK
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 

int calc_node_action::add_ (int n)
{
		switch (status)
		{
			case TOP_DOWN:	 break;
			case PASS_OVER: break;
			case BOTTOM_UP: emitstr (n, "\t\tADD\n"); break;
		}
      return 1; // OK
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 

int calc_node_action::div_ (int n)
{
		switch (status)
		{
			case TOP_DOWN:	 break;
			case PASS_OVER: break;
			case BOTTOM_UP: emitstr (n, "\t\tDIV\n"); break;
		}
      return 1; // OK
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 

int calc_node_action::then_ (int n)
{												
		switch (status)
		{
			case TOP_DOWN:	 emitstr (n, "\t\tBR NZ endif&1\nthen&1:\n"); break;						
			case PASS_OVER: break;
			case BOTTOM_UP: break;
		}
      return 1; // OK
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 

int calc_node_action::else_ (int n)
{
		switch (status)
		{
			case TOP_DOWN:	 emitstr (n, "\t\tBR Z endif&1 \nelse&1:\n"); break;
			case PASS_OVER: break;
			case BOTTOM_UP: break;
		}
      return 1; // OK
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 

int calc_node_action::then2_ (int n)
{												
		switch (status)
		{
			case TOP_DOWN:	 emitstr (n, "\t\tBR NZ else&1 \nthen&1:\n"); break;
			case PASS_OVER: break;
			case BOTTOM_UP: break;
		}
      return 1; // OK
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 

int calc_node_action::else2_ (int n)
{
		switch (status)
		{
			case TOP_DOWN:	 emitstr (n, "\t\tBR endif&1   \nelse&1:\n"); break;
			case PASS_OVER: break;
			case BOTTOM_UP: break;
		}
      return 1; // OK
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 

int calc_node_action::eq_ (int n)
{
		switch (status)
		{
			case TOP_DOWN:	 break;
			case PASS_OVER: break;
			case BOTTOM_UP: emitstr (n, "\t\tEQ\n"); break;
		}
      return 1; // OK
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 

int calc_node_action::ne_ (int n)
{
		switch (status)
		{
			case TOP_DOWN:	 break;
			case PASS_OVER: break;
			case BOTTOM_UP: emitstr (n, "\t\tNE\n"); break;
		}
      return 1; // OK
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 

int calc_node_action::goal_ (int n)
{
	//	int a = nact_arg [node[n].prod]; // Get first node action argument.
		switch (status)
		{
			case TOP_DOWN:	 emitstr (n, "\t\tSTART\n"); break;
			case PASS_OVER: break;
			case BOTTOM_UP: emitstr (n, "\t\tEOF\n"); break;
		}
      return 1; // OK
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 

int calc_node_action::ident_ (int n)
{
		switch (status)
		{
			case TOP_DOWN:	 break;
			case PASS_OVER: break;
			case BOTTOM_UP: emitstr (n, "\t\tLOAD %s\n"); break;
		}
      return 1; // OK
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 

int calc_node_action::if_ (int n)
{
		switch (status)
		{
			case TOP_DOWN:	 emitstr (n, "if&0:\n"); break;
			case PASS_OVER: break;
			case BOTTOM_UP: emitstr (n, "endif&0:\n"); break;
		}
      return 1; // OK
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 

int calc_node_action::int_ (int n)
{
		switch (status)
		{
			case TOP_DOWN:	 break;
			case PASS_OVER: break;
			case BOTTOM_UP: emitstr (n, "\t\tLOAD %s\n"); break;
		}
      return 1; // OK
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 

int calc_node_action::mul_ (int n)
{
		switch (status)
		{
			case TOP_DOWN:	 break;
			case PASS_OVER: break;
			case BOTTOM_UP: emitstr (n, "\t\tMUL\n"); break;
		}
      return 1; // OK
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 

int calc_node_action::program_ (int n)
{
		switch (status)
		{
			case TOP_DOWN:	 emitstr (n, "\t\tPROGRAM %s\n");break;
			case PASS_OVER: break;
			case BOTTOM_UP: emitstr (n, "\t\tEND PROGRAM %s\n"); break;
		}
      return 1; // OK
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 

int calc_node_action::store_ (int n)
{
		switch (status)
		{
			case TOP_DOWN:	 break;
			case PASS_OVER: break;
			case BOTTOM_UP: emitstr (n, "\t\tSTORE\n"); break;
		}
      return 1; // OK
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 

int calc_node_action::sub_ (int n)
{
		switch (status)
		{
			case TOP_DOWN:	 break;
			case PASS_OVER: break;
			case BOTTOM_UP: emitstr (n, "\t\tSUB\n"); break;
		}
      return 1; // OK
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 

int calc_node_action::target_ (int n)
{
		switch (status)
		{
			case TOP_DOWN:	 break;
			case PASS_OVER: break;
			case BOTTOM_UP: emitstr (n, "\t\tLADR %s\n"); break;
		}
      return 1; // OK
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           // 

int calc_node_action::emit (int n) 
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

























