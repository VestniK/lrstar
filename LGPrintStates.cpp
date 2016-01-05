
#ifdef DFASTAR

		#include "ComGlobal.h"
		#include "LGPrintStates.h"

		static
		uchar range[256] = /* characters within a range. */
		{
			  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
			  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,
			  0,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
			  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  0,  0,  0,  0,  0,
			  0,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
			  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  0,  0,  0,  0,  0,

			  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
			  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
			  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
			  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
			  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
			  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
			  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
			  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5 
		};

void  LGPrintStates::PrintStates (int type)
{
    const char *type_of_sm, *optstr;
    int c, t, s, n, A, action, R, i, first_n, count, last_n, a, b, opt;

		switch (type)
		{
			case 1: 
				type_of_sm = "(for checking conflicts)";
				opt = optn [LG_STATELIST];
				for (i = 0; *LGOption[i].name != 0; i++) if (LGOption[i].numb == LG_STATELIST) break;
				optstr = LGOption[i].name;
				break;
			case 2: 
				type_of_sm = "(final optimized one)";
				opt = optn [LG_STATELISTOPT];
				for (i = 0; *LGOption[i].name != 0; i++) if (LGOption[i].numb == LG_STATELISTOPT) break;
				optstr = LGOption[i].name;
				break;
			default:
				InternalError(7);
		}
		
      prt_sta ("\n///////////////////////////////////////////////////////////////////\n");
      prt_sta ("//\n");
      prt_sta ("//   STATE MACHINE %s\n", type_of_sm);
      prt_sta ("//\n");

      if (!opt) 
		{
	      prt_sta ("\n'%s' option was not specified.\n", optstr);
			return;
		}

      prt_sta ("//    +=>  Shift and goto.\n");
      prt_sta ("//     <=  Reduce.\n\n");

      for (s = 0; s < n_states; s++)
      {
         PRT_STA (s);
         prt_sta ("\n");
         if (optn[LG_TRANSITIONS])
         {
				c = 0;
				count   = 0;
				first_n = 0;
				last_n  = 0;
				a = tt_start[s];
				b = tt_start[s+1];
				if (tt_end != NULL) b = tt_end[s];
				for (t = a; t < b; t++)
            {
					c++;
               A = tt_action [t];
               n = tt_symb [t];
					if (range[n] > 0) 
					{
Inside:				if (count == 0) 
						{
							first_n = n; 
							action = A;
							R = range[n];
							count++;
						}
						else if (count > 0) 
						{
							if (A == action && range[n] == R && n == last_n+1) count++;
							else goto Out;
						}
					}
					else
					{
Out:					if (count == 1) 
						{
							P_SYMBOL (first_n); 
							prt_sta (" +=> %4d\n", action);
						}
						else if (count == 2) 
						{
							P_SYMBOL (first_n); 
							prt_sta (" +=> %4d\n", action);
							P_SYMBOL (last_n); 
							prt_sta (" +=> %4d\n", action);
						}
						else if (count > 2) 
						{
							P_RANGE (first_n, last_n);
							prt_sta (" +=> %4d\n", action);
						}
						R = 0;
						count = 0;
						if (range[n] > 0) goto Inside;
						P_SYMBOL (n);
						prt_sta (" +=> %4d\n", A);
					}
					last_n = n;
            }
				if (count == 1) 
				{
					P_SYMBOL (first_n); 
					prt_sta (" +=> %4d\n", action);
				}
				else if (count == 2) 
				{
					P_SYMBOL (first_n); 
					prt_sta (" +=> %4d\n", action);
					P_SYMBOL (last_n); 
					prt_sta (" +=> %4d\n", action);
				}
				else if (count > 2) 
				{
					P_RANGE (first_n, last_n);
					prt_sta (" +=> %4d\n", action);
				}
            if (la_start != NULL)
            {
					a = la_start[s];
					b = la_start[s+1];
					if (la_end != NULL) b = la_end[s];
					for (t = a; t < b; t++)
               {
                  n = la_symb [t];
                  P_SYMBOL (n);
                  A = la_red [t];
                  prt_sta ("  <= %4d\n", A);
               // prt_sta ("  <= %4d  %3d %s\n", A, prod_len[A]-1, head_name[head_sym[A]]);
               }
            }

            if (D_red != NULL && (A = D_red [s]) >= 0 && s != 0 && s != illegal_char_state)
            {
					c++;
               P_SYMBOL (-32767);
					prt_sta ("  <= %4d\n", A);
				//	p_prod (A, -1, "", "\n"); 
            }
				else if (s != accept_state)
				{
					c++;
					prt_sta ("           (error)\n");
				}
				if (c) prt_sta ("\n");

            c = 0;
				a = ntt_start[s];
				b = ntt_start[s+1];
				if (ntt_end != NULL) b = ntt_end[s];
				for (t = a; t < b; t++)
            {
               A = ntt_action [t];
               if (A == 0) continue;
               c++;
               n = ntt_symb [t];
               P_SYMBOL (-n);
               if (A >= 0) prt_sta (" +=> %4d\n", A);
               else        prt_sta (" +<= %4d  %3d %s\n", -A, prod_len[-A]-1, head_name[head_sym[-A]]);
            }
            if (c) prt_sta ("\n");
         }  
      }
      prt_sta ("///////////////////////////////////////////////////////////////////");
		if (n_states > 10  ) prt_sta ("/");
		if (n_states > 100 ) prt_sta ("/");
		if (n_states > 1000) prt_sta ("/");
		prt_sta ("\n\n");
}

