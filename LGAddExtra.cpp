
#ifdef DFASTAR

		#include "ComGlobal.h"
		#include "LGGlobal.h"
		#include "LGActions.h"
		#include "LGAddExtra.h"

		static int    nreds;
		static int    level;
		static int    max_level;
		static int    goto_top;
		static int    trace_top;
		static int    n_symbols;
		static int    n_saved;
		static int    max_saved;
		static int    n_traces;
		static int    n_fatals;
		static int    alt_goto;

		static int*	  recur;
		static int*   saved_prod;
		static int*   saved_state;
		static int*   jumpto;
		static int*   goto_stack;
		static int*   trace_stack;
		static int*   symbol;
		static int*   onstack;
		static int*   incycle;
		static int*   traceable;
		static char** TRANS;

		static int*   ntt_gotox;
		static int*   ntt_symbx;
		static int*   ntt_startx;
		static int*   ntt_endx;
		static int    n_nttgotox;

		static int*   finalx;
		static int*   f_finalx;
		static int*   l_finalx;
		static int    n_finalx;


///////////////////////////////////////////////////////////////////////////////
//
//		Add Extra Productions to states to solve keyword-identifier problem.

int 	LGAddExtra::AddExtra()
{
      int rc;
		rc = 1;
		n_added = 0;
		n_traces = 0;

      ALLOC (recur,      n_heads);
      ALLOC (head_level, n_heads);
      ALLOC (possibil,   n_heads);

      FASTINI (0, head_level, n_heads);
		FASTINI (0, possibil,   n_heads);

      DEFINE_RECUR ();
		DEFINE_POSS ();

      if (!optn[LG_KEYWORDIDENT]) goto Ret;
		if (n_genliterals == n_original_prods) goto Ret;

      ALLOC (incycle,    n_states); 
      ALLOC (traceable,  n_states); 
      ALLOC (jumpto,     n_states); 

		FASTINI (0, incycle,    n_states);
		FASTINI (1, traceable,  n_states);

		DETECT_CYCLES ();

      REALLOC (head_sym,   n_prods, max_prods);
      REALLOC (head_name,  n_heads, max_symbs);
      REALLOC (head_level, n_heads, max_symbs);
      REALLOC (possibil,   n_heads, max_symbs);
      REALLOC (prod_len,   n_prods, max_prods);
      REALLOC (ret_numb,   n_prods, max_prods);
      REALLOC (ret_name,   n_prods, max_prods);
      REALLOC (f_tail,     n_prods, max_prods);
      REALLOC (l_tail,     n_prods, max_prods);
      REALLOC (tail,       n_tails, max_tails);

      ALLOC (trace_stack,  256); 
      ALLOC (goto_stack,   256); 
      ALLOC (symbol,       256); 

      ALLOC (saved_state,  256); 
      ALLOC (saved_prod,   256); 

      ALLOC (finalx,      	max_final);
      ALLOC (f_finalx,    	n_states);
      ALLOC (l_finalx,    	n_states);

      ALLOC (ntt_symbx,   	max_ntt);
      ALLOC (ntt_gotox,   	max_ntt);
      ALLOC (ntt_startx,  	n_states);
      ALLOC (ntt_endx,    	n_states);

		FASTINI (0, f_finalx,   n_states);
		FASTINI (0, l_finalx,   n_states);
		FASTINI (0, ntt_startx, n_states);
		FASTINI (0, ntt_endx,   n_states);

		nreds     = 0;
		n_saved   = 0;
		max_saved = 0;
      n_finalx  = 0;
		n_symbols = 0;
		trace_top = 0;
		goto_top  = 0;

      GOTO (goto_stack [goto_top++] = 0, 0); 

      FREE (goto_stack,  256);
      FREE (trace_stack, 256);
      FREE (symbol,      256);
      FREE (recur,       n_heads);
      FREE (incycle,     n_states);
      FREE (traceable,   n_states);

      REALLOC (head_name,  max_symbs, n_heads);
      REALLOC (head_level, max_symbs, n_heads);
      REALLOC (possibil,   max_symbs, n_heads);
      REALLOC (head_sym,   max_prods, n_prods);
      REALLOC (prod_len,   max_prods, n_prods);
      REALLOC (ret_numb,   max_prods, n_prods);
      REALLOC (ret_name,   max_prods, n_prods);
      REALLOC (f_tail,     max_prods, n_prods);
      REALLOC (l_tail,     max_prods, n_prods);
      REALLOC (tail,       max_tails, n_tails);

		if (n_prods > max_n_prods) max_n_prods = n_prods;

      rc = MERGE_FINALS();
		MERGE_NTTRANS ();

Ret:  if (optn[LG_VERBOSE])
		prt_log ("         %7d reductions added during %d traces.\n", n_added, n_traces);
    	return (rc);
}

void	LGAddExtra::GOTO (int s, int agoto)
{
		int t, x, f, p, h, r, saved_state;
		if (!incycle[s])
		{
		//	printf ("\n--> GOTO (%d)\n", s);
		// p_all();
			for (t = tt_start[s]; t < tt_start[s+1]; t++) 
			{
				x = tt_action[t];
				if (traceable[x])
				{
					goto_stack [goto_top++] = x;
					symbol [n_symbols] = tt_symb[t];
					f_finalx[x] = n_finalx;
					alt_goto = 0;
			  	//	if (agoto == 0)
			  		if (1)
					{
						trace_top = 0;
					  	TRACE (x, 0, 0, ++n_symbols);
					}
			  		else 
					{
						trace_top = n_symbols;
						TRACE (x, agoto, n_symbols, ++n_symbols);  
					}	  
					traceable[x] = 0;
					l_finalx[x] = n_finalx;
					GOTO (x, alt_goto);
					n_symbols--;
					goto_top--;
				}
			}
			for (f = f_final[s]; f < f_final[s+1]; f++) 
			{
				p = final[f];
			//	p_prod(p);
				if (ret_numb[p] == 0) // Not Token -> 
				{ 
					h = head_sym[p];
					goto_top -= prod_len[p] + 1;
					r = goto_stack [goto_top++];
					for (t = ntt_start[r]; t < ntt_start[r+1]; t++)
					{
						if (ntt_symb[t] == h) break;
					}
					x = ntt_action[t];
					if (traceable[x])
					{
						saved_state = goto_stack [goto_top];
						goto_stack [goto_top++] = x;
						GOTO (x, agoto);
						goto_stack [--goto_top] = saved_state;
					}
					goto_top += prod_len [p];					 
				}
			}
			// printf ("<-- State %d\n", s);
		}
}

void 	LGAddExtra::TRACE (int x, int y, int d, int dot)
{
      int t, f, p, z, sym;
      if (d < dot)												 
      {
			n_traces++;
		  	trace_stack [trace_top++] = y;
		//	printf ("\n--> TRACE (%d, %d, %d, %d)\n", x, y, d, dot);
 		//	p_all();
			sym = symbol[d]; // Has to be symbol[d] a terminal, to traverse all paths. 
			for (t = tt_start[y]; t < tt_start[y+1]; t++)
			{
		  		if (tt_symb[t] == sym) 
				{
					TRACE (x, tt_action[t], d+1, dot);
					break;
				}
			}
         for (f = f_final[y]; f < f_final[y+1]; f++) 
         {
            p = final[f];
			//	p_prod(p);
            if (ret_numb[p] == 0) // Not Token -> 
            { 
					z = REDUCE_GOTO (p);
					TRACE (x, z, d, dot); // d, not d+1 here.
					UNDO_REDUCE ();
            }
         }	  
     		trace_top--;
      }
		else 
		{
			TRACE_REDUCE (x, y);
		}
}

