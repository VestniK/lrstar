
@echo off
rem make lrstar ...

prompt $g$s
bin\lrstar  PG.grm PGParser.h.skl PGParser.h PGParser.cpp.skl PGParser.cpp /exp /k /g /c 	  
if errorlevel == 1 goto :end		 		  

bin\dfastar PG.lgr PGLexer.h.skl  PGLexer.h  PGLexer.cpp.skl  PGLexer.cpp  /tm /m /c
if errorlevel == 1 goto :end		

rem gcc *.cpp -lstdc++ -w -olrstar -DLRSTAR 

:end

                                        



                                        
					 