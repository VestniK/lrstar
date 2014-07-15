
		#pragma once
		#include "PGOptimizeStates.h"

		class PGCreateTables : public PGOptimizeStates
		{
			public:
		// Variables:
			static int*   	B_col;
			static int    	B_cols;
	      static int*	  	B_mask;
			static char*  	B_matrix;
			static int*   	B_row;
			static int    	B_rows;
			static int    	B_size;

			static int*   	N_col;
			static int		N_cols;
			static int*   	N_matrix;
			static int*   	N_row;
			static int		N_rows;
			static int    	N_size;

			static int*   	T_col;
			static int    	T_cols;
			static int*   	T_matrix;
			static int*   	T_row;
			static int    	T_rows;
			static int    	T_size;

			static int*   	R_col;
			static int		R_cols;
	      static int*   	R_matrix;
			static int*   	R_row;
			static int		R_rows;
			static int    	R_size;

		// Functions:
			static int		CreateTables();

			private:
			static void  	MakeERI ();
			static void  	MIN_B (int *ti);
			static void  	MIN_T (int *ti, int *tj);
			static void  	MIN_N (int *ti, int *tj);
			static void  	MIN_R (int *ti);
			static int   	BLD_B (int opt1, char*mark);
			static int   	BLD_T (int opt1, int opt2, char*mark);
			static int   	BLD_N (int opt1, int opt2, char*mark);
			static int   	BLD_R (int opt1, char*mark);
			static int   	MAKE_PACKED (char* x, int n);
			static int   	get_type (int *x, int n); 
			static int   	MRG_ROWE1B (char**matrix, int n_terms, int* row, int n_states);
			static int   	MRG_ROWE2R (int** matrix, int n_terms, int* row, int n_states);
			static int   	MRG_ROWZ_T (int** matrix, int n_terms, int* row, int n_states, int opt);
			static int   	MRG_ROWZ_N (int** matrix, int n_heads, int* row, int n_states, int opt);
			static int 	 	MRG_ROWE2T (int** matrix, int n_terms, int* row, int n_states, int opt);
			static int   	MRG_COLE1B(int* col, int n_terms, int n_rows, char** matrix);
			static int   	MRG_COLE2 (int* col, int n_terms, int n_rows, int**  matrix);
			static int   	MRG_COLZ  (int* col, int n_cols,  int n_rows, int**  matrix, int opt);
			static int   	DISP_EQ1B (char**matrix, char*newmat, int* row, int nr, int nc, int opt1);
			static int   	DISP_EQ2  (int** matrix, int* newmat, int* row, int nr, int nc, int n_states, int opt1);
			static int   	DISP_ZEQ  (int** matrix, int* newmat, int* row, int nr, int nc, int n_states);
			static void  	OUT_TOT (int size, int vectors, char *mark);
		};