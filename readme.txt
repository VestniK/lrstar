
How to compile LRSTAR and DFASTAR from the source code.

Visual C++ workspaces are already defined for this.  
If you are not using Windows or Visual C++, do the following:

1. Specify either UNIX or WINDOWS:

	#define UNIX (in ComGlobal.h)
or /D UNIX on the compile command line

	#define WINDOWS (in ComGlobal.h)
or /D WINDOWS on the compile command line

2. Specify:

	#define LRSTAR (in ComGlobal.h)
or /D LRSTAR on the compile command line. 

3. Compile all the code (*.cpp) to make lrstar.exe 

4. Specify:

	#define DFASTAR (in ComGlobal.h)
or /D DFASTAR on the compile command line. 

5. Compile all the code (*.cpp) to make dfastar.exe


Notes: 

A. Do not specify both WINDOWS and UNIX.
B. Do not specify both LRSTAR and DFASTAR.

