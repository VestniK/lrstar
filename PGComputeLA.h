
		#pragma once
		#include "PGBuildLR1.h"

		class PGComputeLA : public PGBuildLR1
		{
			public:
		// Variables:
			static int    n_nditems;
	      static int*   nd_start;
			static int*   nd_term;
			static int*   nd_action;

			static int    c_states;

			static int*   D_red;
			static int*   la_start;
			static int*   la_end;
			static int*   la_symb;
			static int*   la_red;

			static int    n_lookbacks;
			static int    n_lookah;
			static int    n_includes;

		// Functions (methods):
			static int    ComputeLA ();

			private:
			static void   ANALYZE (int s, int f);
			static void   C_LOOKBACKS ();
			static void   C_READS ();
			static void   C_LOOKAHEADS ();
			static void   C_INCLUDE (int state, int inc);
			static int    OPER_PREC (int s, int p, int t);
			static int    GOTO_PREC (int s, int t, int p);
			static int    REDUCE_PREC (int s, int t, int p);
			static void   REPORT_CONFLICTS (int state, int sr, int rr);
			static void   RESOLUTION ();
			static void   PRT_STA (int s);
			static void   prt_state (int s);
			static void	  prt_prod (int p);
			static void	  prt_prod (int p, int dot);
			static void   prt_prod (int p, int dot, char *before);
			static int	  prt_sym (int s, char *sp);
			static void   prt_prod2 (int p, int dot, char *before);
			static int	  prt_sym2 (int s, char *sp);
			static int    LR1_fix (int s, int p, int* action, int t);
			static void   IND_READ (int i, int s);
			static void   LOOK_BACK (int x, int p, int dot, int state);
			static int    NTX (int s, int h);
			static void   BF_TRACE_BACK (int s, int sr, int rr);
			static void   DF_TRACE_BACK (int s, int sr, int rr);
			static void   CYCLE_CHECK (int s);
			static int    bf_look_back (int d, int t, int p, int dot, int orig, int& next, int& last);
			static int    df_look_back (int t, int p, int dot, int orig, int& next, int& last);
			static int    cycle_look_back (int t, int p, int dot, int orig, int& next, int& last);
		};