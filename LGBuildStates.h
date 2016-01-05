
		#pragma once
		#include "ComGlobal.h"
		#include "LGCheckGrammar.h"

		class LGBuildStates : public LGCheckGrammar
		{
			public:
	      static int    n_states;
			static int	  extra_states;
			static int*   ntt_item;
			static int*   accessor;
			static int    accept_state; 
			static char** FIRST;
			static char** FOLLOW;
			static int*   f_kernel;
			static int*   l_kernel;
			static int*   f_final;
			static int*   l_final; 
			static int*   final;
			static int*   kernel;
			static ITEM*  item;    
			static int*   f_item;
			static int    n_ttran;
			static int    n_nttran;
			static int*   ntt_origin;
			static int    n_kernels;
			static int    n_finals;
			static int    org_states;
			static int    n_items;
			static int*   ntt_start;
			static int*   ntt_end;
			static int*   ntt_symb;
			static int    ntt_states;
			static int*   ntt_action;
			static int    n_ttas;
			static int*   tt_start;
			static int*   tt_end;
			static int*   tt_action;
			static int*   tt_symb;
			static int    tt_states;
			static int*   f_camefrom;
			static int*   l_camefrom;
			static int*   camefrom;

			static int   BuildStates ();

			private:
			static void  C_ITEMS ();
			static void  EXPAND (int state);
			static void  DO_CLOSURE (int state);
			static void  ADD_ITEM (int i, int state);
			static void  MAKE_KERNEL (int c);
			static int   TRANSIT (int first, int s);
			static void  MAKE_ACCEPT_STATE ();
			static void  MTSL ();
		};

