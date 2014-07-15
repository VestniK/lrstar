
#ifdef LRSTAR

		#include "ComGlobal.h"
		#include "PGPrintStates.h"

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGPrintStates::PrintStates ()
{
      int t, h, s, n, x, p, i;

      if (optn[PG_STATELIST] > 1) return; // Already output once!

      prt_sta ("\nSTATE MACHINE LISTING:\n\n");
      if (!optn[PG_STATELIST])
		{
	      prt_sta ("'s' option was not specified.\n");
			return;
		}
      prt_sta ("      +=>  Shift and goto next state.\n");
      prt_sta ("      +<=  Shift and reduce.\n");
      prt_sta ("       <=  Reduce.\n\n");

      for (s = 0; s < n_states; s++)
      {
         PRT_STA (s);
         prt_sta ("\n");
         if (optn[PG_TRANSITIONS])
         {
				int na = 0;
            for (i = tt_start[s]; i < tt_start[s+1]; i++)
            {
					if (tt_action[i] != 0)
					{
						na++;
						x = tt_action [i];
						t = tt_symb [i];
						P_SYMBOL (t);
						if (x > 0) prt_sta (" +=> %4d\n", x);
					  	else       prt_sta (" +<= %4d\n", -x);
					}
            }
            if (optn[PG_NONDETER])
            {
               for (i = nd_start[s]; i < nd_start[s+1]; i++)
               {
						na++;
                  t = nd_term[i];
                  P_SYMBOL (t);
                  x = nd_action[i];
                  if (x > 0)
                  {
                     if (x > opt_states) prt_sta (" +<= %4d  nondeterministic\n",  x-opt_states);
                     else                prt_sta (" +=> %4d  nondeterministic\n",  x);
                  }
						else prt_sta ("  <= %4d  nondeterministic\n", -x);
               }
            }
            if (la_start != NULL)
            {
               for (i = la_start[s]; i < la_start[s+1]; i++)
               {
						na++;
                  t = la_symb [i];
                  P_SYMBOL (t);
                  x = la_red [i];
                  prt_sta ("  <= %4d\n", x);
               }
            }

				if (D_red == NULL)
				{
					for (int f = f_final[s]; f < f_final[s+1]; f++)
					{
						na++;
						p = item [final[f]].prod;
		            P_SYMBOL (-32767); // default
			         prt_sta ("  <= %4d\n", p);
					}
				}
				else
				{
					if ((p = D_red[s]) >= 0)
					{
						na++;
						P_SYMBOL (-32767); // default
						prt_sta ("  <= %4d\n", p);
					//	p_prod (p, -1, "", "\n");
					}
					else
					{
					// P_SYMBOL (-32768); // error
					// prt_sta ("\n");
					}
				}
				if (na > 0) prt_sta ("\n");

            n = 0;
            for (i = ntt_start[s]; i < ntt_start[s+1]; i++)
            {
					if (ntt_action[i] != 0)
					{
						n++;
						x = ntt_action [i];
						h = ntt_symb [i];
						P_HEAD (h);
						if (x > 0) prt_sta (" +=> %4d\n", x);
						else       prt_sta (" +<= %4d\n", -x);
					}
            }
				if (n > 0) prt_sta ("\n");

				if (f_camefrom[s+1] > f_camefrom[s])
				{
					prt_sta ("Came from:");
					for (i = f_camefrom[s]; i < f_camefrom[s+1]; i++)
					{
	               prt_sta (" %d", camefrom[i]);
					}
					prt_sta ("\n\n");
				}
         }
      }

      prt_sta ("//\n");
      prt_sta ("///////////////////////////////////////////////////////////////////");
		if (opt_states > 10  ) prt_sta ("/");
		if (opt_states > 100 ) prt_sta ("/");
		if (opt_states > 1000) prt_sta ("/");
		prt_sta ("\n\n");
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGPrintStates::P_HEAD (int s)
{
      int L = 0;
      prt_sta ("           ");
      L = p_head (s, " ");
		L = max_headl-L+1;
	  	if (L <  0) L =  0;
	  	else if (L > 79) L = 79;
      spaces [L] = 0;
      prt_sta ("%s", spaces);
      spaces [L] = ' ';
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGPrintStates::P_SYMBOL (int s)
{
      int L = 0;
      if (s == -32767)
      {
         prt_sta ("           (default)  ");
         L = 11;
      }
   // else if (s == -32768)
   // {
   //    prt_sta ("        /*error*/  ");
   //    L = 11;
   // }
      else
      {
         prt_sta ("           ");
         L = p_sym (s, " ");
      }
		L = max_headl-L+1;
	  	if (L <  0) L =  0;
	  	else if (L > 79) L = 79;
      spaces [L] = 0;
      prt_sta ("%s", spaces);
      spaces [L] = ' ';
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGPrintStates::PRT_STA (int s)
{
      int k, i, f, p, n;

      prt_sta ("///////////////////////////// STATE %d /////////////////////////////\n//\n", s);

		n = 0;
      for (k = f_kernel [s]; k < f_kernel [s+1]; k++)
      {
			n++;
         i = kernel [k];
         if (item[i].symb != -32767)
         {
            prt_sta ("// * ");
            p_prod (item[i].prod, item[i].dot, "", "\n");
         }
      }

      for (f = f_final [s]; f < f_final [s+1]; f++)
      {
			n++;
         p = item [final[f]].prod;
         if (p >= 0)
         {
            prt_sta ("// * ");
            p_prod (p, -1, "", "\n");
         }
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGPrintStates::p_prod (int p, int dot, char *before, char* after)
{
      int t, u, d;
      prt_sta ("%s%4d  %s -> ", before, p, head_name [head_sym [p]]);
      t = F_tail [p];
      u = F_tail [p+1];
      d = t + dot;
      if (dot == -1) d = u;
      for (;;)
      {
         if (t == d) prt_sta (". ");
         if (t >= u) break;
         p_sym (Tail [t], " ");
         t++;
      }
      prt_sta ("%s", after);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int	PGPrintStates::p_head (int s, char *sp)
{
		int len;
      char *p;
      p = head_name[s];
      prt_sta ("%s%s", p, sp);
		len = strlen(p) + strlen(sp);
		return (len);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int	PGPrintStates::p_sym (int s, char *sp)
{
		int len;
      char *p;
      if (s >= 0)                 /* Terminal symbol? */
         p = term_name[s];
      else                        /* Nonterminal symbol? */
         p = head_name[-s];

      prt_sta ("%s%s", p, sp);
		len = strlen(p) + strlen(sp);
		return (len);
}

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif
