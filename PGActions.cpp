
#ifdef LRSTAR

		#include "ComGlobal.h"
      #include "PGGlobal.h"
      #include "PGActions.h"
      #include "PGParser.h"

		#define WIDTH 80

      static int   head_sti;
      static int   nodename_sti;
      static int   n_declared;
      static int   endblock  = 0;
		static char* codeblockstart;
		static int   codeblockline;
		static int   prod_numb = 0;

		#ifdef TOKEN_ACTIONS
																																				
		char  PGTokenAction::errormsg[256];
		int   PGTokenAction::head_column = 0;
		int   PGTokenAction::arrow_column = 0;
		int   PGTokenAction::arrow_length = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //

int   PGTokenAction::error (int t) // New version
{
      if (token.end == token.start) token.end++;
      return (t); // t = 0, usually (passed as argument)
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //

int   PGTokenAction::eof (int t) // New version
{
		if (codeblock) 
		{
			prt_error ("Code block starts here with { or %%{ but has no matching end } or %%} before the end-of-file", 
				codeblockstart, 0, codeblockline);
			Terminate(0);
		}
      return (t); // t = 0, a = 0 usually (passed as argument)
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    firstsym     																			  //

int   PGTokenAction::firstsym (int t) // New version
{
		int la, la_line;
		if (codeblock) return 0; // Return <error>
		if (line_pos == 1) // First symbol on this line?
		{
         if (goal_symbol == 0) // Goal not defined yet?
         {
				la = get_lookahead (la_line);
            if (la == COLON || la == ARROW1)
            {
               goal_symbol = 1;
					head_column = token.column;
               return (GOALSYMBOL);
            }
				return t;
         }
			if (prev_token == SEMICOLON)
			{
				head_column = token.column;
				return (HEADSYMBOL);
			}
			char *p;
			int blankline = 0;
			for (p = token.start; *p != '\n'; p--); // Find previous '\n'
			if (p > input_start)
			{	
				while (*--p == ' ' || *p == '\t');	// Bypass whitespace.
				if (*p == '\n') blankline = 1;		// Blank line?
			}
			la = get_lookahead (la_line);
			if (la == COLON || la == ARROW1 || la == ARROW2 || la == ARROW3 || la == ARROW4)
			{
				if (la_line == token.line) // Arrow is on the same line?
				{
					if (token.column > arrow_column)
					{
						prt_error ("%s is too far right or 'tab' option is wrong", token.start, token.end, token.line); 
						Terminate (0);
						return (0);
					}
					head_column = token.column;
					return (HEADSYMBOL);
				}
				else  // '->' is on the next line!
				{
					if (token.column < arrow_column + arrow_length + 1)	// If too far left?
					{
						if (blankline)
						{
							if (token.column <= head_column || token.column <= arrow_column)	// Symbol looks like a head position?
							{
								head_column = token.column;
								return (HEADSYMBOL);
							}
						}
						if (token.column <= head_column)	// Symbol looks like a head position?
						{
							head_column = token.column;
							return (HEADSYMBOL);
						}
						prt_error ("%s has ambiguous position or 'tab' option is wrong. If tail: move right.\nIf head: move left, or preceed with ';', or follow with '->' on the same line", 
						token.start, token.end, token.line); 
						Terminate (0);
						return (0);
					}
					else // Tail symbol position!
					{
						if (blankline) // Preceeded by a blank line?
						{
							prt_error ("%s looks like a tail symbol continuing the previous rule, but is preceeded by a blank line", 
							token.start, token.end, token.line); 
							Terminate (0);
							return (0);
						}
						return t;
					}
				}
			}
			if (blankline) // Preceeded by a blank line?
			{
				prt_error ("%s looks like a tail symbol continuing the previous rule, but is preceeded by a blank line", 
				token.start, token.end, token.line); 
				Terminate (0);
				return (0);
			}
			if (token.column <= head_column)
			{
				prt_error ("%s looks like a tail symbol but is too far left or 'tab' option is wrong", token.start, token.end, token.line); 
				Terminate (0);
				return (0);
			}
		}
		return t;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    arrow                                                                  //

int   PGTokenAction::arrow (int t) // New version
{
		if (codeblock) return 0; // Return <error>
		if (t == BAR) // '|' symbol 
		{
			if (line_pos == 1) 
			{
				arrow_length = 1;
				arrow_column = token.column;  // If parser did not do a lookahead, otherwise bug here.
			}
			return (t);
		}
		if (goal_symbol == 0)
      {
			prt_error ("Goal symbol is not defined yet", token.start, token.end, token.line);
			Terminate (0);
			return 0;
      }
		arrow_length = strlen (term_symb[t]) - 2;	// Subtract the single quotes.
		arrow_column = token.column;
		if (line_pos == 1) 
		{
			return (t);
		}
		if (line_pos == 2)
		{
			if (prev_token == HEADSYMBOL || prev_token == GOALSYMBOL) 
			{
				return (t);
			}
		}
		prt_error ("'%s' must be the first symbol on a line or second (if preceeded by a head symbol)", 
		token.start, token.end, token.line);
		Terminate (0);
		return 0;
}

#endif
#ifdef PARSE_ACTIONS

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //

		int   PGParseAction::STI;
      char  PGParseAction::errormsg[256];
		int   PGParseAction::production_length;

/////////////////////////////////////////////////////////////////////////////////////////////////////
//

int   PGParseAction::arrow_ (int prod)
{
      int x = arg_numb [pact_arg[prod]] - 1; // Argument points at location x in parse stack.
		PGTokenAction::arrow (-PS[x].sti);		// sti = - terminal symbol number.
      return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//

int   PGParseAction::startdecl_ (int prod) // %start for yacc
{
      int a = pact_arg[prod];			// first argument index.
      int x = arg_numb[a] - 1;	// parse stack index.
      int sti = add_symbol (0, PS[x].start, PS[x].end, TERMINAL|NONTERMINAL, PS[x].line);
      if (symbol[sti].type & TERMINAL)    // Already a terminal?
      {
			prt_error ("Symbol '%s' has already been declared as a terminal", PS[x].start, PS[x].end, PS[x].line);
      }
      else if (symbol[sti].type & NONTERMINAL)  // Already a start symbol?
      {
			prt_error ("Symbol '%s' has already been declared as a start symbol", PS[x].start, PS[x].end, PS[x].line);
      }
		else if (symbol[sti].type & OPERATOR)
      {
			prt_error ("Symbol '%s' has already been declared as an operator", PS[x].start, PS[x].end, PS[x].line);
      }
		else if (symbol[sti].numb >= 0)
		{
			prt_error ("Symbol '%s' has already been declared somewhere", PS[x].start, PS[x].end, PS[x].line);
		}
		start_symbol = sti;  // define the yacc start symbol. 
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::goalsymbol_ (int p)  // LRSTAR/YACC goal symbol.
{
		int sti;
		goal_symbol = 1;
		int x = arg_numb [pact_arg[p]] - 1;
		if (x == -1) // YaccStart goal symbol ($start) ?
		{
			sti = head_sti = add_symbol (0, "Goal", "Goal"+4, 0, 0);
			PS[0].sti   = sti;
			PS[0].start = symbol[sti].start;
			PS[0].end   = symbol[sti].start + symbol[sti].length;
		}
		else
		{
			sti = head_sti = PS[x].sti = add_symbol (0, PS[x].start, PS[x].end, TERMINAL|ARGUMENT|CONSTANT, PS[x].line);
			if (symbol[sti].type & TERMINAL)    // Already a terminal?
			{
				prt_error ("Symbol '%s' has already been defined as a terminal", PS[x].start, PS[x].end, PS[x].line);
			}
			else if (symbol[sti].type & CONSTANT)    // Already a constant?
			{
				prt_error ("Symbol '%s' has already been defined as a constant", PS[x].start, PS[x].end, PS[x].line);
			}
		}
		symbol[sti].type |= NONTERMINAL; // Make it a nonterminal.
		symbol[sti].numb  = N_heads;
		symbol[sti].value = N_heads++;	// Increment nonterminal counter.
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::startsymbol_ (int p)	// Yacc $start symbol.
{
		if (start_symbol == 0) // Start not defined yet?
		{
			prt_warning ("No start symbol has been defined, assuming %s is the start symbol", token.start, token.end, line_numb);
			start_symbol = add_symbol (0, token.start, token.end, 0, line_numb);
		}
		PS[0].sti   = start_symbol;
		PS[0].start = symbol[start_symbol].start;
		PS[0].end   = symbol[start_symbol].start + symbol[start_symbol].length;
      symbol[start_symbol].type |= TAIL;
		return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::endsymbol_ (int p)	// Yacc $end symbol.
{
      int sti = add_symbol (0, "<eof>", "<eof>"+5, 0, 0);
		PS[0].sti   = sti;
		PS[0].start = symbol[sti].start;
		PS[0].end   = symbol[sti].start + symbol[sti].length;
      symbol[sti].type |= TAIL;
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::colon_ (int p)	// Yacc ':' for goal production.
{
		PS[0].sti  = -COLON;
      return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//

int   PGParseAction::errordecl_ (int prod)
{
      int   x     = arg_numb [pact_arg[prod]]; // <error>
      char* start = term_symb[x];
      char* end   = term_symb[x] + strlen (term_symb[x]);
		if (*start == '\'') { start++; end--; }
      int   sti   = add_symbol (0, start, end, TERMINAL, 0);
      symbol[sti].type  = TERMINAL;
      symbol[sti].numb  = N_terms;		// = 0 (should be zero)
      symbol[sti].value = N_terms++;	// Increment terminal counter.
      return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//

int   PGParseAction::options_ (int prod)
{
		char *p, *opt, c;
		int  OK, rc, x, sti;
      rc = 0;
      x = arg_numb [pact_arg[prod]] - 1;
      if (x >= 0)	// Options line found in grammar?
      {
         sti = PS[x].sti = add_symbol (0, PS[x].start, PS[x].end, OPTIONSYM, PS[x].line);
         symbol[sti].type = OPTIONSYM;
         symbol[sti].numb = 0;
         p = symbol[sti].name;

		// Find option ...
         p++;                                   // bypass "
Top:	   while (*p == ' ' || *p == '\t' || *p == ',') p++;   // bypass spaces, tabs & commas.
         opt = p;
		   while (*p != '\n' && *p != 26 && *p != '"' && *p != ',' && *p != ' ') p++;
		   c = *p;
		   *p = 0;
		   OK = SET_OPTN (opt, grmfid, PS[x].line);
         *p++ = c;
	      if (!OK) rc = 1;  // fatal error
			if (c == ' ') goto Top;
			if (c == ',') goto Top;
      }

Done: if (rc > 0) return rc;
		rc = CheckOptions();
		if (rc == 0) return 1; // error
      N_tails     = 0;
      N_prods     = 0;
      N_terms     = 0;
      N_heads     = 0;
      N_generated = 0;
      N_nodes     = 0;
      N_tacts     = 0;
      N_pacts     = 0; 
      N_nacts     = 0;
      N_args      = 0;
      N_ints      = 0;
      N_strings   = 0;
      n_declared  = 0;
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::CheckOptions ()
{
		if (optn[PG_QUIET])					optn[PG_VERBOSE]	  = 0;	  
      if (optn[PG_REDUCEONLY])			optn[PG_CONFLICTS]  = 1;
      if (optn[PG_CONFLICTTRACE])		optn[PG_CONFLICTS]  = 1;
      if (optn[PG_ERRORCOUNT] ==  0)	optn[PG_ERRORCOUNT] = 1;
      if (optn[PG_ERRORCOUNT] > 1000)	optn[PG_ERRORCOUNT] = 1000;

		if (optn[PG_TAB] < 2)	
		{
         n_errors++;
         printf ("%s(%04d) : Tab setting cannot be less than 2.\n", grmfid, 1);
         return (0);
		}
		if (optn [PG_TAB] > 4)	
		{
         n_errors++;
         printf ("%s(%04d) : Tab setting cannot be greater than 4.\n", grmfid, 1);
         return (0);
		}

		PGLexer::tab = optn[PG_TAB]; // Set tab value in lexer !!!

		if (optn[PG_CLR_PARSER]) 
		{
			optn[PG_LR_PARSER]   = 0;
			optn[PG_LALR_PARSER] = 0;
			optn[PG_SHIFTREDUCE] = 0;
			optn[PG_DEFAULTRED]  = 0;
			optn[PG_BOOLMATRIX]  = 1;
			if (optn[PG_NONDETER])
			{
				n_errors++;
	         printf ("%s(%04d) : 'nd' option not available with 'clr' option.\n", grmfid, 1);
				PG::Terminate(0);
   		}
		}
		else if (optn[PG_LALR_PARSER])
		{
			optn[PG_LR_PARSER] = 0;
		}

		max_errors      = optn[PG_ERRORCOUNT];
		option_grammar  = optn[PG_GRAMMAR];
		option_warnings = optn[PG_WARNINGS];
      return (1);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::constant_ (int p)
{
      int x = arg_numb [pact_arg[p]] - 1;
      int sti = PS[x].sti = add_symbol (0, PS[x].start, PS[x].end, CONSTANT|ARGUMENT|TERMINAL, PS[x].line);
      if (symbol[sti].numb >= 0) // Already defined?
      {
			if (symbol[sti].type & CONSTANT)
				prt_error ("Constant symbol '%s' has already been defined", PS[x].start, PS[x].end, PS[x].line);
			else if (symbol[sti].type & TERMINAL)
				prt_error ("Symbol '%s' has already been defined as a terminal", PS[x].start, PS[x].end, PS[x].line);
      }
      else
      {
         symbol[sti].type |= CONSTANT;
         symbol[sti].numb  = n_constants++;  // Increment terminal counter.
      }
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::value_ (int p)
{
      int x = arg_numb [pact_arg[p]] - 1;
      int sti = PS[x].sti = add_symbol (0, PS[x].start, PS[x].end, INTEGER, PS[x].line);
		if (n_declared > 0) 
		{
			prt_error ("Value '%s' must be assigned in the defined constants section above the declarations section", 
			PS[x].start, PS[x].end, PS[x].line);
		}
      if (symbol[sti].numb < 0)
      {
         symbol[sti].type  = INTEGER;
         symbol[sti].numb  = N_ints++;
         symbol[sti].value = atoi (symbol[sti].name); // define the value.
      }
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::terminal_ (int p)
{
      int x = arg_numb [pact_arg[p]] - 1;
      int sti = PS[x].sti = add_symbol (0, PS[x].start, PS[x].end, TERMINAL|ARGUMENT|CONSTANT, PS[x].line);
      if (symbol[sti].numb >= 0) // Already a declared or defined?
      {
			if (symbol[sti].type & CONSTANT)
			{
				prt_error ("'%s' has been defined as a constant and cannot be assigned to a constant", PS[x].start, PS[x].end, PS[x].line);
			}
			else if (symbol[sti].numb == 0)			// <error> symbol?
			{
				if (symbol[sti].line == 0)				// First declaration of <error>?
				{
		      // n_declared++;							// Don't count <error> among the declared, it ruins list_rest_of_terminals().
					symbol[sti].line = PS[x].line;	// Give it the line number.
				}
				else	goto Err;
			}
			else
			{
Err:			prt_error ("'%s' has already been declared", PS[x].start, PS[x].end, PS[x].line);
			}
      }
      else
      {
         symbol[sti].type |= TERMINAL;  // Arguments could come here, have to use |=.
         symbol[sti].numb  = N_terms;
         symbol[sti].value = N_terms++; // Increment terminal counter.
         n_declared++;
      }
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::terminalres_ (int p) // Newer version 20130518
{
      int x = arg_numb [pact_arg[p]] - 1;
      int sti = PS[x].sti = add_symbol (0, PS[x].start, PS[x].end, TERMINAL|ARGUMENT|CONSTANT, PS[x].line);
      if (symbol[sti].numb >= 0) // Already declared or defined?
      {
			if (symbol[sti].type & CONSTANT)
				prt_error ("'%s' has been defined as a constant, cannot be defined as a reserved keyword", PS[x].start, PS[x].end, PS[x].line);
			else
   			prt_error ("'%s' has already been declared", PS[x].start, PS[x].end, PS[x].line);
      }
      else
      {
         symbol[sti].type |= TERMINAL | RESERVED;  // Arguments could come here, have to use |=.
         symbol[sti].numb  = N_terms;
         symbol[sti].value = N_terms++; // Increment terminal counter.
         n_declared++;
      }
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::oper_ (int p)
{
      int x = arg_numb [pact_arg[p]] - 1;
      int sti = PS[x].sti = add_symbol (0, PS[x].start, PS[x].end, TERMINAL|ARGUMENT|CONSTANT, PS[x].line);
      if (symbol[sti].numb < 0) // New terminal or old argument
      {
         symbol[sti].type |= TERMINAL|OPERATOR;
         symbol[sti].numb  = N_terms;
         symbol[sti].value = N_terms++; // Increment terminal counter.
      }
		else if (symbol[sti].type & CONSTANT)
		{
			prt_error ("Constant symbol '%s' cannot used as an operator and given a precedence", PS[x].start, PS[x].end, PS[x].line);
		}
		else if (symbol[sti].type & OPERATOR)
		{
			prt_error ("Symbol '%s' has already been defined as an operator", PS[x].start, PS[x].end, PS[x].line);
		}
		else // ARGUMENT or TERMINAL.
		{
			symbol[sti].type |= TERMINAL|OPERATOR;
		}
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::headsymbol_ (int p)
{
      int x = arg_numb [pact_arg[p]] - 1;
      int sti = head_sti = PS[x].sti = add_symbol (0, PS[x].start, PS[x].end, TERMINAL|TAIL|NONTERMINAL|ARGUMENT|CONSTANT, PS[x].line);
   // printf ("-> headsymbol_ %s\n", symname(sti));
      if (symbol[sti].type & NONTERMINAL) // Already a nonterminal?
      {
			prt_error ("Nonterminal symbol '%s' has already been defined", PS[x].start, PS[x].end, PS[x].line);
      }
      else if (symbol[sti].type & TERMINAL)    // Already a terminal?
      {
			prt_error ("Symbol '%s' has already been defined as a terminal", PS[x].start, PS[x].end, PS[x].line);
      }
      else if (symbol[sti].type & CONSTANT)    // Already a constant?
      {
			prt_error ("Symbol '%s' has already been defined as a constant", PS[x].start, PS[x].end, PS[x].line);
      }
      else
      {
         symbol[sti].type |= NONTERMINAL; // Make it a nonterminal.
         symbol[sti].numb  = N_heads;
         symbol[sti].value = N_heads++;	// Increment nonterminal counter.
      }
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::tailkw_ (int p)
{
      tail_(p);
      keyword_sym = PS[arg_numb [pact_arg[p]]-1].sti;
      symbol[keyword_sym].type |= NONTERMINAL;
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::prod_ (int p)
{
		production_length = 0;
		nodename_sti = 0;
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::init_code_start_ (int prod)
{
		char* p = token.end;		 
		codeblock = 1;
		codeblockstart = p;
		codeblockline  = line_numb;

Scan:	while (*p != 26 && *p != '\n') p++;
		if (*p == '\n') 
		{
			p++;
			line_numb++;
			if (*p == '%')
			{
				p++;
				if (*p == '}')
				{
					p++;
					token.end = p;
					codeblock = 0;
					return 0;
				}
			}
			goto Scan;
		}
		if (*p == 26) // End of file?
		{
			token.end = p;
			return 0; 
		}
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::code_start_ (int prod)
{
		int   level = 1;
		char* p = token.end;
		codeblock = 1;
		codeblockstart = p;
		codeblockline  = line_numb;

Scan: while (*p != '{' && *p != '}' && *p != '\n' && *p != 26 && *p != '/' && *p != '\'' && *p != '"') p++;
		switch (*p)
		{
			case '{':
			{
				p++;
				level++;
				goto Scan;
			}
			case '}':
			{
				p++;
				if (--level) goto Scan;
				token.end = p;
				codeblock = 0;
				return 0;
			}
			case '\n':
			{
				p++;
				line_numb++;
				goto Scan;
			}
			case  26:
			{
				token.end = p;
				return 0;
			}
			case  '/':
			{
				p++;
				if (*p == '/')
				{
					p++;
					while (*p != '\n') p++;
					p++;
					line_numb++;
					goto Scan;
				}
				if (*p == '*')
				{
					p++;
Scan2:			while (*p != '\n' && *p != 26 && *p != '*') p++;
					switch (*p)
					{
						case '\n': 
						{
							p++;
							line_numb++;
							goto Scan2;
						}
						case 26:
						{
							token.end = p;
							return 0;
						}
						case '*':
						{
							p++;
							if (*p == '/')
							{
								p++;
								goto Scan;
							}
							goto Scan2;
						}
					}
				}
				goto Scan;
			}
			case  '\'':
			{
				p++;
Scan3:		while (*p != '\n' && *p != 26 && *p != '\\' && *p != '\'') p++;
				switch (*p)
				{
					case '\n':
					{
						token.end = p;
						return 0;
					}
					case 26:
					{
						token.end = p;
						return 0;
					}
					case '\\':
					{	
						p += 2;
						goto Scan3;
					}
					case '\'':
					{	
						p++;
						goto Scan;
					}
				}
			}
			case  '"':
			{
				p++;
Scan4:		while (*p != '\n' && *p != 26 && *p != '\\' && *p != '"') p++;
				switch (*p)
				{
					case '\n':
					{
						token.end = p;
						return 0;
					}
					case 26:
					{
						token.end = p;
						return 0;
					}
					case '\\':
					{	
						p += 2;
						goto Scan4;
					}
					case '"':
					{	
						p++;
						goto Scan;
					}
				}
			}
		}
		return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::end_code_start_ (int prod)
{
		char* p = token.end;
    	while (*p != 26) p++;
		token.end = p;
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::tailposition_ (int p)
{
		production_length++;
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::prec_ (int p)
{
      int x = arg_numb [pact_arg[p]] - 1;
      int sti = add_symbol (0, PS[x].start, PS[x].end, TERMINAL|NONTERMINAL|TAIL|CONSTANT|ARGUMENT, PS[x].line);
      if (symbol[sti].type & NONTERMINAL) // NT symbol?
      {
			prt_error ("Nonterminal symbol '%s' cannot be used as a precedence symbol", PS[x].start, PS[x].end, PS[x].line);
      }
      symbol[sti].type |= TAIL;
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::tail_ (int p)
{
      int x = arg_numb [pact_arg[p]] - 1;
      int sti = PS[x].sti = add_symbol (0, PS[x].start, PS[x].end, TERMINAL|NONTERMINAL|TAIL|CONSTANT|ARGUMENT, PS[x].line);
      if (symbol[sti].numb == 0 && symbol[sti].type & NONTERMINAL) // Goal symbol?
      {
			prt_error ("Goal symbol '%s' cannot be used in grammar rules", PS[x].start, PS[x].end, PS[x].line);
      }
      else if (symbol[sti].type & CONSTANT)
      {
			prt_error ("Constant symbol '%s' cannot be used in grammar rules", PS[x].start, PS[x].end, PS[x].line);
      }
      symbol[sti].type |= TAIL;
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::sep_ (int p)
{
      int x = arg_numb [pact_arg[p]] - 1;
      int sti = PS[x].sti = add_symbol (0, PS[x].start, PS[x].end, TERMINAL|NONTERMINAL|TAIL|ARGUMENT|CONSTANT, PS[x].line);
      if (symbol[sti].numb == 0 && symbol[sti].type & NONTERMINAL) // Goal symbol?
      {
			prt_error ("Goal symbol '%s' cannot be used in grammar rules", PS[x].start, PS[x].end, PS[x].line);
      }
      else if (symbol[sti].type & CONSTANT)    // Already a constant?
      {
			prt_error ("Symbol '%s' has been defined as a constant, cannot be used in grammar rules", PS[x].start, PS[x].end, PS[x].line);
      }
      symbol[sti].type |= TAIL;
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::token_action_spec_ (int p)
{
      int x = arg_numb [pact_arg[p]] - 1;
      int sti = PS[x].sti = add_symbol (0, PS[x].start, PS[x].end, TACTION, PS[x].line);
      if (symbol[sti].numb < 0)
      {
         symbol[sti].type = TACTION;
         symbol[sti].numb = N_tacts++;
      }
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::parse_action_spec_ (int p)
{
      int x = arg_numb [pact_arg[p]] - 1;
      int sti = PS[x].sti = add_symbol (0, PS[x].start, PS[x].end, PACTION, PS[x].line);
      if (symbol[sti].numb < 0)
      {
         symbol[sti].type = PACTION;
         symbol[sti].numb = N_pacts++;
      }
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::blank_parse_action_ (int p)
{
	   int x, sti;
      x   = arg_numb [pact_arg[p]] - 1;
		sti = add_symbol (0, symbol[nodename_sti].name, symbol[nodename_sti].name + symbol[nodename_sti].length, PACTION, PS[x].line);
      PS[x].sti   = sti;
      PS[x].start = symbol[sti].name;
      PS[x].end   = symbol[sti].name + symbol[sti].length;
      if (symbol[sti].numb < 0)
      {
         symbol[sti].type = PACTION;
         symbol[sti].numb = N_pacts++;
      }
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::parse_action_make_node_ (int p)
{
		if (optn[PG_ASTCONST] == 0) 
		{
			return 0;
		}
		return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::make_node_ (int p)
{
		if (optn[PG_ASTCONST] == 0) 
		{
			return -1; // Don't make node. 
		}
		return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::parse_action_make_node_wa_ (int p)
{
		if (optn[PG_ASTCONST] == 0) 
		{
			return 0;
		}
		return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::make_node_wa_ (int p)
{
		if (optn[PG_ASTCONST] == 0) 
		{
			if (nodename_sti == 0) return 0;
			return -1; // Don't make node. 
		}
		return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::blank_node_action_ (int p)
{
		if (optn[PG_ASTCONST] == 0) return -1; // Don't make node. 
		return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::node_action_name_ (int p)
{
		if (optn[PG_ASTCONST] == 0) return -1; // Don't make node. 
		return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::node_name_ (int p) // +> or *> operator
{
		if (optn[PG_ASTCONST] == 0) 
		{
			nodename_sti = -1;	// Ignore node name, ingnore args.
			return -1;				// Don't make a node for this node name.
		}
		return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::pa_node_name_ (int p) // =+> or =*> operator
{
		if (optn[PG_ASTCONST] == 0) 
		{
			nodename_sti = 0;	// Node name is not used anymore.
			return 0;			// Make a node for this node name.
		}
		return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::parse_action_name_ (int p)
{
		return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::token_action_name_ (int p)
{
		return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::pa_node_spec_ (int p)
{
		if (nodename_sti == -1) 
		{
			return -1; 
		}
		else
		{
			int x = arg_numb [pact_arg[p]] - 1;
			int sti = nodename_sti = PS[x].sti = add_symbol (0, PS[x].start, PS[x].end, NODENAME, PS[x].line);
			if (symbol[sti].numb < 0)
			{
				symbol[sti].type = NODENAME;
				symbol[sti].numb = N_nodes++;
			}
		}
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::node_spec_ (int p)
{
		if (nodename_sti == -1) 
		{
			return -1; 
		}
		else
		{
			int x = arg_numb [pact_arg[p]] - 1;
			int sti = nodename_sti = PS[x].sti = add_symbol (0, PS[x].start, PS[x].end, NODENAME, PS[x].line);
			if (symbol[sti].numb < 0)
			{
				symbol[sti].type = NODENAME;
				symbol[sti].numb = N_nodes++;
			}
		}
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::blank_node_name_ (int p)
{
		if (optn[PG_ASTCONST] == 0) 
		{
			nodename_sti = -1;	// Disable this.
			return -1;				// Don't make node.
		}
      int sti = nodename_sti = add_symbol (0, symbol[head_sti].start, symbol[head_sti].start + symbol[head_sti].length, NODENAME, 0);
      PS[0].sti = sti;
      PS[0].start = symbol[sti].name;
      PS[0].end   = symbol[sti].name + symbol[sti].length;
      PS[0].line  = line_numb;
      if (symbol[sti].numb < 0)
      {
         symbol[sti].type = NODENAME;
         symbol[sti].numb = N_nodes++;
      }
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::pa_blank_node_name_ (int p)
{
		if (optn[PG_ASTCONST] == 0) 
		{
			int nodename_sti = add_symbol (0, symbol[head_sti].start, symbol[head_sti].start + symbol[head_sti].length, NODENAME, 0);
			PS[0].sti = 0;
		}
		else
		{
			int sti = nodename_sti = add_symbol (0, symbol[head_sti].start, symbol[head_sti].start + symbol[head_sti].length, NODENAME, 0);
			PS[0].sti = sti;
			PS[0].start = symbol[sti].name;
			PS[0].end   = symbol[sti].name + symbol[sti].length;
			PS[0].line  = line_numb;
			if (symbol[sti].numb < 0)
			{
				symbol[sti].type = NODENAME;
				symbol[sti].numb = N_nodes++;
			}
		}
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::node_action_ (int p)
{
		if (optn[PG_ASTCONST] == 0) 
		{
			nodename_sti = -1;	// Disable this.
			return -1;				// Don't make node.
		}
      int x = arg_numb [pact_arg[p]] - 1;
      int sti = PS[x].sti = add_symbol (0, PS[x].start, PS[x].end, NACTION, PS[x].line);
      if (symbol[sti].numb < 0)
      {
         symbol[sti].type = NACTION;
         symbol[sti].numb = N_nacts++;
      }
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::blank_node_action_name_ (int p)
{
		if (optn[PG_ASTCONST] == 0) 
		{
			nodename_sti = -1;	// Disable this.
			return -1;				// Don't make node.
		}
      int x = arg_numb [pact_arg[p]] - 1;
      int sti = add_symbol (0, symbol[nodename_sti].name, symbol[nodename_sti].name + symbol[nodename_sti].length, NACTION, PS[x].line);
      PS[x].sti = sti;
      PS[x].start = symbol[sti].name;
      PS[x].end   = symbol[sti].name + symbol[sti].length;
      if (symbol[sti].numb < 0)
      {
         symbol[sti].type = NACTION;
         symbol[sti].numb = N_nacts++;
      }
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::args_ (int p)
{
		if (nodename_sti == -1)  return -1;	// Don't make node.
		return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::arg_first_ (int p)
{
		if (nodename_sti == -1)  return -1;	// Don't make node.
		if (symbol[STI].type & INTEGER)		// Current arg is an integer?
		{
			if (symbol[STI].value > 1 && symbol[STI].value > production_length)
			{
				prt_warning ("First argument is greater than 1 and not pointing at a tail symbol", symbol[STI].name, 0, line_numb);
			}
		}
		else
		{
			prt_warning ("First argument is not an integer (pointing at a tail symbol)", symbol[STI].name, 0, line_numb);
		}
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::arg_alpha_ (int p)
{
		if (nodename_sti == -1)  return -1;	// Don't make node.
      int x = arg_numb [pact_arg[p]] - 1;
      int sti = PS[x].sti = add_symbol (0, PS[x].start, PS[x].end, TERMINAL|NONTERMINAL|TAIL|ARGUMENT|CONSTANT, PS[x].line);
      symbol[sti].type |= ARGUMENT;
      N_args++;
		STI = sti;
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::arg_terminal_ (int p)
{
		if (nodename_sti == -1)  return -1;	// Don't make node.
      int x = arg_numb [pact_arg[p]] - 1;
      int sti = PS[x].sti = add_symbol (0, PS[x].start, PS[x].end, TERMINAL|ARGUMENT|TAIL, PS[x].line);
      symbol[sti].type |= ARGUMENT;
      N_args++;
		STI = sti;
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::arg_integer_ (int p)
{
		if (nodename_sti == -1)  return -1;	// Don't make node.
      int x = arg_numb [pact_arg[p]] - 1;
      int sti = PS[x].sti = add_symbol (0, PS[x].start, PS[x].end, INTEGER|ARGUMENT, PS[x].line);
      if (symbol[sti].numb < 0) // New integer or old argument?
      {
         symbol[sti].type |= INTEGER|ARGUMENT;
         symbol[sti].numb  = N_ints++;
         symbol[sti].value = atoi (symbol[sti].name);
      }
      else // Old integer
		{
			symbol[sti].type |= ARGUMENT;
		}
      N_args++;
		STI = sti;
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::arg_string_ (int p)
{
		if (nodename_sti == -1)  return -1;	// Don't make node.
      int x = arg_numb [pact_arg[p]] - 1;
      int sti = PS[x].sti = add_symbol (0, PS[x].start, PS[x].end, STRING|ARGUMENT, PS[x].line);
      if (symbol[sti].numb < 0)
      {
         symbol[sti].type  = STRING|ARGUMENT;
         symbol[sti].numb  = N_strings;
         symbol[sti].value = N_strings++;
      }
      else // Old string
		{
			symbol[sti].type |= ARGUMENT;
		}
      N_args++;
		STI = sti;
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::arg_null_ (int p)
{
		if (nodename_sti == -1)  return -1;	// Don't make node.
      N_args++;
		STI = 0;
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::arg_expr_ (int p)
{
		if (nodename_sti == -1)  return -1;	// Don't make node.
      N_args++;
		STI = 0;
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::arg_expr_alpha_ (int p)
{
		if (nodename_sti == -1)  return -1;	// Don't make node.
      int x = arg_numb [pact_arg[p]] - 1;
      int sti = PS[x].sti = add_symbol (0, PS[x].start, PS[x].end, TERMINAL|NONTERMINAL|TAIL|ARGUMENT|CONSTANT, PS[x].line);
      symbol[sti].type |= ARGUMENT;
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGParseAction::arg_expr_integer_ (int p)
{
		if (nodename_sti == -1)  return -1;	// Don't make node.
      int x = arg_numb [pact_arg[p]] - 1;
      int sti = PS[x].sti = add_symbol (-PS[x].sti, PS[x].start, PS[x].end, INTEGER|ARGUMENT, PS[x].line);
      if (symbol[sti].numb < 0)
      {
         symbol[sti].type  = INTEGER|ARGUMENT;
         symbol[sti].numb  = N_ints++;
         symbol[sti].value = atoi (symbol[sti].name);
      }
      else // Old expr integer
		{
			symbol[sti].type |= ARGUMENT;
		}
      return 0;
}

#endif
#ifdef NODE_ACTIONS

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //

      int   	PGNodeAction::linelength;
      int   	PGNodeAction::prevtail;
      int   	PGNodeAction::nodesti;
      int   	PGNodeAction::arrow;
      char* 	PGNodeAction::oper;
      int   	PGNodeAction::assoc;
      int   	PGNodeAction::constant;
      int   	PGNodeAction::generated;
      int   	PGNodeAction::precedence;
      int   	PGNodeAction::n_values;
      int   	PGNodeAction::terminal_mode;
      int   	PGNodeAction::separator;
      int   	PGNodeAction::max_prec;
	   char**   PGNodeAction::term_start;
		char**   PGNodeAction::term_end;
		int*     PGNodeAction::term_type;
      char**   PGNodeAction::Int_start;
      int*     PGNodeAction::Tail;
		int*     PGNodeAction::F_tail;
		int*     PGNodeAction::head_sym;
		int*     PGNodeAction::prod_len;
		char**   PGNodeAction::head_name;
		char**   PGNodeAction::term_name;
		int*     PGNodeAction::term_line;
		int*     PGNodeAction::head_line;
		int*     PGNodeAction::prod_line;
		int*     PGNodeAction::nullable;
		int      PGNodeAction::n_nulls;

      Node* 	PGNodeAction::lastnonterm;
      Node* 	PGNodeAction::lastprod;
      Node* 	PGNodeAction::lasttails;
      Node* 	PGNodeAction::lasttail;


/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //

int   PGNodeAction::start_ (Node* np)
{
      static int n_tails_allocated;
		if (pass == SECOND_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               get_last_nonterminal();
               oper  = "...";
               N_tails = 0;
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
               break;
		   }
      }
		else if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               number_the_terminals();
               add_quotes();
               make_keyword_nonterminal();
               n_tails_allocated = N_tails;
		         alloc_struc();
               N_args  = 0;
               N_prods = 0;
               N_tails = 0;
					generated  = 0;
               precedence = 0;
					n_values   = 0;
					terminal_mode = 1;
               memset (string, ' ', sizeof(string));
               linelength = 0;
               prevtail   = 0;
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
               F_prod[N_heads] = N_prods;
               F_tail[N_prods] = N_tails;
               if (N_tails > n_tails_allocated)
                  InternalError(33);
               fill_struc();
               print_struc();
               max_prec = precedence;
               break;
		   }
      }
		return (0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //

void  PGNodeAction::get_last_nonterminal()
{
      Node* np;
      np = root;        // start
      np = np->child;   // grammar
      np = np->child;   // options
      np = np->next;    // declarations
      np = np->next;    // operator precedence
      np = np->next;    // rules
      np = np->child;   // nonterminal
      while (np->next != NULL)
      {
         np = np->next;
      }
      lastnonterm = np;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //
//    Assign sequence numbers to the terminal tail symbols (not declared).
/*
		Note that the classification changes here.  TERMINAL meant declared. 
		But now we have to mark all terminals as TERMINAL and so we mark the 
      declared as DECLARED and leave the undeclared as just TERMINAL.		
*/
void  PGNodeAction::number_the_terminals()
{
      int i;
      for (i = 0; i < n_symbols; i++)
      {
         if (symbol[i].type & TAIL)										// In the grammar?
         {
            if (!(symbol[i].type & NONTERMINAL))					// Not a nonterminal?
            {
               if (!(symbol[i].type & TERMINAL))					// Not declared as terminal?
               {
                  symbol[i].type |= TERMINAL|UNDECLARED;			// Make it a terminal & undeclared.
                  symbol[i].numb = symbol[i].value = N_terms++;
               }
            }			  
         }
      }

      for (i = 0; i < n_symbols; i++)
      {
         if (symbol[i].type & ARGUMENT)								// In the arguments?
         {
            if (!(symbol[i].type & NONTERMINAL))					// Not a nonterminal?
            {
               if (!(symbol[i].type & CONSTANT))					// Not constant?
               {
                  if (!(symbol[i].type & STRING))					// Not string?
                  {
                     if (!(symbol[i].type & INTEGER))				// Not string?
                     {
                        if (!(symbol[i].type & TERMINAL))		// Not declared?
                        {
                           symbol[i].type |= TERMINAL|UNDECLARED;
                           symbol[i].numb = symbol[i].value = N_terms++;
                        }
                     }
                  }
               }
            }
         }
      }

   // The following has to be here, so that list_rest_of_terminals(), called in operprec_(), will not crash.
      ALLOC (term_name,  N_terms);
      ALLOC (term_start, N_terms);
      ALLOC (term_end,   N_terms);
      ALLOC (term_type,  N_terms);
      ALLOC (term_line,  N_terms);

      for (i = 0; i < n_symbols; i++)
      {
         if (symbol[i].type & TERMINAL)
         {
            term_name [symbol[i].numb] = symbol[i].name;
            term_start[symbol[i].numb] = symbol[i].start;
            term_end  [symbol[i].numb] = symbol[i].start + symbol[i].length;
            term_type [symbol[i].numb] = symbol[i].type;
            term_line [symbol[i].numb] = symbol[i].line;
         }
      }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //
//    alloc_struc: allocate data structures.

void  PGNodeAction::alloc_struc ()
{
      ALLOC (head_name,    N_heads);
      ALLOC (head_line,    N_heads);
      ALLOC (prod_line,    N_prods);
      ALLOC (Reverse,      N_prods);
      ALLOC (Prec,         N_prods);
      ALLOC (F_prod,       N_heads+1);
      ALLOC (F_tail,       N_prods+1);
      ALLOC (Tail,         N_tails);

      ALLOC (Tact_start,   N_tacts+1);
      ALLOC (Pact_start,   N_pacts+1);
      ALLOC (Node_start,   N_nodes+1);
      ALLOC (Nact_start,   N_nacts+1);
      ALLOC (Oper_prec,    N_terms);
      ALLOC (Tact_numb,    N_terms);
      ALLOC (Pact_numb,    N_prods);
      ALLOC (Node_numb,    N_prods);
      ALLOC (Nact_numb,    N_prods);
      ALLOC (F_targ,       N_terms);
      ALLOC (F_parg,       N_prods);
      ALLOC (F_narg,       N_prods);
      ALLOC (Arg_numb,     N_args+1);
      ALLOC (Str_start,    N_strings+1);
      ALLOC (Int_start,    N_ints+1);
      ALLOC (Defcon_name,  n_constants+1);
      ALLOC (Defcon_value, n_constants+1);

      FASTINI( 0, Oper_prec, N_terms);
      FASTINI(-1, Tact_numb, N_terms);
      FASTINI(-1, Pact_numb, N_prods);
      FASTINI(-1, Node_numb, N_prods);
      FASTINI(-1, Nact_numb, N_prods);
      FASTINI(-1, F_targ,    N_terms);
      FASTINI(-1, F_parg,    N_prods);
      FASTINI(-1, F_narg,    N_prods);

      make_ast = 0;
      if (N_nodes > 0 && optn[PG_ASTCONST] > 0) make_ast = 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //
//    fill_struc: fill the data structures with values.

void  PGNodeAction::fill_struc ()
{
      int i;
      for (i = 0; i < n_symbols; i++)
      {
         if (symbol[i].type & CONSTANT)
         {
            Defcon_name [symbol[i].numb] = symbol[i].name;
            Defcon_value[symbol[i].numb] = symbol[i].value;
         }
      }
      for (i = 0; i < n_symbols; i++)
      {
         if (symbol[i].type & NONTERMINAL)
         {
            head_name[symbol[i].numb] = symbol[i].name;
         }
      }
      for (i = 0; i < n_symbols; i++)
      {
         if (symbol[i].type & TACTION)
         {
            Tact_start[symbol[i].numb] = symbol[i].name;
         }
      }
      for (i = 0; i < n_symbols; i++)
      {
         if (symbol[i].type & NODENAME)
         {
            Node_start[symbol[i].numb] = symbol[i].name;
         }
      }
      for (i = 0; i < n_symbols; i++)
      {
         if (symbol[i].type & PACTION)
         {
            Pact_start[symbol[i].numb] = symbol[i].name;
         }
      }
      for (i = 0; i < n_symbols; i++)
      {
         if (symbol[i].type & NACTION)
         {
            Nact_start[symbol[i].numb] = symbol[i].name;
         }
      }
      for (i = 0; i < n_symbols; i++)
      {
         if (symbol[i].type & STRING)
         {
            Str_start[symbol[i].numb] = symbol[i].name;
         }
      }
      for (i = 0; i < n_symbols; i++)
      {
         if (symbol[i].type & INTEGER)
         {
            Int_start[symbol[i].numb] = symbol[i].name;
         }
      }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //
//    print_struc: print the data structures.

void  PGNodeAction::print_struc ()
{
      if (optn[PG_VERBOSE] == 2)
      {
         int i;
         prt_lst ("\nConstants\n");
         for (i = 0; i < n_constants; i++)
         {
            prt_lst ("%5d %s\n", i, Defcon_name[i]);
         }
         prt_lst ("\nNonterminals\n");
         for (i = 0; i < N_heads; i++)
         {
            prt_lst ("%5d %s\n", i, head_name[i]);
         }
         prt_lst ("\nTerminals\n");
         for (i = 0; i < N_terms; i++)
         {
            prt_lst ("%5d %s\n", i, term_name[i]);
         }
         prt_lst ("\nTerminal Actions\n");
         for (i = 0; i < N_tacts; i++)
         {
            prt_lst ("%5d %s\n", i, Tact_start[i]);
         }
         prt_lst ("\nParse Actions\n");
         for (i = 0; i < N_pacts; i++)
         {
            prt_lst ("%5d %s\n", i, Pact_start[i]);
         }
         prt_lst ("\nNodes\n");
         for (i = 0; i < N_nodes; i++)
         {
            prt_lst ("%5d %s\n", i, Node_start[i]);
         }
         prt_lst ("\nNode Actions\n");
         for (i = 0; i < N_nacts; i++)
         {
            prt_lst ("%5d %s\n", i, Nact_start[i]);
         }
         prt_lst ("\nStrings\n");
         for (i = 0; i < N_strings; i++)
         {
            prt_lst ("%5d %s\n", i, Str_start[i]);
         }
         prt_lst ("\nIntegers\n");
         for (i = 0; i < N_ints; i++)
         {
            prt_lst ("%5d %s\n", i, Int_start[i]);
         }
         prt_lst ("\nArguments\n");
         for (i = 0; i < N_args; i++)
         {
            prt_lst ("%5d %5d\n", i, Arg_numb[i]);
         }
         prt_lst ("\n");
      }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //

void  PGNodeAction::list_rest_of_terminals()
{
      int i, n;
		if (n_declared) prt_grm ("\n");
		if (optn[PG_GRAMMAR] == 1)
		{
			n = 0;
			for (i = n_declared+1; i < N_terms; i++)
			{
				n++;
				prt_grm ("%4d       %-25s ;\n", i, term_name[i]);
			}
			if (n) prt_grm ("\n");
		}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //

void  PGNodeAction::make_keyword_nonterminal()
{
      int i;
      if (keyword_sym)
      {
			N_generated++;
         symbol[keyword_sym].type |= NONTERMINAL|GENERATED|TAIL;
         symbol[keyword_sym].numb = N_heads++;
         make_head(keyword_sym);
         for (i = 0; i < n_symbols; i++)
         {
            if (symbol[i].type & TERMINAL)
            {
					if (itsakeyword(symbol[i].name))
					{
						if (!(symbol[i].type & RESERVED))
						{
							make_prod(ARROW1);
							make_tail (i);
							N_prods++;
							N_tails++;
						}
					}   
            }
         }
      }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //

int   PGNodeAction::grammar_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:	 prt_grm ("\n/* %s grammar */\n\n", gfn); break;
			   case PASS_OVER: break;
			   case BOTTOM_UP: prt_grm ("\n/* End of %s grammar */\n\n", gfn); break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::options_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
					prt_grm ("/* Options */\n\n");
					if (np->sti > 0)
					{
						prt_sym ("\t", np, "\n\n", 0);
					}
               break;
   		   case PASS_OVER: break;
			   case BOTTOM_UP: break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::constdecls_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
            case TOP_DOWN:
               prt_grm ("/* Constant Declarations */\n\n");
               linelength = 0;
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
               if (np->child) prt_grm ("\n");
               break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::terminaldecls_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
            case TOP_DOWN:
               prt_grm ("/* Terminal Declarations */\n\n");
               linelength = 0;
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
               if (np->child) prt_grm ("\n");
               break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::operprec_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               list_rest_of_terminals();
               prt_grm ("/* Operator Precedence */\n\n");
               break;
			   case PASS_OVER:
               break;
			   case BOTTOM_UP:
               if (np->child) prt_grm ("\n");
               break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::declarations_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               prt_grm ("/* Declarations */\n\n");
               linelength = 0;
               break;
			   case PASS_OVER:
               break;
			   case BOTTOM_UP:
            // if (np->child) prt_grm ("\n");
               break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::constdecl_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:  break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
					constant = 0;
               prt_grm (";\n");
               linelength = 0;
               break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::token_decl_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               char buffer[16];
					if (terminal_mode == 1) 
					{	
						terminal_mode = 2;
						if (n_values) prt_grm ("\n");
					}
					linelength = 0;
					np = np->child;
					if (np != NULL)
					{
						if (np->id == CONSTANT_) np = np->next;
						if      (optn[PG_GRAMMAR] == 1) sprintf (buffer, "%4d       ", symbol[np->sti].numb);
						else if (optn[PG_GRAMMAR] >= 2) strcpy	(buffer, "\t");
						linelength += prt_grm (buffer);
					}
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
					constant = 0;
					np = np->child;
					if (np != NULL) prt_grm (";\n");
               linelength = 0;
               break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::constant_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         char* indent;
         switch (status)
		   {
			   case TOP_DOWN:
					constant = 1;
					if (optn[PG_GRAMMAR] == 1)
					{
						if (terminal_mode == 2) indent = "";
						else                    indent = "           ";
						prt_sym (indent, np, " ", 25);
					}
					if (optn[PG_GRAMMAR] >= 2)
					{
						if (terminal_mode == 2) indent = "";
						else                    indent = "\t";
						prt_sym (indent, np, " ", 25);
					}
               symbol[np->sti].value = symbol[np->next->sti].value; // Get value from value_ node.
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
               break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::value_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
					n_values++;
               prt_sym ("", np, "", 0);
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP: break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::terminal_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
					if (constant) prt_sym ("", np, " ", 25);
               else          prt_sym ("", np, " ", 25);
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP: break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::terminalres_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
					if (constant) prt_sym ("", np, " ", 25);
               else          prt_sym ("", np, " ", 25);
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP: break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::leftassoc_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
            case TOP_DOWN:
               linelength += prt_grm ("\t{ ");
               assoc = (++precedence);
               break;
			   case PASS_OVER: 
					break;
            case BOTTOM_UP: 
					linelength += prt_grm ("} <<\n"); 
					linelength = 0; 
					break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::rightassoc_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               linelength += prt_grm ("\t{ ");
               assoc = -(++precedence);
               break;
			   case PASS_OVER: 
					break;
				case BOTTOM_UP: 
					linelength += prt_grm ("} >>\n"); 
					linelength = 0; 
					break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::oper_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               prt_sym ("", np, " ", 0);
               Oper_prec[symbol[np->sti].numb] = assoc;
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP: break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::rules_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               prt_grm ("/* Rules */\n\n");
               linelength = 0;
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP: break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::goaldef_ (Node* np)
{
      headdef_(np);
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::headdef_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
				char buffer[16];
			   case TOP_DOWN:
               if (symbol[np->sti].type & GENERATED) generated = 1;
					if (optn[PG_GRAMMAR] == 1)
					{
						sprintf (buffer, "%4d       ", symbol[np->sti].numb);
						prt_sym (buffer, np, "\n", 0);
					}
					else if (optn[PG_GRAMMAR] == 2 && generated == 0)
					{
						sprintf (buffer, "\t");
						prt_sym (buffer, np, "\n", 0);
					}
					else if (optn[PG_GRAMMAR] == 3)
					{
						sprintf (buffer, "\t");
						prt_sym (buffer, np, "\n", 0);
					}
               linelength = 0;
               head_line[symbol[np->sti].numb] = np->line;
               F_prod[symbol[np->sti].numb] = N_prods;
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
					prod_numb = 0;
					if      (optn[PG_GRAMMAR] == 1) prt_grm ("                ;\n");
					else if (optn[PG_GRAMMAR] == 2 && generated == 0) prt_grm ("\t\t  ;\n");
					else if (optn[PG_GRAMMAR] == 3) prt_grm ("\t\t  ;\n");
               linelength = 0;
               break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::prod_ (Node* np)
{
		if (pass == SECOND_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
               N_prods++;
               if (np->child->id == TAILS_) // TAILS_ ?
               {
                  np = np->child->child;
                  while (np != NULL)
                  {
                     if (np->id == TAIL_)
                     {
                     // printf ("node %3d, tail = %3d, %s\n", np->numb, N_tails, symname(np->sti));
                        N_tails++;
                     }
                     else if (np->id == TAILKW_)
                     {
                     // printf ("node %3d, tail = %3d, %s\n", np->numb, N_tails, symname(np->sti));
                        N_tails++;
                     }
                     else
                     {
                        printf ("id = %d\n", np->id);
                        InternalError(34);
                     }
                     np = np->next;
                  }
               }
               else InternalError(25);
               break;
		   }
      }
		else if (pass == THIRD_PASS)
      {
         switch (status)
		   {
            char* before;
				char  buffer[16];
			   case TOP_DOWN:
					prod_numb++;
               F_tail [N_prods] = N_tails;
               Reverse[N_prods] = 0;
               Prec   [N_prods] = 0;
					before = " ->";
					if (term_symb[-np->sti][1] == '~') 
					{
						before = " ~>";
						Reverse[N_prods] = 1;
					}
					else if (term_symb[-np->sti][1] == '/')
					{
						before = "/->";
						Prec[N_prods] = 1;
						if (term_symb[-np->sti][2] == '~') 
						{
							before = "/~>";
							Reverse[N_prods] = 1;
						}
					}
					if (optn[PG_GRAMMAR] == 3)
					{
						if (prod_numb == 1) before = "  :";
						else					  before = "  |";	
					}
					if (optn[PG_GRAMMAR] == 1)
					{
						sprintf (buffer, "    %4d      ", N_prods);
						linelength += prt_grm (buffer);
						linelength += prt_grm (before);
					}
					else if (optn[PG_GRAMMAR] == 2 && generated == 0)
					{
						linelength += prt_grm ("\t\t");
						linelength += prt_grm (before);
					}
					else if (optn[PG_GRAMMAR] == 3)
					{
						linelength += prt_grm ("\t\t");
						linelength += prt_grm (before);
					}
               prevtail = 1;
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
               prod_line[N_prods++] = np->line;
					if      (optn[PG_GRAMMAR] == 1)	prt_grm ("\n");
					else if (optn[PG_GRAMMAR] == 2 && generated == 0)	prt_grm ("\n");
					else if (optn[PG_GRAMMAR] == 3)	prt_grm ("\n");
               linelength = 0;
               break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::tails_ (Node* np)
{
		return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::tailkw_ (Node* np)
{
      tail_ (np);
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::taileof_ (Node* np)
{
      tail_ (np);
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::tail_ (Node* np)
{
		if (pass == SECOND_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
            // printf ("%d %s\n", np->sti, symname(np->sti));
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP: break;
         }
      }
		else if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
					if (optn[PG_GRAMMAR] == 1) 
					{
						if (prevtail) linelength += prt_grm(" ");
						prt_sym ("", np, "", 0);
	               prevtail = 1;
					}
					else if (optn[PG_GRAMMAR] == 2 && generated == 0) 
					{
						if (prevtail) linelength += prt_grm(" ");
						prt_sym ("", np, "", 0);
	               prevtail = 1;
					}
					else if (optn[PG_GRAMMAR] == 3) 
					{
						if (prevtail) linelength += prt_grm(" ");
						prt_sym ("", np, "", 0);
	               prevtail = 1;
					}
               if (symbol[np->sti].type & NONTERMINAL)
               {
                  Tail[N_tails] = -symbol[np->sti].numb;
               // printf ("tail[%d] = %s\n", N_tails, symname(np->sti));
               }
               else
               {
                  Tail[N_tails] = symbol[np->sti].numb;
               // printf ("tail[%d] = %s\n", N_tails, symname(np->sti));
               }
            // printf ("node %3d, tail = %3d, %s\n", np->numb, N_tails, symname(np->sti));
               N_tails++;
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP: break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::repeat_ (Node* np)
{
      oper = "...";
      separator = 0; // null symbol
      genrepeat (np);
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::repeatrev_ (Node* np)
{
      oper = "~..";
      separator = 0; // null symbol
      genrepeat (np);
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::repeatsep_ (Node* np)
{
      oper = "...";
      separator = np->child->next->child->sti;
      genrepeat (np);
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::repeatseprev_ (Node* np)
{
      oper = "~..";
      separator = np->child->next->child->sti;
      genrepeat (np);
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::reverse_ (Node* np)
{
      oper = "~";
      separator = 0; // null symbol
      genrepeat (np);
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::genrepeat (Node* np)
{
		if (pass == FIRST_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:	 break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
            {
               if (np->id == REVERSE_)
               {
                  if (np->child->id == TAIL_)         // single tail?
                  {
                     np->parent->child = np->child;   // parent points at child now!
                     np->child->parent = np->parent;  // child points at parent now!
                     np->child->next   = np->next;    // move reverse.next to child.next!
                     np->child->prev   = np->prev;    // move reverse.prev to child.prev!
                  }
                  else if (np->child->id == OPTGROUP_               // optgroup?
                  &&  np->child->child->id == TAILS_
                  &&  np->child->child->next == NULL           // only one tails?
                  &&  np->child->child->child->id == TAIL_
                  &&  np->child->child->child->next == NULL)   // only one tail?
                  {
                     np->parent->child = np->child;   // parent points at child now!
                     np->child->parent = np->parent;  // child points at parent now!
                     np->child->next   = np->next;    // move reverse.next to child.next!
                     np->child->prev   = np->prev;    // move reverse.prev to child.prev!
                  }
               }
               else if (np->id == REPEATREV_)
               {
                  if (np->child->id == TAIL_)         // single tail?
                  {
                     np->id = REPEAT_;
                  }
                  else if (np->child->id == OPTGROUP_               // optgroup?
                  &&  np->child->child->id == TAILS_
                  &&  np->child->child->next == NULL           // only one tails?
                  &&  np->child->child->child->id == TAIL_
                  &&  np->child->child->child->next == NULL)   // only one tail?
                  {
                     np->id = REPEAT_;
                  }
               }
               else if (np->id == REPEATSEPREV_)
               {
                  if (np->child->id == TAIL_)         // single tail?
                  {
                     np->id = REPEATSEP_;
                  }
                  else if (np->child->id == OPTGROUP_               // optgroup?
                  &&  np->child->child->id == TAILS_
                  &&  np->child->child->next == NULL           // only one tails?
                  &&  np->child->child->child->id == TAIL_
                  &&  np->child->child->child->next == NULL)   // only one tail?
                  {
                     np->id = REPEATSEP_;
                  }
               }
               return 0;
            }
         }
      }
		else if (pass == SECOND_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               if (np->child->id == GROUP_)
               {
                  np->sti = GROUP_;
               }
               else if (np->child->id == OPTGROUP_)
               {
                  np->sti = OPTGROUP_;
                  np->child->id = GROUP_;
               }
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
            {
               char *h, *p;
               int head, head2, tail, taillen, seplen, operlen;
               if (np->child->id == TAIL_)
               {
                  tail = np->child->sti;
                  taillen = symbol[tail].length;
                  seplen  = symbol[separator].length;
                  operlen = strlen(oper);
                  if (np->sti == OPTGROUP_)
                  {
                     if (symbol[tail].name[0] == '(') // Group?
                     {
                        h = p = ALLOC (h, taillen+seplen+5);
                        *p++  = '[';
                        if (*(symbol[tail].name+taillen-1) == '~') taillen -= 3;
                        else                                       taillen -= 2;
                        memcpy (p, symbol[tail].name+1, taillen);
                        p += taillen;
                        *p++ = ']';
                     }
                     else                             // Not group!
                     {
                        h = p = ALLOC (h, taillen+seplen+7);
                        *p++ = '[';
                        if (*(symbol[tail].name+taillen-1) == '~') taillen--;
                        memcpy (p, symbol[tail].name, taillen);
                        p += taillen;
                        *p++ = ']';
                     }
                     if (separator)
                     {
                        memcpy (p++, "/", 1);
                        memcpy (p, symbol[separator].name, seplen);
                        p += seplen;
                     }
                     memcpy (p, oper, operlen);
                     p += operlen;
                     *p = 0;
                  }
                  else // GROUP_ || TAIL_
                  {
                     h = p = ALLOC (h, taillen+seplen+5);
                     if (*(symbol[tail].name+taillen-1) == '~') taillen--;
                     memcpy (p, symbol[tail].name, taillen);
                     p += taillen;
                     if (separator)
                     {
                        memcpy (p++, "/", 1);
                        memcpy (p, symbol[separator].name, seplen);
                        p += seplen;
                     }
                     memcpy (p, oper, operlen);
                     p += operlen;
                     *p = 0;
                  }

                  head = add_symbol (0, h, p, NONTERMINAL|TAIL, np->child->line);

                  if (symbol[head].numb < 0) // New symbol?
                  {
							N_generated++;
                     symbol[head].numb = N_heads++;
                     symbol[head].type = NONTERMINAL|GENERATED|TAIL;
                     if (operlen == 1) // '~' reverse only?
                     {
                        make_head (head);
                        if (np->sti == OPTGROUP_) make_prod (ARROW1);
                        make_prod (ARROW1);
                        make_tail (tail);
                     }
                     else if (np->sti == OPTGROUP_)
                     {
                        if (separator)
                        {
                           make_head (head);
                           make_prod (ARROW1);
                           make_prod (ARROW1);
                           taillen = symbol[tail].length;
                           if (symbol[tail].name[0] == '(') // Group?
                           {
                              h = p = ALLOC (h, taillen+seplen+5);
                              *p++  = '(';
                              if (*(symbol[tail].name+taillen-1) == '~') taillen -= 3;
                              else                                       taillen -= 2;
                              memcpy (p, symbol[tail].name+1, taillen);
                              p += taillen;
                              *p++ = ')';
                           }
                           else                             // Not group!
                           {
                              h = p = ALLOC (h, taillen+seplen+5);
                              if (*(symbol[tail].name+taillen-1) == '~') taillen--;
                              memcpy (p, symbol[tail].name, taillen);
                              p += taillen;
                           }
                           if (separator)
                           {
                              memcpy (p++, "/", 1);
                              memcpy (p, symbol[separator].name, seplen);
                              p += seplen;
                           }
                           memcpy (p, oper, operlen);
                           p += operlen;
                           *p = 0;

                           head2 = add_symbol (0, h, p, NONTERMINAL|TAIL, np->child->line);
                           make_tail (head2);

                           if (symbol[head2].numb < 0) // new symbol?
                           {
                           // printf ("%s\n", symbol[head2].name);
										N_generated++;
                              symbol[head2].numb = N_heads++;
                              symbol[head2].type = NONTERMINAL|GENERATED|TAIL;
                              make_head (head2);
                              make_prod (ARROW1);
                              make_tail (tail);
                              make_prod (ARROW1);
                              make_tail (head2);
                              make_tail (separator);
                              make_tail (tail);
                           }
                        }
                        else
                        {
                           make_head (head);
                           make_prod (ARROW1);
                           make_prod (ARROW1);
                           make_tail (head);
                           make_tail (tail);
                        }
                     }
                     else
                     {
                        make_head (head);
                        make_prod (ARROW1);
                        make_tail (tail);
                        make_prod (ARROW1);
                        make_tail (head);
                        if (separator) make_tail (separator);
                        make_tail (tail);
                     }
                  }
                  np->child = NULL;
                  np->id    = TAIL_;
                  np->sti   = head;
                  break;
               }
               else InternalError(6);
               np->sti = 0;
            }
		   }
      }
		else if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:	 break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
               linelength += prt_grm ("...");
               break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::group_ (Node* np)
{
		if (pass == FIRST_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:	 break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
               if (np->child->id == TAILS_)                          // Tails section?
               {
                  if (np->child->next == NULL)                       // Only one tails?
                  {
                     if (np->child->child->id == REVERSE_)           // Reverse?
                     {
                        if (np->child->child->next == NULL)          // Onle one reverse?
                        {
                           np->id = np->child->child->id;            // + reverse?
                           np->child->id = GROUP_;                   //   + optgroup
                           np->child->child->id = TAILS_;            //     + tails
                           np = np->child;                           // Point at optgroup.
                           goto Check;
                        }
                        return 0;
                     }
                     if (np->child->child->id == REPEAT_             // Repeat?
                     ||  np->child->child->id == REPEATREV_)         // Repeatrev?
                     {
                        if (np->child->child->next == NULL)          // Onle one repeat?
                        {
                           np->id = np->child->child->id;            // + repeat?
                           np->child->id = GROUP_;                   //   + group
                           np->child->child->id = TAILS_;            //     + tails
                           np = np->child;                           // Point at optgroup.
                           goto Check;
                        }
                        return 0;
                     }
                     if (np->child->child->id == REPEATSEP_          // Repeatsep?
                     ||  np->child->child->id == REPEATSEPREV_)      // Repeatseprev?
                     {
                        if (np->child->child->next == NULL)          // Onle one repeat?
                        {
                           np->id = np->child->child->id;                     // + repeatsep?
                           np->child->id = GROUP_;                            //   + group
                           np->child->child->id = TAILS_;                     //     + tails
                           np->child->next = np->child->child->child->next;   //   + sepexpr
                           if (np->child->child->child->id == TAIL_)
                           {
                              np->child->child->next = NULL;
                           }
                           else if (np->child->child->child->id == GROUP_)
                           {
                              np->child->child->child->next = NULL;
                           }
                           else if (np->child->child->child->id == OPTGROUP_)
                           {
                              np->child->child->child->next = NULL;
                           }
                           np = np->child;                           // Point at optgroup.
                           goto Check;
                        }
                        return 0;
                     }
Check:               if (np->child->child->id == TAIL_ || np->child->child->id == GROUP_ || np->child->child->id == OPTGROUP_)
                     {
                        if (np->child->child->next == NULL)          // Only one tail?
                        {
                           if (np->parent != NULL)
                           {
                              np->parent->child = np->child->child;  // Make parent's child point at tail.
                              np->child->child->parent = np->parent; // Change parent of tail.
                           }
                           else // if (np->prev != NULL)
                           {
                              np->prev->next = np->child->child;     // Make prev's next point at tail.
                              np->child->child->prev = np->prev;     // Change prev of tail.
                           }
                           np->child->child->next = np->next;        // Change next of tail.
                           np->child->child->prev = np->prev;        // Change prev of tail.
                        }
                        break;
                     }
                  }
               }
               else InternalError(1);
               break;
         }
      }
		else if (pass == SECOND_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:	 break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
            {
               char *h, *p;
               int head, len, x_prods, x_tails;
               len = group_length (np, x_prods, x_tails);
               if (x_tails == 1)
               {
                  np->id    = TAIL_;
                  np->sti   = np->child->child->sti;
                  np->child = NULL;
                  return 0;
               }
               else
               {
                  get_group_symbol (np, len+4, '(', ')', h, p);
                  arrow = ARROW1;
                  if (np->parent != NULL)
                  {
                     if (np->parent->id == REVERSE_ || np->parent->id == REPEATREV_ || np->parent->id == REPEATSEPREV_)
                     {
                        arrow = ARROW2;
                        *p++ = '~';
                        *p = 0;
                     }
                  }
                  head = add_symbol (0, h, p, NONTERMINAL|TAIL, 0);
                  if (symbol[head].numb < 0) // New symbol?
                  {
							N_generated++;
                     symbol[head].numb = N_heads++;
                     symbol[head].type = NONTERMINAL|GENERATED|TAIL;
                     make_head (head);
                     make_prod (arrow);
                     group_prod (np, arrow);
                  }
                  arrow = ARROW1;
                  np->child = NULL;
                  np->id    = TAIL_;
                  np->sti   = head;
                  return 0; // OK
               }
            }
		   }
      }
		else if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               if (prevtail) linelength += prt_grm(" ");
               linelength += prt_grm ("(");
               prevtail = 0;
               break;
			   case PASS_OVER:
               linelength += prt_grm (" | ");
               prevtail = 0;
               break;
			   case BOTTOM_UP:
               linelength += prt_grm (")");
               break;
		   }
      }
		return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::optgroup_ (Node* np)
{
		if (pass == FIRST_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:	 break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
               if (np->child->id == TAILS_)									// Tails section?
               {
                  if (np->child->next == NULL)                       // Only one tail?
                  {
                     if (np->child->child->id == REVERSE_)           // Reverse?
                     {
                        if (np->child->child->next == NULL)          // Onle one reverse?
                        {
                           np->id = np->child->child->id;            // + reverse?
                           np->child->id = OPTGROUP_;                //   + optgroup
                           np->child->child->id = TAILS_;            //     + tails
                           np = np->child;                           // Point at optgroup.
                           goto Check;
                        }
                        return 0;
                     }
                     if (np->child->child->id == REPEAT_             // Repeat?
                     ||  np->child->child->id == REPEATREV_)         // Repeatrev?
                     {
                        if (np->child->child->next == NULL)          // Onle one repeat?
                        {
                           np->id = np->child->child->id;            // + repeat?
                           np->child->id = OPTGROUP_;                //   + optgroup
                           np->child->child->id = TAILS_;            //     + tails
                           np = np->child;                           // Point at optgroup.
                           goto Check;
                        }
                        return 0;
                     }
                     if (np->child->child->id == REPEATSEP_          // Repeatsep?
                     ||  np->child->child->id == REPEATSEPREV_)      // Repeatseprev?
                     {
                        if (np->child->child->next == NULL)          // Onle one repeat?
                        {
                           np->id = np->child->child->id;                     // + repeatsep?
                           np->child->id = OPTGROUP_;                         //   + optgroup
                           np->child->child->id = TAILS_;                     //     + tails
                           np->child->next = np->child->child->child->next;   //   + sepexpr
                           if (np->child->child->child->id == TAIL_)
                           {
                              np->child->child->next = NULL;
                           }
                           else if (np->child->child->child->id == GROUP_)
                           {
                              np->child->child->child->next = NULL;
                           }
                           else if (np->child->child->child->id == OPTGROUP_)
                           {
                              np->child->child->child->next = NULL;
                           }
                           np = np->child;                           // Point at optgroup.
                           goto Check;
                        }
                        return 0;
                     }
Check:               if (np->child->child->id == GROUP_ || np->child->child->id == OPTGROUP_)
                     {
                        if (np->child->child->next == NULL)          // Only one group?
                        {
                           np->child->child->id = OPTGROUP_;
                           if (np->parent != NULL)
                           {
                              np->parent->child = np->child->child;  // Make parent's child point at tail.
                              np->child->child->parent = np->parent; // Change parent of tail.
                           }
                           else // if (np->prev != NULL)
                           {
                              np->prev->child = np->child->child;    // Make parent's child point at tail.
                              np->child->child->prev = np->prev;     // Change parent of tail.
                           }
                           np->child->child->next = np->next;        // Change next of tail.
                           np->child->child->prev = np->prev;        // Change prev of tail.
                        }
                        return 0;
                     }
                  }
               }
               else InternalError(1);
               break;
         }
      }
		else if (pass == SECOND_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:	 break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
            {
               char *h, *p;
               int head, len, x_prods, x_tails;
               if (np->id == OPTGROUP_)
               {
                  len = group_length (np, x_prods, x_tails);
                  get_group_symbol (np, len+3, '[', ']', h, p);
                  head = add_symbol (0, h, p, NONTERMINAL|TAIL, 0);
                  if (symbol[head].numb < 0) // New symbol?
                  {
							N_generated++;
                     symbol[head].numb = N_heads++;
                     symbol[head].type = NONTERMINAL|GENERATED|TAIL;
                     make_head (head);
                     make_prod (ARROW1);
                     make_prod (ARROW1);
                     group_prod (np, ARROW1);
                  }
                  np->child = NULL;
                  np->id    = TAIL_;
                  np->sti   = head;
               }
               else // Move the tails/tail into the optgroup node.
               {
                  np->id    = np->child->child->id;
                  np->prod  = np->child->child->prod;
                  np->sti   = np->child->child->sti;
                  np->line  = np->child->child->line;
                  np->start = np->child->child->start;
                  np->end   = np->child->child->end;
                  np->child = NULL;
               }
               return 0; // OK
            }
		   }
      }
		else if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               if (prevtail) linelength += prt_grm(" ");
               linelength += prt_grm ("[");
               prevtail = 0;
               break;
			   case PASS_OVER:
               linelength += prt_grm (" | ");
               prevtail = 0;
               break;
			   case BOTTOM_UP:
               linelength += prt_grm ("]");
               break;
		   }
      }
		return 0; // OK
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::group_length (Node* npgroup, int& x_prods, int& x_tails)
{
      Node *nptails, *nptail;
      int  len = 0, nprods, ntails;
      x_prods = 0;
      x_tails = 0;
      nptails = npgroup->child;                          // -> tails_ node.
      while(1)                                           // No such thing as empty tails_.
      {
         nptail = nptails->child;                        // -> tail node.
         while(1)                                        // No such thing as empty tail.
         {
            if (nptail->id == GROUP_)                    // Could have (A B C) inside a group.
            {
               len += group_length (nptail, nprods, ntails) + 1;
               x_prods += nprods - 1;
               x_tails += ntails;
            }
            else if (nptail->id == OPTGROUP_)                 // Could have [A B C] inside a group.
            {
               len += group_length (nptail, nprods, ntails) + 1;
               x_prods += nprods - 1;
               x_tails += ntails;
            }
            else if (nptail->id == TAIL_)                     // Could have a single tail.
            {
            // printf ("%s\n", symname(nptail->sti));    // for debugging.
               len += symbol[nptail->sti].length + 1;    // tail symbol length + " "
               x_tails++;
            }
            nptail = nptail->next;
            if (nptail == NULL) break;
         }
         x_prods++;
         nptails = nptails->next;
         if (nptails == NULL) break;
         len += 2;                                       // plus "| "
      }
      return len-1;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGNodeAction::get_group_symbol (Node* np, int len, char before, char after, char*& h, char*& p)
{
      Node *nptails, *nptail;
      h = p = ALLOC (h, len);
      if (before) *p++ = before;                         // ']' or ')'
      nptails = np->child;                               // -> tails_
      while(1)
      {
         nptail = nptails->child;
         while(1)
         {
            if (nptail->id == GROUP_)                    // Could have (A B C).
            {
               p = group_symbol (p, nptail);
            }
            else if (nptail->id == OPTGROUP_)                 // Could have [A B C].
            {
               p = group_symbol (p, nptail);
            }
            else if (nptail->id == TAIL_)
            {
            // printf ("%s\n", symname(nptail->sti));    // for debugging.
               memcpy (p, symbol[nptail->sti].name, symbol[nptail->sti].length);
               p += symbol[nptail->sti].length;
            }
            nptail = nptail->next;
            *p++ = ' ';
            if (nptail == NULL) break;
         }
         nptails = nptails->next;
         if (nptails == NULL) break;
         *p++ = '|';                                     // plus "|"
         *p++ = ' ';                                     // plus " "
      }
      p--;                                               // minus last " "
      if (after) *p++ = after;                           // ']' or ')'
      *p = 0;
   /* if (p-h+1 > len)
      {
         printf ("group_symbol: %s\n", h);
         InternalError(99);
      }
   */
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

char* PGNodeAction::group_symbol (char* p, Node* np)
{
      Node *nptails, *nptail;
      nptails = np->child;                               // -> tails_ node.
      while(1)                                           // No such thing as an empty group (I hope).
      {
         nptail = nptails->child;                        // -> tail node.
         while(1)
         {
            if (nptail->id == GROUP_)                    // Could have (A B C).
            {
               p = group_symbol (p, nptail) + 1;
            }
            else if (nptail->id == TAIL_)
            {
            // printf ("%s\n", symname(nptail->sti));    // for debugging.
               memcpy (p, symbol[nptail->sti].name, symbol[nptail->sti].length);
               p += symbol[nptail->sti].length;
               *p++ = ' ';
            }
            nptail = nptail->next;
            if (nptail == NULL) break;
         }
         p--;
         nptails = nptails->next;
         if (nptails == NULL) break;
      }
      return p;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGNodeAction::group_prod (Node* np, int arrow)
{
      Node *nptails, *nptail;
      nptails = np->child;                               // -> tails_
      while (1)
      {
         nptail = nptails->child;
         do
         {
            if (nptail->id == GROUP_) group_prod (nptail, arrow);
            else                      make_tail (nptail->sti);
            nptail = nptail->next;
         }
         while (nptail != NULL);
         nptails = nptails->next;
         if (nptails == NULL) break;
         make_prod (arrow);                              // ->
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::sepexprrev_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               linelength += prt_grm ("/");
               prevtail = 0;
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
               linelength += prt_grm ("~..");
               prevtail = 1;
               break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::sepexpr_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               linelength += prt_grm ("/");
               prevtail = 0;
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
               linelength += prt_grm ("...");
               prevtail = 1;
               break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::sep_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:  prt_sym ("", np, "", 0); break;
			   case PASS_OVER: break;
			   case BOTTOM_UP: break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::token_action_spec_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
            static int terminal;
			   case TOP_DOWN:
               terminal = symbol[np->parent->prev->sti].numb;
               Tact_numb[terminal] = symbol[np->sti].numb;
               F_targ[terminal] = N_args;
               prt_sym (" ", np, "", 0);
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
               if (N_args == F_targ[terminal]) F_targ[terminal] = -1;
               if (np->child->child == NULL)
                  prt_grm ("()");
               break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::parse_action_spec_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               Pact_numb[N_prods] = symbol[np->sti].numb;
               F_parg[N_prods] = N_args;
               prt_sym ("", np, "", 0);
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
               if (N_args == F_parg[N_prods]) F_parg[N_prods] = -1;
               if (np->sti > 0 && np->child->child == 0)
                  linelength += prt_grm ("()");
               if (np->sti == 0) np->sti = nodesti;
               break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::node_spec_ (Node* np)
{
      static int n_args;
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               n_args = N_args;
            // printf ("%s\n", symbol[np->sti].name);
               Node_numb[N_prods] = symbol[np->sti].numb;
               F_parg[N_prods] = N_args;
               prt_sym ("", np, "", 0);
               nodesti = np->sti;
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
					if (optn[PG_NODEARGS])
					{
						if (N_args - n_args > 0 && N_args - n_args != optn[PG_NODEARGS])
						{
							sprintf (string, "Node operator '+>' or '*>' on this rule, expecting %d argument[s]", optn[PG_NODEARGS]);
							prt_error (string, np->start, np->end, np->line);
						}
					} 
               if (N_args == F_parg[N_prods]) F_parg[N_prods] = -1; // New
					if (np->child != NULL)  // Added June 16 2013
					{
						if (np->child->child == NULL) prt_grm ("()");
					}
               break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::pa_node_spec_ (Node* np)
{
      static int n_args;
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               n_args = N_args;
            // printf ("%s\n", symbol[np->sti].name);
               Node_numb[N_prods] = symbol[np->sti].numb;
               F_parg[N_prods] = N_args;
               prt_sym ("", np, "", 0);
               nodesti = np->sti;
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
					if (optn[PG_NODEARGS])
					{
						if (N_args - n_args > 0 && N_args - n_args != optn[PG_NODEARGS])
						{
							sprintf (string, "Node operator '+>' or '*>' on this rule, expecting %d argument[s]", optn[PG_NODEARGS]);
							prt_error (string, np->start, np->end, np->line);
						}
					} 
               if (N_args == F_parg[N_prods]) F_parg[N_prods] = -1; // New
					if (np->child != NULL)  // Added June 16 2013
					{
						if (np->child->child == NULL) prt_grm ("()");
					}
               break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::blank_parse_action_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               Pact_numb[N_prods] = symbol[np->sti].numb;
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP: break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::blank_node_action_name_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               Nact_numb[N_prods] = symbol[np->sti].numb;
               F_narg[N_prods] = N_args;
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP: 
               if (N_args == F_narg[N_prods]) F_narg[N_prods] = -1;
					break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::node_action_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               Nact_numb[N_prods] = symbol[np->sti].numb;
               F_narg[N_prods] = N_args;
               prt_sym (" ", np, "", 0);
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
               if (N_args == F_narg[N_prods]) F_narg[N_prods] = -1;
               if (np->sti > 0 && np->child->child == 0)
                  linelength += prt_grm ("()");
               if (np->sti == 0) np->sti = nodesti;
               break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::rule_actions_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:	 nodesti = 0; break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
                 break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::token_action_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               prt_sym ("", np, "", 0);
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP: break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::parse_action_ (Node* np)
{
      int space = WIDTH - linelength;
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               if (space > 0)
               {
                  string[space] = 0;
                  prt_grm ("%s", string);
                  string[space] = ' ';
                  prt_sym (" ", np, " ", 0);
               }
               else prt_sym ("\n                      ", np, " ", 0);
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
                 if (np->next != NULL)
                 {
                    prt_grm ("\n               ");
                    linelength = 15;
                 }
                 break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::parse_action_make_node_ (Node* np)
{
      int space = WIDTH - linelength;
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               if (space > 0)
               {
                  string[space] = 0;
                  prt_grm ("%s", string);
                  string[space] = ' ';
						if (optn[PG_ASTCONST] == 0) np->sti = -PARSEACTION;
						prt_sym (" ", np, " ", 0);
               }
               else prt_sym ("\n                      ", np, " ", 0);
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP: break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::make_node_ (Node* np)
{
      int space = WIDTH - linelength;
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               if (space > 0)
               {
                  string[space] = 0;
                  prt_grm ("%s", string);
                  string[space] = ' ';
                  prt_sym (" ", np, " ", 0);
               }
               else prt_sym ("\n                      ", np, " ", 0);
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP: break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::parse_action_make_node_wa_ (Node* np)
{
      int space = WIDTH - linelength;
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               if (space > 0)
               {
                  string[space] = 0;
                  prt_grm ("%s", string);
                  string[space] = ' ';
                  prt_sym (" ", np, " ", 0);
               }
               else prt_sym ("\n                      ", np, " ", 0);
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
               if (np->child->next == NULL) // No explicit node-action name?
               {
                  symbol[np->child->sti].type |= NACTION; // This nodename is also a node-action.
               }
               break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::make_node_wa_ (Node* np)
{
      int space = WIDTH - linelength;
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
               if (space > 0)
               {
                  string[space] = 0;
                  prt_grm ("%s", string);
                  string[space] = ' ';
                  prt_sym (" ", np, " ", 0);
               }
               else prt_sym ("\n                      ", np, " ", 0);
               break;
			   case PASS_OVER: break;
			   case BOTTOM_UP:
               if (np->child->next == NULL) // No explicit node-action name?
               {
                  symbol[np->child->sti].type |= NACTION; // This nodename is also a node-action.
               }
               break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::stackindex_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:	 prt_sym ("", np, "", 0); break;
			   case PASS_OVER: break;
			   case BOTTOM_UP: break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::typeexpr_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:	 break;
			   case PASS_OVER: linelength += prt_grm("|"); break;
			   case BOTTOM_UP: break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::type_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:	 prt_sym ("", np, "", 0); break;
			   case PASS_OVER: break;
			   case BOTTOM_UP: break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::args_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         if (np->child != NULL)
         switch (status)
		   {
			   case TOP_DOWN:
               linelength += prt_grm ("(");
               break;
			   case PASS_OVER:
               linelength += prt_grm (",");
               break;
			   case BOTTOM_UP:
               linelength += prt_grm (")");
               break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::arg_integer_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
                 Arg_numb[N_args++] = symbol[np->sti].value;
                 prt_sym ("", np, "", 0);
                 break;
			   case PASS_OVER: break;
			   case BOTTOM_UP: break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::arg_alpha_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
                 if (symbol[np->sti].type & NONTERMINAL)
                 {
                    prt_error ("at '%s', nonterminal symbols cannot be used as arguments", np->start, np->end, np->line);
                 }
                 else if (!(symbol[np->sti].type & CONSTANT) && !(symbol[np->sti].type & TERMINAL))
                 {
                    symbol[np->sti].type |= TERMINAL|ARGUMENT;
                    symbol[np->sti].numb = N_terms;
                    symbol[np->sti].value = N_terms++;
                    if (!(symbol[np->sti].type & TAIL))
                    prt_error ("at '%s', not used in grammar, not a constant and not declared", np->start, np->end, np->line);
                 }
                 Arg_numb[N_args++] = symbol[np->sti].value;
                 prt_sym ("", np, "", 0);
                 break;
			   case PASS_OVER: break;
			   case BOTTOM_UP: break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::arg_terminal_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
                 Arg_numb[N_args++] = symbol[np->sti].value;
                 prt_sym ("", np, "", 0);
                 break;
			   case PASS_OVER: break;
			   case BOTTOM_UP: break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::arg_null_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
                 Arg_numb[N_args++] = -1;
                 break;
			   case PASS_OVER: break;
			   case BOTTOM_UP: break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::arg_string_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
                 Arg_numb[N_args++] = symbol[np->sti].numb; // sequence number of string arg.
                 prt_sym ("", np, "", 0);
                 break;
			   case PASS_OVER: break;
			   case BOTTOM_UP: break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::arg_expr_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         int  value;
         switch (status)
		   {
			   case TOP_DOWN:
                 break;
			   case PASS_OVER:
                 linelength += prt_grm("|");
                 break;
			   case BOTTOM_UP:
                 value = 0;
                 for (np = np->child; np != NULL; np = np->next)
                 {
                    value |= symbol[np->sti].value;
                 }
                 Arg_numb[N_args++] = value;
                 break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::arg_expr_integer_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
                 prt_sym ("", np, "", 0);
                 break;
			   case PASS_OVER: break;
			   case BOTTOM_UP: break;
		   }
      }
		return (0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGNodeAction::arg_expr_alpha_ (Node* np)
{
		if (pass == THIRD_PASS)
      {
         switch (status)
		   {
			   case TOP_DOWN:
                 if (symbol[np->sti].type & NONTERMINAL)
                 {
                    prt_error ("at '%s', nonterminal symbols cannot be used as arguments", np->start, np->end, np->line);
                 }
                 else if (!(symbol[np->sti].type & CONSTANT) && !(symbol[np->sti].type & TERMINAL))
                 {
                    symbol[np->sti].type |= TERMINAL|ARGUMENT;
                    symbol[np->sti].numb = N_terms;
                    symbol[np->sti].value = N_terms++;
                    if (!(symbol[np->sti].type & TAIL))
                    prt_error ("at '%s', not used in grammar, not a constant and not declared", np->start, np->end, np->line);
                 }
                 prt_sym ("", np, "", 0);
                 break;
			   case PASS_OVER: break;
			   case BOTTOM_UP: break;
		   }
      }
		return (0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //

int   PGNodeAction::prt_sym (char* before, Node* np, char* after, int length)
{
      if (optn[PG_GRAMMAR] == 0) return 0;
      int    sti, len = 0;
      char   *p, *q, *sym;
      char   format[20] = "%s%s%s";

      sti = np->sti;
      if (sti == 0) return 0;
	   if (length > 0) sprintf (format, "%%s%%-%ds%%s", length);
		else            sprintf (format, "%%s%%s%%s");
      if (sti > 0) // from symbol table?
      {
      // printf ("%s\n", symname(sti));
			if (symbol[sti].type & TERMINAL)
			{
				len = prt_grm (format, before, term_name[symbol[sti].numb], after);
			}
			else 
			{
				if (symbol[sti].type & (PACTION|NODENAME|NACTION))
				{
					len = prt_grm (format, before, symbol[sti].name, after);
				} 
				else len = prt_grm (format, before, symbol[sti].name, after);
			}
         linelength += len;
      }
      else if (sti < 0) // from terminal symbols!
      {
      // printf ("%s\n", term_symb[-sti]);
         sym = term_symb[-sti];
         if (*sym == '\'') // Remove single quotes?
         {
            p = string;
            q = sym + 1;
            while (*q != '\'') *p++ = *q++;
            *p = 0;
            len = prt_grm (format, before, string, after);
            linelength += len;
            for (p = string; *p != 0; p++) *p = ' '; // Restore spaces.
            *p = ' ';
         }
         else   
         {
            len = prt_grm (format, before, sym, after);
            linelength += len;
         }
      }
      return len;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGNodeAction::make_head (int sti)
{
      Node* np;
   // printf ("%s ", symname(sti));
      np = lastnonterm;
      np = np->next = (Node*)ALLOC(node, 1);
      np->id      = HEADDEF_;
      np->numb    = n_nodes++;
      np->prod    = -1;
      np->sti     = sti;
      np->line    = 0;
      np->start   = NULL;
      np->end     = NULL;
      np->next    = NULL;
      np->prev    = NULL;
      np->child   = NULL;
      np->parent  = NULL;
      lastnonterm = np;
      lastprod    = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGNodeAction::make_prod (int terminal)
{
      Node* np;
   // printf ("\n   -> ");
      if (lastprod == NULL) // First time for generated prod.
      {
         np = lastnonterm;
         np = np->child = (Node*)ALLOC(node, 1);
         np->parent = lastnonterm;
         np->prev   = NULL;
      }
      else // New production linkage!
      {
         np = lastprod;
         np = np->next = (Node*)ALLOC(node, 1);
         np->parent = NULL;
         np->prev   = lastprod;
      }
      np->id     = PROD_;
      np->numb   = n_nodes++;
      np->prod   = -1;
      np->sti    = -terminal;
      np->line   = 0;
      np->start  = NULL;
      np->end    = NULL;
      np->next   = NULL;
      lastprod   = np;

      np = np->child = (Node*)ALLOC(node, 1);
      np->id     = TAILS_;
      np->numb   = n_nodes++;
      np->parent = lastprod;
      np->prev   = NULL;
      np->prod   = -1;
      np->sti    = 0;
      np->line   = 0;
      np->next   = NULL;
      np->start  = NULL;
      np->end    = NULL;
      np->child  = NULL;
      lasttails  = np;
      lasttail   = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGNodeAction::make_tail (int sti)
{
      Node* np;
   // printf ("%s ", symname(sti));
      if (lasttail == NULL)
      {
         np = lasttails;
         np = np->child = (Node*)ALLOC(node,1);
         np->parent = lasttails;
         np->prev   = NULL;
      }
      else
      {
         np = lasttail;
         np = np->next = (Node*)ALLOC(node,1);
         np->parent = NULL;
         np->prev   = lasttail;
      }
      np->id     = TAIL_;
      np->numb   = n_nodes++;
      np->prod   = -1;
      np->sti    = sti;
      np->line   = 0;
      np->next   = NULL;
      np->start  = NULL;
      np->end    = NULL;
      np->child  = NULL;
      lasttail   = np;
}

///////////////////////////////////////////////////////////////////////////////
//

void  PGNodeAction::add_quotes () // Add quotes to alpha terminal symbols.
{
      char c;
      int  t, x;
      for (t = 0; t < N_terms; t++)
		{
         c = *term_name[t];
			if (c == '\'') continue;		// Already in quotes?
		  	if (c == '<' ) continue;	   // <...> symbol?
		  	if (c == '{' ) continue;	   // {...} symbol?
         int sti, len;
         char *p, *s;
         len = strlen (term_name[t]);
         s = ALLOC (p, len+3);
         *p++ = '\'';
         memcpy (p, term_name[t], len);
         p += len;
         *p++ = '\'';
         *p = 0;
			sti = Symtab::add_symbol (0, s, p, TERMINAL|TAIL|ARGUMENT, term_line[t]);
         if (symbol[sti].numb >= 0) // Already used in grammar?
         {
            x = symbol[sti].numb;
				sprintf (string, "%s and %s were used in grammar, cannot have both", term_name[t], term_name[x]);
            prt_error (string, term_start[t], term_end[t], term_line[t]);
         }
			term_name[t] = s;
		}
}

#endif

//                                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////////////////////

#endif