
@echo off
rem LG make ...

prompt $g$s
bin\lrstar  LG.grm LGParser.h.skl LGParser.h LGParser.cpp.skl LGParser.cpp /k /exp /c
if errorlevel == 1 goto :end		 		  
bin\dfastar LG.lgr LGLexer.h.skl  LGLexer.h  LGLexer.cpp.skl  LGLexer.cpp  /tm /m  
if errorlevel == 1 goto :end		 		  

rem PG make ...

prompt $g$s
bin\lrstar  PG.grm PGParser.h.skl PGParser.h PGParser.cpp.skl PGParser.cpp /exp /k /g /ct 	  
if errorlevel == 1 goto :end		 		  
bin\dfastar PG.lgr PGLexer.h.skl  PGLexer.h  PGLexer.cpp.skl  PGLexer.cpp  /tm /m 
if errorlevel == 1 goto :end		

rem gcc *.cpp -lstdc++ -olrstar 
:end

                                        



                                        
