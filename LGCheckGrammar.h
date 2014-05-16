
		#pragma once
		#include "LGActions.h"

		class LGCheckGrammar : public LGActions
		{
			public:
			static int  CheckGrammar ();

			private:
			static void C_LENG ();
			static void C_NULLS ();
			static void C_HEADSYM ();
			static void CHECK_LEXICALS ();
			static void P_UNDEFINED ();
			static void P_NULL_TOKENS ();
			static void P_USELESS_PROD ();
			static void P_UNREACHABLES ();
			static void P_UNREDUCIBLES ();
			static void RenumberProductions (int n_unreachables);
			static int	p_sym (int s, char *sp);
			static void p_prod (int p, int dot, char *before);
			static void C_CYCLES ();
			static int  C_CYCLES2 (int h);
			static void DO_BACK_SUB ();
			static void scan_prod (int h, int p, int t); 
			static void make_prod (int h, int p); 
			static void PrintGrammar ();
			static void PRT_HEAD (int s);
			static int  prt_head (int s, char *sp);
			static void prt_tran (int s);
			static void prt_camefrom (int s);
			static void PRT_RANGE (int s1, int s2);
			static int  PRT_HEAD (int s, char *after);
			static void PRT_SYMB (int s);
			static int  prt_sym2 (int s, char *after);
		};
