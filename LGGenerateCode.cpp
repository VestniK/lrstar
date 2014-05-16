
#ifdef DFASTAR

		#include "ComGlobal.h"
		#include "ComGenerate.h"	   
		#include "LGGlobal.h"

		#define NONE 1
		#define SOME 2
		#define ALL  3

		// 0 = regular char ('A'). 
		// 1 = hexadecimal char (0x1A). 
		// 2 = escape character ('\n'). 

		unsigned char output_type [256] = 
		{
			  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  1,  1,
			  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,

			  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
			  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
			  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
			  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
			  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
			  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
			  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
			  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1
		};

		static int*  action_count;
		static char* tran_to_id_state;
		static int   id_state;
		static int   bypass_states;
		static FILE* codefp;

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    open_code                                                              //
      
int   Generate::open_code (char* out_fid)
{
      codefp = fopen (out_fid, "w");
      if (codefp == NULL)
      {
         if (++n_errors == 1) prt_log ("\n");
			prt_log ("Output:   %s  cannot be created!\n", out_fid);
         return (0);
      }
		return (1);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    close_code                                                             //
      
void  Generate::close_code ()
{
		if (codefp != NULL) fclose (codefp);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    prt_code                                                               //
      
void  Generate::prt_code (char *format,...)
{
      va_list argptr;
		if (codefp != NULL)
		{
	      va_start (argptr, format);
		   vfprintf (codefp, format, argptr);
			va_end (argptr);
		}
}

void 	Generate::KEYWORD_OPTIMIZATION()
{
		int i, s, *counter, max, count;

	// Count number of transitions from alpha characters to a state ...
		ALLOC (counter, n_states);
		FASTINI (0, counter, n_states);
		for (s = 0; s < n_states; s++)
		{
			for (i = tt_start[s]; i < tt_end[s]; i++)
			{
				if (charcode[tt_symb[i]] & (UPPERCASE|LOWERCASE)) // Alphabetic char?
				{
					if (tt_action[i] != 0) counter[tt_action[i]]++; // Increment counter for goto state.
				}
			}
		}
	// Find id_state (with the most goto transitions) ...
	// A string state ("abc...") may look like an id_state except for the return value is 0
	// until a transition is made on the final " character.	To be more precise we should 
	// check to see that the return value is for <identifier>.  Also a string state does not 
	// have a transition to it from state 0. Anyway these problems only exists then 'ki' option
	// is used erroneously (when there are no keywords in the grammar). When the keywords are 
	// there, max will be much higher for the id_state than for any string state.
		max = 0;
		id_state = -1;
		for (s = 1; s < n_states; s++)
		{
			if (counter[s] > max && D_red[s] != 0) 
			{
				max = counter[s];
				id_state = s;
			}
		}
	// Check for another state also having max transitions to it ...
		for (s = 1; s < n_states; s++)
		{
			if (s != id_state)
			{
				if (counter[s] == max) // Problem ?
				{
					id_state = -1; // Cancel the id_state, ki option error!
					break;
				}
			}
		}
		FREE (counter, n_states);
		if (id_state == -1) return;

	// Check id_state for recursive transitions on all alpha characters ...
		count = 0;
		for (i = tt_start[id_state]; i < tt_end[id_state]; i++)
		{
			if (charcode[tt_symb[i]] & (UPPERCASE|LOWERCASE)) // Alphabetic char?
			{
				if (tt_action[i] == id_state) count++;
			}
		}
		if (count == 52); // Sure thing, we have the id_state. 
		else if (count ==  0) id_state = -1;  // Nope!
		else
		{
			n_warnings++;
			if (optn[LG_WARNINGS])
			prt_logonly ("Choosing state %d for the <identifier> state.\n", id_state);
		}
		if (id_state == -1) return;

	//	Remove transitions to id_state ...
		memset (tran_to_id_state, 0, n_states);
		for (s = 1; s < n_states; s++)
		{
			if (s != id_state)
			{
				if (D_red[s] == D_red[id_state]) // Same default as id_state?
				{
					for (i = tt_start[s]; i < tt_end[s]; i++)
					{
						if (tt_action[i] == id_state) 
						{
							tt_action[i] = 0;	// Deactivate transition.
						}
					}
					D_red[s] = MAX_INT; 
					tran_to_id_state[s] = NONE;
				}
				else
				{
					int n = 0; // Number of transition going to id_state.
					for (i = tt_start[s]; i < tt_end[s]; i++)
					{
						if (tt_action[i] == id_state) 
						{ 
							n++; 
							tt_action[i] = 0; 
						}
					}
					if (n) 
					{
						if (tt_end[s] - tt_start[s] == n) // All transitions going to id_state?
						{
							tran_to_id_state[s] = ALL; // End of keyword and no partial keywords. 
						}
						else // Some transitions NOT going to id_state?
						{
							tran_to_id_state[s] = SOME; // Partial keywords in this state.
						}
					}
				}
			}
		}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
      
int   Generate::OUTPUT_CODE ()
{					
      char *p, *q;
		int *action, *trans, *seq;
	   int s, i, j, k, t, nt, nds, next, n;
		int error_count;

	//	max_char_set = 128; 

		tokenstartstr = "token.start"; 
		tokenendstr   = "token.end"; 
		tokenlinestr  = "token.linenumb"; 
		linenumbstr   = "linenumb"; 
		colnumbstr    = "colnumb"; 
		tabstopstr    = "tabstop";

		if (!optn [LG_LINENUMB]) linenumbstr = ""; // Works fine. 
		if (!optn [LG_COLNUMB ]) colnumbstr  = ""; // Not implemented yet. 
		if (!optn [LG_TAB     ]) tabstopstr  = ""; // Not implemented yet. 

	/*	if (optn [LG_DEBUG]) Not working, fix later or leave this out.
		{
			optn [LG_OPTIMIZE]    = 1;   
			optn [LG_SORTACTIONS] = 1;	// Necessary for speed.
			optn [LG_BLANKGOTOS]  = 0;	// Necessary for speed.
		}
		else // Not DEBUG! */
		{
			optn [LG_OPTIMIZE]    = 1;   
			optn [LG_SORTACTIONS] = 1;	// Necessary for speed.
			optn [LG_BLANKGOTOS]  = 1;	// Necessary for speed.
		}

		id_state = -1;
		ALLOC (action_count, n_states);
		ALLOC (tran_to_id_state, n_states);
		memset(tran_to_id_state, 0, n_states);

		if (optn[LG_KEYWORDIDENT])
		{
			KEYWORD_OPTIMIZATION();
		}

	// Search for any	bypass states ...
		bypass_states = 0;
		for (s = 0; s < n_states; s++) 
		{
			if (D_red[s] != MAX_INT)
			if (ret_numb [D_red[s]] < 0) { bypass_states = 1; break; }
		}

		ALLOC (seq, max_char_set);
		ALLOC (action, max_char_set);

	// Create actions for all states, except illegal character state (the last one).
		n_codelines = 0;
      for (s = 0; s < n_states-1; s++)
      {
		//	printf ("state %d\n", s);
			PRT_STA (s);
			nt = tt_end[s] - tt_start[s];	// Number of transitions.
			if (nt > 0) // Any transitions?
			{
			// Action definitions ...
				for (i = 0; i < max_char_set; i++) // Clear all actions.
				{
					action[i] = MAX_INT; 
				}
				error_count = max_char_set;
				int n = 0;
				if (tran_to_id_state[s] > NONE)
				{
					for (t = tt_start[id_state]; t < tt_end[id_state]; t++)
					{
						n++;
						i = tt_symb[t];
						action[i] = id_state; 
					//	printf ("action[%d] = %d\n", i, action[i]);
						if (optn[LG_INSENSITIVE]) 
						{
							if (lower[i-32] == i) 
							{
								n++;
								action[i-32] = id_state; // Upper gets same action.
							}
						}
					}
					error_count -= n;
				}
				FASTINI (0, action_count, n_states);

				for (t = tt_start[s]; t < tt_end[s]; t++)
				{
					if (tt_action[t] != 0 && tt_action[t] != n_states-1)
					{
						i = tt_symb[t];
						if (action[i] == MAX_INT) error_count--; // Undefined?
						action[i] = tt_action[t];
					//	printf ("'%c' %d -> %d\n", i, i, action[i]);
						if (optn[LG_INSENSITIVE]) 
						{
							if (lower[i-32] == i) 
							{
								if (action[i-32] == MAX_INT) error_count--; // Undefined?
								action[i-32] = tt_action[t]; // Upper gets same action.
							}
						}
					}
				}
				for (i = 0; i < max_char_set; i++)
				{
					if (action[i] != MAX_INT) 
					{
					//	printf ("action[%d] = %d\n", i, action[i]);
						action_count[action[i]]++;  
					}
				}

			// Get default action ...
				int max_count = 0;
				int default_action = -1;
				for (i = 0; i < n_states; i++)
				{
					if (action_count[i] > max_count)
					{
						max_count = action_count[i];
						default_action = i;
					}
				}
			//	if (s == 0) error_count = max_char_set;
				if (error_count > max_count) 
				{
					default_action = 0;
				}
				if (default_action > 0 && default_action != MAX_INT)
				{
					for (t = tt_start[s]; t < tt_end[s]; t++)
					{
						if (tt_action[t] != 0 && tt_action[t] != n_states-1)
						{
							i = tt_symb[t];
							action[i] = tt_action[t];
							if (optn[LG_INSENSITIVE]) 
							{
								if (lower[i-32] == i) action[i-32] = tt_action[t]; // Upper gets same action.
							}
						}
					}
					for (i = 0; i < max_char_set; i++) // Deactivate all actions.
					{
						if (action[i] == default_action) 
						{
							if (*linenumbstr == 0 || i != 10)
							action[i] = MAX_INT; // Disable it. 
						}
						else if (action[i] == MAX_INT)
						{
							action[i] = -1;
						}
					}
				}
		      for (i = 0; i < max_char_set; i++) seq[i] = i;
  				if (optn[LG_SORTACTIONS])
				{
					SORTNUMBS (action, max_char_set, seq);
				}

				DO_SWITCH  (s);
				DO_CASES   (s, action, seq); 
				DO_DEFAULT (s, default_action); 
			}
			else // No transitions!
			{
				DO_RETURN (s); 
			}
      }

		FREE (action_count, n_states);
		FREE (action, max_char_set);
		FREE (seq, max_char_set);
		FREE (tran_to_id_state, n_states);

		return (n_codelines);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

void  Generate::DO_SWITCH (int s)
{
		if (s == 0) 
		{
			if (bypass_states == 0) 
				  prt_code ("         %s = %s;\n",    tokenstartstr, tokenendstr);
			else prt_code ("S%04d:   %s = %s;\n", s, tokenstartstr, tokenendstr);
			if (optn[LG_LINENUMB])
			{
				prt_code ( "         %s = %s;\n", tokenlinestr, linenumbstr);
				n_codelines++;
			}
			prt_code ("         switch ((unsigned char)*%s)\n", tokenendstr); 
		  	prt_code ("         {\n"); 
			n_codelines += 3;
		}
	  	else        
		{
			if (s == id_state) // If <identifier> state?
			{
				prt_code ("S%04d:   %s++;\n", s, tokenendstr);
				if (n_added > 0)
				     prt_code ("S%04da:  switch ((unsigned char)*%s)\n", s, tokenendstr);
				else prt_code ("         switch ((unsigned char)*%s)\n", tokenendstr);
				prt_code ("         {\n");
				n_codelines += 3;
			}
			else
			{
				if (tran_to_id_state[s] > 0) // If keyword state? 
				{
  					prt_code ("S%04d:   %s++;\n", s, tokenendstr);
					if (tran_to_id_state[s] != SOME && tran_to_id_state[s] != ALL)
					{
  						prt_code ("         switch ((unsigned char)*%s)\n", tokenendstr); 
					}
					else
					{
   					prt_code ("         switch ((unsigned char)*%s)\n", tokenendstr); 
					}
  					prt_code ("         {\n"); 
					n_codelines += 3;
				}
				else // Not keyword state!
				{
  					prt_code ("S%04d:   %s++;\n", s, tokenendstr);
  					prt_code ("         switch ((unsigned char)*%s)\n", tokenendstr); 
  					prt_code ("         {\n"); 
					n_codelines += 3;
				}
			}
		}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

void	Generate::DO_CASES (int s, int* action, int* seq) 
{
		int i, j, k, ch;
		for (i = 0; i < max_char_set; i++) 
		{
			j  = i;
			ch = seq[i];	// ch = the character.
			if (action[j] != MAX_INT)
			{
				P_SYMBOL (ch);
				if (optn[LG_BLANKGOTOS]) 
				{
					for (k = j+1; k < max_char_set; k++) // Get next active action.
					{
						if (action[k] != MAX_INT) break;
					}
					if (k < max_char_set && action[k] == action[j]) // Next action is the same?
					{
						if (*linenumbstr != 0) // Linenumb is active?
						{
							if (seq[k] == '\n' && action[k] != -1) // Next char is '\n'?
							{
								goto Print; 
							}
							else if (ch == '\n' && action[k] != -1) // Current char is '\n'?
							{
								prt_code (" %s++;\n", linenumbstr);
								n_codelines += 1;
							}
							else
							{
								prt_code ("\n");
								n_codelines += 1;
							}
						}
						else
						{
							prt_code ("\n");
							n_codelines += 1;
						}
					}
					else 
					{
Print:				if (action[j] >= 0)
						{
							if (ch == '\n') // Current char is '\n'?
							{
								prt_code (" %s++;", linenumbstr);
							}
						   prt_code (" goto S%04d;\n", action[j]);
							n_codelines += 1;
						}
						else 
						{
							if (ret_numb[D_red[s]] >= 0)
							{
								prt_code (" goto R%04d;\n", s); // Return token number.
								n_codelines += 1;
							}
							else // Ignore 
							{
								prt_code (" goto S%04d;\n", 0);  // Ignore this one.
								n_codelines += 1;
							}
						}
					}
				}
				else 
				{
					prt_code (" goto S%04d;\n", action[j]);
					n_codelines += 1;
				}
			}
		}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

void	Generate::DO_DEFAULT (int s, int default_action)
{
	// Extra code after end of switch statement ...
		if (s == 0)
		{
			if (default_action > 0)
				  prt_code ("            default  : goto S%04d;\n", default_action);
			else prt_code ("            default  : goto R%04d;\n", s);
		   prt_code ("         }\n");
		   prt_code ("R%04d:   %s++;\n", s, tokenendstr);
		   prt_code ("         return %d;\n\n", ret_numb[D_red[s]]);
			n_codelines += 5;
		}
		else 
		{
			if (tran_to_id_state[s] == NONE && n_added > 0)
			{
			  	prt_code ("            default  : goto S%04da;\n", id_state);
			  	prt_code ("         }\n\n");
				n_codelines += 3;
			}
			else if (tran_to_id_state[s] == SOME)
			{
				if (default_action > 0)
					prt_code ("            default  : goto S%04d;\n", default_action);
				else
					prt_code ("            default  : goto R%04d;\n", s);
				prt_code ("         }\n");
			   prt_code ("R%04d:   return %d;\n\n", s, ret_numb[D_red[s]]);
				n_codelines += 4;
			}
			else
			{
				if (ret_numb[D_red[s]] >= 0) // Not ignoring this?
				{
					if (default_action == 0) // Regular action?
					{
						prt_code ("            default  : goto R%04d;\n", s);
						prt_code ("         }\n");
						prt_code ("R%04d:   return %d;\n\n", s, ret_numb[D_red[s]]);
						n_codelines += 4;
					}
					else // Default action!
					{
						prt_code ("            default  : goto S%04d;\n", default_action);
						prt_code ("         }\n");
				  		prt_code ("R%04d:   return %d;\n\n", s, ret_numb[D_red[s]]);
						n_codelines += 4;
					}
				}
				else // Ignoring this token!
				{
					if (default_action == 0) // Regular action?
					{
						prt_code ("            default  : goto S%04d;\n", 0); // Ignore this token.
						prt_code ("         }\n\n");
						n_codelines += 3;
					}
					else // default action!
					{
						prt_code ("            default  : goto S%04d;\n", s); // default action.
						prt_code ("         }\n\n");
						n_codelines += 3;
					}
				}
			}
		}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

void 	Generate::DO_RETURN (int s)
{
		prt_code ("S%04d:   %s++;\n", s, tokenendstr);
		if (ret_numb[D_red[s]] >= 0) prt_code ("         return %d;\n", ret_numb[D_red[s]]);
		else                         prt_code ("         goto S%04d;\n", 0);
		if (s < n_states-2)
		{
			prt_code ("\n");
			n_codelines += 1;
		}
		n_codelines += 2;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

void  Generate::P_SYMBOL (int s)
{
      int L;
      prt_code ("            case ");
		switch (output_type[s])
		{
			case 0: // Regular char output ('A')
			{
				if (term_name[s][1] == '\'') prt_code ("'\\'':");
				else if (term_name[s][1] == '\\') prt_code ("'\\\\':");
				else                         prt_code ("%3s :", term_name[s]);
				break;
			}
			case 1: // Hexadecimal char output (0x1A)
			{
				if (optn[LG_OPTIMIZE]) prt_code ("0x%02X:", s);
				else                   prt_code ("%3d :",   s);
				break;
			}
		  	case 2: // Escape char output ('\n').
			{
			  	if (optn[LG_OPTIMIZE]) 
				{
			  		switch (s)
					{
						case  9: prt_code ("'\\t':"); break;
						case 10: prt_code ("'\\n':"); break;
						case 11: prt_code ("'\\v':"); break;
						case 12: prt_code ("'\\f':"); break;
						case 13: prt_code ("'\\r':"); break;
					}	  
				}
				else prt_code ("%3d :", s);
				break;
			}    
		}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

void  Generate::PRT_STA (int s)
{
      int k, i, f, p, missing_prod, n;

		n = 0;
      prt_code ("///////////////////////////// STATE %d /////////////////////////////\n//\n", s);
		n_codelines += 2;

		if (D_red[s] == MAX_INT) missing_prod = 0;
		else					       missing_prod = D_red[s];

      for (k = f_kernel [s]; k < l_kernel [s]; k++)
      {
         i = kernel [k];
         if (item[i].symb != -32767)
         {
				if (++n > 10) break;
            prt_code ("// * ");
            p_prod (item[i].prod, item[i].dot, "", "\n");
				n_codelines += 1;
         }  
      }

      for (f = f_final [s]; f < l_final [s]; f++)
      {
         p = final [f];
			if (p >= 0 && p < n_oprods)
			{
				if (++n > 10) break;
				prt_code ("// * ");
				p_prod (p, -1, "", "\n");
				if (p ==  D_red[s]) missing_prod = 0;
				n_codelines += 1;
			}
      }

		if (s == illegal_char_state)
		{
         prt_code ("// *  Error state\n");
			n_codelines += 1;
		}

		if (missing_prod)
		{
         prt_code ("// * ");
         p_prod (missing_prod, -1, "", "\n");
			n_codelines += 1;
		}

Ret:  prt_code ("\n");
		n_codelines += 1;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

void  Generate::p_prod (int p, int dot, char *before, char* after)
{
      int t, u, d;
      prt_code ("%s%4d  %s -> ", before, p, head_name [head_sym [p]]);
      t = f_tail [p];
      u = l_tail [p];
      d = t + dot;
      if (dot == -1) d = u;
      for (;;)
      {
         if (t == d) prt_code (". ");
         if (t >= u) break;
         p_sym (tail [t], " ");
         t++;
      }
      prt_code ("%s", after);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int	Generate::p_sym (int s, char *sp)
{
      char *p;
      if (s >= 0)                 /* Terminal symbol? */
         p = term_name[s];
      else                        /* Nonterminal symbol? */
         p = head_name[-s];
      prt_code ("%s%s", p, sp);
      return (strlen(p) + strlen(sp));
}

////////////////////////////////////////////////////////////////////////////////

#endif