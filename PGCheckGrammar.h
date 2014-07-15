
		#pragma once
		#include "PGActions.h"

		class PGCheckGrammar	: public PGActions
		{
			public:
			static int   CheckGrammar ();

			private:
			static void  C_LENG ();
			static void  C_NULLS ();
			static void  C_HEADSYM ();
			static void  KEYWORDS_WARNING ();
			static void  KEYWORDS_CHECK ();
			static void  P_UNUSED_OR_UNDECLARED ();
			static void  P_USELESS_PROD ();
			static void  P_UNREACHABLES ();
			static void  P_UNREDUCIBLES ();
			static void  p_prod (int p, int dot, char *before);
			static int	 p_sym (int s, char *sp);
		};
