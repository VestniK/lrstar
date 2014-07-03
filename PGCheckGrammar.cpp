
#ifdef LRSTAR

      #include "ComGlobal.h"
		#include "PGGlobal.h"
		#include "PGCheckGrammar.h"

		static uchar which_case[256] = /* Gives 1 for upper and 2 for lower. */
		{
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
		     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,
			  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
			  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0,  0,

			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		};

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   PGCheckGrammar::CheckGrammar ()
{
      prt_log ("Grammar  %7d rules, %d terminals, %d nonterminals.\n", N_prods, N_terms, N_heads);

		eof_term = Tail [F_tail[0]+1];

      C_LENG ();
      C_NULLS ();
      C_HEADSYM ();

	  	KEYWORDS_CHECK ();
      KEYWORDS_WARNING ();
      P_UNUSED_OR_UNDECLARED ();
      P_USELESS_PROD ();
      P_UNREACHABLES ();
      P_UNREDUCIBLES ();

      FREE (term_start, N_terms);
      FREE (term_end,   N_terms);
   // FREE (term_type,  N_terms); Need this for keyword list at end of Terminate().
   // FREE (term_line,  N_terms); Need this for undefined lexical symbols.

		if (n_errors) return (0);
      return (1);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGCheckGrammar::C_LENG () /* Compute maximum symbol length. */
{
      int i, L;
      if (optn [PG_BACKSLASH])
      {
         for (i = 0; i < N_terms; i++)
         {
            fix_backslash (term_name [i]);
         }
      }
      max_headl = 38 - optn[PG_TAB];
      max_terml = 38 - optn[PG_TAB]; 
		max_symbl = 38 - optn[PG_TAB];
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGCheckGrammar::C_NULLS ()
{
      int h, p, t, s, n;

      n_nulls = 0;
      ALLOC (nullable, N_heads);
      FASTINI (0, nullable, N_heads);
      do
		{
         n = 0;
         for (h = 0; h < N_heads; h++)
         {
            if (nullable [h] == 0)
            {
               for (p = F_prod [h]; p < F_prod [h+1]; p++)
               {
                  for (t = F_tail [p]; t < F_tail [p+1]; t++)
                  {
                     if ((s = Tail [t]) < 0)
                     {
                        if (nullable [-s] == 0) goto nextp;
                     }
                     else goto nextp;
                  }
                  n++;
                  n_nulls++;
                  nullable [h] = 1;
                  goto nexth;
nextp:            continue;
               }
            }
nexth:      continue;
         }
      }
      while (n > 0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGCheckGrammar::C_HEADSYM ()
{
      int h, p;
      ALLOC (head_sym,  N_prods);
      ALLOC (prod_len,  N_prods);
      for (h = 0; h < N_heads; h++)
      {
         for (p = F_prod[h]; p < F_prod[h+1]; p++)
         {
            head_sym[p] = h;
            prod_len[p] = F_tail[p+1] - F_tail[p];
         }
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGCheckGrammar::KEYWORDS_CHECK ()
{
      int t;
		if (!optn[PG_KEYWORDCHECK]) return;

   // Check for undeclared keywords ...
      for (t = 0; t < N_terms; t++)
      {
         if (*term_name[t] == '\'') continue;
         if (*term_name[t] == '<' ) continue;
		   if (*term_name[t] == '{' ) continue;
		  	if (*term_name[t] == '$' ) continue;
         if ((term_type[t] & UNDECLARED)) // If not declared at top of grammar.
			{
				prt_warning ("Keyword %s was not declared", term_start[t], term_end[t], term_line[t]);
			}
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGCheckGrammar::KEYWORDS_WARNING ()
{
		char *p;
      int t, Upper, Lower, upper, lower;

		if (!optn[PG_KEYWORDCHECK]) return;

   // Count number of lower & upper case letters in keywords.
		Upper = Lower = 0;
      for (t = 0; t < N_terms; t++)
      {
         if (itsakeyword (term_name[t]))
			{
				for (p = term_name[t]; *p != 0; p++)
				{
					switch (which_case[*p])
					{
						case 0: break;
						case 1: Upper++; break;
						case 2: Lower++; break;
					}
				}
			}
      }
		if (Upper > Lower) Lower = 0;
		else if (Lower >= Upper) Upper = 0;

   // Check each keyword for case errors.
      for (t = 0; t < N_terms; t++)
      {
         if (itsakeyword (term_name[t]))
			{
				upper = 0;
				lower = 0;
				for (p = term_name[t]; *p != 0; p++)
				{
					switch (which_case[*p])
					{
						case 0: break;
						case 1: upper++; break;
						case 2: lower++; break;
					}
				}
				if (upper > 0 && lower > 0) // Mixed upper and lower case?
				{
					prt_warning ("Keyword %s has both upper and lower case letters", term_start[t], term_end[t], term_line[t]);
				}
				else if ((upper > 0 && Lower > 0) || (lower > 0 && Upper > 0)) // Keyword error ?
				{
					prt_warning ("Keyword %s looks like the wrong case", term_start[t], term_end[t], term_line[t]);
				}
			}
      }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGCheckGrammar::P_UNUSED_OR_UNDECLARED ()
{
      int t;
   // Define whether <error> symbol was used in the grammar ...
      error_sym_used = 0;
      if (term_type[0] & TAIL) error_sym_used = 1;

   // Check terminal symbols for usage in the grammar rules ...
		for (t = 1; t < N_terms; t++)
		{
			if (!(term_type[t] & UNDECLARED)) // Declared ?
			{
				if (!(term_type[t] & TAIL)) // Not used in grammar?
				{
					if (!(term_type[t] & ARGUMENT)) // Not used as argument?
					{
						prt_warning ("Terminal symbol %s was declared but not used anywhere", 
						term_start[t], term_end[t], term_line[t]);
					}
				}
			}
			else // Not declared!
			{
				if (!(term_type[t] & TAIL)) // Not used in grammar?
				{
					if ((term_type[t] & ARGUMENT)) // Used as argument?
					{
						prt_warning ("Terminal symbol %s was used as an argument but not declared and not used in the grammar", 
						term_start[t], term_end[t], term_line[t]);
					}
				}
			}
		}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGCheckGrammar::P_USELESS_PROD ()
{
      int h, p;
		for (h = 1; h < N_heads; h++)
		{
			for (p = F_prod[h]; p < F_prod[h+1]; p++)
			{
				if ((F_tail[p+1]-F_tail[p]) == 1)
				{
					if (h == -Tail[F_tail[p]])
					{
						prt_log ("%s(%04d) : ", grmfid, prod_line[p]); 
						p_prod (-p, -1, "");
						prt_log ("\n");
						prt_error ("Useless production", 0, 0, prod_line[p]);
					}
				}
			}
		}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGCheckGrammar::P_UNREACHABLES ()
{
      char *h_vect;
      int  *h_list, h, p, t, s, i, nh;

      ALLOC (h_vect, N_heads);
      ALLOC (h_list, N_heads);
      memset (h_vect, 0, N_heads);

      h_vect [0] = 1; // Mark it used.
      h_list [0] = 0; // Add zero to list.
      nh         = 1; // Number of heads encountered.

      for (i = 0; i < nh; i++)
      {
         h = h_list [i];                                /* Pick next head.   */
         for (p = F_prod [h]; p < F_prod [h+1]; p++)    /* All of its rules. */
         {
            for (t = F_tail [p]; t < F_tail [p+1]; t++) /* All of its tails. */
            {
               if ((s = Tail [t]) < 0)                  /* Nonterminal?      */
               {
                  if (h_vect [-s] == 0)                 /* Not marked yet?   */
                  {
                     h_vect [-s] = 1;                   /* Mark it used.     */
                     h_list [nh++] = -s;                /* Add it to list.   */
                  }
               }
            }
         }
      }

      for (h = 0; h < N_heads; h++)
      {
		// if (h_vect[h] == 0 && *head_name[h] != '$')  /* Not used?         */
         if (h_vect[h] == 0)  /* Not used?         */
         {
				prt_error ("Nonterminal symbol '%s' is not reachable from the goal symbol", head_name[h], 0, head_line[h]);
         }
      }
      FREE (h_list, N_heads);
      FREE (h_vect, N_heads);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGCheckGrammar::P_UNREDUCIBLES ()
{
      char *red;
      int  h, p, t, s, n, m;

      ALLOC (red, N_heads);
      memset (red, 2, N_heads); /* undefined */
      m = 0;
      do
      {
         n = 0;
         for (h = 0; h < N_heads; h++)
         {
            if (red [h] == 2)
            {
               for (p = F_prod [h]; p < F_prod [h+1]; p++)
               {
                  for (t = F_tail [p]; t < F_tail [p+1]; t++)
                  {
                     if ((s = Tail [t]) < 0)
                     {
                        if (red [-s] != 1) goto nextp;
                     }
                  }
                  n++;
                  red [h] = 1;
                  goto nexth;
nextp:            continue;
               }
            }
nexth:      continue;
         }
         m += n;
      }
      while (n > 0);
      if (m < N_heads)
      {
         for (h = 0; h < N_heads; h++)
         {
            if (red [h] != 1)
            {
					prt_error ("Nonterminal symbol '%s' is in a cycle, cannot derive anything", head_name[h], 0, head_line[h]);
            }
         }
      }
      FREE (red, N_heads);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

void  PGCheckGrammar::p_prod (int p, int dot, char *before)
{
      int t, u, d;
		if (p < 0)
		{
			p = -p;
	      prt_log ("%s%s -> ", before, head_name [head_sym [p]]);
		}
		else
		{
			prt_log ("%s%5d %s -> ", before, p, head_name [head_sym [p]]);
		}
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
      prt_log ("\n");
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int	PGCheckGrammar::p_sym (int s, char *sp)
{
      char *p;

      if (s >= 0)                 /* Terminal symbol? */
         p = term_name[s];
      else                        /* Nonterminal symbol? */
         p = head_name[-s];

      prt_log ("%s%s", p, sp);
      return (strlen(p) + strlen(sp));
}

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif

