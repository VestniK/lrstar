
		#pragma once
		#include "PGComputeLA.h"

		class PGPrintStates : public PGComputeLA
		{
			public:
			static void  PrintStates ();

			private:
			static void  PRT_STA (int s);
			static void  P_HEAD  (int s);
			static void  P_SYMBOL(int s);
			static int   p_head(int s, char *sp);
			static int   p_sym (int s, char *sp);
			static void  p_prod (int p, int dot, char *before, char* after);
		};

