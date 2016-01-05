
		#pragma once
		#include "LGComputeLA.h"		

		class LGOptimizeStates : public LGComputeLA
		{
			public:
			static int  OptimizeStates ();

			private:
			static void PRT_STA (int s);
			static void P_SYMBOL (int s);
			static void P_RANGE (int s1, int s2);
			static void p_prod (int p, int dot, char *before, char* after);
			static void DESTROY_LOOKAHEADS ();
			static void ILLEGAL_CHARS ();
			static int  REMOVE_NT_TRANS ();
			static void REMOVE_DUP_STATES (); 
			static int  REMOVE_SINGLE_RED ();
			static int  TRANSITION (int s, int x);
			static void LOOK_BACK (int p, int dot, int s);
			static int  REMOVE_OTHER_RED ();
			static void COLLECT_LOOKAHEADS (int s); 
			static void LOOK_BACK2 (int p, int dot, int s);
			static int  MODIFY_TERMINAL_TRAN ();						
			static int  NUMBER_OF_STATES_LEFT ();
			static int  COUNT_NT_LEFT ();
			static int  REMOVE_DUPLICATES ();
			static int  GOESTO_CLOSURE (int s);
			static void RENUMBER_STATES ();
		};
