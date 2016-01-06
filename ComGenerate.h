#pragma once

		#ifdef LRSTAR
		class Generate 
		#endif

		#ifdef DFASTAR
		#include "LGComputeLA.h"
class Generate : public LGComputeLA
#endif
{
public:
    bool GenerateCode(const char* sklfid, const char* outfid, int verbose);

protected:
    virtual void initialize() = 0; // TODO: use constructor Luke
    virtual int value(int x) = 0;
    virtual void emit(int x) = 0;
    virtual void emitStr(int x) = 0;
    virtual void emitType(int x) = 0;

private:
    static int open_code (const char* out_fid);
    static void prt_code(const char *format, ...);
			static void  	close_code ();
			static void  	KEYWORD_OPTIMIZATION();
			static void    DO_SWITCH (int s);
			static void	   DO_CASES (int s, int* action, int* seq); 
			static void	   DO_DEFAULT (int s, int ret);
			static void 	DO_RETURN (int s);
			static void  	P_SYMBOL (int s);
			static void  	PRT_STA (int s);
			static void  	p_prod (int p, int dot, char *before, char* after);
			static int	 	p_sym (int s, char *sp);

    void INIT_VARS();
    void EMIT_ALL(int verbose);
    void SCAN();
			static void	 	STAKCOND();		
			static void	 	UNSTAKCOND();	
    char* READ_CODE (char* sk);
			static char* 	GETCODENUM (char *keyword, int& x);
			static char  	GET_OPER (char* p, int x);
			static void	 	GET_STRINGS (char *p);
    void READ_VARS(char* p);
			static bool GET_STR (char* string);
			static int   	GET_NUM (int& num);
    void DEF_T (int *x, int n, int min, int max);
			static void  	SkipRestOfLineOrBlock (char *s);
			static void	 	SkipBlock ();
			static char* 	skip_rest_of_string (char* skel);
			static char* 	skip_rest_of_code (char* sk);
			static char* 	skip_rest_of_line (char* skel);
			static char* 	skip_rest_of_comment (char* skel);
			static void  	ERASE ();
			static int   	GET_STRING (char *string, int size);
    static int GETN(const char* p);
    static const char* READNUM (const char* p, int* num);
			static void  	EMIT_NUM (int num);

			static int   	FIRSTNB (char *p);
			static void  	DUMP_BUFFER ();
			static int   	OUTPUT (char* buffer, int leng);
			static void  	DUMP_SKEL (char* start, char* end, char* newstart);
			static void  	O_TERM_CONS (int* x, int n);
			static void  	O_DATA (int i, int* data, char* text, char* str);
			static void  	O_NA1 (char**s, int* npp, int n, int *seq);
			static void  	O_DEFCON (int *x, char**s, int n);
			static void  	O_ONLY (char**s, int n, char ch);
			static void  	O_ONLY2 (char**s, int n, char ch);
			static void  	O_KEYWRD (char**s, int n);
			static void  	O_SORTED (char**s, int n, int *seq);
			static void  	O_SORTED2(int *s, int n, int *seq);
    static const char* prt_line (int numb, const char* line);
			static char* 	GETFILESPEC ();
			static void  	ISEOL (char*str);

			static int   	N_NP1 (int *chk, int n);
			static int   	N_ONLY (char**s, int n, char ch);
			static int   	N_ONLY2 (char**s, int n, char ch);
			static int   	N_KEYWRD (char**s, int n);
			static void  	O_BITNO (char *x, int n);
			static void  	O_NP1 (char**s, int n, int *chk);
			static void  	O_NA1 (char**s, int n, int *seq, int *chk);
    static void prt_pointer(int numb, const char* line, const char* object);
protected:
			static int 	 	OUTPUT_CODE ();
    void DEF_TYPE(int* x, int n);
    void DEF_TYPEA(int n);
    void DEF_TYPEC(int n);
    void DEF_TYPES(int* x, int n);
			static void  	O_ARRAY (int *x, int n);
			static void  	O_ARRAY_CHAR (char *x, int n);
			static void  	O_NUMTXT (int *x, char**s, int n);
			static void  	O_NUMTXT2(int *x, char**s, int n);
			static void  	O_TERM (char**s, int n);
			static void  	O_HEAD (char**s, int n);
			static void  	O_TXT (char**s, int n);
			static void  	O_TXTN (char**s, int m, int n);
			static void  	O_TXTSTR (char**s, int n);
    static void PRT_ERR(const char *p, int line);
			static void  	PUT_FILE (char* string, int leng);
    static int SPRINT(const char* format, int indx, int *data, int *text);

protected:
			static char  	 skl_fid [MAX_PATH];
			static char  	 out_fid [MAX_PATH];
			static int   	 linenumb;
			static char* 	 skel;
			static int   	 n_addedlines;
			static char  	 format[50];

private:
    char str_char[32];
    char str_uchar[32];
    char str_short[32];
    char str_ushort[32];
    char str_int[32];
    char str_uint[32];
    char str_charp[32];
			static int    	 num_char;
			static int    	 num_uchar;
			static int    	 num_short;
			static int    	 num_ushort;
			static int    	 num_int;
			static int    	 num_uint;
			static int    	 num_charp;
			static STAKTYPE STAK[MAXTOP];
			static int   	 staktop;
			static int   	 maxtop;
			static char* 	 group_start;
			static int   	 skip_code;
			static int   	 g_size;
			static char  	 in_group;
			static int   	 width;
			static int   	 bytes_out;
			static int   	 first_err;
			static int   	 last_sep_l;
			static int   	 fd;
			static int   	 count;
			static int   	 mult;
			static int   	 plus;
			static char  	 middle[2000];
			static char  	 sep[2000];
			static char  	 end[2000];
			static char  	 arg4[30];
			static char  	 arg5[30];
			static char  	 arg6[30];
			static int   	 middle_l;
			static int   	 sep_l;
			static int   	 end_l;
			static int   	 n_out;
			static char* 	 skelbeg;
			static char* 	 skelend;
			static char* 	 buffer;
			static char* 	 buffptr;
			static char* 	 buffend;
			static int   	 operation_flag;
			static int   	 n_origlines;
			static int   	 max_outbuff;
};
