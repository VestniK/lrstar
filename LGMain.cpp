
#ifdef DFASTAR

      #include "ComGlobal.h"
      #include "LGGlobal.h"	

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   LG::Main (int na, char** arg) /* Display program information. */
{
		if (Start (na, arg)) 
		{
			if (Parse())
			{
				if (CheckGrammar())
				{
					if (BuildStates())
					{
						int rc;
						rc  = AddExtra();
						rc += ComputeLA();
						PrintStates (1);
						if (rc == 2)
						{
							if (OptimizeStates())
							{
								PrintStates (2);
								if (!optn[LG_ANALYZEONLY])
								{
									CreateTables();
									Terminate (0);
									return 1;
								}
							}
						}
					}
				}
			}
		}
		Terminate (0);
		return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

int   LG::Start (int na, char *arg []) /* Display program information. */
{
      int i, ne, filedesc, a, x, rc;
		char dn[256], fn[256], ft[256], string[64];

      time1 = clock();
		memory_usage  		 =  0;
		memory_max    		 =  0;
      ne            		 =  0;
      n_errors      		 =  0;
		max_child_usage    =  0;
      exefid[0]          =  0;
		illegal_char_state = -1; 

		for (i = 0; i < 256; i++) spaces [i] = ' ';

		strcpy (gft, ".lgr");
		strcpy (grmfid, gdn);
		strcat (grmfid, gfn);
		strcat (grmfid, gft);

		open_con (grmfid);
		open_grm (grmfid);
		open_sta (grmfid);

		strcpy (gft, ".lex");
		strcpy (grmfid, gdn);
		strcat (grmfid, gfn);
		strcat (grmfid, gft);

		if (lexer_input == 0) // Lexical grammar not part of .grm file?
		{
			filedesc = open (grmfid, 0); // Open .lex file first. 
			if (filedesc < 0) // .lex file not found?
			{
				strcpy (gft, ".lgr");
				strcpy (grmfid, gdn);
				strcat (grmfid, gfn);
				strcat (grmfid, gft);
				if (!inputi ()) return 0;
				lex_input_start = 0;
				lex_input_end   = 0;
				lex_line_ptr    = 0;
			}
			else
			{
				if (!inputi ()) return 0;
				lex_input_start = input_start;
				lex_input_end   = input_end;
				lex_line_ptr    = line_ptr;
			}
		}
		else
		{
			if (!inputi ()) return 0;
			lex_input_start = input_start;
			lex_input_end   = input_end;
			lex_line_ptr    = line_ptr;
		}

		for (i = 0; i < 159; i++) spaces [i] = ' ';

		memory_usage  = 0;
		memory_max    = 0;
      ne            = 0;
      n_warnings    = 0;
      n_errors      = 0;
		return 1;
}

///////////////////////////////////////////////////////////////////////////////
//    
      
int   LG::Terminate (int rc)
{
      double dsec;
		int    min, sec, thou, i;
		char   num[14] = "             ";

		if (n_states > org_states) org_states = n_states;
		if (n_prods > max_n_prods) max_n_prods = n_prods; // brute force fix.
		else max_n_prods++;										  // brute force fix again.
		if (n_prods < 0) n_prods = 0; // In case of early syntax error.

	  	if (optn[LG_VERBOSE] > 1)
		{
         optncount[MAX_SYM]   = n_symbs;
         optncount[MAX_PRO]   = n_prods;
         optncount[MAX_TAIL]  = n_tails;
         optncount[MAX_EBNF]  = amt_space;
         optncount[MAX_STA]   = org_states;
         optncount[MAX_FIN]   = n_finals;
         optncount[MAX_KER]   = n_kernels;
         optncount[MAX_NTT]   = n_nonttran;
         optncount[MAX_TT]    = n_termtran;
         optncount[MAX_TTA]   = n_ttas;
         optncount[MAX_LB]    = n_lookbacks;
         optncount[MAX_LA]    = n_lookah;
         optncount[MAX_INC]   = n_includes;
         optncount[MAX_CH]    = max_child_usage;
         optncount[MAX_ND]    = 0;
			for (i = 0; *MAOption[i].name != 0; i++)
			{
				prt_num (MAOption[i].desc, optncount[MAOption[i].numb], MAOption[i].name, optn[MAOption[i].numb]);
			}
	  	   prt_logonly ("\n");
		}

		char* es  = "s";
		char* ws  = "s";

		if (n_errors    == 1) es  = "";
		if (n_warnings  == 1) ws  = "";										   

      time2 = clock ();
      dsec  = (double)(time2-time1) / CLOCKS_PER_SEC;
      min   =  dsec/60;
      sec   =  dsec-min*60;
      thou  = (dsec-min*60-sec)*1000;
      int x = memory_max/1024/1024;
      int y = memory_max/1024 - 1024*x;

		prt_log ("%1d min %1d.%03d sec, %d.%03d MB, %d warning%s, %d error%s.\n\n", 
		min, sec, thou, x, y, n_warnings, ws, n_errors, es);

		close_con ();
		close_grm ();
		close_sta ();
		close_lst ();

      if (n_errors > 0) quit (n_errors);
		return 0;
}

#endif

/*--- End of Main Program. ------------------------------------------------- */

