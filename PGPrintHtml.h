
		#include "PGCheckGrammar.h"

		class PGPrintHtml : public PGCheckGrammar
		{
			public:
			static void  PrintHtml();

			private:
			static int   ph_sym (int s, char *sp);
			static int   ph_head (int s, char *sp);
			static void  ph_spaces (int ns);
			static void  ph_arg (int i);
			static int   ph_gensym (int h, char *sp); 
			static int   ph_str (int s, char *p, char *sp); 
		};