void	LGAddExtra::TRACE_REDUCE (int x, int y) 
{
		int p, z;
		trace_stack[trace_top++] = y;
   //	printf ("\n--> TRACE_REDUCE (%d, %d)\n", x, y);
   //	p_all();
	   if (x != y)
		{	
			if (st_type[y] == RO_STATE)
			{
				p = final [f_final [y]];
			//	p_prod(p);
				if (head_level [head_sym [p]] >= 2) // Not <identifier> -> letter or ?
				{
					z = REDUCE_GOTO (p);
					TRACE_REDUCE (x, z);												
					UNDO_REDUCE ();
				}
				else // or (NO_REDUCE_GOTO (x, y))
				{
					ADDING (x, y);
				}
			}
			else // or (NO_REDUCE_GOTO (x, y))
			{
				ADDING (x, y);
			}
	  	}
		else
		{
		//	printf ("%d == %d, rejected\n", x, y);
		}    
		trace_top--;
}

void	LGAddExtra::ADDING (int x, int y) 
{
		char name[16];
		int len, b, i, h;
	//	printf ("\n--> ADDING (%d, %d)\n", x, y);
	//	p_all();
		n_added++;
		alt_goto = y;
		b = trace_stack[0];	// Get origin state.
		for (i = ntt_startx[b]; i < ntt_endx[b]; i++)
		{
			if (ntt_gotox[i] == y) // Already added NTT for y?
			{
				h = ntt_symbx[i];
				break;
			}
		}
		if (i == ntt_endx[b]) // Did not find head symbol?
		{
			h = n_heads++;									// Create new head number.
			sprintf (name, "NT%d-%d", b, y);			// Create new head name.
			len = strlen (name) + 1;					// Get length + 1 for '\0'
			head_name [h] = LGActions::ADDSYMBOL (name,len);	// Save name in symbol space.
			head_level [h] = MAX_INT;	
			possibil   [h] = 1;	
			ntt_symbx  [n_nttgotox] = h; 
			ntt_gotox  [n_nttgotox++] = y; 
			ntt_endx[b] = n_nttgotox;
		}
	// printf ("\nTO STATE %d, ADDING: %d %s ->", x, n_prods, head_name[h]);
		f_tail [n_prods] = n_tails;
		for (i = 1; i < goto_top; i++)			
		{
			if (n_tails > max_tails) 
				MemCrash ("Number of tail symbols", max_tails);
			tail [n_tails++] = accessor [goto_stack [i]];
		// p_sym (tail [n_tails-1]);
		}
	// printf ("\n\n");
		l_tail   [n_prods] = n_tails;
		head_sym [n_prods] = h;	
		prod_len [n_prods] = goto_top - 1;	
		ret_numb [n_prods] = 0;
		finalx[n_finalx++] = n_prods++; 
}

int   LGAddExtra::REDUCE_GOTO (int p) 
{
		int x, h, t;
 		trace_top -= prod_len[p]+1;
		x = trace_stack[trace_top++];
		h = head_sym[p];
		saved_prod [n_saved] = p;
		saved_state[n_saved++] = trace_stack[trace_top];
	//	if (n_saved > max_saved) max_saved = n_saved;
      for (t = ntt_start[x]; t < ntt_start[x+1]; t++)
      {
         if (ntt_symb[t] == h)		
         {
            return (ntt_action[t]);	
         }
      }
		return (0);
}

void	LGAddExtra::UNDO_REDUCE () 
{
		n_saved--;
		trace_stack[trace_top++] = saved_state [n_saved];
		trace_top += prod_len [saved_prod [n_saved]]-1;
}

int   LGAddExtra::NO_MATCHING_REDUCE (int x) 
{
		int f, p, d, t;
		for (f = f_final[x]; f < f_final[x+1]; f++)
		{
			p = final[f];
			p_prod(p);
			if (head_level [head_sym [p]] > 2)
			{
				if (prod_len[p] == goto_top-1)
				{
					d = 1;
					for (t = f_tail[p]; t < l_tail[p]; t++)
					{
						if (tail[t] != accessor [goto_stack [d++]]) goto Next;
					}
					return 0;
				}
			}		
Next:		continue;
		}
		return 1;
}

int	LGAddExtra::NO_REDUCE_GOTO (int x, int newx)
{
      int f, p, nj;
      for (f = f_final[x]; f < f_final[x+1]; f++) 
      {
         p = final[f];
         if (ret_numb[p] == 0) // Not Token -> ?
         { 
				nj = 0;
            LOOK_BACK (p, prod_len[p], x, nj, jumpto);
				for (int j = 0; j < nj; j++)
				{	
					if (jumpto[j] == newx) return 0;
				}
         }
      }
      return (1);
}

void  LGAddExtra::p_all ()
{
		p_symbols();
		p_goto_stack();
		p_trace_stack();
}

void  LGAddExtra::p_symbols ()
{
		int i;
		printf ("Symbols =");
		if (n_symbols == 0) printf (" [empty]");
		for (i = 0; i < n_symbols; i++) 
		{
			p_sym (symbol[i]); 
		}
		printf ("\n");
}

void  LGAddExtra::p_goto_stack ()
{
		int i, sym;
		char* symstr;
		printf ("Goto stack  =");
		if (goto_top == 0) printf (" [empty]");
		for (i = 0; i < goto_top; i++) 
		{
			sym = accessor [goto_stack[i]];
			if (sym < 0) symstr = head_name [-sym];
			else			 symstr = term_name [sym];
			printf (" [%d %s]", goto_stack[i], symstr); 
		}
		printf ("\n");
}

void  LGAddExtra::p_trace_stack ()
{
		int i, sym;
		char* symstr;
  		printf ("Trace stack =");
		if (trace_top == 0) printf (" [empty]");
		for (i = 0; i < trace_top; i++) 
		{
			sym = accessor [trace_stack[i]];
			if (sym < 0) symstr = head_name [-sym];
			else			 symstr = term_name [sym];
			printf (" [%d %s]", trace_stack[i], symstr); 
		}
		printf ("\n");
}

int 	LGAddExtra::MERGE_FINALS ()
{
		int s, f, rc, ff, nf;
		rc = 1;
      n_finals = 0;
      ALLOC (final2,  max_final);
      for (s = 0; s < n_states; s++) 
      {
			nf = 0;
         ff = n_finals;	// Save this.
         for (f = f_final[s]; f < f_final[s+1]; f++)
         {
				nf++;
				if (n_finals >= max_final) MemCrash ("Number of final items", max_final);
            final2[n_finals++] = final[f];
         }
			for (f = f_finalx[s]; f < l_finalx[s]; f++)
			{
				nf++;
				if (n_finals >= max_final) MemCrash ("Number of final items", max_final);
				final2[n_finals++] = finalx[f];	
			}
		/*	if ((l_finalx[s] - f_finalx[s]) > 1) 
   		Not all multiple extra productions are ambiguous, so we can't do this.
			{
				rc = 0;
				PRT_STA (s);
				prt_con ("\nFatal ambiguity in this state!\n\n");
			}	*/
			if (nf > 1) st_type[s] = MR_STATE;
         f_final[s] = ff;  // Now we can move it to f_final.
      }
      f_final[s] = n_finals;
      FREE    (final,    n_finals);
      FREE    (finalx,   n_finalx);
      FREE    (f_finalx, n_states);
      FREE    (l_finalx, n_states);
      REALLOC (final2,   max_final, n_finals);
      final =  final2;
		return (rc);
}

