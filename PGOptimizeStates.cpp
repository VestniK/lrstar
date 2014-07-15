
#ifdef LRSTAR

      #include "ComGlobal.h"
		#include "PGOptimizeStates.h"

		int   *pr_type;
		int   *hs_type;
		int   *old_state;
		int   *new_state;
      int   *reductions;

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
      
int   PGOptimizeStates::OptimizeStates ()
{
      accept_state = FIND_ACCEPT_STATE ();

      MAKE_SHIFT_REDUCE_ACTIONS ();
      ELIMINATE_CHAIN_REDUCTIONS (); 

		if (optn[PG_SHIFTREDUCE])
		{
			n_states   = opt_states+1;
			tt_states  = opt_states+1;
			ntt_states = opt_states+1;
		}
		else
		{
			tt_states  = n_states;
			ntt_states = n_states;
		}
		return (1);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//		Find accept state (need this for PG_CLR_PARSER, works for the others too) //
//		This is a generalized version. Could have searched state 1 for <eof>. 
      
int   PGOptimizeStates::FIND_ACCEPT_STATE ()
{
		int ntt, tt;
		int state = 0;
		int symb  = Tail [F_tail [F_prod[0]]];
		if (symb < 0) // Nonterminal symbol?
		{
			for (ntt = ntt_start[state]; ntt < ntt_start[state+1]; ntt++)
			{
				if (ntt_symb[ntt] == -symb) 
				{
					state = ntt_action[ntt];
					break;
				}
			}
		}
		else // Terminal symbol!
		{
			for (tt = tt_start[state]; tt < tt_start[state+1]; tt++)
			{
				if (tt_symb[tt] == symb) 
				{
					state = tt_action[tt];
					break;
				}
			}
		}
		symb = Tail [F_tail [F_prod[0]] + 1]; // <eof> symbol
		for (tt = tt_start[state]; tt < tt_start[state+1]; tt++)
		{
			if (tt_symb[tt] == symb) 
			{
				state = tt_action[tt];
				break;
			}
		}
		return state;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
      
void  PGOptimizeStates::MAKE_SHIFT_REDUCE_ACTIONS ()
{
      int state, i;
      if (optn[PG_SHIFTREDUCE]) // Shift-reduce actions wanted?
      {
         for (state = 0; state < opt_states; state++)
         {
            for (i = tt_start [state]; i < tt_start [state+1]; i++)
            {
               if (tt_action[i] > opt_states)
               {
                  tt_action[i] = -D_red [tt_action[i]];
               }
            }
            for (i = ntt_start [state]; i < ntt_start [state+1]; i++)
            {
               if (ntt_action[i] >= opt_states)
               {
                  ntt_action[i] = -D_red [ntt_action[i]];
               }
            }
			   if (optn[PG_NONDETER])
			   {
				   for (i = nd_start[state]; i < nd_start[state+1]; i++)
				   {
					   if (nd_action[i] >= opt_states)
						    nd_action[i]  = opt_states + D_red [nd_action[i]];
				   }
			   }
         }
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
      
void  PGOptimizeStates::ELIMINATE_CHAIN_REDUCTIONS () // Eliminate Chain Shift-Reduce Actions.   
{
      int state, i, h, p, j, n_changed, total = 0;
      if (optn [PG_OPTIMIZE])
		{
			for (state = 0; state < opt_states; state++)
			{
Loop:       n_changed = 0;
				for (i = ntt_start[state]; i < ntt_start[state+1]; i++)
				{
					p = -ntt_action[i];
					if (p > 0 && prod_len[p] == 1 && Pact_numb[p] == -1 && Node_numb[p] == -1)
					{
						h = head_sym[p];
						for (j = ntt_start[state]; j < ntt_start[state+1]; j++)
						{
							if (ntt_symb[j] == h) break;
						}
						ntt_action[i] = ntt_action [j];
						n_changed++;
                  total++;
					}  
				}
				if (n_changed > 0) goto Loop;

				for (i = tt_start[state]; i < tt_start[state+1]; i++)
				{
					p = -tt_action[i];
					if (p > 0 && prod_len[p] == 1 && Pact_numb[p] == -1 && Node_numb[p] == -1)
					{
						h = head_sym[p];
						for (j = ntt_start[state]; j < ntt_start[state+1]; j++)
						{
							if (ntt_symb[j] == h) break;
						}
						tt_action[i] = ntt_action[j];
					}
				}  
			}
         if (optn[PG_VERBOSE] > 1)
         prt_log ("        %8d nonterminal transitions were removed by optimization.\n", total);
		}
}

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
      
#endif
