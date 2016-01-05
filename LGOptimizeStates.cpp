
#ifdef DFASTAR

		#include "ComGlobal.h"
		#include "LGGlobal.h"
		#include "LGOptimizeStates.h"

		static int   nosl;
		static int   tnsr;
		static int   n_cres;
		static int   nt_left;
		static int   n_changed;
		static int   n_usedheads;
		static int   n_usedprods;
		static int   n_jumps;
		static int   nrs;
		static int   depth;
		static int*  active;

		static int*  jumpto;
		static int*  done;
		static int*  T_hit;
		static int*  goesto;
		static int*  LA;
		static int*  nt_problem;
		static int*  tta_end;
		static int*  tta_start;
		static int*  nu_list;
		static int*  nu_symb;

		static uchar* tta_symb;
		static int*   tta_list;

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    LGOptimizeStates                                                               //

int   LGOptimizeStates::OptimizeStates ()
{
	   int i, s, p, rc = 0;
   // Disable the accept state.
      tt_start[accept_state] =  0; // Remove $end transition.
      tt_end  [accept_state] =  0;
      D_red   [accept_state] = -1;
      la_start[accept_state] =  0;
      la_end  [accept_state] =  0;

		rc = 0;
		if (REMOVE_NT_TRANS ())			
		{
			REMOVE_DUP_STATES();
	     	RENUMBER_STATES ();
			DESTROY_LOOKAHEADS();
		  	ILLEGAL_CHARS ();
			rc = 1;
		}

	//	opt_stat_memory = memory_max;
		return rc;
}
  
void  LGOptimizeStates::DESTROY_LOOKAHEADS ()
{
		int s;
		for (s = 0; s < n_states; s++)
		{
			la_start[s] = 0;
			la_end[s] = 0;
		}
		la_start[s] = 0;
}

void  LGOptimizeStates::ILLEGAL_CHARS ()
{
		int i, n, t;
		int* buffer;

		ALLOC (buffer, n_terms);
		for (t = 0; t < n_terms; t++) buffer[t] = 0;	 

		n = 0;
		for (i = tt_start[0]; i < tt_end[0]; i++)
		{
			n++; 
			t = tt_symb[i];	
			if (optn[LG_INSENSITIVE] && charcode[t] == LOWERCASE)
			{	
				n++; 
				buffer[t-32] = tt_action[i];	// buffer['A'] = same as buffer['a']
			}
			buffer [t] = tt_action[i];	
		}

		n = n_terms - n; // get number of illegal characters.
		illegal_char_state = 0; // 0 means we have done optimization (for LGPrintStates).
		if (n > 0) // Any illegal characters?
		{	
			if (optn[LG_DIRECTCODE] == 0)
			prt_log ("         %7d state added for %d illegal characters.\n", 1, n);
		/*	if (optn[LG_VERBOSE] > 1)
			{
				char* str = "";
				if (n != 1) str = "s";
				prt_logonly ("%d illegal character%s found in state 0, lexer will return %d for:", n, str, 0);
				n = 0;
				for (t = 0; t < n_terms; t++) 
				{
					if (buffer[t] == 0) 
					{
						char* nl = "";
						if (n++ % 16 == 0) nl = "\n";
						prt_logonly ("%s %4s", nl, term_name[t]);
					}
				}
				prt_logonly ("\n\n");
			}  */

			illegal_char_state   = n_states; 
			accept_state         = 0; // This should be zero for LGPrintStates.
			ntt_start [n_states] = 0;
			ntt_end   [n_states] = 0;
			tt_start  [n_states] = 0;
			tt_end    [n_states] = 0;
			la_start  [n_states] = 0;
			la_end    [n_states] = 0;
			f_kernel  [n_states] = 0;
			l_kernel  [n_states] = 0;
			f_final   [n_states] = 0;
			l_final   [n_states] = 0;
			f_camefrom[n_states] = 0;
			l_camefrom[n_states] = 0;
			D_red     [n_states] = 0; 
			ret_numb  [0]        = 0;

		// Move transitions for state 0 ...
			i = tt_start[0];
			tt_start[0] = n_totalterm;
			for (; i < tt_end[0]; i++)
			{
				tt_action [n_totalterm  ] = tt_action[i];				
				tt_symb [n_totalterm++] = tt_symb[i];
			}

		// Add illegal character transitions for state 0 ...
			for (t = 0; t < n_terms; t++) 
			{
				if (buffer[t] == 0)		
				{
					tt_action [n_totalterm  ] = n_states; 
					tt_symb [n_totalterm++] = t;
				}
			}
			tt_end[0] = n_totalterm;
			n_states++;
		}
		if (optn[LG_DIRECTCODE] == 0)
		     prt_log ("         %7d states, in final DFA state machine.\n", n_states);
		else prt_log ("         %7d states, in final DFA state machine.\n", n_states-1);
		FREE (buffer, n_terms);
}
  
