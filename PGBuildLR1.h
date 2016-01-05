
		#pragma once
		#include "ComGlobal.h"
		#include "PGCheckGrammar.h"

      typedef struct LRKERNEL
      {
         ushort item; // Item.
         ushort LA;   // Lookahead. 
      }
      LRKERNEL;       // LR(1) Kernel.

		class PGBuildLR1 : public PGCheckGrammar
		{
			public:
		// Variables:
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

		// Functions:
			static int   BuildLR1 ();

			private:
			static void  C_ITEMS ();
			static void  EXPAND (int state);
			static void  MAKE_STATE (int);
			static void  DO_CLOSURE (int state);
			static void  ADD_ITEM (int i, int la);
			static void  MAKE_KERNEL (int s);
			static int   MAKE_KERNEL (int state, int symb);
			static int   TRANSITION (int symb);
			static void  PRT_CLO (int state);
			static void  PRT_STA (int state);
			static void  PRT_LRSTA (int state);
			static void  P_HEAD (int s);
			static void  P_SYMB (int s);
			static int 	 LR0_SORT (LRKERNEL* lrkernel, int fk, int lk);
			static void	 LR1_SORT (LRKERNEL* lrkernel, int fk, int lk);
			static int   COMPATIBLE (int, int);
			static void  MAKE_LR0_KERNELS (); 
			static void  MAKE_LR0_TRANSITIONS ();
			static void  MODIFY_TRANSITIONS ();
			static void  MTSL ();

			static void  PRT_TRAN (int s);
			static void  prt_item (char* before, int i, int la);
			static void  prt_prod (int p, int dot, char* before, char* after);
			static int   prt_sym (int s, char *after);
			static int   prt_head (int s, char *after);
		};

