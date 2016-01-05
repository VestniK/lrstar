
		#pragma once
		#include "LGBuildStates.h"

		class LGAddExtra : public LGBuildStates
		{
			public:
			static int    AddExtra ();

			private:
			static void   DEFINE_POSS ();
			static int 	  POSSIBIL (int h);
			static void   GOTO (int, int);
			static int    JUMP_TO (int);
			static void   DEF_CYCLES ();
			static void	  DEFINE_RECUR ();
			static int    RECURSIVE (int h);
			static void   DETECT_CYCLES ();
			static int    MERGE_FINALS ();
			static void	  MERGE_NTTRANS ();
			static int    REMOVE_FINAL (int);
			static int    N_TRAN (int x, int sym); 
			static int    NO_REDUCE_GOTO (int, int);
			static void   LOOK_BACK (int p, int dot, int x, int& nj, int* jumpto);
			static int    REDUCE_GOTO (int p); 
			static void	  UNDO_REDUCE (); 
			static void   TRACE (int, int, int, int);
			static void	  TRACE_REDUCE (int, int);
			static void	  ADDING (int x, int z); 
			static int    NO_MATCHING_REDUCE (int x); 
			static void   PRT_STA (int s);
			static void   p_all ();
			static void   p_symbols ();
			static void   p_goto_stack ();
			static void   p_trace_stack ();
			static void   p_sym (int s);
			static void   p_prod (int p);
			static void   p_prod (int p, int dot);
			static int	  p_sym2 (int s, char *sp);
			static void   p_prod2 (int p, int dot, char *before, char* after);
		};