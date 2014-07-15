
#ifdef LRSTAR

      #include "ComGlobal.h"
		#include "PGCreateTables.h"

		int*   	PGCreateTables::B_col;
		int    	PGCreateTables::B_cols;
	   int*	  	PGCreateTables::B_mask;
		char*  	PGCreateTables::B_matrix;
		int*   	PGCreateTables::B_row;
		int    	PGCreateTables::B_rows;
		int    	PGCreateTables::B_size;

		int*   	PGCreateTables::N_col;
		int		PGCreateTables::N_cols;
		int*   	PGCreateTables::N_matrix;
		int*   	PGCreateTables::N_row;
		int		PGCreateTables::N_rows;
		int    	PGCreateTables::N_size;

		int*   	PGCreateTables::T_col;
		int    	PGCreateTables::T_cols;
		int*   	PGCreateTables::T_matrix;
		int*   	PGCreateTables::T_row;
		int    	PGCreateTables::T_rows;
		int    	PGCreateTables::T_size;

		int*   	PGCreateTables::R_col;
		int		PGCreateTables::R_cols;
	   int*   	PGCreateTables::R_matrix;
		int*   	PGCreateTables::R_row;
		int		PGCreateTables::R_rows;
		int    	PGCreateTables::R_size;

      int		quiet;
      int		T0, T1, T2;
      int		total0, total1, total2;
		int		T_total, N_total;							   

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

int   PGCreateTables::CreateTables ()
{
      char* m;
      int   Bi = 0;
      int   Ti = 0, Tj = 0;
      int   Ni = 0, Nj = 0;
      int   Ri = 0;

		if (optn[PG_ANALYZEONLY]) return 0;

      m = " ";
      total0 = total1 = total2 = 0;
		if (optn [PG_MINIMIZE]) 
		{
         if (optn[PG_VERBOSE])
		        prt_log     ("            rows   cols          matrix        list       vect      total\n");
         else prt_logonly ("            rows   cols          matrix        list       vect      total\n");
			MIN_B (&Bi);
			MIN_T (&Ti, &Tj);
			MIN_N (&Ni, &Nj);
			MIN_R (&Ri);
			m = "*"; 
         if (optn[PG_VERBOSE])
		        prt_log     ("\n");
         else prt_logonly ("\n");
		}

      if (optn[PG_VERBOSE])
		     prt_log     ("            rows   cols          matrix        list       vect      total\n");
      else prt_logonly ("            rows   cols          matrix        list       vect      total\n");
		BLD_B (Bi, m);
		BLD_T (Ti, Tj, m);
  		BLD_N (Ni, Nj, m);
		BLD_R (Ri, m);
		char n1[16] = "               ";
		char n2[16] = "               ";
		char n3[16] = "               ";
		number (total1, n1);  
		number (total2, n2);  
		number (total1+total2, n3);  
      if (optn[PG_VERBOSE])
		     prt_log     ("Total                                     %9s +%9s =%9s\n\n", n1, n2, n3);
      else prt_logonly ("Total                                     %9s +%9s =%9s\n\n", n1, n2, n3);

      FREE (tt_symb,    n_ttran);   
      FREE (tt_action,    n_ttran);  
      FREE (tt_start,   n_states+1);
      FREE (ntt_start,  n_states+1);
      FREE (ntt_symb,   n_nttran);
      FREE (ntt_action,   n_nttran);
      FREE (la_start,   n_states+1);
      FREE (la_red,     max_lookah);
      FREE (la_symb,    max_lookah);

	// Reduce production length by 1, so no "-1" adjustment is required in skl file.
		for (int p = 0; p < N_prods; p++) prod_len[p]--;
		return (1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

void  PGCreateTables::MIN_B (int *ti)
{
      int i;
      int  m, minimum = 1024000;

      T0 = total0;
      T1 = total1;
      T2 = total2;

      for (i = 1; i <= 2; i++) 
      {
         m = BLD_B (i, "");
         FREE (B_matrix, B_size);
			FREE (B_mask, N_terms);
         FREE (B_col,  N_terms);
         FREE (B_row,  n_states);
         if (m < minimum)
         {
            minimum = m;
            *ti = i;
         }  
      }
      total0 = T0;
      total1 = T1;
      total2 = T2;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

void  PGCreateTables::MIN_T (int *ti, int *tj)
{
      int i, j;
      int  m, minimum = 1024000;

      T0 = total0;
      T1 = total1;
      T2 = total2;

      for (i = 1; i <= 2; i++)
      {
         for (j = 1; j <= 2; j++)
         {
            m = BLD_T (i, j, "");
            FREE (T_matrix, T_size);
            FREE (T_col,  N_terms);
            FREE (T_row,  tt_states);
            if (m < minimum)
            {
               minimum = m;
               *ti = i;
               *tj = j;
            }  
         }  
      }
      total0 = T0;
      total1 = T1;
      total2 = T2;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

void  PGCreateTables::MIN_N (int *ni, int *nj)
{
      int i, j;
      int  m, minimum = 1024000;

      T0 = total0;
      T1 = total1;
      T2 = total2;

		int n = 0;
      for (i = 1; i <= 2; i++)
      {
         for (j = 1; j <= 2; j++)
         {
            m = BLD_N (i, j, "");
            FREE (N_matrix, N_size);
            FREE (N_col, N_prods);
            FREE (N_row, ntt_states);
            if (m < minimum)
            {
               minimum = m;
               *ni = i;
               *nj = j;
            }  
         }  
      }
      total0 = T0;
      total1 = T1;
      total2 = T2;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

void  PGCreateTables::MIN_R (int *ti)
{
      int i;
      int  m, minimum = 1024000;

      T0 = total0;
      T1 = total1;
      T2 = total2;

      for (i = 1; i <= 2; i++) 
      {
         m = BLD_R (i, "");
         FREE (R_matrix, R_size);
         FREE (R_col,  N_terms);
         FREE (R_row,  n_states);
	      if (m < minimum)
         {
            minimum = m;
            *ti = i;
         }  
      } 
      total0 = T0;
      total1 = T1;
      total2 = T2;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

int   PGCreateTables::MAKE_PACKED (char* x, int n)
{
      uchar* z;
      int i, j, m, r;
      m = (n+7)/8;
      ALLOC (z, m);
      for (i = 0; i < n; i++)
      {
         j = i / 8;
         if (i % 8 == 0) 
         {
            z[j] = 0;
            r = 1;
         }
         z[j] |= x[i] * r;
         r *= 2;
      }
      for (i = 0; i < m; i++)
      {
			x[i] = z[i];
      }
      FREE (z, m);
      return (m);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

int   PGCreateTables::BLD_B (int opt1, char *mark) // Build Boolean Matrix. 
{
      char** Matrix;
      int    r, org_size;

		if (optn[PG_BOOLMATRIX] == 0) return 0;

      if (optn[PG_VERBOSE]) prt_log     ("B matrix");
      else               prt_logonly ("B matrix");

      ALLOC (B_row,  n_states);
      ALLOC (B_col,  N_terms);
      ALLOC (Matrix, n_states+1);

      B_rows = MRG_ROWE1B (Matrix, N_terms, B_row, n_states);

      if (optn[PG_VERBOSE]) prt_log     ("  %6d ", B_rows);
      else               prt_logonly ("  %6d ", B_rows);

      B_cols = MRG_COLE1B (B_col, N_terms, B_rows, Matrix);
      B_size = B_rows * B_cols;
      total0 += B_size;

		char num[12] = "           ";
		number (B_size, num);  // Gives 9 digits.

      if (optn[PG_VERBOSE]) prt_log     ("x%5ld x 1 =%10s", B_cols, num);
      else               prt_logonly ("x%5ld x 1 =%10s", B_cols, num);

		org_size = (B_rows)*((B_cols+7)/8*8); 
      ALLOC (B_matrix, org_size);
      memset (B_matrix, 0, org_size);
      B_size = DISP_EQ1B (Matrix, B_matrix, B_row, B_rows, B_cols, opt1);

      for (r = 0; r < B_rows; r++) FREE (Matrix[r], N_terms);
      FREE (Matrix, n_states+1);
		int vectors = 0;
		if (optn[PG_BOOLMATRIX] > 1) 
		{
			vectors += N_terms;
			B_size = MAKE_PACKED (B_matrix, B_size); 
		}
		REALLOC (B_matrix, org_size, B_size);
		vectors += get_type (B_row, n_states) * n_states; // row vector
		vectors += get_type (B_col, N_terms)  * N_terms;  // column vector
		OUT_TOT (B_size, vectors, mark);
      return  (B_size + vectors);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

int   PGCreateTables::BLD_T (int opt1, int opt2, char *mark) /* Build Terminal Matrix. */
{
      int **Matrix, r, multiplier, org_size;

      if (optn[PG_VERBOSE]) prt_log     ("T matrix");
      else               prt_logonly ("T matrix");

      ALLOC (T_row,  tt_states);
      ALLOC (T_col,  N_terms);
      ALLOC (Matrix, tt_states);    

		if (optn[PG_BOOLMATRIX] == 0)
			  T_rows = MRG_ROWE2T (Matrix, N_terms, T_row, tt_states, opt1);
		else T_rows = MRG_ROWZ_T (Matrix, N_terms, T_row, tt_states, opt1);

      if (optn[PG_VERBOSE]) prt_log     ("  %6d ", T_rows);
      else               prt_logonly ("  %6d ", T_rows);

		if (optn[PG_BOOLMATRIX] == 0)
	        T_cols = MRG_COLE2 (T_col, N_terms, T_rows, Matrix);
		else T_cols = MRG_COLZ  (T_col, N_terms, T_rows, Matrix, opt2);
      org_size = T_rows * T_cols;

		if (N_prods > 32767 || tt_states > 32767) multiplier = 4; // int
		else if (N_prods >   127 || tt_states >   127) multiplier = 2; // short
		else                                      multiplier = 1; // char

		T_total = multiplier*org_size;
		char num[12] = "           ";
		number (T_total, num); // Gives 9 digits.

      if (optn[PG_VERBOSE]) prt_log     ("x%5ld x %d =%10s", T_cols, multiplier, num);
      else               prt_logonly ("x%5ld x %d =%10s", T_cols, multiplier, num);

		total0 += T_total;
      ALLOC (T_matrix, org_size);
      FASTINI (0, T_matrix, org_size);
		if (optn[PG_BOOLMATRIX] == 0) 
			  T_size = DISP_EQ2 (Matrix, T_matrix, T_row, T_rows, T_cols, tt_states, opt1); 
      else T_size = DISP_ZEQ (Matrix, T_matrix, T_row, T_rows, T_cols, tt_states); 
      for (r = 0; r < T_rows; r++) FREE (Matrix [r], N_terms);
      FREE (Matrix, tt_states);
      REALLOC (T_matrix, org_size, T_size);

		int vectors = 0;
		vectors += get_type (T_row, tt_states) * tt_states; // row vector
		vectors += get_type (T_col, N_terms)   * N_terms;   // column vector
      OUT_TOT (multiplier*T_size, vectors, mark);
      return  (multiplier*T_size + vectors);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

int   PGCreateTables::BLD_N (int opt1, int opt2, char *mark) /* Build Nonterminal Matrix. */
{
      int **Matrix, p, r, multiplier, *n_col, org_size;

      if (optn[PG_VERBOSE]) prt_log     ("N matrix");
      else               prt_logonly ("N matrix");

      ALLOC (N_row,  ntt_states);
      ALLOC (n_col,  N_heads);
      ALLOC (Matrix, ntt_states);

      N_rows = MRG_ROWZ_N (Matrix, N_heads, N_row, ntt_states, opt1);

      if (optn[PG_VERBOSE]) prt_log     ("  %6d ", N_rows);
      else               prt_logonly ("  %6d ", N_rows);

      N_cols = MRG_COLZ (n_col, N_heads, N_rows, Matrix, opt2);
      org_size = N_rows * N_cols;

		if (N_prods > 32767 || ntt_states > 32767) multiplier = 4; // int
		else if (N_prods >   127 || ntt_states >   127) multiplier = 2; // short
		else                                       multiplier = 1; // char

		N_total = multiplier*org_size;
		char num[12] = "           ";
		number (N_total, num); // Gives 9 digits.

      if (optn[PG_VERBOSE]) prt_log     ("x%5ld x %d =%10s", N_cols, multiplier, num);
      else               prt_logonly ("x%5ld x %d =%10s", N_cols, multiplier, num);

		total0 += N_total;
      ALLOC (N_matrix, org_size);
      FASTINI (0, N_matrix, org_size);
      N_size = DISP_ZEQ (Matrix, N_matrix, N_row, N_rows, N_cols, ntt_states); 
      for (r = 0; r < N_rows; r++) FREE (Matrix [r], N_heads);
      FREE (Matrix, ntt_states);
      REALLOC (N_matrix, org_size, N_size);

      ALLOC (N_col, N_prods);
      for (p = 0; p < N_prods; p++)
      {
         N_col[p] = n_col[head_sym[p]];
      }
		FREE (n_col, N_heads);

		int vectors = 0;
		vectors += get_type (N_row, ntt_states) * ntt_states; // row vector
		vectors += get_type (N_col, N_prods)    * N_prods;    // column vector
      OUT_TOT (multiplier*N_size, vectors, mark);
      return  (multiplier*N_size + vectors);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

int   PGCreateTables::BLD_R (int opt1, char *mark) /* Build Reduction Matrix. */
{
      int **Matrix, s, r, multiplier, org_size;

      if (optn[PG_VERBOSE]) prt_log     ("R matrix");
      else               prt_logonly ("R matrix");

      ALLOC (R_row,  n_states);
      ALLOC (R_col,  N_terms);
      ALLOC (Matrix, n_states+1);

      R_rows = MRG_ROWE2R (Matrix, N_terms, R_row, n_states);

      if (optn[PG_VERBOSE]) prt_log     ("  %6d ", R_rows);
      else               prt_logonly ("  %6d ", R_rows);

      R_cols = MRG_COLE2 (R_col, N_terms, R_rows, Matrix);
      org_size = R_rows * R_cols;

      ALLOC (R_matrix, org_size);
      FASTINI (0, R_matrix, org_size);
      R_size = DISP_EQ2 (Matrix, R_matrix, R_row, R_rows, R_cols, n_states, opt1);
      for (r = 0; r < R_rows; r++) FREE (Matrix [r], N_terms);
      FREE (Matrix, n_states+1);
      REALLOC (R_matrix, org_size, R_size);

		if (optn[PG_DEFAULTRED])
		{
			for (s = 0; s < n_states; s++)
			{
				if (D_red[s] > 0) R_row[s] =  D_red[s];
				else              R_row[s] = -R_row[s];	 
			}
		}

		multiplier = get_type (R_matrix, R_size);
		char num[12] = "           ";
		number (multiplier*R_size, num);  

      if (optn[PG_VERBOSE]) prt_log     ("x%5ld x %d =%10s", R_cols, multiplier, num);
      else               prt_logonly ("x%5ld x %d =%10s", R_cols, multiplier, num);

      total0 += multiplier*R_size;
		int vectors = 0;
		vectors += get_type (R_row, n_states) * n_states; // row vector
		vectors += get_type (R_col, N_terms)  * N_terms;  // column vector
		OUT_TOT (multiplier*R_size, vectors, mark);
      return  (multiplier*R_size + vectors);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

int   PGCreateTables::MRG_ROWE1B (char **matrix, int n_terms, int *row, int n_states)
{
		char *buffer;
      int  s, i, r, nr;

      nr = 1;
      ALLOC (buffer, n_terms);
      ALLOC (matrix[0], n_terms);
      memset (matrix[0], 0, n_terms);  // Lay row of zeros. 

      for (s = 0; s < n_states; s++)
      {
         memset (buffer, 0, n_terms);  // Initialize to zeros. 
         for (i = tt_start[s]; i < tt_start[s+1]; i++)
         {
				if (tt_action[i] != 0) buffer [tt_symb[i]] = 1; 
         }
         for (r = 0; r < nr; r++)
         {
            if (memcmp (buffer, matrix[r], n_terms) == 0) goto Old;
         }
         ALLOC (matrix[nr], n_terms);
         memcpy (matrix[nr], buffer, n_terms);
         nr++;
Old:     row [s] = r;
      }

      FREE (buffer, n_terms);
      return (nr);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

int   PGCreateTables::MRG_ROWE2R (int **matrix, int n_terms, int *row, int n_states)
{
      int s, i, r, nr, *buffer;

      nr = 0;
      ALLOC (buffer, n_terms);
      for (s = 0; s < n_states; s++) // For all states.
      {						
			if (D_red[s] > 0) // Default reduction for this state?  
			{
				if (optn[PG_DEFAULTRED]) 
				{ 
					row[s] = 0; 
					goto Cont; 
				}
				FASTINI (D_red[s], buffer, n_terms); // Fill in with the default.
			}
			else // Use the list of reductions and terminal symbols. 
			{
			  	FASTINI (0, buffer, n_terms); // Initialize to 0 (error).
				for (i = la_start[s]; i < la_start[s+1]; i++)
				{
	            buffer [la_symb[i]] = la_red[i]; // Place reduction in buffer.
				}
			}
         for (r = 0; r < nr; r++)
         {
            if (FASTCMP (buffer, matrix[r], n_terms)) goto Old;
         }
         ALLOC (matrix[nr], n_terms);
         FASTCPY (buffer, matrix[nr], n_terms);
         nr++;
Old:     row [s] = r;
Cont:    continue;
      }
      FREE (buffer, n_terms);
      return (nr);
}
  
/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //
//    Newer version (faster)

int   PGCreateTables::MRG_ROWE2T (int **matrix, int n_terms, int *row, int n_states, int opt)
{
		uint hash, n_cells, hash_divide;
      int  nr, nc, nt, s, t, i, r, *buffer, *vector, value, cell, *count, c;

      nr = 0;
      ALLOC (buffer, n_terms);
      ALLOC (count,  n_states);

		n_cells = 2*n_states;
		ALLOC (vector, n_cells);
		FASTINI (-1, vector, n_cells);
		hash_divide = (uint)0xFFFFFFFF / n_cells + 1;

	//	nc = 0; // number of collisions = 0.
      for (s = 0; s < n_states; s++) // For all states ...
      {
			count[s] = 0;
			hash = MAX_INT;
			for (t = 0; t < n_terms; t++) buffer[t] = 0;	 
			for (i = tt_start[s]; i < tt_start[s+1]; i++)
			{
				if (tt_action[i] != 0)
				{
					count[s]++;
					t = tt_symb[i];				
					buffer[t] = tt_action[i];				
			  		hash += tt_action[i]*i;
				}
			}
			cell = hash % n_cells; 			// Get first cell.
			r = vector [cell];					// Get symbol index.
			while (r >= 0)
			{
				if (count[s] != count[r]) goto Cont;
  				for (i = tt_start[s]; i < tt_start[s+1]; i++) // Compare rows.
				{
					if (tt_action[i] != 0)
					{
						t = tt_symb[i];		
						if (buffer[t] != matrix[r][t]) goto Cont;
					}
				}
				goto Old;
Cont:			cell = (hash *= 65549) / hash_divide;	// Get new cell number.
				r = vector[cell];		   					// Get row index.
			//	nc++;	// number of collisions.
			}
			vector[cell] = nr;
         r = nr++;
			ALLOC (matrix[r], n_terms); // Create a new row ...
			for (t = 0; t < n_terms; t++)
			{
				matrix[r][t] = buffer[t];
			}
Old:     row[s] = r; // Define row pointer.	  
      }

      FREE (buffer, n_terms);	 
      FREE (count,  n_states);	 
		FREE (vector, n_cells);
      return (nr);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

int   PGCreateTables::MRG_ROWZ_T (int **matrix, int n_terms, int *row, int n_states, int opt)
{
      int *density, *indx;
      int s, i, r, nr, t, x, v;

      if (opt)
      {
			ALLOC (indx, n_states);
         ALLOC (density, n_states);
			for (s = 0; s < n_states; s++) indx [s] = s;
         for (s = 0; s < n_states; s++)
         {
            t = tt_start[s+1] - tt_start[s];
            if (opt == 1) density [s] = -t;  /* Maximize Number of Columns. */
            else          density [s] =  t;  /* Minimize Number of Columns. */
         }
         SORT2 (density, indx, n_states);
         FREE (density, n_states);
      }
      nr = 0;
      for (x = 0; x < n_states; x++) // For all states.
      {
         if (opt) s = indx[x]; else s = x;
         for (r = 0; r < nr; r++) // For all rows defined.
         {
            for (i = tt_start[s]; i < tt_start[s+1]; i++)
            {
					if (tt_action[i] != 0)
               {
                  v = *(matrix[r]  + tt_symb[i]);
                  if (v != 0 && v != tt_action[i]) goto Next;
               }
            }
            goto Old;
Next:       continue;
         }
         ALLOC (matrix[nr], n_terms);
         FASTINI (0, matrix[nr], n_terms);
         nr++;
Old:     row [s] = r;
         for (i = tt_start[s]; i < tt_start[s+1]; i++)
         {
				if (tt_action[i] != 0) 
            {
               *(matrix[r] + tt_symb[i]) = tt_action[i];
            }
         }  
      }
      if (opt) FREE (indx, n_states);
      return (nr);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

int   PGCreateTables::MRG_ROWZ_N (int **matrix, int n_heads, int *row, int n_states, int opt)
{
      int *density, *indx;
      int s, i, r, nr, t, x, v;

      if (opt)
      {
			ALLOC (indx, n_states);
         ALLOC (density, n_states);
			for (s = 0; s < n_states; s++) indx [s] = s;
         for (s = 0; s < n_states; s++)
         {
            t = ntt_start[s+1] - ntt_start[s];
            if (opt == 1) density [s] = -t;  /* Maximize Number of Columns. */
            else          density [s] =  t;  /* Minimize Number of Columns. */
         }
         SORT2 (density, indx, n_states);
         FREE (density, n_states);
      }
      nr = 0;
      for (x = 0; x < n_states; x++) // For all states.
      {
         if (opt) s = indx[x]; else s = x;
         for (r = 0; r < nr; r++) // For all rows defined.
         {
            for (i = ntt_start[s]; i < ntt_start[s+1]; i++)
            {
					if (ntt_action[i] != 0)
               {
                  v = *(matrix[r]  + ntt_symb[i]);
                  if (v != 0 && v != ntt_action[i]) goto Next;
               }
            }
            goto Old;
Next:       continue;
         }
         ALLOC (matrix[nr], n_heads);
         FASTINI (0, matrix[nr], n_heads);
         nr++;
Old:     row [s] = r;
         for (i = ntt_start[s]; i < ntt_start[s+1]; i++)
         {
				if (ntt_action[i] != 0) 
            {
               *(matrix[r] + ntt_symb[i]) = ntt_action[i];
            }
         }  
      }
      if (opt) FREE (indx, n_states);
      return (nr);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

int   PGCreateTables::MRG_COLE1B (int *col, int n_terms, int n_rows, char **matrix)
{
		char** newmat = NULL;
      int *ones = NULL, *newloc = NULL;
      int c, d, k, r, n_cols, i;

      n_cols = n_terms;
      ALLOC (ones, n_terms);
		ALLOC (B_mask, n_terms);
      for (c = 0; c < n_terms; c++) 
		{
			col[c] = c;
			ones[c] = 0;
		}
      for (c = 0; c < n_terms; c++) 
		{
			for (r = 0; r < n_rows; r++)
			{
				if (*(matrix[r]+c) == 1) ones[c]++;
			}
		}
      for (c = 1; c < n_terms; c++)
      {
         for (d = 0; d < c; d++)
         {
            if (col[d] == d // Not equal to previous column?
				&& ones[c] == ones[d]) // Same number of ones?
            {
               for (r = 0; r < n_rows; r++)
               {
                  if (*(matrix[r]+d) != *(matrix[r]+c)) goto nope;
               }
					col[c] = d;
               n_cols--;
               goto cont;
            }
nope:       continue;
         }
cont:    continue;
      }
		FREE (ones, n_terms);
      k = 0;
      ALLOC (newloc, n_terms);
      ALLOC (newmat, n_rows); 
      for (i = 0; i < n_rows; i++)
      {
         ALLOC (newmat[i], n_cols); 
      }
      for (c = 0; c < n_terms; c++)
      {
         if (col[c] == c)
         {
            for (r = 0; r < n_rows; r++)
            {
               *(newmat[r]+k) = *(matrix[r]+c);
            }
            newloc[c] = k++;
			}  
		}
      for (c = 0; c < n_terms; c++) 
		{
			col[c] = newloc[col[c]];
			B_mask[c] = 1 << (col[c] % 8);
			if (optn[PG_BOOLMATRIX] > 1) col[c] /= 8;
		}
      for (r = 0; r < n_rows; r++) memcpy (matrix[r], newmat[r], n_cols);
      for (i = 0; i < n_rows; i++) FREE (newmat[i], n_cols);
      FREE (newmat, n_rows);
      FREE (newloc, n_terms);
      return (n_cols);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

int   PGCreateTables::MRG_COLE2 (int *col, int n_terms, int n_rows, int **matrix)
{
      int c, d, k, r, n_cols, *newloc = NULL, **newmat = NULL, i;

      n_cols = n_terms;
      for (c = 0; c < n_terms; c++) col [c] = c;
      for (c = 1; c < n_terms; c++)
      {
         for (d = 0; d < c; d++)
         {
            if (col [d] == d)
            {
               for (r = 0; r < n_rows; r++)
               {
                  if (*(matrix [r] + d) != *(matrix [r] + c)) goto nope;
               }
					col[c] = d;
               n_cols--;
               goto cont;
            }
nope:       continue;
         }
cont:    continue;
      }
      k = 0;
      ALLOC (newloc, n_terms);
      ALLOC (newmat, n_rows);
      for (i = 0; i < n_rows; i++)
      {
         ALLOC (newmat[i], n_cols); 
      }
      for (c = 0; c < n_terms; c++)
      {
         if (col[c] == c)
         {
            for (r = 0; r < n_rows; r++)
            {
               *(newmat[r] + k) = *(matrix[r] + c);
            }
            newloc[c] = k++;
         }  
      }
      for (c = 0; c < n_terms; c++) col[c] = newloc[col[c]];
      for (r = 0; r < n_rows; r++)
      {
         FASTCPY (newmat[r], matrix[r], n_cols);
      }
      for (i = 0; i < n_rows; i++)
      {
         FREE (newmat[i], n_cols);
      }
      FREE (newmat, n_rows);
      FREE (newloc, n_terms);
      return (n_cols);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

int   PGCreateTables::MRG_COLZ (int *col, int n_cols, int n_rows, int **matrix, int opt)
{
      int *density, *indx, x, i;
      int c, d, k, r, new_cols, *newloc, **newmat;

      ALLOC (indx, n_cols);
      for (c = 0; c < n_cols; c++) indx [c] = c;

      if (opt)
      {
         ALLOC (density, n_cols);
         for (c = 0; c < n_cols; c++)
         {
            density [c] = 0;
            for (r = 0; r < n_rows; r++)
            {
               if (*(matrix[r]+c) != 0)
               {
                  if (opt == 1) density [c]--;
                  else density [c]++;
               }  
            }  
         }
         SORT2 (density, indx, n_cols);
         FREE (density, n_cols);
      }

      new_cols = n_cols;
      for (c = 0; c < n_cols; c++) col [c] = c;
      for (x = 0; x < n_cols; x++)
      {
         c = indx [x];
         for (d = 0; d < n_cols; d++)
         {
            if (d != c && col [d] == d)
            {
               for (r = 0; r < n_rows; r++)
               {
                  if (*(matrix[r]+c) != 0)
                  if (*(matrix[r]+d) != 0)
                  if (*(matrix[r]+d) != *(matrix[r]+c)) goto nope;
               }
               for (r = 0; r < n_rows; r++)
               {
                  *(matrix[r]+d) |= *(matrix[r]+c);
               }
               for (k = 0; k < n_cols; k++) if (col [k] == c) col [k] = d;
               new_cols--;
               goto cont;
            }
nope:       continue;
         }
cont:    continue;
      }
      k = 0;
      ALLOC (newloc, n_cols);
      ALLOC (newmat, n_rows); 
      for (i = 0; i < n_rows; i++)
      {
         ALLOC (newmat[i], new_cols); 
      }
      for (c = 0; c < n_cols; c++)
      {
         if (col[c] == c)
         {
            for (r = 0; r < n_rows; r++) 
            {
               *(newmat[r]+k) = *(matrix[r]+c);
            }
            newloc[c] = k++;
         }  
      }
      for (c = 0; c < n_cols; c++) col [c] = newloc [col [c]];
      for (r = 0; r < n_rows; r++) FASTCPY (newmat [r], matrix [r], new_cols);
      for (i = 0; i < n_rows; i++) FREE (newmat [i], new_cols);
      FREE (newmat, n_rows);
      FREE (indx,   n_cols);
      FREE (newloc,  n_cols);
      return (new_cols);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

int   PGCreateTables::DISP_EQ1B (char** matrix, char* newmat, int* row, int nr, int nc, int opt1)
{
      char* p;
      int*  base;
      int*  indx;
      int*  density;
      int   size, i, n;
      int   r, x, c, inc;

      ALLOC (base, nr);
		if (opt1)
		{
			ALLOC (indx, nr);
	      ALLOC (density, nr);
			for (r = 0; r < nr; r++)
			{
				indx[r] = r;
				p = matrix[r];
				if (opt1 == 1) /* Decreasing order of density, slower. */
				{
					density[r] = 0;
					for (c = 0; c < nc; c++) 
					{
		 				if (*p++ != 0) density[r]--;
					}  
				} 
				else if (opt1 == 2) /* Increasing order of density, faster. */
				{
					density[r] = 0;
					for (c = 0; c < nc; c++) 
		  			{
		  				if (*p++ != 0) density[r]++;
					}  
		  		}	
			}

			SORT2 (density, indx, nr);
			FREE (density, nr);
		}
      size = 0;
		inc  = 8;							// Faster
		if (optn[PG_MINIMIZE]) inc = 1;	// Slower
		if (optn[PG_BOOLMATRIX] > 1) inc = 8;	// Must be 8.
      for (x = 0; x < nr; x++)
      {
			if (opt1) r = indx[x];
			else		 r = x;
         for (i = 0; i < size; i += inc)
         {
            n = size - i;
            if (n > nc) n = nc;
            if (memcmp (matrix[r], newmat+i, n) == 0) break;
         }
         base[r] = i;
         if (i + nc > size)
         {
            size = i + nc;
            memcpy (newmat+i, matrix[r], nc);
         }  
      }
      for (i = 0; i < n_states; i++) 
		{
			row[i] = base[row[i]];
			if (optn[PG_BOOLMATRIX] > 1) row[i] /= 8;
		}
		if (opt1) FREE (indx, nr);
      FREE (base, nr);
      return ((size+7)/8*8);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

int   PGCreateTables::DISP_EQ2 (int **matrix, int *newmat, int *row, int nr, int nc, int n_states, int opt1)
{
      int  size, i, n;
      int  r, x, c, inc;
      int  *base, *p, *density, *indx;

      ALLOC (base, nr);
      ALLOC (indx, nr);
      for (r = 0; r < nr; r++) indx [r] = r;
		if (opt1)
		{
			ALLOC (density, nr);
			for (r = 0; r < nr; r++)
			{
				indx [r] = r;
				p = matrix [r];
				density [r] = 0; 
				if (opt1 == 1) // Must do this order for R_matrix. (why?)
					for (c = 0; c < nc; c++) if (*p++ != 1) density [r]--; // Do most dense rows first. 
				else if (opt1 == 2)
					for (c = 0; c < nc; c++) if (*p++ != 1) density [r]++; // Do most dense rows last. 
			}
			SORT2 (density, indx, nr);
			FREE (density, nr);
		}

		inc = 1;
      size = 0;
      for (x = 0; x < nr; x++)
      {
         r = indx [x];
			for (i = 0; i < size; i += inc)
			{
				n = size - i;
				if (n > nc) n = nc;
				if (FASTCMP (newmat+i, matrix[r], n)) break;
			}
         base [r] = i;
         if (i + nc > size)
         {
            size = i + nc;
            FASTCPY (matrix[r], newmat+i, nc);
         }  
      }

      for (i = 0; i < n_states; i++) 
		{
			row [i] = base [row [i]];
		}
      FREE (indx, nr);
      FREE (base, nr);
      return (size);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

int   PGCreateTables::DISP_ZEQ (int **matrix, int *newmat, int *row, int nr, int nc, int n_states)
{                                                                       
      int  r, c, x, i, j, size;
      int  *p, *base, *density, *indx;

      ALLOC (base, nr);
      ALLOC (indx, nr);
      ALLOC (density, nr);
      for (r = 0; r < nr; r++)
      {
         indx [r] = r;
         density [r] = 0;
         p = matrix [r];
         if (optn [PG_MINIMIZE]) /* Slower. */
         {
            for (c = 0; c < nc; c++) if (*p++) density[r]--; // Most dense rows first
         }
         else /* Faster but slightly larger. */
         {
            for (c = 0; c < nc; c++) if (*p++) density[r]++; // Least dense rows first
		   }  
		}
      SORT2 (density, indx, nr);
      FREE (density, nr);

      size = 0;
      for (x = 0; x < nr; x++)
      {
         r = indx [x];
         for (i = 0; i < size; i++)
         {
				for (j = 0; j < nc; j++)
				{
				  	if ( matrix[r][j] == 0) continue;
					if ((newmat+i)[j] == 0) continue;
					if ( matrix[r][j] != (newmat+i)[j]) goto Next;   
				}
				goto Load;
Next:			continue;
         }
Load:    base [r] = i;
			for (j = 0; j < nc; j++)
			{
				(newmat+i)[j] |= matrix[r][j];
			}
         if (i + nc > size) size = i + nc;
      }

      for (i = 0; i < n_states; i++) row [i] = base [row [i]];
      FREE (indx, nr);
      FREE (base, nr);
      return (size);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

void  PGCreateTables::OUT_TOT (int size, int vectors, char *mark)
{
		char n1[12] = "           ";
		char n2[12] = "           ";
		char n3[12] = "           ";
		number (size, n1);  
		number (vectors, n2);  
		number (size+vectors, n3);  

      if (optn[PG_VERBOSE]) prt_log     (" ->%9s +%9s =%9s %s\n", n1, n2, n3, mark);
      else               prt_logonly (" ->%9s +%9s =%9s %s\n", n1, n2, n3, mark);

		total1 += size;
		total2 += vectors;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

void  PGCreateTables::MakeERI () /* Make Error Reduction Items lists. */
{
      int s, k, i, prod, dot;

      ALLOC (f_eri, (int)(n_states+1));
      ALLOC (eri_prod, n_kernels);
      ALLOC (eri_dot,  n_kernels);

      n_eris = 0;
      f_eri [0] = 0;
      for (s = 0; s < n_states; s++)
      {
         f_eri [s+1] = f_eri [s];
         for (k = f_kernel [s]; k < f_kernel [s+1]; k++)
         {
            i = kernel [k];
            if (item[i].symb != -32767) /* If not final item. */
            {
               prod = item[i].prod;
               dot  = item[i].dot;
               eri_prod [n_eris] = prod;
               eri_dot  [n_eris] = dot;
               f_eri [s+1] = ++n_eris;
            }
         }  
      }
}
   
///////////////////////////////////////////////////////////////////////////////////////////////////
//																																 //

int   PGCreateTables::get_type (int *x, int n) 
{
		int i, max = 0, min = 0;
		for (i = 0; i < n; i++)
		{
			if (x[i] > max) max = x[i];
			else if (x[i] < min) min = x[i];
		}
		if (min == 0)
		{
			if (max <         256) return (1); // 1 byte
			else if (max <  65536) return (2); // 2 bytes
			else                   return (4); // 4 bytes
		}
		else if (-min < max) // -(-127) < 128
		{
			if (max <         128) return (1);
			else if (max <  32768) return (2);
			else                   return (4);
		}
		else
		{
			if (min >        -128) return (1);
			else if (min > -32768) return (2);
			else                   return (4);
		}
		return (0); // never gets here.
}

//																																 //
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif
