
		#pragma once
		#include "LGComputeLA.h"

		class LGPrintStates : public LGComputeLA
		{
			public:
			static void  PrintStates(int type);

			private:
			static void  PRT_STA (int s);
			static void  P_SYMBOL (int s);
			static void  P_RANGE (int s1, int s2);
			static int   p_sym (int s, char *sp);
			static void  p_prod (int p, int dot, char *before, char* after);
		};

