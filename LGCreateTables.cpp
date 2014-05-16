
#ifdef DFASTAR

		#include "ComGlobal.h"
		#include "LGCreateTables.h"

		int*   LGCreateTables::B_col;
		int    LGCreateTables::B_cols;
		char*  LGCreateTables::B_matrix;
		int*   LGCreateTables::B_row;
		int    LGCreateTables::B_rows;
		int    LGCreateTables::B_size;

		int*   LGCreateTables::T_col;
		int    LGCreateTables::T_cols;
		int*   LGCreateTables::T_matrix;
		int*   LGCreateTables::T_row;
		int    LGCreateTables::T_rows;
		int    LGCreateTables::T_size;

		int	 LGCreateTables::err_ret_numb;	

      static int    quiet;
      static int    T0, T1, T2;
      static int    total0, total1, total2;
		static int    T_total, N_total;	

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //

int   LGCreateTables::CreateTables ()
{
      char *m;
		int s, nx = 1;

		if (optn[LG_DIRECTCODE])
		{
			prt_log ("\n");
			return 1;
		}
		err_ret_numb = 0;
  	  	for (s = 0; s < n_states; s++)
		{
			if (D_red[s] > 0) 
			{
				D_red[s] = ret_numb[D_red[s]];
			}
		}
		tt_states = n_states; // pbm 20070907

      m = " ";
      total0 = total1 = total2 = 0;
		char* tablesize;
		if      (optn[LG_TABL_SMALL])  tablesize = "Small ";
		else if (optn[LG_TABL_MEDIUM]) tablesize = "Medium";
		else if (optn[LG_TABL_LARGE])  tablesize = "Large ";
		else                           tablesize = "??????";
      if (optn[LG_VERBOSE])
           prt_log     ("\n%6s      rows   cols          matrix        list       vect      total\n", tablesize);
      else prt_logonly ("\n%6s      rows   cols          matrix        list       vect      total\n", tablesize);
		if (optn[LG_TABL_SMALL])
		{
			BLD_B (0, m);
			BLD_T (0, 0, m);
		}
		else if (optn[LG_TABL_MEDIUM])
		{
			BLD_TDFA ();
		}
		else if (optn[LG_TABL_LARGE])
		{
			BLD_LARGE ();
		}
		else
		{
			InternalError (500);
		}
		char n1[16] = "               ";
		char n2[16] = "               ";
		char n3[16] = "               ";
		number (total1, n1);  
		number (total2, n2);  
		number (total1+total2, n3);  

      if (optn[LG_VERBOSE])
		     prt_log     ("Total                                     %9s +%9s =%9s\n\n", n1, n2, n3);
      else prt_logonly ("Total                                     %9s +%9s =%9s\n\n", n1, n2, n3);

      FREE (tt_start,   n_states+2);
      FREE (ntt_start,  n_states+2);
      FREE (tt_end,     n_states);
      FREE (ntt_end,    n_states);
      FREE (ntt_symb,   n_nonttran);
      FREE (ntt_action,   n_nonttran);
      FREE (la_start,   n_states+1);
      FREE (la_end,     n_states);
      FREE (la_red,     max_lookah);
      FREE (la_symb,    max_lookah);

	// Reduce production length by 1, so no "-1" adjustment is required in skl file.
		for (int p = 0; p < n_prods; p++) prod_len[p]--;
	//	gen_tabl_memory = memory_max;
		return (1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //

int   LGCreateTables::BLD_B (int opt1, char *mark) // Build Boolean Matrix. 
{
      int    r, org_size;
      char** Matrix;
      if (optn[LG_VERBOSE]) prt_log     ("B matrix");
      else               prt_logonly ("B matrix");

      ALLOC (B_row,  n_states);
      ALLOC (B_col,  n_terms);
      ALLOC (Matrix, n_states+1);

      B_rows = MRG_ROWE1B (Matrix, B_row, n_states);
      B_cols = MRG_COLE1B (B_col, B_rows, Matrix);
      B_size = B_rows * B_cols;
      total0 += B_size;

		char num[12] = "           ";
		number (B_size, num);  // Gives 9 digits.
      if (optn[LG_VERBOSE]) prt_log     ("x%5ld x 1 =%10s", B_cols, num);
      else               prt_logonly ("x%5ld x 1 =%10s", B_cols, num);

		org_size = (B_rows)*((B_cols+7)/8*8); 
      ALLOC (B_matrix, org_size);
      memset (B_matrix, 0, org_size);
      B_size = DISP_EQ1B (Matrix, B_matrix, B_row, B_rows, B_cols, opt1);
      for (r = 0; r < B_rows; r++) FREE (Matrix[r], n_terms);
      FREE (Matrix, n_states+1);
		int vectors = 0;
		REALLOC (B_matrix, org_size, B_size);
		vectors += get_type (B_row, n_states) * n_states; // row vector
		vectors += get_type (B_col, n_terms)  * n_terms;  // column vector
		OUT_TOT (B_size, vectors, mark);
  		if (optn[LG_INSENSITIVE])
		{
			for (int i = 'a'; i <= 'z'; i++)
			{
				B_col[i-32] = B_col[i]; // Copy action for lower to upper.
			}
		}
		return  (B_size + vectors);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //

int   LGCreateTables::BLD_T (int opt1, int opt2, char *mark) /* Build Terminal Matrix. */
{
      int **Matrix, r, s, multiplier, org_size;

      if (optn[LG_VERBOSE]) prt_log     ("T matrix");
      else               prt_logonly ("T matrix");
      ALLOC (T_row,  tt_states);
      ALLOC (T_col,  n_terms);
      ALLOC (Matrix, tt_states);    

	// T_rows = MRG_ROWE2T (Matrix, T_row, tt_states);	// Total time is slower with 'M' option turned on.
 	   T_rows = MRG_ROWZ (tt_start, tt_end, tt_action, tt_symb, Matrix, T_row, tt_states, n_terms, opt1); // Older code, much faster.
	   T_cols = MRG_COLZ (T_col, n_terms, T_rows, Matrix, opt2);
      org_size = T_rows * T_cols;

		if (n_prods > 32767 || tt_states > 32767) multiplier = 4; // int
		else if (n_prods >   127 || tt_states >   127) multiplier = 2; // short
		else                                      multiplier = 1; // char

		T_total = multiplier*org_size;
		char num[12] = "           ";
		number (T_total, num); // Gives 9 digits.
      if (optn[LG_VERBOSE]) prt_log     ("x%5ld x %d =%10s", T_cols, multiplier, num);
		else               prt_logonly ("x%5ld x %d =%10s", T_cols, multiplier, num);
		total0 += T_total;

      ALLOC (T_matrix, org_size);
      FASTINI (0, T_matrix, org_size);
		if (optn[LG_MINIMIZE])
		     T_size = DISP_ZEQ_SEQ (Matrix, T_matrix, T_row, T_rows, T_cols, tt_states); 
		else T_size = DISP_ZEQ_RAND(Matrix, T_matrix, T_row, T_rows, T_cols, tt_states); 
      for (r = 0; r < T_rows; r++) FREE (Matrix [r], n_terms);
      FREE (Matrix, tt_states);
      REALLOC (T_matrix, org_size, T_size);

		if (optn[LG_INSENSITIVE])
		{
			for (int i = 'a'; i <= 'z'; i++)
			{
				T_col[i-32] = T_col[i]; // Copy action for lower to upper.
			}
		}

		int vectors = 0;
		vectors += get_type (T_row, tt_states) * tt_states; // row vector
		vectors += get_type (T_col, n_terms)   * n_terms;   // column vector
		vectors += get_type (D_red, tt_states) * tt_states; // terminal vector
      OUT_TOT (multiplier*T_size, vectors, mark);
      return  (multiplier*T_size + vectors);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //

int   LGCreateTables::BLD_TDFA () /* Build Terminal DFA Matrix. */
{
      int **Matrix, r, s, multiplier, org_size, i;
      if (optn[LG_VERBOSE]) prt_log     ("T matrix");
	   else               prt_logonly ("T matrix");

      ALLOC (T_row,  tt_states);
      ALLOC (T_col,  n_terms);
      ALLOC (Matrix, tt_states);    

		if (optn[LG_TABL_LARGE]) 
		{
			T_rows = MRG_ROWE2T (Matrix, T_row, tt_states);
			for (i = 0; i < n_terms; i++) T_col[i] = i;
			T_cols = n_terms;
		}
		else 
		{
			T_rows = MRG_ROWE2T (Matrix, T_row, tt_states);
			T_cols = MRG_COLE2 (T_col, T_rows, Matrix);
		}
      org_size = T_rows * T_cols;

		if (tt_states > 65535) multiplier = 4; // int  
		else if (tt_states >   255) multiplier = 2; // ushort
		else                   multiplier = 1; // uchar

		T_total = multiplier*org_size;
		char num[12] = "           ";
		number (T_total, num); // Gives 9 digits.
      if (optn[LG_VERBOSE]) prt_log     ("x%5ld x %d =%10s", T_cols, multiplier, num);
		else               prt_logonly ("x%5ld x %d =%10s", T_cols, multiplier, num);
		total0 += T_total;

		ALLOC (T_matrix, org_size);
		FASTINI (0, T_matrix, org_size);
		if (optn[LG_MINIMIZE] > 0)
		     T_size = DISP_EQ2  (Matrix, T_matrix, T_row, T_rows, T_cols, tt_states, 0);
		else T_size = DISP_EQ2T (Matrix, T_matrix, T_row, T_rows, T_cols, tt_states);
		for (r = 0; r < T_rows; r++) FREE (Matrix [r], n_terms);
		FREE (Matrix, tt_states);
		REALLOC (T_matrix, org_size, T_size);

		if (optn[LG_INSENSITIVE])
		{
			for (int i = 'a'; i <= 'z'; i++)
			{
				T_col[i-32] = T_col[i]; // Copy action for lower to upper.
			}
		}

		int vectors = 0;
		vectors += get_type (T_row, tt_states) * tt_states; // row vector
		vectors += get_type (T_col, n_terms)   * n_terms;   // column vector
		vectors += get_type (D_red, tt_states) * tt_states; // terminal vector
      OUT_TOT (multiplier*T_size, vectors, "");
      return  (multiplier*T_size + vectors);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //

int   LGCreateTables::BLD_LARGE () /* Build Terminal Matrix for Large Tables. */
{
		char num[12] = "           ";
      int  r, s, multiplier, org_size, i;

      if (optn[LG_VERBOSE]) prt_log     ("T matrix");
	   else                  prt_logonly ("T matrix");

		T_rows = tt_states;
		T_cols = n_terms;
      org_size = T_rows * T_cols;
		if      (org_size > 65535) multiplier = 4;	// int  
		else if (org_size >   255) multiplier = 2;	// ushort
		else                       multiplier = 1;	// uchar
		T_total = multiplier*org_size;
		number (T_total, num); // Gives 9 digits.

      if (optn[LG_VERBOSE]) prt_log     ("  %6d ", T_rows);
		else                  prt_logonly ("  %6d ", T_rows);

      if (optn[LG_VERBOSE]) prt_log     ("x%5ld x %d =%10s", T_cols, multiplier, num);
		else                  prt_logonly ("x%5ld x %d =%10s", T_cols, multiplier, num);

		total0 += T_total;
		T_size = org_size;
		ALLOC (T_matrix, org_size);
		FASTINI (0, T_matrix, org_size);

      for (s = 0; s < n_states; s++)
      {
         for (int i = tt_start [s]; i < tt_end [s]; i++)
         {
				T_matrix [s*T_cols + tt_symb[i]] = tt_action[i]*T_cols; 
         }
			if (optn[LG_INSENSITIVE])
			{
				for (int i = 'a'; i <= 'z'; i++)
				{
					T_matrix [s*T_cols + i-32] = T_matrix [s*T_cols + i]; // Copy action for lower to upper.
				}
			}
      }

		int vectors = 0;
      OUT_TOT (multiplier*T_size, vectors, "");
      return  (multiplier*T_size + vectors);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //

int   LGCreateTables::MRG_ROWE1B (char **matrix, int *row, int n_states)
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
         for (i = tt_start [s]; i < tt_end [s]; i++)
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
      if (optn[LG_VERBOSE]) prt_log     ("  %6d ", nr);
		else               prt_logonly ("  %6d ", nr);
      return (nr);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //
//    Newer version (faster)

int   LGCreateTables::MRG_ROWE2T (int **matrix, int *row, int n_states)
{
		uint hash, n_cells, hash_divide;
      int  nr, nc, nt, s, t, i, r, *buffer, *vector, value, cell, *count, c;

      nr = 0;
      ALLOC (buffer, n_terms);
      ALLOC (count,  n_states);
		n_cells = 2*n_states;
		ALLOC (vector, n_cells);
		FASTINI (-1, vector, n_cells);
		hash_divide = UINT_MAX / n_cells + 1;

	//	nc = 0; // number of collisions = 0.
      for (s = 0; s < n_states; s++) // For all states ...
      {
			count[s] = 0;
			hash = MAX_INT;
			for (t = 0; t < n_terms; t++) buffer[t] = 0;	 
			for (i = tt_start[s]; i < tt_end[s]; i++)
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
  				for (i = tt_start[s]; i < tt_end[s]; i++) // Compare rows.
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
      if (optn[LG_VERBOSE]) prt_log     ("  %6d ", nr);
		else                  prt_logonly ("  %6d ", nr);
      return (nr);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //
  
int   LGCreateTables::MRG_ROWZ (int *start, int *end, int *list, int *symb, int **matrix, int *row, int n_states, int n_terms, int opt)
{
      int *density, *indx;
      int s, i, r, nr, t, x, v, *m;

		ALLOC (indx, n_states);
		for (s = 0; s < n_states; s++) indx [s] = s;

      if (opt)
      {
         ALLOC (density, n_states);
         for (s = 0; s < n_states; s++)
         {
            t = end [s] - start [s];
            if (opt == 1)
                 density [s] = -t;  /* Maximize Number of Columns. */
            else density [s] =  t;  /* Minimize Number of Columns. */
         }
         SORT2 (density, indx, n_states);
         FREE (density, n_states);
      }

      nr = 0;
      for (x = 0; x < n_states; x++) // For all states.
      {
         s = indx[x]; 
         for (r = 0; r < nr; r++) // For all rows defined.
         {
				m = matrix[r];
            for (i = start[s]; i < end[s]; i++)
            {
					if (list[i] == 0) continue;
					v = *(m + symb[i]);
					if (v ==       0) continue;
					if (v != list[i]) goto Next;
            }
            goto Old;
Next:       continue;
         }
         ALLOC (matrix[nr], n_terms);
			m = matrix[nr];
         for (i = 0; i < n_terms; i++) 
			{
				*(m+i) = 0;
			}
         nr++;
Old:     row [s] = r;
			m = matrix[r];
         for (i = start [s]; i < end [s]; i++)
         {
				if (list[i] != 0) *(m + symb[i]) = list[i];
         }  
      }

      if (optn[LG_VERBOSE]) prt_log     ("  %6d ", nr);
      else               prt_logonly ("  %6d ", nr);
      FREE (indx, n_states);
      return (nr);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //
  
int   LGCreateTables::MRG_COLE1B (int *col, int n_rows, char **matrix)
{
		char** newmat = NULL;
      int *ones, *new_col;
      int c, d, k, r, n_cols, i;

      n_cols = n_terms;
      ALLOC (ones, n_terms);
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
      ALLOC (new_col, n_terms);
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
            new_col[c] = k++;
			}  
		}
      for (c = 0; c < n_terms; c++) 
		{
			col[c] = new_col[col[c]];
		}
      for (r = 0; r < n_rows; r++) memcpy (matrix[r], newmat[r], n_cols);
      for (i = 0; i < n_rows; i++) FREE (newmat[i], n_cols);
      FREE (newmat, n_rows);
      FREE (new_col, n_terms);
      return (n_cols);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //
  
int   LGCreateTables::MRG_COLE2 (int *col, int n_rows, int**& matrix)
{
      int c, d, i, k, r, n_cols, *new_col, **old_matrix;

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

		old_matrix = matrix;		// Save the old matrix ptr.
      ALLOC (matrix, n_rows); // Allocate new matrix space
      for (r = 0; r < n_rows; r++)
      {
         ALLOC (matrix[r], n_cols);		 // Allocate new matrix space
      }

	// Move the old into the new. 
      k = 0;
      ALLOC (new_col, n_terms);
      for (c = 0; c < n_terms; c++)
      {
         if (col[c] == c) // if column not moved.
         {
            for (r = 0; r < n_rows; r++)
            {
               *(matrix[r] + k) = *(old_matrix[r] + c);
            }
            new_col[c] = k++;
         }  
      }
	// Define the new column locations.
      for (c = 0; c < n_terms; c++) 
		{
			col[c] = new_col [col[c]];
		}
	// Free the old matrix.
      for (r = 0; r < n_rows; r++)
      {
         FREE (old_matrix[r], n_terms); // Free the old rows.
      }
      FREE (old_matrix, n_rows);
      FREE (new_col, n_terms);
      return (n_cols);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //

int   LGCreateTables::MRG_COLZ (int *col, int n_cols, int n_rows, int **matrix, int opt)
{
      int *density, *indx, *m, x, i;
      int c, d, k, r, new_cols, *new_col, **newmat;

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
						m = matrix[r];
                  if (*(m+c) == 0) continue;
                  if (*(m+d) == 0) continue;
                  if (*(m+d) != *(m+c)) goto nope;
               }
               for (r = 0; r < n_rows; r++)
               {
						m = matrix[r];
                  *(m+d) |= *(m+c);
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
      ALLOC (new_col, n_cols);
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
            new_col[c] = k++;
         }  
      }
      for (c = 0; c < n_cols; c++) 
		{
			col[c] = new_col[col[c]];
		}
		for (r = 0; r < n_rows; r++) 
		{
			for (c = 0; c < new_cols; c++)
			{
				matrix[r][c] = newmat[r][c];
			}
		}
      for (i = 0; i < n_rows; i++) 
		{
			FREE (newmat [i], new_cols);
		}
      FREE (newmat,  n_rows);
      FREE (indx,    n_cols);
      FREE (new_col, n_cols);
      return (new_cols);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //

int   LGCreateTables::DISP_EQ1B (char** matrix, char* newmat, int* row, int nr, int nc, int opt1)
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
		inc  = 1;							
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
		}
		if (opt1) FREE (indx, nr);
      FREE (base, nr);
      return ((size+7)/8*8);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //

int   LGCreateTables::DISP_EQ2T (int** matrix, int* newmat, int* row, int nr, int nc, int n_states)
{
      int  s, r, size, *base;

      ALLOC (base, nr);
		for (r = 0; r < nr; r++) 
		{
			base[r] = r*nc;
			FASTCPY (matrix[r], newmat + base[r], nc);
		}
      for (s = 0; s < n_states; s++) row [s] = base [row [s]];
      FREE (base, nr);
		return (nr*nc);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //

int   LGCreateTables::DISP_EQ2 (int **matrix, int *newmat, int *row, int nr, int nc, int n_states, int opt1)
{
      int  size, i, n;
      int  r, x, c, inc;
      int  *base, *p, *density, *indx;

      ALLOC (base, nr);
/*    ALLOC (indx, nr);
      for (r = 0; r < nr; r++) indx [r] = r;
		if (opt1) // Not used (waste of time?)
		{
			ALLOC (density, nr);
			for (r = 0; r < nr; r++)
			{
				indx [r] = r;
				p = matrix [r];
				density [r] = 0; 
				if (opt1 == 1) 
					for (c = 0; c < nc; c++) if (*p++ != 1) density [r]++; // Do most dense rows last. 
				else if (opt1 == 2)
					for (c = 0; c < nc; c++) if (*p++ != 1) density [r]--; // Do most dense rows first. 
			}
			SORT2 (density, indx, nr);
			FREE (density, nr);
		}	*/

      size = 0;
		inc = optn[LG_MINIMIZE]; // 20100409 pbm
		if (inc > nc) inc = nc;
      for (x = 0; x < nr; x++)
      {
         r = x;
      // r = indx [x];
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
   // FREE (indx, nr);
      FREE (base, nr);
      return (size);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //
//    New version using random placement, works great! Fast! (20100412 pbm) 

int   LGCreateTables::DISP_ZEQ_RAND (int **matrix, int *newmat, int *row, int nr, int nc, int n_states)
{                                                                       
      int  r, c, x, i, j, size, *base, n;
		uint hash_divide, n_cells, hash;

      ALLOC (base, nr);
		i = 0;
      size = 0;
		hash = 32767/3;
	//	printf ("\n");
      for (r = 0; r < nr; r++)
      {
			n = 0;
Loop:		for (j = 0; j < nc; j++)
			{
			  	if ( matrix[r][j] == 0) continue;
				if ((newmat+i)[j] == 0) continue;
				if ( matrix[r][j] != (newmat+i)[j])
				{
					if (++n > (size-nc)/nc) 
					{
						i = size; // put at end.
						break;
					}
				//	printf ("%5d ", i);
					i = (hash *= 65549) / hash_divide; 
					goto Loop;
				}
			}
         base [r] = i;
		//	printf ("\nputting at %d\n", i);
			for (j = 0; j < nc; j++)
			{
				(newmat+i)[j] |= matrix[r][j];
			}
         if (i + nc > size) 
			{
				size = i + nc;
			   int div = size - nc;
				if (div < nc) div = nc;   
	    		hash_divide = UINT_MAX / div + 1;
			}
			i = (hash *= 65549) / hash_divide; 
      }
      for (i = 0; i < n_states; i++) row [i] = base [row [i]];
      FREE (base, nr);
      return (size);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //
//    ORIGINAL VERSION  

int   LGCreateTables::DISP_ZEQ_SEQ (int **matrix, int *newmat, int *row, int nr, int nc, int n_states)
{                                                                       
      int  r, c, x, i, j, size, *p, *base, *density, *indx, inc, n;

      ALLOC (base, nr);
/*    ALLOC (indx, nr);
      for (r = 0; r < nr; r++) indx[r] = r; */
/*  	if (optn[LG_MINIMIZE]) 
		{
			ALLOC (density, nr);
			for (r = 0; r < nr; r++)
			{
				density [r] = 0;
				p = matrix [r];
			  	for (c = 0; c < nc; c++) if (*p++) density [r]--; // smaller?  most dense rows first.
			// for (c = 0; c < nc; c++) if (*p++) density [r]++; // larger?	most dense rows last.
			}
		  	SORT2 (density, indx, nr);
			FREE (density, nr);
		}	*/
		inc = nc;
      size = 0;
		if (optn[LG_MINIMIZE]) 
		{
			inc = optn[LG_MINIMIZE]; // 20100409 pbm
			if (inc > nc) inc = nc;
		}
      for (r = 0; r < nr; r++)
      {
      // r = indx [x];
			for (i = 0; i < size; i += inc)
			{
				n = size - i;
				if (n > nc) n = nc;
				for (j = 0; j < n; j++)
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
   // FREE (indx, nr);
      FREE (base, nr);
      return (size);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //

void  LGCreateTables::OUT_TOT (int size, int vectors, char *mark)
{
		char n1[12] = "           ";
		char n2[12] = "           ";
		char n3[12] = "           ";
		number (size, n1);  
		number (vectors, n2);  
		number (size+vectors, n3);  
      if (optn[LG_VERBOSE]) prt_log     (" ->%9s +%9s =%9s %s\n", n1, n2, n3, mark);
      else               prt_logonly (" ->%9s +%9s =%9s %s\n", n1, n2, n3, mark);
		total1 += size;
		total2 += vectors;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //

int   LGCreateTables::get_type (int *x, int n) 
{
		int i, max = 0, min = 0;
		for (i = 0; i < n; i++)
		{
			if (x[i] > max) max = x[i];
			else if (x[i] < min) min = x[i];
		}
		if (min == 0)
		{
			if (max <        256) return (1); // 1 byte
			else if (max <      65536) return (2); // 2 bytes
			else                  return (4); // 4 bytes
		}
		else if (-min < max) // -(-127) < 128
		{
			if (max <        128) return (1);
			else if (max <      32768) return (2);
			else                  return (4);
		}
		else
		{
			if (min >       -128) return (1);
			else if (min >     -32768) return (2);
			else                  return (4);
		}
		return (0); // never gets here.
}

#endif

//                                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////////////////////




