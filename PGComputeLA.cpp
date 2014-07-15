
#ifdef LRSTAR

      #include "ComGlobal.h"
		#include "PGComputeLA.h"

      #ifdef _DEBUG 
      #define PRINTF printf
      #else
      #define PRINTF prt_con
      #endif

		int    PGComputeLA::n_nditems;
	   int*   PGComputeLA::nd_start;
		int*   PGComputeLA::nd_term;
		int*   PGComputeLA::nd_action;
		int    PGComputeLA::c_states;

		int*   PGComputeLA::D_red;
		int*   PGComputeLA::la_start;
		int*   PGComputeLA::la_end;
		int*   PGComputeLA::la_symb;
		int*   PGComputeLA::la_red;

		int    PGComputeLA::n_lookbacks;
		int    PGComputeLA::n_lookah;
		int    PGComputeLA::n_includes;

      static int    sr_impacts;
      static int    rr_impacts;
      static int    impacted_states;
      static char*  ntt_done;
      static int    top;
      static int*   ntt_stack;
      static int    nw;
      static int    sum;
      static char** LA;
      static int*   ntt_include;						   
      static int*   action;
      static int*   actionindx;
      static char*  firstaction;
      static int*   conflict;
      static int    rr_errors;
      static int    n_ndstates;
      static int*   already;
		static int	  depth;
      static int*   f_lookback;
      static int    inc_on;
      static int*   include;
      static int*   lookback;
      static int    max_lookback;
      static int    max_include;
		static int    max_nditems;
      static int    n_bytes;
      static int    n_relations;
      static int    n_words;
      static int*   newloc;
      static int*   redconf;
      static int    rr_con;
      static int    sr_con;
		static int    n_states_resolved;

      static int    sr, rr;
      static char*  LASUM;

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGComputeLA::ComputeLA () /* Analyze States */
{
      if (optn [PG_CONFLICTS])
      {
			prt_con("\n");
			if (optn[PG_REDUCEONLY])
			prt_con ("REDUCE-REDUCE ");
         prt_con ("CONFLICTS REPORT:\n\n");
      }
		else 
		{
         prt_con ("\nCONFLICTS REPORT:\n\n");
			prt_con ("None of the options: 'c', 'rr', 'ct' was specified.\n\n");
		}

		impacted_states = 0;
		sr_impacts      = 0;
		rr_impacts      = 0;
      c_states        = 0;
      n_nditems       = 0;
      sr_con          = 0;
      rr_con          = 0;
      rr_errors       = 0;
      n_lookah        = 0;

		#ifdef USINGBITS
		n_words = (N_terms + 31)/32;  // Number of 32-bit words. 
		#else                 
		n_words = (N_terms +  3)/4;   // Number of 4-byte words. 
		#endif

      n_bytes     = 4*n_words;           /* Number of bytes to allocate. */
      max_lookah  = optn[MAX_LA];  
      max_child   = optn[MAX_CH];
      
      ALLOC (D_red,    n_states); 
      ALLOC (la_start, n_states+1); 
      ALLOC (la_symb,  max_lookah);
      ALLOC (la_red,   max_lookah);

	 	C_LOOKBACKS ();
		C_READS ();
		C_LOOKAHEADS ();

      char *ch;
      if (optn[PG_NONDETER])
      {
			ch = "s";
         if (n_ndstates == 1) ch = ""; 
			if (optn[PG_NONDETER] == 1) 
			prt_log ("ND(1)    %7d nondeterministic state%s for RR conflicts.\n",        n_ndstates, ch);
			if (optn[PG_NONDETER] == 2) 
			prt_log ("ND(2)    %7d nondeterministic state%s for SR and RR conflicts.\n", n_ndstates, ch);
      }
		if (optn[PG_CONFLICTS])
		{
			if (sr_con + rr_con > 0) // Any conflicts?
			prt_con ("/////////////////////////////////////////////////////////////////////\n\n");
		}

      ch = "s";
      if (c_states == 1) ch = ""; 
      prt_log ("Conflicts%7d state%s, %d shift-reduce, %d reduce-reduce conflicts.\n", c_states, ch, sr_con, rr_con);
	/*	if (optn[PG_VERBOSE])
		{
			ch = "s";
			if (impacted_states == 1) ch = ""; 
			prt_log ("Impacts  %7d state%s, %d shift-reduce, %d reduce-reduce impacts.\n", 
			impacted_states, ch, sr_impacts, rr_impacts, ch);
		}
	*/
		prt_con ("CONFLICTS: %d state%s, %d shift-reduce, %d reduce-reduce conflicts.\n\n", c_states, ch, sr_con, rr_con);
	//	prt_con ("IMPACTS:   %d state%ss, %d shift-reduce, %d reduce-reduce impacts.\n\n", impacted_states, ch, sr_impacts, rr_impacts);

      if (optn[PG_NONDETER])
      {
		  	if (n_states_resolved > 0)
			{
				ch = "s";
				if (n_states_resolved == 1) ch = ""; 
		      prt_log ("         %7d conflict state%s resolved in favor of shift action.\n", n_states_resolved, ch);
			}	
      }
		prt_log ("\n");

		char* msg;
      if (c_states) // Conflicts?
		{
			msg = "not LALR(1) and not LR(1)";
		}
		else // No conflicts!
		{
			if (extra_states) // Extra states added?
			{
				msg = "LR(1) and not LALR(1)";
			}
			else // No extra states added!
			{
				if (n_lookah)
				{
					if (optn[PG_CLR_PARSER]) msg = "LR(1) and may be LALR(1) or even LR(0)";
					else                     msg = "LALR(1)";
				}
				else msg = "LR(0)";
			}
		}
		prt_con ("Grammar is %s", msg);
		if (optn[PG_VERBOSE])
		prt_log ("Grammar is %s", msg);
		if (extra_states || optn[PG_LR_PARSER])
		{
			char* s = "s were";
			if (extra_states == 1) s = " was";
			prt_con (", %d extra state%s added", extra_states, s);
			if (optn[PG_VERBOSE])
			prt_log (", %d extra state%s added", extra_states, s);
		}
		prt_con (".\n\n");
		if (optn[PG_VERBOSE])
		prt_log (".\n\n");

		if (optn[PG_NONDETER] == 0) 
			; // nothing
		else if (optn[PG_NONDETER] == 1)
			prt_con ("nd=1 was specified, %d reduce-reduce conflicts were ignored.\n\n", rr_con);
		else if (optn[PG_NONDETER] == 2)
			prt_con ("nd=2 was specified, %d shift-reduce and %d reduce-reduce conflicts were ignored.\n\n", sr_con, rr_con);
		else 
			prt_con ("nd=%d was specified, %d shift-reduce and %d reduce-reduce conflicts were ignored.\n\n", optn[PG_NONDETER], sr_con, rr_con);

      return 1; // For stupid compiler complaint.
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGComputeLA::C_LOOKAHEADS ()
{
      int  nfi, t, h, i, p, s, f, n;

      for (i = 0; i < n_includes; i++) 
      {
         include[i] = -(include[i]+1); // Reset includes.
      }

		max_nditems = optn[MAX_ND]; 
      ALLOC (LASUM,       n_bytes);
      ALLOC (action,      N_terms);
      ALLOC (actionindx,  N_terms);
      ALLOC (firstaction, N_terms);
      ALLOC (conflict,    N_terms);
		ALLOC (nd_start,    n_states+1);
		ALLOC (nd_term,     max_nditems);
		ALLOC (nd_action,   max_nditems);

      n_ndstates = 0;
		n_states_resolved = 0;
		nw = n_warnings;
      for (s = 0; s < opt_states; s++)
      {  
		//	printf ("state = %d\n", s);
			sr = rr = 0;
			la_start[s] = n_lookah;
			nfi = f_final[s+1] - f_final[s];
			D_red[s] = -1;	                        // No default reduction.
			nd_start[s] = n_nditems;
			if (nfi)			                        // Any reductions in this state?
			{
            memset (firstaction, 1, N_terms);
				FASTINI (0, conflict,   N_terms);	
				FASTINI (0, action,     N_terms);	
				for (i = tt_start [s]; i < tt_start [s+1]; i++)
				{
					action [tt_symb[i]] = tt_action[i]; 
               actionindx [tt_symb[i]] = i;
				}

				for (f = f_final [s]; f < f_final [s+1]; f++) // For each reduction.
				{  
					ANALYZE (s, f);
				}

            sr_con += sr;
		      rr_con += rr;
            if (sr+rr > 0) c_states++; 

            if (optn[PG_NONDETER] == 0)
            {
               if (sr+rr > 0) 
			      {
						REPORT_CONFLICTS (s, sr, rr);
						DF_TRACE_BACK (s, sr, rr);
			      }
            }
            else if (optn[PG_NONDETER] == 1)
            {
               if (sr > 0) 
			      {                           
				      REPORT_CONFLICTS (s, sr, 0);
						DF_TRACE_BACK (s, sr, 0);
			      }
            }
            else if (optn[PG_NONDETER] > 1)
            {
               if (sr+rr > 0) 
               {
                  nd_start[s+1] = n_nditems;
                  CYCLE_CHECK (s);
               }
            }
            else InternalError(103);
				if (n_nditems > nd_start[s])
				{
               n = nd_start[s];
					for (i = nd_start[s]; i < n_nditems; i++)   
					{
                  if (nd_action[i] != 0) // nd action not deleted?
                  {
						   t            = nd_term[i];       // Get terminal symbol.
                     nd_term[n]   = nd_term[i];       // Move term. 
                     nd_action[n] = nd_action[i];     // Move action.
				 	  	   if (action[t] > 0)               // Shift and goto action for this terminal?
                        tt_action[actionindx[t]] = 0; // Cancel tt_action.
                     action[t] = 0;	                  // Cancel action.
                     n++;
                  }
					}
               n_nditems = n;
               if (n_nditems > nd_start[s]) // Any nd entries left?
               {
				  	   SORT2 (nd_term+nd_start[s], nd_action+nd_start[s], n_nditems-nd_start[s]);
				      for (t = 0; t < N_terms; t++)	// For all terminal symbols. 
				      {  
					      if (action[t] < 0)
					      {
                        if (n_lookah >= max_lookah) MemCrash ("Number of lookaheads", max_lookah); 
						      la_symb[n_lookah] = t;
						      la_red [n_lookah++] = -action[t];
					      }
				      }
                  n_ndstates++;
                  goto Next;
               }
					else n_states_resolved++;
				}
				if (optn[PG_DEFAULTRED])
				{
				   p = -1;											// No default reduction.
					for (t = 0; t < N_terms; t++)				// For all terminal symbols. 
					{  
						if (action[t] < 0)						// Reduction?
						{
							if (p == -1) p = -action[t];				
							else if (p != -action[t]) goto NoDef;
						}
					}
					D_red[s] = p; // Same reduction for all lookaheads.
				}
				else // !PG_DEFAULTRED
				{
NoDef:			D_red[s] = -1;									// No default reduction.
					for (t = 0; t < N_terms; t++)				// For all terminal symbols. 
					{  
						if (action[t] < 0)						// Reduction?
						{
							if (n_lookah >= max_lookah) 
							{ 
								n_lookah++; 
								MemCrash ("Number of lookaheads", max_lookah); 
							}
							la_symb[n_lookah] = t;
							la_red [n_lookah++] = -action[t];
						}
					}
				}
         }
Next:    continue;
      }		
		for (s = opt_states; s < n_states; s++)
		{
			la_start[s] = n_lookah;
			nd_start[s] = n_nditems;
			D_red[s] = item [final[f_final[s]]].prod;
		}
		la_start[s] = n_lookah;
		nd_start[s] = n_nditems;

      FREE (conflict,   N_terms);
      FREE (action,     N_terms);
      FREE (actionindx, N_terms);
      FREE (LASUM,      n_bytes);

      for (i = 0; i < n_includes; i++) FREE (LA [i], n_bytes);
      FREE (LA, n_includes);

      for (h = 0; h < N_heads; h++) FREE (FIRST [h], n_bytes);
      FREE (FIRST, N_heads);  

      FREE (lookback,   max_lookback);
	   FREE (f_lookback, n_finals+2);
		FREE (prod_line, N_prods);
		FREE (head_line, N_heads);
}
  
///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void	PGComputeLA::ANALYZE (int s, int f)
{
		int p, lb, t;
		p = item [final[f]].prod; 
      FASTINI (0, LASUM, n_words);
		for (lb = f_lookback[f]; lb < f_lookback[f+1]; lb++)
		{
			FASTOR (LA[lookback[lb]], LASUM, n_words);
		}
		for (t = 0; t < N_terms; t++)								// For all terminal symbols. 
		{  
			#ifdef USINGBITS
			ifbit1(LASUM,t)											// Reduction defined for t?
			#else
			if (LASUM[t])												// Reduction defined for t?
			#endif
			{  
				if (action[t] == 0)								   // Current action undefined?
				{
					action[t] = -p;									// No conflict!
				}
				else if (action[t] < 0)									// Reduce already defined for t?
				{
					if (!REDUCE_PREC(s,t,p))						// No reduce precedence defined with /-> arrow?
					{
				      rr++;					                     // Increment number of RR conflicts.
			  	      if (optn[PG_NONDETER])
				      {
                     if (firstaction[t])                    // If first action for this t ...
                     {
						      if (n_nditems >= max_nditems) MemCrash ("Number of ND entries", max_nditems);
						      nd_term   [n_nditems  ] =  t;           // Assign t to terminal list.
   					      nd_action [n_nditems++] = action[t];	   // Assign original action.
                        firstaction[t] = 0;                 // Clear first action flag.
                     }
					      if (n_nditems >= max_nditems) MemCrash ("Number of ND entries", max_nditems);
					      nd_term   [n_nditems  ] =  t;
					      nd_action [n_nditems++] = -p;		         // Assign previous reduction, keep in order.
				      }
			  	      else
				      {
					      if (-action[t] < p) 
                     {
                        conflict[t] = -p;	            // Save the reduction first seen in the grammar.
                     }
					      else										
					      {
						      conflict[t] = action[t];	   // Switch to reduction first seen.
						      action[t]   = -p;							
					      }
				      }
               }
				}
				else if (!OPER_PREC(s,p,t))						// No operator precedence defined?
				{
					if (!GOTO_PREC(s,t,p))							// No goto precedence defined with /-> arrow?
					{
						sr++;											   // Increment number of SR conflicts.
				  		if (optn[PG_NONDETER] > 1)
						{
                     if (firstaction[t])                 // If first action for this t ...
                     {
   						   if (n_nditems >= max_nditems) MemCrash ("Number of ND entries", max_nditems);
   						   nd_term   [n_nditems  ] =  t;        // Assign t to terminal list.
                        nd_action [n_nditems++] = action[t]; // Assign original action.
                        firstaction[t] = 0;              // Clear first action flag.
                     }
						   if (n_nditems >= max_nditems) MemCrash ("Number of ND entries", max_nditems);
						   nd_term   [n_nditems  ] =  t;
						   nd_action [n_nditems++] = -p;		      // Assign previous reduction, keep in order.
						}
					}										  
				}  
			}  
		}  
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGComputeLA::REPORT_CONFLICTS (int state, int sr, int rr)
{
		int nc, t, first = 1;

      if (rr) // Do reduce-reduce conflicts first. 	
      {
			nc = 0; // Number of conflicts.
		   for (t = 0; t < N_terms; t++)
		   {
			   if (conflict[t] != 0)	// Got conflict on this terminal?
			   {
				   if (action[t] < 0)	// Reduce action?
				   {
						nc++;
						if (optn[PG_CONFLICTS])
						{
 							if (first)
							{
								first = 0;
								PRT_STA (state);
								prt_con ("\n");
							}
							prt_con ("Conflict on lookahead %s, ", term_name[t]);
							prt_con ("reduce %d or reduce %d, ", -action[t], -conflict[t]);
							prt_con ("choosing reduce %d.\n",    -action[t]);
						}
				   }
			   }
		   }			
			int ns = f_camefrom[state+1] - f_camefrom[state]; // Number of states coming here.
			if (optn[PG_CONFLICTS])
				prt_con ("\nConflict impact: (%d states come here) x (%d conflicts) = %d.\n", ns, nc, ns*nc);
			rr_impacts += ns*nc;
      }
      if (sr) // Do shift-reduce conflicts. 		
      {
			nc = 0; // Number of conflicts.
		   for (t = 0; t < N_terms; t++)
		   {
			   if (conflict[t] != 0) // Got conflict on this terminal?
			   {
				   if (action[t] > 0) // Shift action?
				   {
						nc++;
						if (optn[PG_CONFLICTS] && !optn[PG_REDUCEONLY])
						{
							if (first)
							{
								first = 0;
								PRT_STA (state);
								prt_con ("\n");
							}
							prt_con ("Conflict on lookahead %s, ", term_name[t]);
							prt_con ("shift or reduce %d, ", -conflict[t]);
							prt_con ("choosing shift and goto state %d.\n", action[t]);
						}
				   }
			   }
		   }
			int ns = f_camefrom[state+1] - f_camefrom[state]; // Number of states coming here.
			if (optn[PG_CONFLICTS] && !optn[PG_REDUCEONLY])
				prt_con ("\nConflict impact: (%d states come here) x (%d conflicts) = %d.\n", ns, nc, ns*nc);
			sr_impacts += ns*nc;
      }
		if (sr + rr > 0) impacted_states++;
		if (first == 0) prt_con ("\n");
}
  
///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGComputeLA::OPER_PREC (int s, int p, int t)					// Operator precedence defined? 
{
      int a, b;
      a = abs (Oper_prec [t]);
      if (a == 32767) return (0);						// Already warned. 
		if (a == -32768)										// Non associative, make syntax error at runtime. 
		{
			/* nothing yet */
		}
      if (prod_len [p]         >=  3					// Prod length >= 3?    
      && Tail [F_tail [p]]     == -head_sym [p]		// Left recursive?      
      && Tail [F_tail [p+1]-1] == -head_sym [p])	// Right recursive?     
      {
         if (a == 0)											// Prec undefined?
         {
				n_warnings++;
				Oper_prec [t] = 32767;
				sprintf (string, "%s has no operator precedence in state %d", term_name[t], s);
				prt_warning (string, NULL, NULL, 0);
            return (0);
         }
         b = abs (Oper_prec [Tail [F_tail [p] + 1]]);
         if (a > b)		// Higher? 
         {
            return (1); // Choose shift. 
         }
         if (a == b && Oper_prec [t] < 0) // Right Associativity? 
         {
            return (1); // Choose shift. 
         }  
			tt_action[actionindx[t]] = 0; // Cancel shift and goto action. 
         action [t] = -p;			      // Force reduction instead.
         return (1);
      }
      return (0); 
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGComputeLA::GOTO_PREC (int state, int t, int reduce)					
{
      int k, i, s, sym, p, red_prec, goto_prec;

      goto_prec = 1;						// Start with 1.
      red_prec  = Prec[reduce];		// 0 or 1. 

      for (k = f_kernel[state]; k < f_kernel[state+1]; k++)
      {
         i = kernel[k];
         p = item[i].prod;
			if (p != reduce)							// Not the reduction.
			{
			//	if (head_sym[p] == head_sym[reduce]) // Only when head symbols match (?). 
				{
					sym = item[i].symb;           // Get goto symbol. 
					if (sym >= 0)                 // Terminal?
					{
						if (sym == t)              // Same as conflict symbol?
						{
							goto_prec &= Prec[p];	// And Prec onto goto_prec. 
						}									// All have to be given priority to avoid a conflict message.
					}
					else if (sym != -32767)            // Nonterminal (final item can be in the kernel set).
					{
						s = -sym;		
						#ifdef USINGBITS
						ifbit2(FIRST,s,t)				// If t is in FIRST set?
						#else
						if (FIRST[s][t])				// If t is in FIRST set?
						#endif
						{
							goto_prec &= Prec[p];	// And Prec onto goto_prec. 
						}
					}
				}
			//	else // Head symbols don't match, cannot avoid conflict message!
			//	{
			//		conflict[t] = -reduce;			// Save reduction for reporting.
			//		return (0); // Allow conflict message and keep goto action as default.
			//	}
			}
      }
		if (goto_prec == 1)
		{
			if (red_prec == 0) 
			{
				return (1);	// Avoid conflict message, keep goto.
			}
			else               
			{
				conflict[t] = -reduce;			// Save reduction for reporting.
				return (0);	// Print conflict, keep goto. 
			}
		}
		else // goto_prec == 0
		{
			if (red_prec == 0) 
			{
				conflict[t] = -reduce;			// Save reduction for reporting.
				return (0);							// Print conflict, keep goto. 
			}
			else										// Avoid conflict message, keep reduce!
			{
				n_warnings++;
				Oper_prec [t] = 32767;
				sprintf (string, "Precedence (/->|/~>) was given to reduce %d in state %d", reduce, state);
				prt_warning (string, NULL, NULL, prod_line[reduce]);
				tt_action[actionindx[t]] = 0;	// Cancel goto action. 
		      action [t] = -reduce;			// Force reduction instead.
			   return (1);							// Avoid conflict message. 
			}											
		}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGComputeLA::REDUCE_PREC (int state, int t, int p)					
{
      int red_prec1, red_prec2;

      red_prec1 = Prec[-action[t]];	
      red_prec2 = Prec[p];		

		if (red_prec1 + red_prec2 != 1) // Both 0 or both 1?
		{
         return (0); // No decision, ambiguous!
      }
		if (red_prec1 == 1) 
		{
			return (1);	// Keep the current reduction.
		}
		else               
		{
		   action [t] = -p; // Change the reduction.
			return (1);	
		}
}

void	PGComputeLA::prt_prod (int p)
{
		printf ("%6d %s -> ", p, head_name[head_sym[p]]);
		for (int i = F_tail[p]; i < F_tail[p+1]; i++)
		{
			char* symb;
			int   s = Tail[i];
			if (s >= 0) symb = term_name[ s];
			else        symb = head_name[-s];
			printf ("%s ", symb);
		}
		printf ("\n");
}

void	PGComputeLA::prt_prod (int p, int dot)
{
      int d = 0;
		printf ("%6d %s -> ", p, head_name[head_sym[p]]);
		for (int i = F_tail[p]; i < F_tail[p+1]; i++)
		{
			char* symb;
			int   s = Tail[i];
			if (s >= 0) symb = term_name[ s];
			else        symb = head_name[-s];
         if (d++ == dot) printf (". ");
			printf ("%s ", symb);
		}
		printf ("\n");
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGComputeLA::C_LOOKBACKS ()
{
      int p, dot, i, state, f;

      i            = 0;  /* include number. */
      inc_on       = 0;  /* Include switch. */
      n_lookbacks  = 0;
      n_childs     = 0;
      n_relations  = 0;
      n_includes   = 0;
		max_child_usage   = 0; 

      max_lookback = optn[MAX_LB]; 
      max_include  = optn[MAX_INC];

      ALLOC (f_lookback,  n_finals+2);
      ALLOC (lookback,    max_lookback);
      ALLOC (include,     max_include);
      ALLOC (ntt_include, n_nttran);
      ALLOC (f_child,     n_nttran);
      ALLOC (child,       max_child);

      FASTINI (-1, ntt_include, n_nttran);
      FASTINI (-1, f_child,     n_nttran);

      for (state = 0; state < n_states; state++) // for all states.
      {
         for (f = f_final [state]; f < f_final [state+1]; f++) // for each final item.
         {
            p = item [final[f]].prod;                          // Get production number.        
            dot = prod_len [p];                                // Get dot position.
            f_lookback[f] = n_lookbacks;                       // Set first lookback number.
            LOOK_BACK (i++, p, dot, state);                    // Find all lookbacks.
         }
         f_lookback [f] = n_lookbacks;                         // Set last lookback number.
      }
   // FREE    (ntt_include, n_nttran);
      REALLOC (lookback, max_lookback, n_lookbacks);
      REALLOC (include,  max_include,  n_includes);
      REALLOC (f_child,  n_nttran,     n_includes);
      REALLOC (child,    max_child,    n_relations);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGComputeLA::LOOK_BACK (int i, int p, int dot, int state)
{
      int cfi, inc;
      if (dot-- > 0)
      {
         for (cfi = f_camefrom [state]; cfi < f_camefrom [state+1]; cfi++)
         {
            LOOK_BACK (i, p, dot, camefrom [cfi]);
         }
      }
	   else if ((inc = NTX (state, head_sym [p])) >= 0)
      {
         if (inc_on == 0)
         {
            if (n_lookbacks >= max_lookback) MemCrash ("Number of lookbacks", max_lookback);
            lookback [n_lookbacks++] = inc;
            inc_on = 1;
            C_INCLUDE (state, inc);
            inc_on = 0;
         }
         else if (i != inc && ATTACH (i, inc) == 0)
         {
            n_relations++;
            C_INCLUDE (state, inc);
         }  
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int	PGComputeLA::NTX (int s, int h) /* Nonterminal transition. */
{
      int ntti;
      for (ntti = ntt_start[s]; ntti < ntt_start[s+1]; ntti++)
      {
         if (ntt_symb[ntti] == h)
         {
            if (ntt_include[ntti] < 0) // New include?
            {
               if (n_includes >= max_include) MemCrash ("Number of includes", max_include);
               ntt_include[ntti] = n_includes;
               include[n_includes] = ntti;
               return (n_includes++);
            }
            return (ntt_include[ntti]); // Old include!
         }  
      }
      return -1;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGComputeLA::C_INCLUDE (int s, int inc)
{
      int ntti, S, k, p, dot, t;

      ntti = include [inc];
      if (ntti >= 0) /* If not done before? */
      {
         include [inc] = -(ntti+1); /* Mark it done. */
         S = ntt_action [ntti];
         for (k = f_kernel [S]; k < f_kernel [S+1]; )
         {
            dot = item[kernel[k]].dot;
            if (dot > 0) /* If not null production? */
            {
               p = item[kernel[k]].prod;
               for (t = F_tail[p]+dot; t < F_tail[p+1]; t++)
               {
                  if (Tail [t] >= 0) goto Next;
                  if (!nullable [-Tail [t]]) goto Next;
               }
               LOOK_BACK (inc, p, (int)(dot-1), s);
            }
Next:       k++;
         }  
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGComputeLA::C_READS ()
{
      int i, s, m, ntti;

//		if (optn[PG_VERBOSE]) 
//			prt_log ("LA matrix size = %d\n", n_includes*n_bytes);

      ALLOC (LA, n_includes); 
      for (i = 0; i < n_includes; i++)
      {
         ALLOC (LA[i], n_bytes); 
         memset (LA[i], 0, n_bytes);
      }

      ALLOC (already, n_states);
      ALLOC (newloc, n_states);
      FASTINI (-1, already, n_states);
      FASTINI (-1, newloc, n_states);

      for (i = 0; i < n_includes; i++)
      {
         ntti = include [i];
         if (ntti < 0) ntti = -(ntti+1);
         if ((s = ntt_action [ntti]) > 0)
         {
            if (newloc [s] == -1)
            {
               IND_READ (i, s);
               newloc [s] = i;
            }
            else
            {
               m = newloc [s];
            // PRINTF ("copying LA[%4d] to LA[%4d]\n", m, i);
               FASTCPY (LA[m], LA[i], n_words);
            }  
         }  
      }

      FREE (newloc,   n_states);
      FREE (already, n_states);
      T_GRAPH (LA,   n_includes, N_terms);
      FREE (child,   n_relations);
      FREE (f_child, n_includes);
   // FREE (include, n_includes);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGComputeLA::IND_READ (int i, int s)
{
      int x, j, t;
      already [s] = i;
      for (j = ntt_start [s]; j < ntt_start [s+1]; j++)
      {
         if ((x = ntt_action [j]) > 0)
         {
            if (nullable [ntt_symb[j]])
            {
               if (already [x] != i)
               {
                  IND_READ (i, x);
               }  
            }  
         }  
      }
      for (j = tt_start [s]; j < tt_start [s+1]; j++)
      {
      // Some goto's may be disabled by ND option or disambiguating rules.
         if (tt_action [j] > 0) 
         {
            t = tt_symb [j];
				#ifdef USINGBITS
				setbit2(LA,i,t);
				#else
				LA[i][t] = 1;
				#endif
         }  
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGComputeLA::RESOLUTION ()
{
      int t, l;
      prt_con ("\n   Lookahead symbol and action chosen:\n");
      for (t = 0; t < N_terms; t++)
      {  
         if (redconf [t] != 0)
         {  
            prt_con ("   ");
            l = prt_sym (t, " ");
            if (l > max_terml) l = max_terml;
            spaces [max_terml-l] = 0;
            prt_con ("%s", spaces);
            spaces [max_terml-l] = ' ';
            if (action [t] < 0) /* If shift action for t? */
            {
               prt_con(" shift, not reduce %d\n", abs (redconf [t]));
            }
            else /* If two reductions for t? */
            {
               prt_con(" reduce %d,  not %d\n", action[t], abs (redconf [t]));
            }
         }  
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGComputeLA::PRT_STA (int s)
{
      int k, i;
      int f, p;

      prt_con ("///////////////////////////// STATE %d /////////////////////////////\n\n", s);

      for (k = f_kernel [s]; k < f_kernel [s+1]; k++)
      {
         i = kernel [k];
         if (item[i].symb != -32767)
         {
            prt_con ("* ");
            prt_prod (item[i].prod, item[i].dot, "");
         }  
      }

      for (f = f_final [s]; f < f_final [s+1]; f++)
      {
         p = item [final[f]].prod;
         if (p >= 0)
         {
            prt_con ("* ");
            prt_prod (p, -1, "");
         }  
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGComputeLA::prt_prod (int p, int dot, char *before)
{
      int t, u, d;
      prt_con ("%s%5d %s -> ", before, p, head_name [head_sym [p]]);
      t = F_tail [p];
      u = F_tail [p+1];
      d = t + dot;
      if (dot == -1) d = u;
      for (;;)
      {
         if (t == d) prt_con (". ");
         if (t >= u) break;
         prt_sym (Tail [t], " ");
         t++;
      }
      prt_con ("\n");
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int	PGComputeLA::prt_sym (int s, char *sp)
{
      char *p;

      if (s >= 0)                 /* Terminal symbol? */
         p = term_name[s];
      else                        /* Nonterminal symbol? */
         p = head_name[-s];

      prt_con ("%s%s", p, sp);
      return (strlen(p) + strlen(sp));
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    Bredth-first trace back. 

void  PGComputeLA::BF_TRACE_BACK (int state, int sr, int rr) 
{
      int lb, ntti, orig, next, last, i, t, f, p, h, n, dot;
      if (!optn[PG_CONFLICTTRACE]) return;

      #ifdef _DEBUG
      PRINTF ("TRACE BACK from state %d:\n", state);
      prt_state (state);
      #endif

      ALLOC (ntt_done, n_nttran);
      memset (ntt_done, 0, n_nttran);

   // PRINTF ("\n");
      for (t = 0; t < N_terms; t++)								// For all terminal symbols. 
      {  
         if (conflict[t])
         {
            n = 0;
            PRINTF ("CHOICES for lookahead '%s'\n\n", term_name[t]);
            if (action[t] >= 0)  // shift-reduce conflict? (zeroed action is possible)
            {
               PRINTF ("%d   shift        %s\n", ++n, term_name[t]);
               PRINTF ("    goto    %4d\n", action[t]);
               prt_state (action[t]);
               PRINTF ("\n");
            }
            else                 // reduce-reduce conflict!
            {
               if (rr == 0) continue; // skip this one.
            }
	         for (f = f_final[state]; f < f_final[state+1]; f++)      // For each reduction.
	         {  
               p = item [final[f]].prod;
               h = head_sym[p];
               if (p == -conflict[t] || p == -action[t])
               {
                  PRINTF ("%d   reduce ", ++n);
                  prt_prod (p, -1, "");
                  for (lb = f_lookback[f]; lb < f_lookback[f+1]; lb++)  // For all lookbacks of final item.
                  {
							#ifdef USINGBITS
			            ifbit2(LA,lookback[lb],t)								   // Reduction defined for t?
							#else
			            if (LA[lookback[lb]][t])								   // Reduction defined for t?
							#endif
			            {  
                        ntti = include[lookback[lb]];
                        next = ntt_action[ntti]; // Get next state. 
                        for (i = tt_start[next]; i < tt_start[next+1]; i++)
                        {
                           if (tt_symb[i] == t) 
                           {
                              last = tt_action[i];
                              goto Shift;
                           }
                        }   
                        p = item[ntt_item[ntti]].prod;
                        if (prod_len[p] > 0)
                        {
                           dot  = item[ntt_item[ntti]].dot;
                           orig = ntt_origin[ntti]; // Back to origin state.
                           ntt_done[ntti] = 1;
                        // printf ("    backto  %4d\n", orig);
                           for (depth = 1;; depth++)
                           {
                           // printf ("\ndepth = %d\n", depth);
                              if (bf_look_back (1, t, p, dot, orig, next, last))
                              {
Shift:                           PRINTF ("    goto    %4d\n", next);
                                 prt_state (next);
                                 PRINTF ("    shift        %s\n", term_name[t]);
                                 PRINTF ("    goto    %4d\n", last);
                                 prt_state (last);
                              // PRINTF ("\n");
                                 ntt_done[ntti] = 0;
                                 goto Cont;
                              }
                           }
                           ntt_done[ntti] = 0;
                        }
                        else
                        {
                           printf ("production length = 0\n");
                           InternalError(101);
                        }
                     }   
                  }
               }
Cont:          continue;
            }
         }
	   }
      FREE (ntt_done, n_nttran);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGComputeLA::bf_look_back (int d, int t, int p, int dot, int state, int& next, int& last)
{
      int cfi, h, ntti, i;
      if (dot-- > 0)
      {
         for (cfi = f_camefrom[state]; cfi < f_camefrom[state+1]; cfi++)
         {
            if (bf_look_back (d, t, p, dot, camefrom[cfi], next, last))
            {
               return 1;
            }
         }
         return 0;
      }
      else // Get nonterminal transition from this state.
      {
         h = head_sym[p];
         for (ntti = ntt_start[state]; ntti < ntt_start[state+1]; ntti++)
         {
            if (ntt_symb[ntti] == h)
            {
               if (ntt_done[ntti] == 0)
               {
                  if (d == depth)
                  {
                     next = ntt_action[ntti];
							#ifdef USINGBITS
                     ifbit2 (LA, ntt_include[ntti], t)
							#else
                     if (LA[ntt_include[ntti]][t])
							#endif
                     {
                        for (i = tt_start[next]; i < tt_start[next+1]; i++)
                        {
                           if (tt_symb[i] == t) 
                           {
                              PRINTF ("    backto  %4d\n", state);
                              prt_state (state);
                              last = tt_action[i];
                              return 1;
                           }
                        }
                     }
                  }
                  else
                  {
                     ntt_done[ntti] = 1;
                     p   = item[ntt_item[ntti]].prod;
                     dot = item[ntt_item[ntti]].dot;
                  // prt_prod (p, dot, "");
                     if (bf_look_back (++d, t, p, dot, state, next, last))
                     {
                        ntt_done[ntti] = 0;
                        return 1;
                     }    
                     ntt_done[ntti] = 0;
                  }
               }
               return 0;
            }
         }   
      }
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    Depth-first trace back. 

void  PGComputeLA::DF_TRACE_BACK (int state, int sr, int rr) 
{
      int lb, ntti, orig, next, last, i, t, f, p, h, n, dot;
      if (!optn[PG_CONFLICTTRACE]) return;

      PRINTF ("TRACE BACK from state %d:\n", state);
      prt_state (state);
   // PRINTF ("\n");

      ALLOC (ntt_done, n_nttran);
      memset (ntt_done, 0, n_nttran);

      for (t = 0; t < N_terms; t++) // For all terminal symbols. 
      {  
         if (conflict[t]) // Conflict on this terminal?
         {
            n = 0;
            PRINTF ("CHOICES for lookahead: %s\n\n", term_name[t]);
            if (action[t] >= 0)  // shift-reduce conflict? (zeroed action is possible)
            {
               PRINTF ("%2d. shift        %s\n", ++n, term_name[t]);
               PRINTF ("    goto    %4d\n", action[t]);
               prt_state (action[t]);
            // PRINTF ("\n");
            }
            else                 // reduce-reduce conflict!
            {
               if (rr == 0) continue; // skip this one.
            }
	         for (f = f_final[state]; f < f_final[state+1]; f++)      // For each reduction.
	         {  
               p = item [final[f]].prod;
               h = head_sym[p];
               if (p == -conflict[t] || p == -action[t])
               {
                  PRINTF ("%2d. reduce ", ++n);
                  prt_prod (p, -1, "");
                  for (lb = f_lookback[f]; lb < f_lookback[f+1]; lb++)  // For all lookbacks of final item.
                  {
							#ifdef USINGBITS
			            ifbit2(LA,lookback[lb],t)								   // Reduction defined for t?
							#else
			            if (LA[lookback[lb]][t])								   // Reduction defined for t?
							#endif
			            {  
                     // printf ("\nLookback %d\n", lb);
                        ntti = include[lookback[lb]];
                        if (ntt_done[ntti] == 0)
                        {
                           orig = ntt_origin[ntti];
                        // printf ("    BACKTO  %4d\n", orig);
                           next = ntt_action[ntti]; // Get next state. 
                           for (i = tt_start[next]; i < tt_start[next+1]; i++)
                           {
                              if (tt_symb[i] == t) 
                              {
                                 last = tt_action[i];
                                 goto Shift;
                              }
                           }   
                           p = item[ntt_item[ntti]].prod;
                           if (prod_len[p] > 0)
                           {
                              dot  = item[ntt_item[ntti]].dot;
                           // prt_prod (p, dot, "           ");
                              ntt_done[ntti] = 1;
                              if (df_look_back (t, p, dot, orig, next, last))
                              {
Shift:                           PRINTF ("    goto    %4d\n", next);
                                 prt_state (next);
                                 PRINTF ("    shift        %s\n", term_name[t]);
                                 PRINTF ("    goto    %4d\n", last);
                                 prt_state (last);
                                 goto Cont; // next final item.
                              }
                           }
                           else
                           {
                              printf ("production length = 0\n");
                              InternalError(101);
                           }
                        }
                     }   
                  }
               }
Cont:          continue;
            }
			   PRINTF ("\n");
         }
	   }
      FREE (ntt_done, n_nttran);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGComputeLA::df_look_back (int t, int p, int dot, int state, int& next, int& last)
{
      int cfi, h, ntti, i;
      if (dot-- > 0)
      {
         for (cfi = f_camefrom[state]; cfi < f_camefrom[state+1]; cfi++)
         {
            if (df_look_back (t, p, dot, camefrom[cfi], next, last))
            {
               return 1;
            }
         }
         return 0;
      }
      else // Get nonterminal transition from this state.
      {
      // printf ("    backto  %4d\n", state);
         h = head_sym[p];
         for (ntti = ntt_start[state]; ntti < ntt_start[state+1]; ntti++)
         {
            if (ntt_symb[ntti] == h)
            {
            // printf ("ntti = %d\n", ntti);
               if (ntt_done[ntti] == 0)
               {
                  next = ntt_action[ntti];
						#ifdef USINGBITS
                  ifbit2 (LA, ntt_include[ntti], t)
						#else
                  if (LA[ntt_include[ntti]][t])
						#endif
                  {
                     for (i = tt_start[next]; i < tt_start[next+1]; i++)
                     {
                        if (tt_symb[i] == t) 
                        {
                           PRINTF ("    backto  %4d\n", state);
                           prt_state (state);
                           last = tt_action[i];
                           ntt_done[ntti] = 1;
                           return 1;
                        }
                     }
                     ntt_done[ntti] = 1;
                  }
                  p    = item[ntt_item[ntti]].prod;
                  dot  = item[ntt_item[ntti]].dot;
               // prt_prod (p, dot, "           ");
                  if (state > 0)
                  {
                     ntt_done[ntti] = 1;
                     if (df_look_back (t, p, dot, state, next, last))
                     {
                        ntt_done[ntti] = 1;
                        return 1;
                     }
                     else
                     {
                     // printf ("ntti = %d\n", ntti);
                     // printf ("    failed\n\n");
                        ntt_done[ntti] = 1;
                        return 0;
                     }
                  }
                  else 
                  {
                  // printf ("ntti = %d\n", ntti);
                  // printf ("    failed\n\n");
                     ntt_done[ntti] = 1; // Done and failed.
                     return 0;
                  }
               }
               else
               {
                  return 0;
               }
            }
         }   
      }
		PRINTF ("\nNonfatal error caused by 'ct' option\n");
		return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGComputeLA::CYCLE_CHECK (int state) 
{
      int lb, ntti, orig, next, last, i, t, f, p, h, n, dot;

//    PRINTF ("\nCYCLE CHECK for state %d:\n\n", state);
//    prt_state (state);
//    PRINTF ("\n");

      ALLOC (ntt_done, n_nttran);
      memset (ntt_done, 0, n_nttran);

      for (t = 0; t < N_terms; t++) // For all terminal symbols. 
      {  
         if (conflict[t] && action[t] >= 0) // conflict and shift action?
         {
            n = 0;
            if (action[t] == 0) action[t] = tt_action [actionindx[t]];
	         for (f = f_final[state]; f < f_final[state+1]; f++)      // For each reduction.
	         {  
               p = item [final[f]].prod;
               h = head_sym[p];
               if (p == -conflict[t] || p == -action[t])
               {
               // PRINTF ("%d   reduce ", ++n);
               // prt_prod (p, -1, "");
                  for (lb = f_lookback[f]; lb < f_lookback[f+1]; lb++)  // For all lookbacks of final item.
                  {
							#ifdef USINGBITS
			            ifbit2(LA,lookback[lb],t)								   // Reduction defined for t?
							#else
			            if (LA[lookback[lb]][t])								   // Reduction defined for t?
							#endif
			            {  
                     // printf ("\nLookback %d\n", lb);
                        ntti = include[lookback[lb]];
                        if (ntt_done[ntti] == 0)
                        {
                           orig = ntt_origin[ntti];
                        // printf ("    BACKTO  %4d\n", orig);
                           next = ntt_action[ntti]; // Get next state. 
                           for (i = tt_start[next]; i < tt_start[next+1]; i++)
                           {
                              if (tt_symb[i] == t) 
                              {
                                 last = tt_action[i];
                                 goto Shift;
                              }
                           }   
                           p = item[ntt_item[ntti]].prod;
                           if (prod_len[p] > 0)
                           {
                              dot  = item[ntt_item[ntti]].dot;
                           // prt_prod (p, dot, "           ");
                              ntt_done[ntti] = 1;
                              if (cycle_look_back (t, p, dot, orig, next, last))
                              {
Shift:                           if (next == state)
                                 {
												PRINTF ("Note: Cycle detected in state %d:\n\n", state);
                                    prt_state (state);
                                 // PRINTF ("\n");
                                    PRINTF ("%d   shift        %s\n", ++n, term_name[t]);
                                    PRINTF ("    goto    %4d\n", action[t]);
                                    prt_state (action[t]);
                                 // PRINTF ("\n");
                                    PRINTF ("%d   reduce ", ++n);
                                    prt_prod (item[final[f]].prod, -1, "");
                                    PRINTF ("    goto    %4d\n", next);
                                    prt_state (next);
                                    PRINTF ("    shift        %s\n", term_name[t]);
                                    PRINTF ("    goto    %4d\n", last);
                                    prt_state (last);
                                 // PRINTF ("\n");
                                    PRINTF ("Choosing shift action, instead of reduce.\n");
                                    PRINTF ("Nondeterministic action was avoided here.\n\n");
                                    for (i = nd_start[state]; i < nd_start[state+1]; i++)
                                    {
                                       if (nd_term[i] == t) nd_action[i] = 0; // Delete this action.
                                    }
                                    goto Nextt; // next terminal symbol.
                                 }
                                 goto Nextf; // next final item.
                              }
                           }
                           else
                           {
                              printf ("production length = 0\n");
                              InternalError(101);
                           }
                        }
                     }   
                  }
               }
Nextf:         continue;
            }
            tt_action [actionindx[t]] = 0; // delete deterministic action. 
         }
Nextt:   continue;
	   }
      FREE (ntt_done, n_nttran);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGComputeLA::cycle_look_back (int t, int p, int dot, int state, int& next, int& last)
{
      int cfi, h, ntti, i;
      if (dot-- > 0)
      {
         for (cfi = f_camefrom[state]; cfi < f_camefrom[state+1]; cfi++)
         {
            if (cycle_look_back (t, p, dot, camefrom[cfi], next, last))
            {
               return 1;
            }
         }
         return 0;
      }
      else // Get nonterminal transition from this state.
      {
      // printf ("    backto  %4d\n", state);
         h = head_sym[p];
         for (ntti = ntt_start[state]; ntti < ntt_start[state+1]; ntti++)
         {
            if (ntt_symb[ntti] == h)
            {
            // printf ("ntti = %d\n", ntti);
               if (ntt_done[ntti] == 0)
               {
                  next = ntt_action[ntti];
						#ifdef USINGBITS
                  ifbit2 (LA, ntt_include[ntti], t)
						#else
                  if (LA[ntt_include[ntti]][t])
						#endif
                  {
                     for (i = tt_start[next]; i < tt_start[next+1]; i++)
                     {
                        if (tt_symb[i] == t) 
                        {
                        // PRINTF ("    backto  %4d\n", state);
                        // prt_state (state);
                           last = tt_action[i];
                           ntt_done[ntti] = 1;
                           return 1;
                        }
                     }
                     ntt_done[ntti] = 1;
                  }
                  p    = item[ntt_item[ntti]].prod;
                  dot  = item[ntt_item[ntti]].dot;
               // prt_prod (p, dot, "           ");
                  if (state > 0)
                  {
                     ntt_done[ntti] = 1;
                     if (cycle_look_back (t, p, dot, state, next, last))
                     {
                        ntt_done[ntti] = 1;
                        return 1;
                     }
                     else
                     {
                     // printf ("ntti = %d\n", ntti);
                     // printf ("    failed\n\n");
                        ntt_done[ntti] = 1;
                        return 0;
                     }
                  }
                  else 
                  {
                  // printf ("ntti = %d\n", ntti);
                  // printf ("    failed\n\n");
                     ntt_done[ntti] = 1; // Done and failed.
                     return 0;
                  }
               }
               else
               {
                  return 0;
               }
            }
         }   
      }
		PRINTF ("\nNonfatal error caused by 'ct' option\n");
      return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGComputeLA::prt_state (int s)
{
      int k, i, f, p;
		PRINTF ("\n    //STATE %4d ////////////////////////////////////////////////////////////////\n", s);
      for (k = f_kernel [s]; k < f_kernel [s+1]; k++)
      {
         i = kernel [k];
         if (item[i].symb != -32767)
         {
            PRINTF ("    // rule");
            prt_prod2 (item[i].prod, item[i].dot, "");
         }  
      }
      for (f = f_final [s]; f < f_final [s+1]; f++)
      {
         p = item [final[f]].prod;
         PRINTF ("    // rule");
         prt_prod2 (p, -1, "");
      }  
		PRINTF ("    /////////////////////////////////////////////////////////////////////////////\n\n");
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGComputeLA::prt_prod2 (int p, int dot, char *before)
{
      int t, u, d;
      PRINTF ("%s%5d %s -> ", before, p, head_name [head_sym [p]]);
      t = F_tail [p];
      u = F_tail [p+1];
      d = t + dot;
      if (dot == -1) d = u;
      for (;;)
      {
         if (t == d) PRINTF (". ");
         if (t >= u) break;
         prt_sym2 (Tail [t], " ");
         t++;
      }
      PRINTF ("\n");
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int	PGComputeLA::prt_sym2 (int s, char *sp)
{
      char *p;
      if (s >= 0) p = term_name[s];
      else        p = head_name[-s];
      PRINTF ("%s%s", p, sp);
      return 1;
}

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif

