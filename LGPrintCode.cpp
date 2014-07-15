
#if 0 // This was used to create DFAC, but table-driven lexers are better overall. 

		#include "ComGlobal.h"
		#include "ComGenerate.h"	   
		#include "LGGlobal.h"

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

		static void  PRT_STA (int s);
		static void  P_SYMBOL (int s);
		static void  p_prod (int p, int dot, char *before, char* after);
		static int   p_sym (int s, char *sp);
		static void  KEYWORD_OPTIMIZATION();
		static void  DO_SWITCH (int i, int s);
		static void	 DO_CASES  (int i, int s, int* action, int* seq); 
		static void	 DO_DEFAULT(int i, int s); 
		static void  DO_RETURN (int iteration, int s);

		static char* hit;
		static int   id_state;
		static int   bypass_states;
		static FILE* codefp;

      static char  labelstr[32] = "S";

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
			if (optn[WARNINGS])
			prt_logonly ("Choosing state %d for the <identifier> state.\n", id_state);
		}
		if (id_state == -1) return;

	//	Remove transitions to id_state ...
		memset (hit, 0, n_states);
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
							tt_action[i] = 0;
						}
					}
					D_red[s] = MAX_INT; 
					hit[s] = 1;
				}
				else
				{
					int n = 0;
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
						hit[s] = 2;
						if (tt_end[s] - tt_start[s] - n) hit[s] = 3; // non zero.
					}
				}
			}
		}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
      
void  Generate::OUTPUT_CODE ()
{					
      char *p, *q;
		int *action, *trans, *seq, iteration;
	   int s, i, j, k, t, nt, nds, next, n;

		if (!optn [LINENUMB]) (char*)linenumbstr = 0;
		if (!optn [COLNUMB ]) (char*)colnumbstr  = 0;
		if (!optn [COLNUMB ]) (char*)tabstopstr  = 0;

		if (optn [DEBUG]) 
		{
			optn [OPTIMIZE]    = 1;   
			optn [SORTACTIONS] = 1;	// Necessary for speed.
			optn [BLANKGOTOS]  = 0;	// Necessary for speed.
		}
		else // Not DEBUG!
		{
			optn [OPTIMIZE]    = 1;   
			optn [SORTACTIONS] = 1;	// Necessary for speed.
			optn [BLANKGOTOS]  = 1;	// Necessary for speed.
		}

		id_state = -1;
		ALLOC (hit, n_states);
		memset (hit, 0, n_states);
		if (optn[KEYWORDIDENT])
		{
   		prt_code ("         int r;\n");
	   	prt_code ("         unsigned char c;\n\n");
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

	// Create all states actions ...
		n_codelines = 0;
      for (s = 0; s < n_states; s++)
      {
		//	if (ret_numb [D_red[s]] == 0) ret_numb [D_red[s]] = 1; // Zero is not used for <error>, 1 is.
			PRT_STA (s);
			nt = tt_end[s] - tt_start[s];	// Number of transitions.
			if (nt > 0 && hit[s] != 2)		// Any transitions but not keyword end state?
			{
			// Action definitions ...
				for (i = 0; i < max_char_set; i++) 
				{
					action[i] = MAX_INT; // Deactivate this.
				}
				for (t = tt_start[s]; t < tt_end[s]; t++)
				{
					if (tt_action[t] != 0)
					{
						i = tt_symb[t];
						action[i] = tt_action[t];
						if (optn[INSENSITIVE]) 
						{
							if (lower[i-32] == i) action[i-32] = tt_action[t]; // Upper gets same action.
						}
					}
				}
		      for (i = 0; i < max_char_set; i++) seq[i] = i;
  				if (optn[SORTACTIONS])
				{
					SORTNUMBS (action, max_char_set, seq);
				}

				n = 1;
				if (s == id_state) n = 2;
				for (iteration = 0; iteration < n; iteration++)
				{
					DO_SWITCH (iteration, s);
					DO_CASES  (iteration, s, action, seq); 
					DO_DEFAULT(iteration, s); 
				}
			}
			else // No transitions!
			{
				DO_RETURN (iteration, s); 
			}
      }
		FREE (action, max_char_set);
		FREE (seq, max_char_set);
		FREE (hit, n_states);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

void  Generate::DO_SWITCH (int iteration, int s)
{
		if (s == 0) 
		{
			if (bypass_states == 0) 
				  prt_code ( "         %s = %s;\n",              tokenstartstr, tokenendstr);
			else prt_code ("%s%04d:   %s = %s;\n", labelstr, s, tokenstartstr, tokenendstr);
		  	prt_code ("         switch ((unsigned char)*%s)\n         {\n",     tokenendstr); 
			n_codelines += 3;
		}
	  	else        
		{
			if (s == id_state) // If <identifier> state?
			{
				if (iteration == 0)
				{
               if (n_added != 0) // Number of extra reductions added?
               {
					   if (colnumbstr != 0)
					   {
   					   prt_code ("%s%04d:   %s++;\n         %s++;\n         c = (unsigned char)*%s;\n%s%04da:  switch (c)\n         {\n", 
						   labelstr, s, colnumbstr, tokenendstr, tokenendstr, labelstr, s);
						   n_codelines += 5;
					   }
 					   else 
					   {
						   prt_code ("%s%04d:   %s++;\n         c = (unsigned char)*%s;\n%s%04da:  switch (c)\n         {\n", 
						   labelstr, s, tokenendstr, tokenendstr, labelstr, s);
						   n_codelines += 4;
					   }
               }
               else // Avoid "Unreferenced label warning message from compiler.
               {
					   if (colnumbstr != 0)
					   {
   					   prt_code ("%s%04d:   %s++;\n         %s++;\n         c = (unsigned char)*%s;\n         switch (c)\n         {\n", 
						   labelstr, s, colnumbstr, tokenendstr, tokenendstr);
						   n_codelines += 5;
					   }
 					   else 
					   {
						   prt_code ("%s%04d:   %s++;\n         c = (unsigned char)*%s;\n         switch (c)\n         {\n", 
						   labelstr, s, tokenendstr, tokenendstr);
						   n_codelines += 4;
					   }
               }
				}
				else
				{
					prt_code ("%s%04db:  switch (c)\n         {\n", labelstr, s);
					n_codelines += 2;
				}
			}
			else
			{
				if (hit[s] > 0) // If keyword state? 
				{
					if (colnumbstr != 0)
					{
   					prt_code ("%s%04d:   %s++;\n         %s++;\n         switch (c = (unsigned char)*%s)\n         {\n", 
						labelstr, s, colnumbstr, tokenendstr, tokenendstr);
						n_codelines += 4;
					}
 					else 
					{
						prt_code ("%s%04d:   %s++;\n         switch (c = (unsigned char)*%s)\n         {\n", 
						labelstr, s, tokenendstr, tokenendstr);
						n_codelines += 3;
					}
				}
				else // Not keyword state!
				{
					if (colnumbstr != 0)
					{
   					prt_code ("%s%04d:   %s++;\n         %s++;\n         switch ((unsigned char)*%s)\n         {\n", 
						labelstr, s, colnumbstr, tokenendstr, tokenendstr);
						n_codelines += 4;
					}
 					else 
					{
						prt_code ("%s%04d:   %s++;\n         switch ((unsigned char)*%s)\n         {\n", 
						labelstr, s, tokenendstr, tokenendstr);
						n_codelines += 3;
					}
				}
			}
		}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

void	Generate::DO_CASES (int iteration, int s, int* action, int* seq) 
{
		int i, j, k, ch;
		for (i = 0; i < max_char_set; i++) 
		{
			j  = i;
			ch = seq[i];	// ch = the character.
			if (action[j] != MAX_INT)
			{
				P_SYMBOL (ch);
				if (ch == 9) // tab 
				{
					if (colnumbstr != 0) 
					{
						prt_code (" %s += %s - (%s %% %s);", colnumbstr, tabstopstr, colnumbstr, tabstopstr);
						prt_code (" goto %s%04d;\n", labelstr, action[j]);
						n_codelines += 1;
						continue;
					}
				}
				else if (ch == 10) // newline
				{
					if (linenumbstr != 0) 
					{
						prt_code (" %s++;", linenumbstr);
						if (colnumbstr != 0) prt_code (" %s = 0;", colnumbstr);
						prt_code (" goto %s%04d;\n", labelstr, action[j]);
						n_codelines += 1;
						continue;
					}
				}
				else if (ch == 26) // eof
				{
					if (colnumbstr != 0) 
					{
						prt_code (" %s = 0;", colnumbstr);
						prt_code (" goto %s%04d;\n", labelstr, action[j]);
						n_codelines += 1;
						continue;
					}
				}
				if (optn[BLANKGOTOS]) 
				{
					for (k = j+1; k < max_char_set; k++) // Get next active action.
					{
						if (action[k] != MAX_INT) break;
					}
					if (k < max_char_set 
					&& action[k] == action[j] 
					&& (linenumbstr == 0 || seq[k] != '\n') 
					&& (colnumbstr  == 0 || seq[k] != '\t'))
					{
						prt_code ("\n");
						n_codelines += 1;
					}
					else 
					{
						prt_code (" goto %s%04d;\n", labelstr, action[j]);
						n_codelines += 1;
					}
				}
				else 
				{
					prt_code (" goto %s%04d;\n", labelstr, action[j]);
					n_codelines += 1;
				}
			}
		}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

void	Generate::DO_DEFAULT (int iteration, int s)
{
	// Extra code after end of switch statement ...
		if (s == 0)
		{
		   prt_code ("            default  : %s++; return %d; // error\n         }\n\n", 
			tokenendstr, ret_numb[D_red[s]]);
			n_codelines += 3;
		}
		else 
		{
			if (iteration == 0)
			{
				if (hit[s] == 1 && n_added != 0)
				{
					prt_code ("            default  : goto %s%04da;\n         }\n\n", labelstr, id_state);
					n_codelines += 3;
				}
				else if (hit[s] == 3)
				{
					prt_code ("            default  : r = %d; goto %s%04db;\n         }\n\n", 
						ret_numb[D_red[s]], labelstr, id_state);
					n_codelines += 3;
				}
				else
				{
					if (ret_numb[D_red[s]] >= 0) 
						  prt_code ("            default  : return %d;\n         }\n\n", ret_numb[D_red[s]]);
					else prt_code ("            default  : goto %s%04d;\n\t\t}\n\n", labelstr, 0);
					n_codelines += 3;
				}
			}
			else
			{
				prt_code ("            default  : return r;\n         }\n\n");
				n_codelines += 3;
			}
		}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

void 	Generate::DO_RETURN (int iteration, int s)
{
		if (hit[s] == 2) // End of keyword?
		{
			prt_code ("%s%04d:   %s++;\n         c = (unsigned char)*%s;\n         r = %d;\n         goto %s%04db;\n\n", 
			labelstr, s, tokenendstr, tokenendstr, ret_numb[D_red[s]], labelstr, id_state);
			n_codelines += 5;
		}
		else
		{
			if (colnumbstr != 0)
			{
				prt_code ("%s%04d:   %s++;\n         %s++;\n", labelstr, s, colnumbstr, tokenendstr);
				n_codelines += 2;
			}
			else 
			{
				prt_code ("%s%04d:   %s++;\n", labelstr, s, tokenendstr);
				n_codelines += 1;
			}
			if (ret_numb[D_red[s]] >= 0) prt_code ("         return %d;\n\n", ret_numb[D_red[s]]);
			else                         prt_code ("         goto %s%04d;\n\n", labelstr, 0);
			n_codelines += 2;
		}
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
				if (optn[OPTIMIZE]) prt_code ("0x%02X:", s);
				else                prt_code ("%3d :",   s);
				break;
			}
		  	case 2: // Escape char output ('\n').
			{
				if (optn[OPTIMIZE]) prt_code ("0x%02X:", s);
				else                prt_code ("%3d :",   s);
			/*	switch (s)
				{
					case  9: prt_code ("'\\t':"); break;
					case 10: prt_code ("'\\n':"); break;
					case 11: prt_code ("'\\v':"); break;
					case 12: prt_code ("'\\f':"); break;
					case 13: prt_code ("'\\r':"); break;
					case 32: prt_code ("' ' :" ); break;
				}	*/
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
				if (++n == 10) goto Ret;
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
				if (++n == 10) goto Ret;
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
