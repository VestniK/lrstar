
		#pragma once

		#include "PGCreateTables.h"
		#include "PGPrintHtml.h"
		#include "PGPrintStates.h"

   // Grammar Symbol Types ...
      #define TOKEN                 1
      #define TERMINAL              2
      #define NONTERMINAL           4
      #define TAIL                  8
      #define ARGUMENT             16
      #define CONSTANT             32
      #define GENERATED            64
      #define STRING              128
      #define RESERVED            256
      #define NODENAME            512
      #define TACTION            1024
      #define PACTION            2048
      #define NACTION            4096
      #define INTEGER            8192
      #define UNDECLARED        16384
      #define OPTIONSYM         32768
		#define OPERATOR			  65536

   	class PG	: public PGCreateTables, public PGPrintStates, public PGPrintHtml
		{
			public:
			static char** line_ptr;
			static char*  input_start;
			static char*  input_end;
			static int    Terminate (int rc);
			static int    Main (int na, char** arg);

			private:
			static int    Start (int na, char *arg []);
			static void   OutputLexicalSymbols ();
			static void   PrintGrammar ();
			static char*  slash_inside (char* term_name);
			static char*  slash_inside_keyword (char* term_name, char quote);
		};

