
#ifdef LRSTAR

		#include "ComGlobal.h"
		#include "PGBuildLR1.h"

//		#define PG_DEBUG
  		#define PRINT prt_sta

      #define TT_STATE   1		// State has terminal transitions.
      #define NT_STATE   2		// State has nonterminal transitions.
      #define MR_STATE   4		// State has multiple reductions.
      #define RO_STATE   8		// State is a reduce only state. 
      #define UN_STATE  16		// State is unused, remove it.

      int    PGBuildLR1::n_states;
		int	 PGBuildLR1::extra_states;
		int*   PGBuildLR1::ntt_item;
		int*   PGBuildLR1::accessor;
		int    PGBuildLR1::accept_state; 
		char** PGBuildLR1::FIRST;
		char** PGBuildLR1::FOLLOW;
		int*   PGBuildLR1::f_kernel;
		int*   PGBuildLR1::l_kernel;
		int*   PGBuildLR1::f_final;
		int*   PGBuildLR1::l_final; 
		int*   PGBuildLR1::final;
		int*   PGBuildLR1::kernel;
		ITEM*  PGBuildLR1::item;    
		int*   PGBuildLR1::f_item;
		int    PGBuildLR1::n_ttran;
		int    PGBuildLR1::n_nttran;
		int*   PGBuildLR1::ntt_origin;
		int    PGBuildLR1::n_kernels;
		int    PGBuildLR1::n_finals;
		int    PGBuildLR1::org_states;
		int    PGBuildLR1::n_items;
		int*   PGBuildLR1::ntt_start;
		int*   PGBuildLR1::ntt_end;
		int*   PGBuildLR1::ntt_symb;
		int    PGBuildLR1::ntt_states;
		int*   PGBuildLR1::ntt_action;
		int    PGBuildLR1::n_ttas;
		int*   PGBuildLR1::tt_start;
		int*   PGBuildLR1::tt_end;
		int*   PGBuildLR1::tt_action;
		int*   PGBuildLR1::tt_symb;
		int    PGBuildLR1::tt_states;
		int*   PGBuildLR1::f_camefrom;
		int*   PGBuildLR1::l_camefrom;
		int*   PGBuildLR1::camefrom;

	// Variables.
      static int			state;
      static uint			hash_no;
      static LRKERNEL*	lrkernel; 
		static LRKERNEL*	closure;
		static LRKERNEL** Closure;
		static int			final_state;
		static char*     	first_time;
		static int			n_clo;
		static int*     	reduce_state;
		static char**    	lookahead;
		static int       	n_newheads;
		static short*    	new_head;
		static char*     	inclosure_term;
		static char*     	inclosure_head;
		static short**   	Closure_Term;
		static short**   	Closure_Head;
		static int       	nfi_lalr;
		static int*      	N_clo;
		static int*     	reduction_x;
		static int*     	reduction_y;
		static char*     	item_added;
		static char*     	prod_added;
		static int       	n_collisions;
		static int       	top;
		static int		  	max_clo;
		static int       	max_states;
		static int       	max_finals;
		static int       	max_tails;
		static int       	max_lrkernels;
		static int       	max_kernels;
		static int       	max_closure;
		static int			max_lookback;
		static int			max_include;
		static int			max_ntt;
		static int			max_lookahead;
		static int			max_hashes;
		static uint			hash_div;
		static int*			st_type;
		static int*       f_lrkernel;
		static int*       l_lrkernel;
		static int**      Tgotos;
		static int**      Ngotos;
		static int*			hash_vector;
		static int			n_lrkernels;
		static int			n_nonttran;
		static int			n_termtran;
		static int			ro_states;
		static int			s_states;
		static int        rr_states;
		static int			sr_states;
		static int			nt_states;
		static int*			L_tail;
		static char*      Grammar;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
  
