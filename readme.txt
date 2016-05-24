
HOW TO COMPILE LRSTAR & DFASTAR FROM THE SOURCE CODE.

The source code in LRSTAR 6.3.012 compiles without error
when compiling with GCC 4.6.1.  This is based on my 
experience on Windows as of March 28 2014.  If not using 
the option "-w", many warning messages will be displayed, 
but these are not a problem.  

---------------------------------------------------------

ON WINDOWS SYSTEMS:

   Visual C++ workspaces are already defined for this in 
   the "source" directory.  

   If you are not using Visual C++, do the following:

   1. Specify WINDOWS and LRSTAR on the command line:

      /D WINDOWS /D LRSTAR

      Compile all the code (*.cpp) to make lrstar.exe 

   2. Specify WINDOWS and DFASTAR on the command line:

      /D WINDOWS /D DFASTAR 

      Compile all the code (*.cpp) to make dfastar.exe

---------------------------------------------------------

ON UNIX/LINUX SYSTEMS:

	For compiling with GCC:

   1. Specify UNIX and LRSTAR on the command line:

      gcc *.cpp -lstdc++ -w -DUNIX -DLRSTAR -olrstar

   2. Specify UNIX and DFASTAR on the command line:

      gcc *.cpp -lstdc++ -w -DUNIX -DDFASTAR -odfastar

---------------------------------------------------------

ON MAC OS X SYSTEMS:

	I don't know about this, sorry.

---------------------------------------------------------