void 	LGAddExtra::MERGE_NTTRANS ()
{
		int s, i, size;
		size = n_nonttran;
      n_nonttran = 0;
      ALLOC (ntt_symb2,  max_ntt);
      ALLOC (ntt_goto2,  max_ntt);
      for (s = 0; s < n_states; s++) 
      {
         i = ntt_start[s];
         ntt_start[s] = n_nonttran;
         for (; i < ntt_start[s+1]; i++)
         {
            ntt_symb2[n_nonttran]   = ntt_symb[i];
            ntt_goto2[n_nonttran++] = ntt_action[i];
         }
			for (i = ntt_startx[s]; i < ntt_endx[s]; i++)
			{
				ntt_symb2[n_nonttran]   = ntt_symbx[i];	
				ntt_goto2[n_nonttran++] = ntt_gotox[i];	
			}
      }
      ntt_start[s] = n_nonttran;
      FREE    (ntt_symb,  size);
      FREE    (ntt_action,  size);
      FREE    (ntt_symbx, max_ntt);
      FREE    (ntt_gotox, max_ntt);
      FREE    (ntt_startx,n_states);
      FREE    (ntt_endx,  n_states);
      REALLOC (ntt_symb2, max_ntt, n_nonttran);
      REALLOC (ntt_goto2, max_ntt, n_nonttran);
      ntt_symb = ntt_symb2;
      ntt_action = ntt_goto2;
}

void  LGAddExtra::DETECT_CYCLES ()
{
		int s, t, f, x, sym, p, nj;

		#ifdef USINGBITS
      n_words = (n_states + 31)/32; // Number of 32-bit words. 
		#else
      n_words = (n_states +  3)/4;	// Number of 4-byte words. 
		#endif
      n_bytes = 4*n_words;				// Number of bytes to allocate. 
      ALLOC (TRANS, n_states);          
      for (s = 0; s < n_states; s++)
      {
         ALLOC  (TRANS[s], n_bytes);    
         memset (TRANS[s], 0, n_bytes);
      }

		n_childs  = 0;
      ALLOC   (f_child, n_states);
      ALLOC   (child,  max_child);
		FASTINI (-1, f_child, n_states);

		for (s = 0; s < n_states; s++)
		{
			for (t = tt_start[s]; t < tt_start[s+1]; t++) 
			{
				x   = tt_action[t];			
				sym = tt_symb[t];
			//	printf ("T-transition: [%2d,%s] -> %2d\n", s, term_name[sym], x);
			 	ATTACH (s, x);
				#ifdef USINGBITS
				setbit2 (TRANS, s, x);
				#else
				TRANS[s][x] = 1;
				#endif
			}
			for (t = ntt_start[s]; t < ntt_start[s+1]; t++) 
			{
				x   = ntt_action[t];			
				sym = ntt_symb[t];
			//	printf ("N-transition: [%2d,%s] -> %2d\n", s, head_name[sym], x);
			 	ATTACH (s, x);
				#ifdef USINGBITS
				setbit2 (TRANS, s, x);
				#else
				TRANS[s][x] = 1;
				#endif
			}
			for (f = f_final[s]; f < f_final[s+1]; f++) 
			{
				nj = 0;
				p = final[f];
				if (head_level[head_sym[p]] <= 1) continue;
				LOOK_BACK (p, prod_len[p], s, nj, jumpto);
				for (int j = 0; j < nj; j++)
				{
		 			ATTACH (s, jumpto[j]);
					#ifdef USINGBITS
					setbit2 (TRANS, s, jumpto[j]);
					#else
					TRANS[s][jumpto[j]] = 1;
					#endif
				//	printf ("Reduction:  [%2d,%3d] -> %2d\n", s, p, jumpto[j]);
				}
			}																 
		}

	// P_GRAPH (TRANS, n_states, n_states);
		T_GRAPH (TRANS, n_states, n_states);
	//	P_GRAPH (TRANS, n_states, n_states);

	  	DEF_CYCLES ();

      for (s = 0; s < n_states; s++) FREE (TRANS[s], n_bytes);    
      FREE (TRANS,   n_states);          
      FREE (child,   max_child);
      FREE (f_child, n_states);
}