int   PGBuildLR1::BuildLR1 () /* Build Canonical LR1 States */
{
      int h, ns;
      max_clo = 0;

		C_ITEMS ();

		max_states    = optn[MAX_STA];
      max_finals    = optn[MAX_FIN];
      max_kernels   = optn[MAX_KER];
      max_child     = optn[MAX_CH];	// Global variable.
		max_lookback  = optn[MAX_LB];
      max_lookahead = optn[MAX_LA];
      max_include   = optn[MAX_INC]; 
		max_ntt		  = optn[MAX_NTT];
		max_tails     = optn[MAX_TAIL];

      max_closure   = max_tails;	// ? 
      max_lrkernels = max_kernels;  // ? 

      max_hashes  = 2*max_states + 1;
      hash_div    = UINT_MAX / max_hashes + 1;

      ALLOC (ntt_item,     max_ntt    );
      ALLOC (accessor,     max_states );
      ALLOC (f_final,      max_states );
      ALLOC (l_final,      max_states );
      ALLOC (f_lrkernel,   max_states );
      ALLOC (l_lrkernel,   max_states );
      ALLOC (lrkernel,     max_lrkernels); 
      ALLOC (final,        max_finals  );

      ALLOC (Tgotos,       max_states );
      ALLOC (Ngotos,       max_states );
      ALLOC (N_clo,        max_states );

      ALLOC (closure,      max_closure);
      ALLOC (inclosure_term, N_terms    );
      ALLOC (inclosure_head, N_heads    );

      ALLOC (Closure,      max_states );
      ALLOC (Closure_Term, max_states );
      ALLOC (Closure_Head, max_states );
                           
      ALLOC (hash_vector,  max_hashes );

      ALLOC (new_head,     N_heads);
      ALLOC (reduction_x,  N_terms);
      ALLOC (reduction_y,  N_terms);
      ALLOC (item_added,   n_items);
      ALLOC (prod_added,   N_prods);
      ALLOC (first_time,   N_heads);
      ALLOC (reduce_state, N_prods);

      ALLOC (lookahead, N_heads);
      for (h = 0; h < N_heads; h++) ALLOC (lookahead[h], N_terms);

      ALLOC (L_tail, N_prods);
		for (int p = 0; p < N_prods; p++) L_tail[p] = F_tail[p+1];

      FASTINI ( 0, reduce_state, N_prods);
      FASTINI (-1, hash_vector, max_hashes);

      n_finals      = 0;
      n_lrkernels   = 0;
      n_kernels     = 0;
      accessor[0]   = 0;
      n_nonttran    = 0;
      n_termtran    = 0;
      ro_states     = 0;
      s_states      = 0;
      rr_states     = 0;
      sr_states     = 0;
      top           = 0;
      n_states      = 1;
      tt_states     = 0;
      nt_states     = 0;
      extra_states  = 0;
      n_collisions  = 0;

      C_FIRST (N_heads, N_terms, N_prods, F_prod, F_tail, Tail, FIRST, nullable, head_sym);

      MAKE_KERNEL(0);
      DO_CLOSURE (0);

		if (optn[PG_STATELIST] > 1) PRINT ("\nSTATE MACHINE WITH CLOSURE ITEMS:\n\n"); 
      for (state = 0; state < n_states; state++)
      {
         EXPAND (state);
      }
      opt_states = n_states; // Optimum number of states, without reduce-only states for LALR.

      MAKE_LR0_KERNELS ();

      FREE (lrkernel,   max_lrkernels);
      FREE (f_lrkernel, max_states );
      FREE (l_lrkernel, max_states );

		char* Grammar;
      if      (optn[PG_LALR_PARSER]) Grammar = "LALR(1) "; 
      else if (optn[PG_LR_PARSER]  ) Grammar = "LR(1)   "; 
      else                           Grammar = "CLR(1)  "; 

		if (optn[PG_SHIFTREDUCE]) ro_states--;
		else					        ro_states = 0;

		if      (optn[PG_LALR_PARSER]) prt_log ("%s %7d states in LALR(1) state machine.\n", Grammar, n_states);
		else if (optn[PG_LR_PARSER]  ) prt_log ("%s %7d states in Minimal LR(1) state machine.\n", Grammar, n_states);
		else                           prt_log ("%s %7d states in Canonical LR(1) state machine.\n", Grammar, n_states);

		if (optn[PG_SHIFTREDUCE])
			  prt_log ("         %7d states after implementing shift-reduce actions.\n", n_states - ro_states);
		else prt_log ("         %7d states removed for shift-reduce actions.\n", 0);

      REALLOC (final,    max_finals, n_finals+1);
      REALLOC (f_final,  max_states, n_states+1);
      REALLOC (accessor, max_states, n_states  );
      REALLOC (Tgotos,   max_states, opt_states);
      REALLOC (Ngotos,   max_states, opt_states);
      REALLOC (ntt_item, max_ntt,    n_nonttran);

      for (h = 0; h < N_heads; h++) FREE (lookahead[h], N_terms);
      FREE (lookahead, N_heads);

		FREE (l_final,			 max_states);
      FREE (N_clo,			 max_states);
      FREE (closure,			 max_closure);
      FREE (Closure,			 max_states);
      FREE (hash_vector,	 max_hashes);

      FREE (reduction_x,	 N_terms);
      FREE (reduction_y,	 N_terms);
      FREE (first_time,		 N_heads);
      FREE (new_head,		 N_heads);
      FREE (inclosure_term, N_terms);
      FREE (inclosure_head, N_heads);
      FREE (item_added,		 n_items);
      FREE (prod_added,		 N_prods);

		MAKE_LR0_TRANSITIONS ();

		for (int s = 0; s < opt_states; s++) FREE (Tgotos[s], Tgotos[s][0]+1);
		FREE (Tgotos, opt_states);

		for (int s = 0; s < opt_states; s++) FREE (Ngotos[s], Ngotos[s][0]+1);
		FREE (Ngotos, opt_states);

      MODIFY_TRANSITIONS ();
      MTSL ();
		C_CAMEFROM (n_states, tt_start, tt_action, ntt_start, ntt_action, f_camefrom, camefrom); 

      FREE (reduce_state, N_prods);
		n_kernels = n_lrkernels; // For stats list in Terminate. 
		if (n_errors) return (0);
      return (1);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGBuildLR1::C_ITEMS ()
{
      int h, i, p, d, t, np;
      i = 0;
		np = 0;
      for (h = 0; h < N_heads; h++)
      {
         for (p = F_prod[h]; p < F_prod[h+1]; p++)
         {
				np++;
            for (t = F_tail[p]; t < F_tail[p+1]; t++)
            {
               i++;
            }
            i++;
         }
      }
      n_items = i;
      ALLOC (f_item, np+1);
      ALLOC (item,   n_items);
      i = 0;
      for (h = 0; h < N_heads; h++)
      {
         for (p = F_prod[h]; p < F_prod[h+1]; p++)
         {
            d = 0;
            f_item [p] = i;
            for (t = F_tail[p]; t < F_tail[p+1]; t++)
            {
               item[i].symb  = Tail [t];
               item[i].prod  = p;
               item[i++].dot = d++;
            }
            item[i].prod   = p;
            item[i].dot    = d;
            item[i++].symb = -32767;
         }
      }
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
  
void  PGBuildLR1::EXPAND (int state)
{
      int i, c, h, t, nt, tt, nfi;

   // Define all Nonterminal transitions out of this state.
      ALLOC (Ngotos[state], Closure_Head[state][0]+1);
      Ngotos[state][0] =    Closure_Head[state][0];
      nt = 1;
      for (c = 1; c <= Closure_Head[state][0]; c++)
      {
         h = Closure_Head[state][c];  
         ntt_item[n_nonttran++] = MAKE_KERNEL (state, -h);
         Ngotos[state][nt++] = TRANSITION (-h);  
      }

   // Define all Terminal transitions out of this state.
      ALLOC (Tgotos[state], Closure_Term[state][0]+1);
      Tgotos[state][0] =    Closure_Term[state][0];
      tt = 1;
      for (c = 1; c <= Closure_Term[state][0]; c++) 
      {
         t = Closure_Term[state][c];
         MAKE_KERNEL (state, t);
         Tgotos[state][tt++] = TRANSITION (t);
      }
      n_termtran += --tt;

  		if (optn[PG_STATELIST] > 1)
		{
	      PRT_LRSTA (state);
		   PRT_CLO (state);
			PRT_TRAN (state);
		}
  
   // Free up the stuff we no longer need.
      FREE (Closure_Term[state], N_terms); 
      FREE (Closure_Head[state], N_heads);
      FREE (Closure[state], n_clo);

}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
  
void  PGBuildLR1::MAKE_KERNEL (int state)          // Make lrkernel items for state 0.
{
      int p, head;
      head = 0;                        // Head symbol is the Goal symbol.
      f_lrkernel[state] = 0;
      for (p = F_prod [head]; p < F_prod [head+1]; p++) // Should be only one production.                                      
      {
         lrkernel [n_lrkernels].item = f_item[p];
         lrkernel [n_lrkernels].LA   = 0; // Not used anyway, 0 is safe.      
         if (++n_lrkernels >= max_lrkernels)  MemCrash ("Number of LR(1) kernels", max_lrkernels);
      }
      l_lrkernel[state] = n_lrkernels;
      f_final [state]   = 0;           // Nothing.
      l_final [state]   = 0;           // Nothing.
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
  
int	PGBuildLR1::MAKE_KERNEL (int camefrom, int symb)
{
      int i, la, c, first_item;
//		prt_log ("\nFROM STATE %d MAKING STATE %d\n\n", camefrom, n_states);
      hash_no = 0;
		first_item = -1;
      f_lrkernel[n_states] = n_lrkernels;
      for (c = 0; c < N_clo[camefrom]; c++)		// For all closure items.  
      {
         i = Closure[camefrom][c].item;			// Get item #.
         if (item[i].symb == symb)					// If head symbol match?
         {
				if (first_item == -1) first_item = i;
            la = Closure[camefrom][c].LA;       // Get old LA.
            hash_no += N_terms*(i+1) + la;		// Add both to hash number. 
            lrkernel [n_lrkernels].item = i+1;  // Make new LR1 item.
            lrkernel [n_lrkernels].LA   = la;   // Keep same look ahead.
//          prt_item ("Making LR1 kernel ", i+1, la);
            if (++n_lrkernels >= max_lrkernels) MemCrash ("Number of LR(1) kernels", max_lrkernels);
         }
      }
      l_lrkernel[n_states] = n_lrkernels;
      if (n_lrkernels == f_lrkernel[n_states]) InternalError (255);
//    PRT_LRSTA (n_states);
		return first_item;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
  
int   PGBuildLR1::TRANSITION (int sym)
{
      uint probe;
		LRKERNEL temp;
		int  compatible = 1;
      int  fk, lk, nk, ni, fi, i, j, k, x, y, p;

	//	PRT_LRSTA (n_states);
		fk = f_lrkernel[n_states];
		lk = l_lrkernel[n_states];
      nk = lk - fk;					
		fi = 0;
		x  = n_states;
		#ifdef PG_DEBUG
	  	PRT_LRSTA (x);
		#endif
		if (optn[PG_CLR_PARSER]) 
		{
			if (nk > 1)						
			{
				LR1_SORT (lrkernel, fk, lk);	 
			}
			probe = hash_no % max_hashes;
			while ((y = hash_vector[probe]) != -1)	// Get state y with same hash cell. 
			{
				if (l_lrkernel[y] - f_lrkernel[y] == nk) // Same number of kernels?
				{
					for (i = f_lrkernel[x], j = f_lrkernel[y]; i < l_lrkernel[x]; i++, j++)
					{
						if (lrkernel[i].item != lrkernel[j].item)	goto Next1;	
						if (lrkernel[i].LA   != lrkernel[j].LA  ) goto Next1;
					}
					n_lrkernels = fk;						// Reset this.
					return (y);								// Return old state number.
				}  
Next1:		probe = (hash_no *= 65549) / hash_div;
			}
		}
		else // PG_LR_PARSER or PG_LALR_PARSER
		{
			ni = LR0_SORT (lrkernel, fk, lk);	 
		  	if (ni == 1)									// Number of lrkernel items is 1 or 0?
			{
	  		  	i = lrkernel[fk].item;					// Get item #
				if (item[i].symb == -32767) 
				{
					if (optn[PG_LALR_PARSER])
					{
						p = item [i].prod;				// Get production.
						reduce_state[p] = 1;				// Mark production for reduce-state creation.   
						return (-p);						// Return production number.
					}
				  	fi = i;									// Save the final item.
				}
			}	 
			probe = hash_no % max_hashes;
			while ((y = hash_vector[probe]) != -1)	// Get state y with same hash cell. 
			{
				#ifdef PG_DEBUG
			  	PRINT ("Comparing to:\n\n");
			  	PRT_LRSTA (y);
				#endif
				int n = 0, m = 0;
				memset (item_added, 0, n_items);
				for (i = f_lrkernel[x]; i < l_lrkernel[x]; i++)
				{
					if (item_added[lrkernel[i].item] == 0)	// Item not added yet?
					{
						n++; item_added [lrkernel[i].item] = 1; // Add it.
					}
				}
				for (i = f_lrkernel[y]; i < l_lrkernel[y]; i++)
				{
					if (item_added[lrkernel[i].item] == 0) goto Next2; // Missmatch!
					if (item_added[lrkernel[i].item] == 1)				   // Match!?
					{
						m++; item_added [lrkernel[i].item] = 2;
					}
				}
				if (n == m) // LALR(1) satisfied?
				{
					if (!optn[PG_LALR_PARSER]) 
					{
						if (!COMPATIBLE (x, y)) 
						{
							compatible = 0;
							goto Next2;
						}
					}
					#ifdef PG_DEBUG
				  	PRINT ("Got a match!\n");
					#endif
					n_lrkernels = fk;					// Reset this, not a new state.
					if (fi)								// If final item defined?
					{
						p = item [fi].prod;			// Get production.
						reduce_state[p] = 1;			// Mark production for reduce-state creation.   
						return (-p);					// Return production number.
					}
					return (y);			 // Return old state number.
				}
Next2:		probe = (hash_no *= 65549) / hash_div;
			}
		}
		if (!compatible) extra_states++;
		if (fi)								// If final item defined by LR or LALR?
		{
			#ifdef PG_DEBUG
  	  		PRINT ("Reduce-ony state!\n");
			#endif
			p = item [fi].prod;			// Get production.
			reduce_state[p] = 1;			// Mark production for reduce-state creation.   
			return (-p);					// Return production number.
		}

	// NEW STATE ...
		#ifdef PG_DEBUG
  	  	PRINT ("New state %d\n", n_states);
		#endif
      accessor [n_states] = sym;
      hash_vector [probe] = n_states;
      DO_CLOSURE (n_states); 
      if (n_states % 50000 == 0) prt_log ("         %7d states\n", n_states);
      if (n_states >= max_states) MemCrash ("Number of states", max_states);
      return (n_states++);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
  
void  PGBuildLR1::LR1_SORT (LRKERNEL* lrkernel, int fk, int lk)
{
		LRKERNEL temp;
		int i, j, first, last, itemnumb; 

	// Sort the items ...
		for (i = fk+1; i < lk; i++)
		{	
			for (j = i; j > fk; j--)
			{
				if (lrkernel[j].item >= lrkernel[j-1].item) break;
				temp = lrkernel[j-1];
				lrkernel[j-1] = lrkernel[j];
				lrkernel[j] = temp;
			}
		}

	// Sort the lookaheads also ...
		first = fk;
		itemnumb = lrkernel[first].item;			
		for (last = first+1; last <= lk; last++)
		{	
			if (lrkernel[last].item != itemnumb || last == lk) 
			{
				for (i = first+1; i < last; i++)
				{	
					for (j = i; j > first; j--)
					{
						if (lrkernel[j].LA >= lrkernel[j-1].LA) break;
						temp = lrkernel[j-1];
						lrkernel[j-1] = lrkernel[j];
						lrkernel[j] = temp;
					}
				}
				first = last;
				itemnumb = lrkernel[first].item;			
			}
		}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
  
int	PGBuildLR1::LR0_SORT (LRKERNEL* lrkernel, int fk, int lk)
{
		LRKERNEL temp;
		int i, j, k, first, last, itemnumb, ni; 

	// Sort the items ...
		for (i = fk+1; i < lk; i++)
		{	
			for (j = i; j > fk; j--)
			{
				if (lrkernel[j].item >= lrkernel[j-1].item) break;
				temp = lrkernel[j-1];
				lrkernel[j-1] = lrkernel[j];
				lrkernel[j] = temp;
			}
		}

	// Count the number of items ...
		ni = 1;
		hash_no = itemnumb = lrkernel[fk].item;			
		for (k = fk; k < lk; k++)
		{	
			if (lrkernel[k].item != itemnumb) 
			{
				ni++;
				hash_no += itemnumb = lrkernel[k].item;			
			}
		}
		return ni;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
  
int	PGBuildLR1::COMPATIBLE (int x, int y) // Check for minimal LR(1) compatibility. 
{
		int f, k, nfi, i, la, p;

		FASTINI (0, reduction_x, N_terms);
		for (k = f_lrkernel[x]; k < l_lrkernel[x]; k++) // X state final items check
		{	
			i  = lrkernel[k].item;
			if (item[i].symb == -32767) // Final item?
			{
				la = lrkernel[k].LA;
				p  = item[i].prod;
				if      (reduction_x[la] == 0) reduction_x[la] =  p; // Define new reduction.
				else if (reduction_x[la] != p) reduction_x[la] = -1; // Mark as conflict.
			}
		}

	//	FASTINI (0, reduction_y, N_terms); // For research only.
		for (k = f_lrkernel[y]; k < l_lrkernel[y]; k++)	// Y state final items check
		{	
			i  = lrkernel[k].item;
			if (item[i].symb == -32767) // Final item?
			{
				la = lrkernel[k].LA;
				p  = item[i].prod;
				if (reduction_x[la] == 0) // LA not previuosly used (not a conflict)?
				{
				//	return 0; // Not compatible (special case for research only).
				}
				else if (reduction_x[la] == -1) // LA already conflicted by state x!
				{
					/* ignore this */
				}
				else if (reduction_x[la] != p) // Definitely a conflict?
				{
					return 0; // NOT COMPATIBLE !!!
				}
			//	if (reduction_y[la] == 0) reduction_y[la] =  p;	// Define new reduction.
			//	if (reduction_y[la] != p) reduction_y[la] = -1; // Mark as conflict.
			}
		}

	// Could possibly do something with conflicted (-1) entries comparing x to y.
	// Nothing for now, maybe later.
	   return 1;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
  
void  PGBuildLR1::DO_CLOSURE (int state)
{
      int head, i, k, h, c, t, la, p, nc, n, nfi, nla;

      memset (inclosure_term, 0, N_terms);
      memset (inclosure_head, 0, N_heads);
      memset (first_time,     1, N_heads);
      memset (prod_added,     0, N_prods);

      n_clo = 0;
      n_newheads = 0;
      f_final[state] = n_finals;
      for (k = f_lrkernel[state]; k < l_lrkernel[state]; k++)
      {
         ADD_ITEM (lrkernel[k].item, lrkernel[k].LA);
      }

Top:  nc = n_clo;
      for (h = 0; h < n_newheads; h++) 
      {
         head = new_head[h];
         if (first_time[head])
         {
            first_time[head] = 0;
            memset (lookahead[head], 0, N_terms);
         }
			nla = 0;
         for (c = 0; c < n_clo; c++)
         {
            i = closure[c].item;
            if (item[i].symb == -head)							// If next tail is head symbol?
            {
Next:          la = item[++i].symb;								// Get follow symbol.
					if (la < 0)											// If nonterminal symbol?
					{
						if (la == -32767)								// If final item?
						{
							la = closure[c].LA;						// Use item lookahead.
							if (lookahead[head][la] == 0)			// If it's a new lookahead,
							{
								nla++; lookahead[head][la] = 1;	// Add to lookahead set.
							}
						}
						else												// Nonterminal!
						{
							la = -la;
							for (t = 0; t < N_terms; t++)			// For all terminal symbols
							{
								#ifdef USINGBITS
								ifbit2(FIRST,la,t)					// If t is in FIRST set?
								#else
								if (FIRST[la][t])						// If t is in FIRST set?
								#endif
								{
									if (lookahead[head][t] == 0)	// If it's a new lookahead,
									{
										nla++; lookahead[head][t] = 1;	// Add to lookahead set.
									}
								}
							}
							if (nullable[la]) goto Next;			// Get next tail symbol in prod.
						}
               }
               else													// Terminal symbol!
               {
						if (lookahead[head][la] == 0)				// If it's a new lookahead,
						{
							nla++; lookahead[head][la] = 1;		// Add to lookahead set.
						}
               }
            }
         }
			if (nla > 0)
			{
				for (t = 0; t < N_terms; t++)					// For all terminals.
				{
					if (lookahead[head][t] == 1)				// Is terminal a new lookahead?
					{
						lookahead[head][t] = 2;					// Mark it already done!
						for (p = F_prod [head]; p < F_prod [head+1]; p++)
						{
							ADD_ITEM (f_item[p], t);
						}  
					}
				}
			}
      }

      if (n_clo > nc) goto Top;

      l_final[state] = n_finals;
      if (n_clo > max_clo) max_clo = n_clo;

      N_clo[state] = n_clo;
      ALLOC (Closure[state], n_clo);
      for (i = 0; i < n_clo; i++)
      {
         Closure[state][i].item = closure[i].item;
         Closure[state][i].LA   = closure[i].LA;
      }

	// Save nonterminal closure items ...
      n = 0;
      for (h = 0; h < N_heads; h++) 
      {
         if (inclosure_head[h]) n++;
      }
      ALLOC (Closure_Head[state], n+1);
      Closure_Head[state][0] = n; 
      n = 1;
      for (h = 0; h < N_heads; h++) 
      {
         if (inclosure_head[h]) Closure_Head[state][n++] = h;
      }

	// Save terminal closure items ...
      n = 0;
      for (t = 0; t < N_terms; t++) 
      {
         if (inclosure_term[t]) n++;
      }
      ALLOC (Closure_Term[state], n+1);
      Closure_Term[state][0] = n; 
      n = 1;
      for (t = 0; t < N_terms; t++) 
      {
         if (inclosure_term[t]) Closure_Term[state][n++] = t;
      }
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
  
void  PGBuildLR1::ADD_ITEM (int i, int la)
{
      int symb, p;
//		prt_log  ("Adding LR(1) item: ");
//    prt_item ("", i, la);
      if ((symb = item[i].symb) != -32767)            // If not final item. 
      {
         closure [n_clo].item = i;                    // Put item in closure set.
         closure [n_clo].LA   = la;                   // Put lookahead in closure set.
         if (++n_clo >= max_closure) 
         {
            if (n_clo > max_clo) max_clo = n_clo;
            MemCrash ("Number of tail symbols", max_closure);  // Same as max_tails.
         }
         if (symb >= 0) // Terminal symbol? 
         {
            inclosure_term[symb] = 1;
         }  
         else if (inclosure_head[-symb] == 0)
         {
            inclosure_head[-symb] = 1;
            new_head[n_newheads++] = -symb;
         }
      }
      else // It's a final item.
      {
         p = item[i].prod;
         if (prod_added[p] == 0)
         {
            prod_added[p] = 1;
            final [n_finals] = i; // i and not p.
         // prt_prod (p, -1, "Adding final item ", "\n");
            if (++n_finals >= max_finals) MemCrash ("Number of final items", max_finals);
         }
      }
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
  
void  PGBuildLR1::MAKE_LR0_KERNELS () 
{
      int state, k, i; 

		if (!optn[PG_CLR_PARSER])
		{
			ro_states = 0;
			for (int p = 0; p < N_prods; p++)
			{
				if (reduce_state[p] != 0) ro_states++;
			}
			n_states += ro_states;
		}

      ALLOC (kernel, max_lrkernels);
      ALLOC (f_kernel, n_states+1 );   // Final state not made, so use n_states+1.

      n_kernels = 0;
      for (state = 0; state < opt_states; state++)  // For all states.
      {
         f_kernel[state] = n_kernels;
         memset (item_added, 0, n_items);
		//	printf ("f_lrkernel[%d] = %d, l_lrkernel[%d] = %d\n", s, f_lrkernel[s], s, l_lrkernel[s]);
         for (k = f_lrkernel[state]; k < l_lrkernel[state]; k++) // For all LR(1) kernels.
         {
            i = lrkernel[k].item;                        // Get LR(1) item #.
            if (item_added[i] == 0)                      // Item not added ?
            {
               item_added[i] = 1;                     
               kernel[n_kernels++] = i; 
            }
         }
      }
	  	f_kernel[state] = n_kernels; // Very important. 
		f_final[state]  = l_final[state-1];

		if (!optn[PG_CLR_PARSER])
		{
			n_states = opt_states;
			for (int p = 0; p < N_prods; p++)
			{
				if (reduce_state[p] != 0) 
				{
					reduce_state[p] = state++;
					MAKE_STATE (p);
				}
			}
		}

      REALLOC (kernel, max_kernels, n_kernels);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGBuildLR1::MAKE_STATE (int prod)
{
      int state;
      state = reduce_state[prod]; 
      accessor[state]   = Tail [F_tail [prod+1]-1];
		f_final[state]    = n_finals;
      final [n_finals]  = f_item[prod] + prod_len[prod];
      kernel[n_kernels] = f_item[prod] + prod_len[prod];

      if (++n_finals  >= max_finals ) MemCrash ("Number of final items", max_finals);
      if (++n_kernels >= max_kernels) MemCrash ("Number of kernel items", max_kernels);
      if (++n_states  >= max_states ) MemCrash ("Number of states", max_states);

      f_final  [n_states] = n_finals;
      f_kernel [n_states] = n_kernels;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//		Compute tt_start, ntt_start, tt_action, ntt_action ...
  
void 	PGBuildLR1::MAKE_LR0_TRANSITIONS () 
{
		int s, tt, ntt, t, n, k, sym, i;
      n_ttran  = n_termtran;
      n_nttran = n_nonttran;
		org_states = n_states;

		ALLOC (tt_start,   n_states+1);
		ALLOC (ntt_start,  n_states+1);
		ALLOC (tt_action,  n_termtran);
		ALLOC (ntt_action, n_nonttran);

		tt = 0; 
		ntt = 0;
		for (s = 0; s < opt_states; s++)
		{
			tt_start[s] = tt;
			n = Tgotos[s][0];				// Number of terminal transition for this state.
			for (t = 1; t <= n; t++)	// For all terminal transitions.
			{
				tt_action[tt++] = Tgotos[s][t];	// Get goto state.
			}
			ntt_start[s] = ntt;
			n = Ngotos[s][0];				// Number of nonterminal transition for this state.
			for (t = 1; t <= n; t++)	// For all nonterminal transitions.
			{
				ntt_action[ntt++] = Ngotos[s][t];	// Get goto state.
			}
		}

		for (s = opt_states; s < n_states; s++)
		{
			tt_start [s] = tt;
			ntt_start[s] = ntt;
		}

		tt_start [s] = tt;
		ntt_start[s] = ntt;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGBuildLR1::MODIFY_TRANSITIONS () 
{
      int state, i;

   // Modify negative transitions.
      for (state = 0; state < opt_states; state++)
      {
			for (i = tt_start[state]; i < tt_start[state+1]; i++)
			{
				if (tt_action[i] <= 0)
				{
					tt_action[i] = reduce_state[-tt_action[i]];
				}
			}
			for (i = ntt_start[state]; i < ntt_start[state+1]; i++)
			{
				if (ntt_action[i] < 0)
				{
					ntt_action[i] = reduce_state[-ntt_action[i]];
				}
			}
		}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
      
void  PGBuildLR1::prt_prod (int p, int dot, char* before, char* after)
{
      int t, u, d;
      PRINT ("%s%5d %s -> ", before, p, head_name [head_sym [p]]);
      t = F_tail [p];
      u = L_tail [p];
      d = t + dot;
      if (dot == -1) d = u;
      for (;;)
      {
         if (t == d) PRINT (". ");
         if (t >= u) break;
         prt_sym (Tail [t], " ");
         t++;
      }
      PRINT (after);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
      
void  PGBuildLR1::prt_item (char* before, int i, int la)
{
      if (la >= 0)
      {
		//	PRINT ("%4d ", i);
			prt_prod (item[i].prod, item[i].dot, before, " {");
		//	PRINT ("%3d ", la);
			prt_sym (la, "}\n");
      }
      else 
      {
         prt_prod (item[i].prod, item[i].dot, before, "\n");
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
      
void  PGBuildLR1::PRT_LRSTA (int state)
{
      int k, i, f, p, n, m;

      PRINT ("///////////////////////////// STATE %d /////////////////////////////\n//\n", state);

		n = 0;
      for (k = f_lrkernel [state]; k < l_lrkernel [state]; k++)
      {
         i = lrkernel[k].item;
         if (item[i].symb != -32767)
         {
				n++;
            prt_item ("// *", i, lrkernel[k].LA);
         }  
      }
		m = 0;
      for (k = f_lrkernel [state]; k < l_lrkernel [state]; k++)
      {
         i = lrkernel[k].item;
         if (item[i].symb == -32767)
         {
				m++;
            prt_item ("// *", i, lrkernel[k].LA);
         }  
      }
		if (m == 0)
		{
			m++;
			for (f = f_final [state]; f < l_final [state]; f++)
			{
				n++;
				p = item [final[f]].prod;
				prt_prod (p, -1, "// f", "\n");
			}
		}

		if (n+m) PRINT ("\n");
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
  
void  PGBuildLR1::PRT_CLO (int state)
{
      int  c, i, la, n; 
		n = 0;
      for (c = 0; c < N_clo[state]; c++)
      {
         i  = Closure[state][c].item;
			if (item[i].dot == 0)
			{
				n++;
				la = Closure[state][c].LA;
				prt_item ("   c", i, la);
			}
      }
		if (n) PRINT ("\n");
}
  
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
      
void  PGBuildLR1::PRT_STA (int state)
{
      int k, i;

      PRINT ("State %d\n", state);
      for (k = f_kernel [state]; k < f_kernel [state+1]; k++)
      {
         i = kernel[k];
         if (item[i].symb != -32767)
         {
            prt_prod (item[i].prod, item[i].dot, "* ", "\n");
         }  
      }

      for (k = f_kernel [state]; k < f_kernel [state+1]; k++)
      {
         i = kernel[k];
         if (item[i].symb == -32767)
         {
            prt_prod (item[i].prod, item[i].dot, "* ", "\n");
         }  
      }
      PRT_TRAN (state);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGBuildLR1::PRT_TRAN (int s)
{
      int i, h, t, x, n, p;
      if (optn[PG_TRANSITIONS])
      {
			n = 0;
         for (i = 1; i <= Ngotos[s][0]; i++)
         {
            x = Ngotos[s][i];
            if (x > 0)
            {
	            n++;
               h = -accessor[x];
               if (h == 0) P_HEAD (h);
               else        P_SYMB (-h);
               PRINT (" +=> %4d\n", x);
            }
            if (x < 0)
            {
	            n++;
               p = -x;
               h = -Tail[L_tail[p]-1];
               if (h == 0) P_HEAD (h);
               else        P_SYMB (-h);
               PRINT (" +<= %4d\n", p);
            }
         }
			if (n) 
				PRINT ("\n");
			n = 0;
         for (i = 1; i <= Tgotos[s][0]; i++)
         {
            x = Tgotos[s][i];
            if (x > 0)
            {
	            n++;
               t = accessor[x];
               P_SYMB (t);
               PRINT (" +=> %4d\n", x);
            }
            if (x < 0)
            {
	            n++;
               p = -x;
               t = Tail[L_tail[p]-1];
               P_SYMB (t);
               if (x == -32767) x = 0;
               PRINT (" +<= %4d\n", p);
            }
         }
         if (n) PRINT ("\n");
      }  
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGBuildLR1::P_HEAD (int s)
{
      int L;
      PRINT ("        ");
      L = prt_head (s, " ");
      spaces [max_symbl-L+1] = 0;
      PRINT ("%s", spaces);
      spaces [max_symbl-L+1] = ' ';
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGBuildLR1::P_SYMB (int s)
{
      int L;
      PRINT ("        ");
      L = prt_sym (s, " ");
      spaces [max_symbl-L+1] = 0;
      PRINT ("%s", spaces);
      spaces [max_symbl-L+1] = ' ';
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGBuildLR1::prt_sym (int s, char *after)
{
      char *p;
      if (s >= 0) p = term_name[s];
      else        p = head_name[-s];
      PRINT ("%s%s", p, after);
      return (strlen(p) + strlen(after));
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGBuildLR1::prt_head (int s, char *after)
{
      char *p;
      p = head_name[-s];
      PRINT ("%s%s", p, after);
      return (strlen(p) + strlen(after));
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGBuildLR1::MTSL () // Make Transition Symbols Lists. 
{
      int i, state, n;

      ALLOC (tt_symb,    n_ttran);
      ALLOC (ntt_symb,   n_nttran);
      ALLOC (ntt_origin, n_nttran);

      for (i = 0; i < n_ttran; i++)
      {
         if (tt_action[i] > 0)
            tt_symb[i] = accessor [tt_action [i]];
         else
            tt_symb[i] = Tail[F_tail[-tt_action[i]+1]-1];
      }

      for (i = 0; i < n_nttran; i++) 
      {
         if (ntt_action[i] > 0)
            ntt_symb[i] = -accessor [ntt_action [i]];
         else
            ntt_symb[i] = -Tail[F_tail[-ntt_action[i]+1]-1];
      }

      n = 0;
      for (state = 0; state < n_states; state++)
      {
         for (i = ntt_start[state]; i < ntt_start[state+1]; i++)
         {
            ntt_origin[n++] = state;
         }
      }
}

//                                                                           //
////////////////////////////////////////////////////////////////////////////////

#endif