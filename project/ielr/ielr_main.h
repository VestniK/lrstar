
/* Original source code by Paul Mann with tabs = 3 */

		#ifdef MAIN										   
		#define EXTERN 
		#else 
		#define EXTERN extern					   
		#endif

	// Include Files ...
		#include "stdio.h"
	   #include "sys/types.h"									  
		#include "sys/stat.h"
      #include "stdlib.h"
      #include "string.h"
      #include "fcntl.h"
      #include "time.h"
      #include "io.h"

   // Defines ...
		#define  cbool     const bool
		#define  cchar     const char
		#define  cshort    const short
		#define  clong     const char
		#define  cuchar    const unsigned char
		#define  cushort   const unsigned short
      #define  uchar     unsigned char
      #define  ushort    unsigned short
      #define  ulong     unsigned long
      #define  uint      unsigned int

  		#define EOL_CHAR          10 // End Of Line marker.                    
		#define EOF_CHAR          26 // End Of File marker.                    
		#define BUFFSIZE     1024000 // Input buffer size in bytes.            

		class Input
		{
			public:
			static char* input_start;		// First byte of input area.              
			static char* input_end;			// Byte after input.                      
			static char* line_ptr[1];		// Pointers to input lines.
			static int   line_numb;			// Line number of input file.             
			static char* filename;			// Filename of input file.

			static int   init (char* fn);
			static void  term ();                       

			private:
			static int   filesize;
			static int   filedesc;
		};

		class Option
		{
			public:
				char  ast;        // Output abstract syntax tree.    
				char  debug;      // Debug mode.                          
				char  echo;       // Echo source code on screen.     
				char  help;       // Help mode for syntax error.     
				char  symbols;    // Output symbol names.            
				char  trace;      // Trace AST nodes during traversal.             
				char  verbose;    // Verbose mode.     
				
				int   init (int na, char* arg[]);
		};

  		class Output 
		{
			public:
				static char  filename[256]; // File name.  
				static FILE* filedesc;      // File descriptor.

				int   init (char*);
				void  term ();
		};

	// Global Objects ...
		EXTERN Option Option;
		EXTERN Input  Input;
		EXTERN Output Output;

   // Global Functions ...
		extern char* number (int x);

/*--- End of Header File. ---*/

