
#if 0 // Not used anymore, since moving to LR(1). 

      #include "ComGlobal.h"
		#include "PGGlobal.h"

      static int       max_final;
      static int       max_kernel;
      static int       max_ttran;
      static int       max_nttran;
      static int       head_free;
      static uint      max_hashes;
      static int*      h_vector;
      static uint      hash_div;
      static uint      hash_no;
      static int*      already;
      static int*      added;
      static int       term_free;
		static int		  r_states;
      static CLOSURE*  closure;
      static int       max_states;
      static int       n_clo;
      static int       n_symbs;
      static int*      reduce_state;
      static SYMLIST*  symlist;

		static int	     sqrt (int n);
		static void      EXPAND (int state);
		static void      DO_CLOSURE (int state);
		static void      ADD_ITEM (int i, int state);
		static void      MAKE_KERNEL (int c);
		static int       TRANSIT (int first, int symbol);
		static void      MAKE_STATE (int prod);
		static void      MODIFY (int state);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //

int   PGBuildLR0 () /* Build LR0 States */
{
      int p, state;

		max_states  = optn[MAX_STA];
      max_final   = optn[MAX_FIN];
      max_kernel  = optn[MAX_KER];
      max_ttran   = optn[MAX_TT] ;
      max_nttran  = optn[MAX_NTT];
		max_child   = optn[MAX_CH];

      max_hashes  = 2*max_states + 1;
      hash_div    = (uint)0xFFFFFFFF / max_hashes + 1;
      n_symbs     = N_heads + N_terms;

      ALLOC (accessor,   max_states);
      ALLOC (f_final,    max_states+2);
      ALLOC (f_kernel,   max_states+2);
      ALLOC (tt_start,   max_states+1);
      ALLOC (ntt_start,  max_states+1);

      ALLOC (final,      max_final);
      ALLOC (kernel,     max_kernel);
      ALLOC (tt_action,  max_ttran);
      ALLOC (ntt_action, max_nttran);
      ALLOC (ntt_item,   max_nttran);

      ALLOC (added,        N_heads);
      ALLOC (h_vector,     max_hashes);
      ALLOC (symlist,        n_symbs+1);
      ALLOC (closure,      2*N_prods+1);
      ALLOC (already,      n_symbs+1);
      ALLOC (reduce_state, N_prods);

      FASTINI ( 0, reduce_state, N_prods   );
      FASTINI (-1, added,        N_heads   );
      FASTINI (-1, h_vector,     max_hashes);

      n_finals     = 0;
      n_kernels    = 0;
      f_final [0]  = 0;
      f_kernel [0] = 0;
      accessor [0] = 0;

		C_ITEMS (item, f_item, N_heads, F_prod, F_tail, Tail);
      C_FIRST (N_heads, N_terms, N_prods, F_prod, F_tail, Tail, FIRST);

      for (p = F_prod [0]; p < F_prod [1]; p++)
      {
         kernel [n_kernels++] = f_item [p];
      }

      f_kernel[1] = n_kernels;
      added[0]    = 0;
      n_states    = 1;
      n_ttran     = 0;
      n_nttran    = 0;

   // Create non-reduce-only states.
      for (state = 0; state < n_states; state++)
      {
         EXPAND (state);
      }
      tt_start  [state] = n_ttran;
      ntt_start [state] = n_nttran;
	// opt_states (optimum states) = n_states so far generated. 
	// Not to inlcude final state and all reduce-only states.
      opt_states = n_states;   

   // Create reduce-only states.
      state = n_states;
      for (p = 0; p < N_prods; p++)
      {
      // If production is marked for reduce-state creation ...
      // (not all productions need to have a corresponding reduce state).
         if (reduce_state[p] != 0) 
         {
            reduce_state[p] = state++;
            MAKE_STATE (p);
         }
      }

   // Modify negative transitions.
      for (state = 0; state < n_states; state++)
      {
         MODIFY (state);
      }
		org_states = n_states;

		char* Grammar;
		Grammar = "LALR(1) ";

		int ro_states = n_states - (opt_states+1); // +1 for final state (goal reduction).
		prt_log ("%s %7d states in LALR(1) state machine.\n",  
		Grammar, n_states);

      if (optn[SHIFTREDUCE])
		     prt_log ("         %7d states after implementing shift-reduce actions.\n", n_states - ro_states);
      else prt_log ("         %7d states removed for shift-reduce actions.\n", 0);

      FREE (already,  n_symbs+1);
      FREE (closure,  2*N_prods+1);
      FREE (symlist,    n_symbs+1);
      FREE (h_vector, max_hashes);
      FREE (added,    N_heads);

      REALLOC (ntt_item,   max_nttran,   n_nttran);	
      REALLOC (ntt_action, max_nttran,   n_nttran);	
      REALLOC (tt_action,  max_ttran,    n_ttran); 
      REALLOC (kernel,     max_kernel,   n_kernels );
      REALLOC (final,      max_final,    n_finals  );
      REALLOC (ntt_start,  max_states+1, n_states+1);
      REALLOC (tt_start,   max_states+1, n_states+1);
      REALLOC (f_kernel,   max_states+2, n_states+1);
      REALLOC (f_final,    max_states+2, n_states+1);
      REALLOC (accessor,   max_states,   n_states  );
      return (1);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  EXPAND (int state)
{
      int i, n, w;
      DO_CLOSURE (state);
      tt_start  [state] = n_ttran;
      ntt_start [state] = n_nttran;
      if (n_clo)
      {
         for (i = N_heads; i < term_free; i++)
         {
            n = n_kernels;
            MAKE_KERNEL (symlist[i].start);
         // printf ("symbol = %d\n", symlist[i].symb);
            w = TRANSIT (n, symlist[i].symb);
            tt_action [n_ttran++] = w;
            if (n_ttran >= max_ttran) MemCrash ("Number of terminal transitions", max_ttran);
         }
         for (i = 0; i < head_free; i++)
         {
            n = n_kernels;
            MAKE_KERNEL (symlist[i].start);
            ntt_item [n_nttran] = closure[symlist[i].start].item;
            w = TRANSIT (n, (int)-symlist[i].symb);
            ntt_action [n_nttran++] = w;
            if (n_nttran >= max_nttran) MemCrash ("Number of nonterminal transitions", max_nttran);
         }  
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  MODIFY (int state) // Modify negative transitions, change to goto state.
{
      int i;
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

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  DO_CLOSURE (int state)
{
      int k;
      n_clo = 0;
      head_free = 0; 
      term_free = N_heads;
      FASTINI (-1, already, n_symbs+1);
      for (k = f_kernel [state]; k < f_kernel [state+1]; k++)
      {
         ADD_ITEM (kernel [k], state);
      }
      f_final [state+1] = n_finals;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  ADD_ITEM (int i, int state)
{
      int symbol, symb_i, symb_free, p;
      if ((symbol = item[i].symb) != -32767) /* IF NOT FINAL */
      {
         closure [n_clo].item = i;
         closure [n_clo].next = -1;
         if (already [symbol+N_heads] == -1) /* New symbol. */
         {
            if (symbol < 0)
            {
               symlist [symb_free = head_free++].symb = -symbol;
               symlist [symb_free].start = n_clo;
               symlist [symb_free].end = n_clo;
               already [symbol+N_heads] = symb_free;
               n_clo++;
               if (added [-symbol] != state)
               {
                  for (p = F_prod [-symbol]; p < F_prod [-symbol+1]; p++)
                  {
                     ADD_ITEM (f_item [p], state);
                  }
                  added [-symbol] = state;
               }  
            }
            else
            {
               symlist [symb_free = term_free++].symb = symbol;
               symlist [symb_free].start = n_clo;
               symlist [symb_free].end = n_clo;
               already [symbol+N_heads] = symb_free;
               n_clo++;
            }  
         }
         else
         {
            symb_i = already [symbol+N_heads];
            closure [symlist [symb_i].end].next = n_clo;
            symlist [symb_i].end = n_clo;
            n_clo++;
         }
      }
      else
      {
         if (n_finals >= max_final) MemCrash ("Number of final items", max_final);
         final [n_finals++] = i;
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  MAKE_KERNEL (int c)
{
      int i;
      hash_no = 0;
      do
      {
         i = closure[c].item + 1;
         if (n_kernels >= max_kernel) MemCrash ("Number of kernel items", max_kernel);
         hash_no += kernel[n_kernels++] = i;
      }
      while ((c = closure[c].next) != -1);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   TRANSIT (int ki, int sym)
{
      int state, k1, ni;
      ni  = n_kernels - ki;                           // Get number of items.
      if (ni == 1)                                    // Number of items is 1?
      {
         if (item[kernel[ki]].symb == -32767)         // Final item?
         {
            int p;
            n_kernels = ki;                           // Reset this.
            p = item [kernel[ki]].prod;               // Get production.
            reduce_state[p] = 1;                      // Mark production for reduce-state creation.   
            return (-p);                              // Return production number.
         }
      }
      else 
      {
         SORT (kernel + ki, kernel + n_kernels);
      }

		uint probe;
		probe = hash_no % max_hashes;
		while ((state = h_vector[probe]) != -1)
		{
			k1 = f_kernel[state];
			if (f_kernel[state+1] - k1 == ni)
			{
				if (FASTCMP (kernel + k1, kernel + ki, ni))
				{
					n_kernels = ki;                     // Reset this.
					return (state);
				}  
			}  
			probe = (hash_no *= 65549) / hash_div;
		}
		h_vector [probe] = n_states;

      accessor  [n_states] = sym;
      f_kernel  [n_states+1] = n_kernels;
      if (n_states >= max_states) MemCrash ("Number of states", max_states);
	//	if (((n_states+1) %  100) == 0) printf ("%4d ", n_states+1);
	// if (((n_states+1) % 1600) == 0) printf ("\n");
      return (n_states++);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  MAKE_STATE (int prod)
{
      int state;
      state = reduce_state[prod]; 
      accessor[state]   = Tail [F_tail [prod+1]-1];
      final [n_finals]  = f_item[prod] + prod_len[prod];
      kernel[n_kernels] = f_item[prod] + prod_len[prod];

      if (++n_finals  >= max_final ) MemCrash ("Number of final items", max_final);
      if (++n_kernels >= max_kernel) MemCrash ("Number of kernel items", max_kernel);
      if (++n_states  >= max_states) MemCrash ("Number of states", max_states);

      tt_start [n_states] = n_ttran;
      ntt_start[n_states] = n_nttran;
      f_final  [n_states] = n_finals;
      f_kernel [n_states] = n_kernels;
}

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif