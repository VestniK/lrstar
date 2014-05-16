
@echo off
rem make dfastar ...

prompt $g$s
bin\lrstar  LG.grm LGParser.h.skl LGParser.h LGParser.cpp.skl LGParser.cpp /k /exp /c /g /s /d
if errorlevel == 1 goto :end		 		  

bin\dfastar LG.lgr LGLexer.h.skl  LGLexer.h  LGLexer.cpp.skl  LGLexer.cpp  /tm /m /g /c
if errorlevel == 1 goto :end		 		  

rem gcc *.cpp -lstdc++ -w -odfastar -DDFASTAR

:end

                                        



                                        