void  LGPrintStates::P_SYMBOL (int s)
{
      int L;
      if (s == -32767)
      {
         prt_sta ("           (default) ");
         L = 10;
      }
      else
      {
         prt_sta ("           ");
         L = p_sym (s, " ");
      }
		L = max_headl-L+1;
		if (L < 0) L = 0;
      spaces [L] = 0;
      prt_sta ("%s", spaces);
      spaces [L] = ' ';
}

void  LGPrintStates::P_RANGE (int s1, int s2)
{
      int L;
      prt_sta ("           ");
      L = p_sym (s1, "..");
		L += p_sym (s2, "");
		L = max_headl-L+1;
		if (L < 0) L = 0;
      spaces [L] = 0;
      prt_sta ("%s", spaces);
      spaces [L] = ' ';
}

void  LGPrintStates::PRT_STA (int s)
{
      int k, i, f, p, n, a, b, d_red;
      prt_sta ("///////////////////////////// STATE %d /////////////////////////////\n//\n", s);

		n = 0;
		d_red = D_red[s];
		a = f_kernel[s];
		if (l_kernel != NULL) b = l_kernel[s];
		else                  b = f_kernel[s+1];
		for (k = a; k < b; k++)
      {
			n++;
         i = kernel [k];
         if (item[i].symb != -32767)
         {
            prt_sta ("// * ");
            p_prod (item[i].prod, item[i].dot, "", "\n");
         }  
      }

		a = f_final[s];
		if (l_final != NULL) b = l_final[s];
		else                 b = f_final[s+1];
      for (f = a; f < b; f++)
      {
			n++;
         p = final [f];
         if (p >= 0)
         {
				if (p == d_red) d_red = 0;
				if (p >= n_oprods) prt_sta ("// . ");
				else               prt_sta ("// * ");
				p_prod (p, -1, "", "\n");
         }  
      }	

		if (n == 0 && s == n_states-1) // Must be illegal_char state.
		{
			prt_sta ("// *    Illegal character state.\n");
		}

		if (d_red > 0)
		{
			prt_sta ("// * ");
			p_prod (d_red, -1, "", "\n");
		}
}

void  LGPrintStates::p_prod (int p, int dot, char *before, char* after)
{
      int t, u, d, h;
		h = head_sym[p];
		if (h < 0)
		     prt_sta ("%s%4d  [%d] -> ", before, p, h);
		else prt_sta ("%s%4d  %s -> ",   before, p, head_name [h]);
      t = f_tail [p];
      u = l_tail [p];
      d = t + dot;
      if (dot == -1) d = u;
      for (;;)
      {
         if (t == d) prt_sta (". ");
         if (t >= u) break;
         p_sym (tail [t], " ");
         t++;
      }
      prt_sta ("%s", after);
}
  
int	LGPrintStates::p_sym (int s, char *sp)
{
      char *p;
      if (s >= 0)                 /* Terminal symbol? */
         p = term_name[s];
      else                        /* Nonterminal symbol? */
         p = head_name[-s];

      prt_sta ("%s%s", p, sp);
      return (strlen(p) + strlen(sp));
}

#endif

/*----------------------------------------------------------------*/