void  LGOptimizeStates::REMOVE_DUP_STATES () 
{
		int s, s1, s2, t, *trans, nt1, nt2, nsr, tnsr = 0, nc = 0;

		if (optn[LG_REMOVEDUP] == 0) goto Done;
		ALLOC (trans, n_terms);
		do
		{
    		nsr = 0;
			for (s1 = 1; s1 < n_states-1; s1++)
			{
				if (st_type[s1] != 0)
				{
					nt1 = tt_end[s1] - tt_start[s1];
					for (t = 0; t < n_terms; t++) trans[t] = 0;
					for (t = tt_start[s1]; t < tt_end[s1]; t++) 
					{
						trans[tt_symb[t]] = tt_action[t];
					}
					for (s2 = s1+1; s2 < n_states-1; s2++)
					{
						if (st_type[s2] != 0)
						{
							if (D_red[s1] == D_red[s2])
							{
								nt2 = tt_end[s2] - tt_start[s2];
								if (nt1 == nt2)
								{
									nc++;
									for (t = tt_start[s2]; t < tt_end[s2]; t++) 
									{
										int sym = tt_symb[t];
										if (trans[sym] != tt_action[t])  
										{
											if (tt_action[t] != s1 && tt_action[t] != s2) goto NextS2;
											if (trans[sym] != s1 && trans[sym] != s2) goto NextS2;
										}  
									}
									nsr++;
									st_type[s2] = 0; // Remove s2.
									for (s = 0; s < n_states; s++) // For all states.
									{
										if (st_type[s] != 0) // If still in use.
										{
											for (t = tt_start[s]; t < tt_end[s]; t++) // For all transitions.
											{
												if (tt_action[t] == s2) tt_action[t] = s1;
											}
										}
									}
								}
							}
						}
NextS2:				continue;
					}
				}
			}
			tnsr += nsr;
		}
		while (nsr > 0);
		FREE (trans, n_terms);

Done: char* ss = "s";
		if (tnsr == 1)	ss = "";
		if (optn[LG_VERBOSE])
  		prt_log ("         %7d state%s removed, %d comparisons.\n", tnsr, ss, nc);
}

int   LGOptimizeStates::REMOVE_NT_TRANS ()
{
	   int   s;
	   char* schar;

		ALLOC (nt_problem, n_heads);
		FASTINI (0, nt_problem, n_heads);

	   if (REMOVE_SINGLE_RED()) 
		{
		  	if (REMOVE_OTHER_RED())		
			{
				MODIFY_TERMINAL_TRAN();
			}	  
		}

		nt_left = COUNT_NT_LEFT ();
		if (nt_left == 0)
		{
			for (s = 0; s < n_states; s++)
			{
				ntt_start[s] = ntt_end[s] = 0;
			}
		}

		nosl  = NUMBER_OF_STATES_LEFT();
		for (s = 0; s < n_states; s++) 
		{
			if (active[s] == 0) st_type[s] = 0; // For renumbering the states later.
		}

		if (optn[LG_VERBOSE])
	  	prt_log ("         %7d states, after converting to a DFA.\n", nosl);	// +1 for error state creatd later.
		FREE (nt_problem, n_heads);
		FREE (jumpto, n_nonttran);
		return (nt_left == 0 ? 1 : 0);
}

int   LGOptimizeStates::REMOVE_SINGLE_RED () // Remove NT Transitions for Final Items in Terminal States.
{
		int  s;	// state
		int  t;	// transition #

		ALLOC (jumpto, n_nonttran);
		for (s = 0; s < n_states; s++) 
		{
			for (t = tt_start[s]; t < tt_end[s]; t++) 
			{
				n_jumps = 0;
				tt_action[t] = TRANSITION (s, tt_action[t]);
			}
			for (t = ntt_start[s]; t < ntt_end[s]; t++) 
			{
				n_jumps = 0;
				ntt_action[t] = TRANSITION (s, ntt_action[t]);
			}
		}
	//	return (nt_left == 0 ? 1 : 0);
	  	return (1);	// Picks up more NT's remaining this way, more messages for user!
}

int   LGOptimizeStates::TRANSITION (int s, int x)				
{
		int  p, nj, j, tran;	
		if (st_type[x] == RO_STATE)					// if reduce-only state?
		{
			p = final[f_final[x]];						// get production
			if (ret_numb[p] == 0)						// if not a "Token ->" production
			{
				nj = n_jumps;								// get current top of jump list
 				LOOK_BACK (p, prod_len [p]-1, s);	// from here (s) not there (x)	
				tran = ntt_action [jumpto[nj]];			// default transition is first one
				for (j = nj+1; j < n_jumps; j++)		// all jumps go to the same state?
				{
					if (ntt_action [jumpto[j]] != tran) 
					{ 
						if (nt_problem [ntt_symb [jumpto[0]]] == 0)
						{
							nt_left++;
							nt_problem [ntt_symb [jumpto[0]]] = 1;
						}
						n_jumps = nj;						// reset top of jump list
				    	return (x);
					}
				}
				n_jumps = nj;								// reset top of jump list
				return (tran);
			}
		}
    	return (x);
}

void  LGOptimizeStates::LOOK_BACK (int p, int dot, int s)
{
		int i;
      if (dot-- > 0) // If not at the origin yet.
      {
         for (i = f_camefrom[s]; i < f_camefrom[s+1]; i++)
         {
            LOOK_BACK (p, dot, camefrom[i]);
         }
      }
		else  // Find NT transition in origin state.
      {
			for (i = ntt_start[s]; i < ntt_end[s]; i++)
			{
	         if (ntt_symb[i] == head_sym[p])
				{
					jumpto[n_jumps++] = i;
					ntt_action[i] = TRANSITION (s, ntt_action[i]);
					return;
				}
			}
      }    
}

int   LGOptimizeStates::REMOVE_OTHER_RED () // Remove NT Transitions for all lookahead actions.
{
		int s, i, t, x, y, z; 

		max_ttas = optn[MAX_TTA];

		ALLOC (LA,        n_terms);
		ALLOC (T_hit,     n_terms);
		ALLOC (tta_start, n_states);
		ALLOC (tta_end,   n_states);
		ALLOC (tta_symb,  max_ttas); 
		ALLOC (tta_list,  max_ttas);

		FASTINI (0, tta_start, n_states);
		FASTINI (0, tta_end,   n_states);

		n_ttas = 0;
      for (s = 0; s < n_states; s++)
      {
			n_jumps = 0;
			FASTINI (0, LA, n_terms);
	  		for (i = tt_start [s]; i < tt_end [s]; i++)	
			{
				LA [tt_symb[i]] = -1; // Put shift actions in LA as -1.
			}
			COLLECT_LOOKAHEADS (s);
			tta_start[s] = n_ttas;					// set start of list
			for (t = 0; t < n_terms; t++)				// for all terminals
			{
				if (LA[t] > 0)								// new added lookahead?
				{
					if (n_ttas >= max_ttas) 
						MemCrash ("Number of terminal transitions added", max_ttas);
					tta_symb [n_ttas] = t;		   // add terminal symbol
					tta_list [n_ttas++] = LA[t];	// add transition
				//	prt_log ("ADD %s -> state %d\n", term_name [t], LA[t]);
				}
			}	   
			tta_end[s] = n_ttas;					// set end of list
      }
		return (nt_left == 0 ? 1 : 0);
}