void  LGAddExtra::LOOK_BACK (int p, int dot, int x, int& nj, int* jumpto)
{
      if (dot-- > 0)
      {
         for (int i = f_camefrom[x]; i < f_camefrom[x+1]; i++)
         {
            LOOK_BACK (p, dot, camefrom[i], nj, jumpto);
         }
      }
		else
		{
			int h = head_sym[p];
         for (int i = ntt_start[x]; i < ntt_start[x+1]; i++)
         {
            if (ntt_symb[i] == h) 
            {
               jumpto[nj++] = ntt_action[i];		
		      // printf ("Jumps to %d\n", jumpto[nj-1]);
					break;
            }
         }
		}
}

void  LGAddExtra::DEF_CYCLES ()
{
		int n = 0, s, i, f, p, sum;
  	// printf ("\n*****************************************\n\n");
		for (s = 0; s < n_states; s++)
		{
			#ifdef USINGBITS
         ifbit2(TRANS, s, s) // In cycle?
			#else
         if (TRANS[s][s])    // In cycle?
			#endif
			{
			//	n++;
				incycle[s] = 1; 
			//	printf ("trace_stack %2d\n", s);
			}
		}
		for (s = 0; s < n_states; s++)
		{
			if (traceable[s])
			{
				int scan_row = 1;
				for (f = f_final[s]; f < f_final[s+1]; f++)
				{
					p = final[f];
					if (head_level [head_sym[p]] > 2) 
					{
						scan_row = 0;
						break;
					}
				}
				if (scan_row)
				{
     				sum = 0;
					for (i = 0; i < n_states; i++) // Check this row.
					{
						#ifdef USINGBITS
						ifbit2(TRANS, s, i) sum++;
						#else
						if (TRANS[s][i]) sum++;
						#endif
					}
					if (sum == 0) // terminal state?
					{
						traceable[s] = 0;
					}
				}
			}
		}
  	/* printf ("In cycle states:\n");
		for (s = 0; s < n_states; s++)
		{
         if (incycle[s])
			{
  		  	// printf ("In cycle, state %2d\n", s);
			}
		}	*/
  	/* printf ("Non-traceable States:\n");
		for (s = 0; s < n_states; s++)
		{
         if (!traceable[s])
			{
			  	n++;
		  	// printf ("Not traceable, state %2d\n", s);
			}
		}
	  	if (n == 0) 
		{
		// printf ("None!\n");
		}  */
  	// printf ("\n*****************************************\n");
}

///////////////////////////////////////////////////////////////////////////////
//
//		Define recusion (1|0) & count number of productions for each head symbol. 

void	LGAddExtra::DEFINE_RECUR ()
{
		level = -1;
		max_level = 1;
      ALLOC (onstack, n_heads);
      FASTINI (0, onstack, n_heads);

      RECURSIVE (0); 

      FREE (onstack, n_heads);
}

int 	LGAddExtra::RECURSIVE (int h)
{
      int p, t, sym;
      if (!onstack[h])
      {
			level++;
			if (level > max_level) max_level = level;
         recur[h] = 0;
         onstack[h] = 1;
			if (head_level[h] ==    0) head_level[h] = level;
			else if (head_level[h] > level) head_level[h] = level;
         for (p = f_prod[h]; p < l_prod[h]; p++)
         {
         //	p_prod(p, -1);
            sym = -tail[f_tail[p]];	// Get first tail in prod.
            if (sym == h)				// Same as head symbol we're processing?
            {
               recur[h] = 1;			// Mark it recursive.
            //	printf ("recur[%15s] = %d\n", head_name[h], recur[h]);
            }
            for (t = f_tail[p]; t < l_tail[p]; t++) // Check rest of symbols in prod.
            {
               if ((sym = -tail[t]) > 0)
               {
                  recur[h] |= RECURSIVE (sym); 
               }
            }
         }
      //	printf ("recur      [%15s] = %d\n", head_name[h], recur[h]);
      //	printf ("head_level [%15s] = %d\n", head_name[h], head_level[h]);

			level--;
         onstack[h] = 0;
         return (recur[h]);
      }
      return (0);
}
  
///////////////////////////////////////////////////////////////////////////////
//
//		Define number of possibilities for each head symbol (letter = 52). 

