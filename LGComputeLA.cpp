
#ifdef DFASTAR

		#include "ComGlobal.h"
		#include "LGGlobal.h"
		#include "LGComputeLA.h"

		int    LGComputeLA::c_states;
		int*   LGComputeLA::D_red;
		int*   LGComputeLA::la_start;
		int*   LGComputeLA::la_end;
		int*   LGComputeLA::la_symb;
		int*   LGComputeLA::la_red;

		int    LGComputeLA::n_lookbacks;
		int    LGComputeLA::n_lookah;
		int    LGComputeLA::n_includes;
		static int    nwarn;
		static char** LA;
	//	static int*   term; not used?
		static int*   lb_num;						   
		static int*   action;
		static int*   conflict;
		static int    n_fatals;
		static int    rr_errors;
		static int    numberofterms;
		static int    sr, rr;
		static char*  LASUM;
		static int*   already;
		static int    inc_on;
		static int*   include;
		static int*   f_lookback;
		static int*   lookback;

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   LGComputeLA::ComputeLA () /* Compute Look-Aheads */
{
      int  rc;
		n_fatals = 0;
      if (optn [LG_CONFLICTS])
      {
			prt_con("\n");
         prt_con ("CONFLICTS REPORT:\n\n");
      }
		else 
		{
         prt_con ("\nCONFLICTS REPORT:\n\n");
			prt_con ("'c' option was not specified.\n\n");
		}
      c_states    = 0;
      sr_con      = 0;
      rr_con      = 0;
      rr_errors   = 0;
      n_lookah    = 0;
		#ifdef USINGBITS
      n_words = (n_terms + 31)/32;	// Number of 32-bit words. 
		#else
      n_words = (n_terms +  3)/4;	// Number of 4-byte words. 
		#endif
      n_bytes     = 4*n_words;      // Number of bytes to allocate. 
		max_lookah  = optn[MAX_LA];  
      ALLOC (D_red,    n_states+1); // +1 in case of illegal_char state.
      ALLOC (la_start, n_states+1); 
      ALLOC (la_symb,  max_lookah);
      ALLOC (la_red,   max_lookah);

	  	C_LALR ();

      n_terms = max_char_set; // Remove the end_symb. 
      if (c_states == 0) // No conflicts?
		{
         prt_con ("No conflicts were found!\n");
		}
		else
		{
	      if (optn [LG_CONFLICTS])
	      {
				if (rr_con > 0) // Any RR conflicts?
					prt_con ("/////////////////////////////////////////////////////////////////////\n\n");
				else if (sr_con > 0 && !optn[LG_REDUCEONLY])
					prt_con ("/////////////////////////////////////////////////////////////////////\n\n");
			}
			if (sr_con == 0) prt_con ("No shift-reduce conflicts were found!\n\n");
			else             prt_con ("%d shift-reduce conflicts were found, but resolved by choosing shift.\n\n", sr_con);
			if (rr_con == 0) prt_con ("No reduce-reduce conflicts were found!\n\n");
			else             prt_con ("%d reduce-reduce conflicts were found!\n\n", rr_con);
      }	 

      ALLOC (tt_end,     n_states+1);  // +1 in case of illegal_char state.
      ALLOC (ntt_end,    n_states+1);
      ALLOC (l_kernel,   n_states+1);
      ALLOC (l_final,    n_states+1);
      ALLOC (l_camefrom, n_states+1);
      ALLOC (la_end,     n_states+1);

      for (int s = 0; s < n_states; s++)
      {
         tt_end[s]     = tt_start[s+1];
         ntt_end[s]    = ntt_start[s+1];
         l_kernel[s]   = f_kernel[s+1];
         l_final[s]    = f_final[s+1];
         l_camefrom[s] = f_camefrom[s+1];
			la_end[s]     = la_start[s+1];
      }

      char *ch;
      if (c_states == 1) ch = ""; else ch = "s";
      prt_log ("Conflicts");
      prt_log ("%7d conflict state%s, ", c_states, ch);
      prt_log ("%d shift-reduce, ", sr_con);
      prt_log ("%d reduce-reduce.\n", rr_con);

		rc = 1;
		if (n_fatals) 
		{
			prt_log ("\n");
			rc = 0; 
		}
	//	anl_stat_memory = memory_max;
      return rc;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  LGComputeLA::C_LALR ()
{
      int  nfi, t, i, p, s, f, nc;

	//	numberofterms = max_char_set; 
	  	numberofterms = n_terms; 

      C_LOOKBACKS ();
      C_READS  ();

      ALLOC (LASUM, n_bytes);
      ALLOC (action, numberofterms);
      ALLOC (conflict, numberofterms);
   // ALLOC (term, numberofterms); not used?

	//	nwarn = n_warnings;
      for (s = 0; s < n_states; s++)
      {  
		// printf ("--> STATE %d\n", s);
			nc = sr = rr = 0;
			la_start[s] = n_lookah;
			nfi = f_final [s+1] - f_final [s];
			if (nfi) 
			{
				FASTINI (0, action, numberofterms); // Fill with zeros.
				FASTINI (0, conflict, numberofterms); // Fill with zeros.
				for (i = tt_start [s]; i < tt_start [s+1]; i++)
				{
					action [tt_symb[i]] = tt_action[i]; // goto state
				}
				for (f = f_final [s]; f < f_final [s+1]; f++) // For each reduction.
				{  
					ANALYZE (nc, s, f);
				}
				p = 0; 
				for (t = 0; t < numberofterms; t++)	// For all terminal symbols. 
				{  
					if (action[t] < 0 && t != end_symb) // Reduction?
					{
						if (p == 0) p = -action[t];
						else if (p != -action[t]) goto DoLA;
					}
				}
			  	if (action[end_symb] == 0 || action[end_symb] == -p) 
				{
					D_red[s] = p;  // Same reduction for all lookaheads (terminals).
					// printf ("D_red[%d] = %d\n", s, D_red[s]);
				}
				else
				{
	   			for (t = 0; t < numberofterms; t++)	// For all terminal symbols. 
					{  
						if (action[t] < 0 && action[t] != action[end_symb])
						{
							if (n_lookah >= max_lookah) MemCrash ("Number of lookaheads", max_lookah);
							la_symb[n_lookah] = t;
							la_red [n_lookah++] = -action[t];
						}
					}
					D_red[s] = -action[end_symb];
					// printf ("D_red[%d] = %d\n", s, D_red[s]);
				}
				goto Next;

DoLA:			for (t = 0; t < numberofterms; t++)	// For all terminal symbols. 
				{  
					if (action[t] < 0 && action[t] != action[end_symb])
					{
						if (n_lookah >= max_lookah) MemCrash ("Number of lookaheads", max_lookah);
						la_symb[n_lookah] = t;
						la_red [n_lookah++] = -action[t];
					}
				}
				D_red[s] = -action[end_symb];
				// printf ("D_red[%d] = %d\n", s, D_red[s]);
				goto Next;
         }
			D_red[s] = -1; // No default reduction.

Next:    if (sr+rr > 0) 
			{
				c_states++; 
				if (sr) 
				{
					sr_con++;
					if (!optn[LG_REDUCEONLY]) prt_con ("\n");
				}
				if (rr) 
				{
					rr_con++;
					prt_con ("\n");
				}
			}
      }						 
		la_start[n_states] = n_lookah;

      FREE (conflict, numberofterms);
      FREE (action,   numberofterms);
      FREE (LASUM,    n_bytes);

      for (i = 0; i < n_includes; i++) FREE (LA [i], n_bytes);
      FREE (LA, n_includes);

      FREE (lookback,   max_lookback);
      FREE (f_lookback, n_finals+2);
}
  
///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void	LGComputeLA::ANALYZE (int& nc, int s, int f)
{
		int p, lb, t;

		p = final [f]; 
      FASTINI (0, LASUM, n_words);
		for (lb = f_lookback [f]; lb < f_lookback [f+1]; lb++)
		{
			FASTOR (LA [lookback[lb]], LASUM, n_words);
		}
		for (t = 0; t < numberofterms; t++)	// For all terminal symbols. 
		{  
			#ifdef USINGBITS
			ifbit1(LASUM,t)						// Reduction defined for t ?
			#else
			if (LASUM[t])							// Reduction defined for t ?
			#endif
			{  
				if (action[t] == 0) action[t] = -p;	// No conflict!
				else if (action[t] <  0)				// RR conflict?
				{
					int HLP = head_level [head_sym [p]];				
					int HLA = head_level [head_sym [-action[t]]];
					if (HLA <= 2)	
					{
						if (HLP <= 2) // HLA <= 2 && HLP <= 2
						{
      					if (possibil [head_sym [-action[t]]] == 1)
							{
								if (possibil [head_sym [p]] > 1); // Keep action[t].
								else  // conflict between 2 originals with poss == 1
								{
									conflict[t] = -p;			
									prt_rrcon (nc, s, t); // Report fatal conflict.
								}
							}
							else
							{
								if (possibil [head_sym [p]] == 1) action[t] = -p; // Change action.
								else // conflict between 2 originals with poss > 1
								{
									conflict[t] = -p;			
									prt_rrcon (nc, s, t); // Report fatal conflict.
								}
							}
						}
						else;	// HLA <= 2 && HLP > 2 (keep action). 
					}
					else // HLA > 2
					{
						if (HLP <= 2) // HLA > 2 && HLP <= 2
						{
							action[t] = -p; // change action.
						}
						else // HLA > 2 && HLP > 2
						{
         				if (HLA < MAX_INT)
							{
								if (HLP == MAX_INT); // keep action.
								else // conflict between 2 original productions
								{
									conflict[t] = -p;			
									prt_rrcon (nc, s, t); // report fatal conflict.
								}
							}
							else // HLA == MAX_INT
							{
								if (HLP < MAX_INT) action[t] = -p; // change action.
								else  // conflict between 2 added productions.
								{
								  	conflict[t] = -p;     // report this to avoid looping in LGOptimizeStates.
								  	prt_rrcon (nc, s, t); // report fatal conflict.
								}								 
							}
						}
					}
				}
				else // SR conflict!				
				{
					if (p < n_oprods)
					{
					  	prt_srcon (nc, s, t, p);
					  	if (conflict[t] == 0) conflict[t] = -p; // Note conflict.
					   else if (p < -conflict[t]) conflict[t] = -p; // Note conflict.
					}
				}	
			}  
		}  
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  LGComputeLA::prt_rrcon (int& nc, int s, int t)
{
	  	if (ret_numb[-action[t]] != 0 && ret_numb[-conflict[t]] == 0)
		{
		//	rr++;
		//	if (optn [LG_CONFLICTS])
		//	{
		//		if (nc == 0)
		//		{
		//			PRT_STA (s);
		//			prt_con ("\n");
		//		}
		//		prt_con ("Conflict on lookahead %s, ", term_name[t]);
		//		prt_con ("reduce %d or reduce %d, ", -action[t], -conflict[t]);
		//		prt_con ("choosing reduce %d.\n", -action[t]);
		//	}
		}
		else // Same type of reductions, no priority!
		{
			rr++;
			n_fatals++;
			if (optn [LG_CONFLICTS])
			{
				if (nc == 0) // First time for this state?
				{
					PRT_STA (s);
					prt_log ("FATAL CONFLICTS in state %d, see conflict report file.\n", s);
				}
	  			st_type[s] = MR_STATE;
				prt_con ("Fatal conflict on LA  %s, ", term_name[t]);
				prt_con ("reduce %d or reduce %d.\n", -action[t], -conflict[t]);
			//	prt_con ("choosing reduce %d.\n", -action[t]);
				n_errors++;
				if (n_errors >= max_errors) 
				{
					prt_con ("\nMaximum error count (%d) reached.\n", max_errors);
					prt_log ("Maximum error count (%d) reached.\n", max_errors);
					LG::Terminate (1);
				}
				nc++;
			}
		}
}
  
///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  LGComputeLA::prt_srcon (int& nc, int s, int t, int p)
{
		sr++;
      if (optn [LG_CONFLICTS]) // Report conflicts!
      {
			if (!optn[LG_REDUCEONLY])
			{ 
		  		if (nc == 0)
		  		{
		  			PRT_STA (s);
		  			prt_con ("\n");
		  		}
		  		prt_con ("Conflict on lookahead %s, ", term_name[t]);
		  		prt_con ("goto %d or reduce %d, ", action[t], p);
		  		prt_con ("choosing goto %d.\n", action[t]);
			  	nc++;
			}
	  	}
}
  
///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  LGComputeLA::C_LOOKBACKS ()
{
      int p, dot, x, s, f;

      x            = 0;  /* include number. */
      inc_on       = 0;  /* Include switch.  */
      n_lookbacks  = 0;
      n_childs     = 0;
      n_includes   = 0;

		max_lookback = optn[MAX_LB ];  
		max_include  = optn[MAX_INC];  

      ALLOC (f_lookback, n_finals+2);
      ALLOC (lookback,   max_lookback);
      ALLOC (include,    max_include);
      ALLOC (f_child,    n_nonttran);
      ALLOC (child,      max_child);
      ALLOC (lb_num,     n_nonttran);

      FASTINI (-1, lb_num,  n_nonttran);
      FASTINI (-1, f_child, n_nonttran);

      for (s = 0; s < n_states; s++)
      {
         for (f = f_final [s]; f < f_final [s+1]; f++)
         {
            p = final [f];
				dot = prod_len [p];
				f_lookback [f] = n_lookbacks;
				LOOK_BACK (x++, p, dot, s);
         }
         f_lookback [f] = n_lookbacks;
      }
      FREE (lb_num,  n_nonttran);
      REALLOC (lookback, max_lookback, n_lookbacks);
      REALLOC (include,  max_include,  n_includes);
      REALLOC (f_child,  n_nonttran,   n_includes);
      REALLOC (child,    max_child,    n_childs);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  LGComputeLA::LOOK_BACK (int x, int p, int dot, int s)
{
      int y;
      int z, last;
      if (dot-- > 0)
      {
         last  = -1;
         for (y = f_camefrom [s]; y < f_camefrom [s+1]; y++)
         {
            if ((z = camefrom [y]) != last)
            {
               LOOK_BACK (x, p, dot, last = z);
            }  
         }
      }
	   else if ((y = NTX (s, head_sym[p])) >= 0)
      {
         if (inc_on == 0)
         {
            if (n_lookbacks >= max_lookback) MemCrash ("Number of lookbacks", max_lookback);
            lookback [n_lookbacks++] = y;
            inc_on = 1;
            C_INCLUDE (s, y);
            inc_on = 0;
         }
         else if (x != y && ATTACH (x, y) == 0)
         {
            C_INCLUDE (s, y);
         }  
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int	LGComputeLA::NTX (int s, int h) /* Nonterminal transition. */
{
      int i;
      for (i = ntt_start [s]; i < ntt_start [s+1]; i++)
      {
         if (ntt_symb[i] == h)
         {
            if (lb_num [i] < 0)  
            {
               lb_num [i] = n_includes;
               if (n_includes >= max_include) 
						MemCrash ("Number of includes", max_include);
               include [n_includes] = i;
               return (n_includes++);
            }
            return (lb_num [i]);
         }  
      }
      return (-1);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  LGComputeLA::C_INCLUDE (int s, int y)
{
      int lb, S, k, p, dot, t;

      lb = include [y];
      if (lb >= 0) /* If not done before? */
      {
         include [y] = -(lb+1); /* Mark it done. */
         S = ntt_action [lb];
         for (k = f_kernel [S]; k < f_kernel [S+1]; )
         {
            dot = item[kernel[k]].dot;
            if (dot > 0) /* If not null production? */
            {
               p = item[kernel[k]].prod;
               for (t = f_tail[p]+dot; t < l_tail[p]; t++)
               {
                  if (tail [t] >= 0) goto Next;
                  if (!nullable [-tail [t]]) goto Next;
               }
               LOOK_BACK (y, p, (int)(dot-1), s);
            }
Next:       k++;
         }  
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  LGComputeLA::C_READS ()
{
      int* whereisit;
      int  i, s, m, lb;

	 	if (optn[LG_VERBOSE] > 2) 
	 	prt_log ("LA matrix size = %d\n", n_includes*n_bytes);

      ALLOC (LA, n_includes); 
      for (i = 0; i < n_includes; i++)
      {
         ALLOC (LA[i], n_bytes); 
         memset (LA[i], 0, n_bytes);
      }

      ALLOC (already, n_states);
      ALLOC (whereisit, n_states);
      FASTINI (-1, already, n_states);
      FASTINI (-1, whereisit, n_states);

      for (i = 0; i < n_includes; i++)
      {
         lb = include [i];
         if (lb < 0) lb = -(lb+1);
         if ((s = ntt_action [lb]) > 0)
         {
            if (whereisit [s] == -1)
            {
               IND_READ (i, s);
               whereisit [s] = i;
            }
            else
            {
               m = whereisit [s];
               FASTCPY (LA[m], LA[i], n_words);
            }  
         }  
      }

      FREE (whereisit,   n_states);
      FREE (already, n_states);

      T_GRAPH (LA, n_includes, n_terms);

      FREE (child,   n_childs);
      FREE (f_child, n_includes);
      FREE (include, n_includes);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  LGComputeLA::IND_READ (int i, int s)
{
      int x, j, a;
      already [s] = i;
      for (j = ntt_start [s]; j < ntt_start [s+1]; j++)
      {
         if ((x = ntt_action [j]) > 0)
         {
            if (nullable [ntt_symb [j]])
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
         a = tt_symb [j];	
			#ifdef USINGBITS
			setbit2(LA,i,a);
			#else
			LA[i][a] = 1;
			#endif
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  LGComputeLA::PRT_STA (int s)
{
      int k, i, f, p;
      prt_con ("///////////////////////////// STATE %d /////////////////////////////\n\n", s);
      for (k = f_kernel [s]; k < f_kernel [s+1]; k++)
      {
         i = kernel [k];
         if (item[i].symb != -32767)
         {
            prt_con ("* ");
            p_prod (item[i].prod, item[i].dot, "");
         }  
      }
      for (f = f_final [s]; f < f_final [s+1]; f++)
      {
         p = final [f];
         if (p >= 0)
         {
            if (p < n_oprods) prt_con ("* ");
            else              prt_con (". ");
            p_prod (p, -1, "");
         }  
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  LGComputeLA::p_prod (int p, int dot, char *before)
{
      int t, u, d;
      prt_con ("%s%5d %s -> ", before, p, head_name [head_sym [p]]);
      t = f_tail [p];
      u = l_tail [p];
      d = t + dot;
      if (dot == -1) d = u;
      for (;;)
      {
         if (t == d) prt_con (". ");
         if (t >= u) break;
         p_sym (tail [t], " ");
         t++;
      }
      prt_con ("\n");
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int	LGComputeLA::p_sym (int s, char *sp)
{
      char *p;
      if (s >= 0)                 /* Terminal symbol? */
         p = term_name[s];
      else                        /* Nonterminal symbol? */
         p = head_name[-s];
      prt_con ("%s%s", p, sp);
      return (strlen(p) + strlen(sp));
}

#endif

/*--- End of Anl_stat.C -----------------------------------------------------*/

