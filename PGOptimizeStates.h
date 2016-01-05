
		#pragma once
		#include "PGComputeLA.h"

		class PGOptimizeStates : public PGComputeLA
		{
			public:
			static int  OptimizeStates ();

			private:
			static int  FIND_ACCEPT_STATE ();
			static void MAKE_SHIFT_REDUCE_ACTIONS ();
			static void ELIMINATE_CHAIN_REDUCTIONS ();
		};