void  LGOptimizeStates::COLLECT_LOOKAHEADS (int s) 
{
		int  nki, nfi, f, p, nj, j, tran, t, x; 
	//	prt_log ("Collect Lookaheads from state %d\n", s);
		if (st_type[s] != RO_STATE)
		{
			for (f = f_final[s]; f < f_final[s+1]; f++)	// Check all final items.
			{
				p = final [f];										// get production
				if (ret_numb[p] == 0)							// not a "Token ->" production?
				{
					nj = n_jumps;									// get current top of jump list
					LOOK_BACK2 (p, prod_len[p], s);			// Look back from this state.
					tran = ntt_action [jumpto[nj]];	 			// default transition is first one
					for (j = nj+1; j < n_jumps; j++)			// all jumps go to the same state?
					{
						if (ntt_action [jumpto[j]] != tran) 
						{
							if (nt_problem [ntt_symb [jumpto[0]]] == 0)
							{
								nt_left++;
								nt_problem [ntt_symb [jumpto[0]]] = 1;
							}
							n_jumps = nj;							// reset top of jump list
							return;
						}
					}
					n_jumps = nj;									// reset top of jump list

				  	COLLECT_LOOKAHEADS (tran);					// collect more lookaheads

				//	prt_log ("\nAdd transitions to state %d\n", s);
			  		for (t = tt_start [tran]; t < tt_end [tran]; t++) // for tran state
					{
						if (LA [tt_symb[t]] == 0)				// If not defined yet.
						{
							LA [tt_symb[t]] = tt_action[t];		// add transitions to collection
					  	//	prt_log ("add %s -> state %d\n", term_name [tt_symb[t]], tt_action[t]);
						}
					}	   
					if (D_red[s] != 0)							// If not disabled, added 20091106 PBM
					{
						if (ret_numb [D_red[s]] == 0)		// not a 'Token ->' rule?
						{
							if (D_red[tran] != -1)           // If default exists?
							D_red[s] = D_red[tran];			   // change default reduction
						}
					}
				}
			}
		}
}

void  LGOptimizeStates::LOOK_BACK2 (int p, int dot, int s)
{
		int i;
      if (dot-- > 0) // If not at the origin yet.
      {
         for (i = f_camefrom[s]; i < f_camefrom[s+1]; i++)
         {
            LOOK_BACK2 (p, dot, camefrom[i]);
         }
      }
		else  // Find NT transition in origin state.
      {
			for (i = ntt_start[s]; i < ntt_end[s]; i++)
			{
	         if (ntt_symb[i] == head_sym[p])
				{
					jumpto[n_jumps++] = i;
					return;
				}
			}
      }    
}

int   LGOptimizeStates::MODIFY_TERMINAL_TRAN ()						
{
	   int s, t, n, start, nfi, first = 1;

	// Count the number of terminal transitions. 
		n_totalterm = 0;
		for (s = 0; s < n_states; s++)
		{
			n_totalterm += (tt_end[s] - tt_start[s]) + (tta_end[s] - tta_start[s]);
		}

		ALLOC (nu_list, n_totalterm+256);  // +256 in case of illegal_char state.
		ALLOC (nu_symb, n_totalterm+256);

		n = 0;
	// Merge original and newly added terminal transitions.
		for (s = 0; s < n_states; s++)
		{
			start = n;
			for (t = tt_start[s]; t < tt_end[s]; t++)
			{
				nu_list[n]   = tt_action[t];
				nu_symb[n++] = tt_symb[t];
			}
			for (t = tta_start[s]; t < tta_end[s]; t++)
			{
				nu_list[n]   = tta_list[t];
				nu_symb[n++] = tta_symb[t];
			}
			tt_start[s] = start;
			tt_end  [s] = n;
		}

		FREE (tt_action, n_termtran);
		FREE (tt_symb, n_termtran);

		FREE (tta_list,  max_ttas);
		FREE (tta_symb,  max_ttas);
		FREE (tta_start, n_states);
		FREE (tta_end,   n_states);

		tt_action = nu_list;
		tt_symb = nu_symb;

		for (s = 0; s < n_states; s++)
		{
			if (D_red[s] == -1) D_red[s] = 0; // Error.
		}
		if (first == 0) prt_log ("\n");
		return (1);
}