void	LGAddExtra::DEFINE_POSS ()
{
      ALLOC (onstack, n_heads);
      FASTINI (0, onstack, n_heads);

      POSSIBIL (0); 

	/*	for (int h = 0; h < n_heads; h++)
		{
       	printf ("possibil [%15s] = %d\n", head_name[h], possibil[h]);
		}	*/
      FREE (onstack, n_heads);
}

int 	LGAddExtra::POSSIBIL (int h)
{
      int p, t, x, combinations, sym;
      if (!onstack[h])
      {
         if (possibil[h] == 0) // not defined yet?
			{
				onstack[h] = 1;
				possibil[h] = 0;
				for (p = f_prod[h]; p < l_prod[h]; p++)
				{
					combinations = 1;
					for (t = f_tail[p]; t < l_tail[p]; t++) 
					{
						if ((sym = -tail[t]) > 0) // head symbol?
						{
							x = POSSIBIL(sym); 
							if (x == MAX_INT) 
							{
								combinations = MAX_INT;
								break;
							}
							else combinations *= x;
						}
					}
					if (combinations == MAX_INT) 
					     possibil[h]  = MAX_INT;
					else possibil[h] += combinations;
				//	printf ("possibil[%s] = %d\n", head_name[h], possibil[h]);
				}
				onstack[h] = 0;
			}
         return (possibil[h]);
      }
		else
		{
			return (possibil[h] = MAX_INT);
		}
}
  
void  LGAddExtra::p_prod (int p)
{
		p_prod (p, -1);
}

void  LGAddExtra::p_prod (int p, int dot)
{
      int t, u, d;
      printf ("%5d %s ->", p, head_name [head_sym[p]]);
      t = f_tail [p];
      u = l_tail [p];
      d = t + dot;
      if (dot == -1) d = u;
      for (;;)
      {
         if (t == d) printf (" .");
         if (t >= u) break;
         p_sym (tail [t]);
         t++;
      }
      printf ("\n");
}

void  LGAddExtra::p_sym (int s)
{
      char *p;
      if (s >= 0) p = term_name[s];
      else        p = head_name[-s];
      printf (" %s", p);
}

void  LGAddExtra::PRT_STA (int s)
{
      int k, i, f, p, n, a, b;

		if (++n_fatals == 1) 
		prt_con ("\nFATAL AMBIGUITIES REPORT\n\n");

      prt_con ("///////////////////////////// STATE %d /////////////////////////////\n//\n", s);

		n = 0;
		a = f_kernel[s];
		b = f_kernel[s+1];
		for (k = a; k < b; k++)
      {
			n++;
         i = kernel [k];
         if (item[i].symb != -32767)
         {
            prt_con ("// * ");
            p_prod2 (item[i].prod, item[i].dot, "", "\n");
         }  
      }

		a = f_final[s];
		b = f_final[s+1];
      for (f = a; f < b; f++)
      {
			n++;
         p = final [f];
         if (p >= 0)
         {
				prt_con ("// * ");
				p_prod2 (p, -1, "", "\n");
         }  
      }	

		a = f_finalx[s];
		b = l_finalx[s];
      for (f = a; f < b; f++)
      {
			n++;
         p = finalx [f];
         if (p >= 0)
         {
				prt_con ("// . ");
				p_prod2 (p, -1, "", "\n");
         }  
      }	
}

void  LGAddExtra::p_prod2 (int p, int dot, char *before, char* after)
{
      int t, u, d, h;
		h = head_sym[p];
		prt_con ("%s%4d  %s -> ",   before, p, head_name [h]);
      t = f_tail [p];
      u = l_tail [p];
      d = t + dot;
      if (dot == -1) d = u;
      for (;;)
      {
         if (t == d) prt_con (". ");
         if (t >= u) break;
         p_sym2 (tail [t], " ");
         t++;
      }
      prt_con ("%s", after);
}
  
int	LGAddExtra::p_sym2 (int s, char *sp)
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

///////////////////////////////////////////////////////////////////////////////

