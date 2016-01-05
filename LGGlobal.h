
		#pragma once
		#include "LGCreateTables.h"
		#include "LGPrintStates.h"

   // State Types. 
      #define TT_STATE			1	// State has terminal transitions.
      #define NT_STATE			2	// State has nonterminal transitions.
      #define MR_STATE			4	// State has multiple reductions.
      #define RO_STATE			8	// State is a reduce-only state. 
      #define UN_STATE		  16	// State is unused, remove it.
      #define RR_CONFL  	  32	// State has reduce-reduce conflict(s).

   // Symbol Types. 
      #define TOKEN           1
      #define TERMINAL        2
      #define NONTERMINAL     4
      #define SETNAME         8
      #define SETNAMEREF     16
      #define LEXICON        32
      #define CONSTANT       64
      #define GENERATED     128
      #define STRING        256
      #define UNREACHABLE   512
      #define LEXFILE      1024
      #define IGNORESYM    2048
      #define OUTPUTSYM    4096
      #define ACTIONCODE   8192

      #define MAXPRODLENG	 256	// Maximum number of symbols per production.

		enum prodtype
		{
			PRODARROW,    
			PRODADD,      
			PRODSUB,      
			PRODTO
		};

		class LG : public LGCreateTables, public LGPrintStates
		{
			public:
			static int  Main (int na, char** arg);
			static int  Start (int na, char *arg []);
			static int  Terminate (int rc);
		};