int   LGOptimizeStates::NUMBER_OF_STATES_LEFT ()
{
		int i, s, nosl;

		ALLOC (active, n_states);
		FASTINI (0, active, n_states); // Set active status to inactive. 

	// Mark all active states from transitions ...
		active[0] = 1; 
		for (s = 0; s < n_states; s++) // For all states.
		{
			for (i = tt_start[s]; i < tt_end[s]; i++) // For all terminal transitions. 
			{
				if (tt_action[i] > 0) active [tt_action[i]] = 1;	// Mark this state active. 
			}
			for (i = ntt_start[s]; i < ntt_end[s]; i++) // For all terminal transitions. 
			{
				if (ntt_action[i] > 0) active [ntt_action[i]] = 1;	// Mark this state active. 
			}
		}
		active[accept_state] = 0; // Token -> accept_state, so take it out. 

	// Count the number of states left ...
		nosl = 0;
		for (s = 0; s < n_states; s++)
		{
			if (active [s]) nosl++;
		}
		return (nosl);
}

int   LGOptimizeStates::COUNT_NT_LEFT ()
{
	   int h, n = 0;
		for (h = 0; h < n_heads; h++) 
		{
			if (nt_problem [h]) 
			{
				n++;
				prt_error ("Nonterminal '%s' could not be removed, lexical grammar cannot generatae a DFA", head_name[h], 0, head_line[h]);
			}
		}
		return (n);
}

int   LGOptimizeStates::REMOVE_DUPLICATES () // Not being used 20100512 
{
		int s, t, x, nd; 
		nd = 0;
		ALLOC (goesto, n_states);
		for (s = 0; s < n_states-1; s++) goesto[s] = s;
		for (s = 0; s < n_states-1; s++)
		{
			if (active[s])
			{
				FASTINI (0, T_hit, n_terms);
				for (t = tt_start[s]; t < tt_end[s]; t++)
				{
					if (tt_action[t] > 0) T_hit[tt_symb[t]] = tt_action[t]; 
				}
				for (x = s+1; x < n_states; x++)
				{
					if (active[x])
					{
						for (t = tt_start[x]; t < tt_end[x]; t++)
						{
							if (tt_action[s] >  0)
							{
								if (T_hit [tt_symb[t]] == tt_action[t]) T_hit [tt_symb[t]] = 0;
								else goto Next;
							}
						}
						for (t = 0; t < n_terms; t++)
						{
							if (T_hit[t] != 0) goto Next;
						}
						if (D_red[s] != D_red[x]) goto Next;
						goesto[s] = x;
						nd++;
					}
Next:				continue;
				}
			}
		}
		if (nd) 
		{
			for (s = 0; s < n_states; s++)
			{
				if (active[s])
				{
					if (s != goesto[s])
					{
						active[s] = 0;
						goesto[s] = GOESTO_CLOSURE (goesto[s]);
					}
				}
			}
		}
		for (s = 0; s < n_states; s++)
		{
			if (active[s])
			{
				for (t = tt_start[s]; t < tt_end[s]; t++)
				{
					if (tt_action[t] > 0) tt_action[t] = goesto [tt_action[t]]; 
				}
			}
		}
		FREE (goesto, n_states);
		return (nd);
}

int   LGOptimizeStates::GOESTO_CLOSURE (int s)
{
		if (s != goesto[s])
		{
			active[s] = 0;
			goesto[s] = GOESTO_CLOSURE (goesto[s]);
			return (goesto[s]);
		}
		return (s);
}

void  LGOptimizeStates::RENUMBER_STATES ()
{
      int*  newplace;
      char* replaced;
      int   FK, LK, FF, LF, FC, LC, ST, AC;
      int   NTS, NTE, TTS, TTE, LAS, LAE, DR;
      int   i, s, x, newstate, origin, op_states;

      newstate = 0;
      ALLOC (newplace, n_states);
		FASTINI (-1, newplace, n_states);

   // Do all NT states first.
      for (s = 0; s < n_states; s++) 
      {
         if (st_type[s] & NT_STATE) 
         {
            newplace[s] = newstate++;
         }
      }
      ntt_states = newstate;

   // Do all TT states next.
      for (s = 0; s < n_states; s++) 
      {
         if (s != accept_state)
         {
            if (st_type[s] & TT_STATE && !(st_type[s] & NT_STATE)) 
            {
               newplace[s] = newstate++;
            }
         }
      }
      tt_states = newstate;

   // Do all MR states next.
      for (s = 0; s < n_states; s++) 
      {
         if (st_type[s] == MR_STATE) 
         {
            newplace[s] = newstate++;
         }
      }

   // Do RO states next.
      for (s = 0; s < n_states; s++) 
      {
         if (st_type[s] == RO_STATE) 
         {
            newplace[s] = newstate++;
         }
      }
		op_states = newstate;

	// Do zero states last.
		for (s = 0; s < n_states; s++) 
		{
		  	if (st_type[s] == 0)		// Zero?
			{
				newplace[s] = newstate++;
			}
		}

   // MOVE STATES ALGORITHM ...
      origin = 0;
      ALLOC (replaced, n_states);					
      memset (replaced, 0, n_states);
      for (s = 0; s < n_states; s++)
      {												 
Top:     x = newplace[s];
         if (x != s) // If this one moves. 
         {
            if (!replaced [x]) // If newstate not replaced yet.
            {
            // prt_log ("\nState %d -> %d\n", s, x);
               if (origin == 0) origin = s;
               else if (origin == x)
               {
                  replaced[x] = 1;
                  origin = 0;
                  s = x + 1; 
                  goto Top;
               }

            // Save target state data structures.
            // prt_log ("Saving %d\n", x);
               NTS = ntt_start[x];
               NTE = ntt_end  [x];
               TTS = tt_start[x];
               TTE = tt_end  [x];
               LAS = la_start[x];
               LAE = la_end  [x];
               FK  = f_kernel[x];
               LK  = l_kernel[x]; // Don't care about these for DFA creation.
               FF  = f_final [x];
               LF  = l_final [x];
               FC  = f_camefrom[x];
               LC  = l_camefrom[x];
               ST  = st_type [x];
               AC  = accessor[x];
               DR  = D_red   [x];

            // Move origin state into the new state. 
            // prt_log ("Moving %d to %d\n", origin, x);
               ntt_start[x]  = ntt_start[origin];
               ntt_end  [x]  = ntt_end  [origin];
               tt_start[x]   = tt_start[origin];
               tt_end  [x]   = tt_end  [origin];
               la_start[x]   = la_start[origin];
               la_end  [x]   = la_end  [origin];
               f_kernel[x]   = f_kernel[origin];
               l_kernel[x]   = l_kernel[origin];
               f_final [x]   = f_final [origin];
               l_final [x]   = l_final [origin];
               f_camefrom[x] = f_camefrom[origin];
               l_camefrom[x] = l_camefrom[origin];
               st_type [x]   = st_type [origin];
               accessor[x]   = accessor[origin];
               D_red   [x]   = D_red   [origin]; 
            
            // Move saved state into origin state. 
            // prt_log ("Moving saved state to %d\n", origin);
               ntt_start[origin]  = NTS;
               ntt_end  [origin]  = NTE;
               tt_start[origin]   = TTS;
               tt_end  [origin]   = TTE;
               la_start[origin]   = LAS;
               la_end  [origin]   = LAE;
               f_kernel[origin]   = FK; 
               l_kernel[origin]   = LK;
               f_final [origin]   = FF;
               l_final [origin]   = LF;
               f_camefrom[origin] = FC;
               l_camefrom[origin] = LC;
               st_type [origin]   = ST;
               accessor[origin]   = AC;
               D_red   [origin]   = DR;

               replaced[x] = 1; // Mark new state as replaced. 
               s = x;           // Now set the origin to the new state.
               goto Top;
            }
         }
      }

   // CORRECT TRANSITIONS ...
      for (s = 0; s < n_states; s++)
      {
         for (i = tt_start[s]; i < tt_end[s]; i++) 
         {
				if (tt_action[i] > 0) tt_action[i] = newplace [tt_action[i]];
         }
         for (i = ntt_start[s]; i < ntt_end[s]; i++) 
         {
            ntt_action[i] = newplace [ntt_action[i]];
         }
         for (i = f_camefrom[s]; i < l_camefrom[s]; i++) 
         {
            camefrom[i] = newplace [camefrom[i]];
         }	  
      }

      accept_state = newplace [accept_state];
      n_states     = op_states;
		if (active  == NULL) ALLOC (active, n_states);
		for (s = 0; s < n_states; s++) active[s] = 1;
   // prt_log ("        %6d nt states, %d tt states.\n", nt_states, tt_states);

      FREE (newplace, n_states);
      FREE (replaced, n_states);
}

#endif

/*--- End of Make_DFA.CPP ---------------------------------------------------*/

