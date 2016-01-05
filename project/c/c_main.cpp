
/* Original source code with tabs = 3 */

	// Include Files ...
		#include "stdio.h"
	   #include "sys/types.h"									  
		#include "sys/stat.h"
      #include "stdlib.h"
      #include "string.h"
      #include "fcntl.h"
      #include "time.h"
      #include "io.h"

      #include "c_parser.h"    

		#ifdef  _DEBUG
  		#include "conio.h"
		#endif																						 

   // Defines ...
      #define  uchar     unsigned char
      #define  ushort    unsigned short
      #define  uint      unsigned int
      #define  ulong     unsigned long

  		#define EOL_CHAR          10 // End Of Line marker.                    
		#define EOF_CHAR          26 // End Of File marker.                    

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
				char   ast;        // Output abstract syntax tree.    
				char   debug;      // Debug mode.                          
				char   echo;       // Echo source code on screen.     
				char   help;       // Help mode for syntax error.     
				char   symbols;    // Output symbol names.            
				char   trace;      // Trace AST nodes during traversal.             
				char   verbose;    // Verbose mode.     
				
				int    init (int na, char* arg[]);
		};

  		class Output 
		{
			public:
				static char  filename[256]; // File name.  
				static FILE* filedesc;      // File descriptor.

				int    init (char*);
				void   term ();
		};

		Option Option;
		Input  Input;
		Output Output;
		void   quit();
		char*  number (int x);

		char* Input::input_start;    // First byte of input area.              
		char* Input::input_end;      // Byte after input.                      
		char* Input::filename;       // Filename of input file.
		int   Input::filedesc;		 
		int   Input::filesize;

		char  Output::filename[];      // Filename of output file.
		FILE* Output::filedesc;      // File descriptor.

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 // 
//    main     																				  

int   __cdecl main (int na, char *arg[])
{
      int t, time1, time2, thou, sec, nlps, nl, rc;
   
		rc = 0;
		nl = 0;
		if (!Option.init (na, arg)) { rc = 1; goto Ret; }
		
		printf  ("\nC_parser\n\n");
	   time1 = clock();	// Get start time.

	// Open and read the input file 1000 times ...
	   for (int i = 0; i < 1000; i++) 
		{
			if (!Input.init  (arg[1]) ) { rc = 2; goto Ret; }
			if (!Output.init (arg[2]) ) { rc = 3; goto Ret; }
			if (!c_parser::init (Input.filename, Output.filedesc, 1000, 20000)) { rc = 4; goto Ret; }
			int n = c_parser::parse (Input.input_start);
		  	if (n <= 0) 
			{
				nl += -n;
				break;
			}
			nl += n;	  										        
			c_parser::term();
			Input.term ();
			Output.term();
		}
		time2 = clock (); // Get end time.

		t = time2 - time1;
		if (t == 0) t = 1;
		nlps = CLOCKS_PER_SEC * (nl/t);
      thou = t * 1000 / CLOCKS_PER_SEC;
      sec  = thou / 1000;
      thou -= sec * 1000;

	// Print results ...
      printf  ("%6d.%03ld seconds.\n", sec, thou);
      printf  ("%10s lines per second.\n\n", number(nlps));

Ret:  printf ("Return code = %d\n", rc);
		#ifdef _DEBUG
	  	printf ("Press any key to continue ...\n");
	  	while (!_kbhit());
		#endif
		return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 // 
//    init - initialization.     														  

int   Option::init (int na, char* arg[])
{
      if (na < 3)
      {
         printf ("c_parser\n\n");
         printf ("parse <input file> <output file>\n");
         printf ("\n");
			return 0;
      }    
		return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 // 
//    init - initialization of input.													  

int   Input::init (char* fn)
{
      int  nb;												// Number of bytes read.
		filename = fn;
		filedesc = _open (filename, 0);           // Open the file.             
		if (filedesc < 0)                         // If open error.             
		{
			printf ("File %s not found.\n", filename);
			return 0;
		}
		filesize = _filelength (filedesc);				// Get filesize.             
		input_start = new char [filesize+6];			// Get some RAM space.       
		*input_start++ = '\n';								// Put <eol> at beginning. 
		
      nb = _read (filedesc, input_start, filesize);	// Read size bytes into buffer. 
      if (nb <= 0)												// If read error.               
      {
         printf ("Read error on file %s.\n", filename);
         return 0;
      }
      input_end = input_start + nb;				// Set end-of-buffer pointer.         
      *input_end++ = EOL_CHAR;					// Put end-of-line here.
      *input_end++ = EOF_CHAR;					// Put first <eof> here.
      *input_end++ = EOF_CHAR;					// Put second <eof> here. 
      *input_end++ = EOL_CHAR;					// Put end-of-line here.
      *input_end++ = 0;								// Put zero byte here. 
      _close (filedesc);							// Close file.                        

		return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 // 
//    term - termination of input. 														  

void  Input::term ()
{
		input_start--;
  		delete [] input_start;                  
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 // 
//    init - initialization.     														  

int   Output::init (char* fn)
{
		strcpy (filename, fn);        
		filedesc = fopen (filename, "w");  

      if (filedesc == NULL)
      {
         printf ("\n\nOutput file %s cannot be created.\n\n", filename);
         return 0;
      }
		return 1;
}
                                        
/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 // 
//    term - termination.         														   

void  Output::term ()
{
      fclose (filedesc);  
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 // 
//    number																					  

char* number (int x)
{
		int i, j, k;
		char buff[16];
		static char string[12] = "           ";

		_itoa (x, buff, 10);
		k = strlen (buff);
		i = k + (k-1)/3;
		string[i--] = 0;
		j = 0;
		while (1)
		{
			string[i--] = buff[--k];
			if (i < 0) break;
			if (++j % 3 == 0) string[i--] = ',';
		}
		return (string);
}

//                                                                                                 // 
/////////////////////////////////////////////////////////////////////////////////////////////////////

