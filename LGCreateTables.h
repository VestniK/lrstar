
		#pragma once
		#include "LGOptimizeStates.h"

		class LGCreateTables : public LGOptimizeStates
		{
			public:
	      static int	  err_ret_numb;	

			static int*   B_col;
			static int    B_cols;
			static char*  B_matrix;
			static int*   B_row;
			static int    B_rows;
			static int    B_size;

			static int*   T_col;
			static int    T_cols;
			static int*   T_matrix;
			static int*   T_row;
			static int    T_rows;
			static int    T_size;

			static int    CreateTables();

			private:
			static int  BLD_B (int opt1, char *mark);
			static int  BLD_T (int opt1, int opt2, char *mark);
			static int  BLD_TDFA ();
			static int  MRG_ROWE1B (char **matrix, int *row, int n_states);
			static int  MRG_ROWE2T (int **matrix, int *row, int n_states);
			static int  MRG_ROWZ (int *start, int *end, int *list, int *symb, int **matrix, int *row, int n_states, int n_terms, int opt);
			static int  MRG_COLE1B (int *col, int n_rows, char **matrix);
			static int  MRG_COLE2 (int *col, int n_rows, int**& matrix);
			static int  MRG_COLZ (int *col, int n_cols, int n_rows, int **matrix, int opt);
			static int  DISP_EQ1B (char** matrix, char* newmat, int* row, int nr, int nc, int opt1);
			static int  DISP_EQ2T (int** matrix, int* newmat, int* row, int nr, int nc, int n_states);
			static int  DISP_EQ2 (int **matrix, int *newmat, int *row, int nr, int nc, int n_states, int opt1);
			static int  DISP_ZEQ_RAND (int **matrix, int *newmat, int *row, int nr, int nc, int n_states);
			static int  DISP_ZEQ_SEQ (int **matrix, int *newmat, int *row, int nr, int nc, int n_states);
			static void OUT_TOT (int size, int vectors, char *mark);
			static int  get_type (int *x, int n);
		};
