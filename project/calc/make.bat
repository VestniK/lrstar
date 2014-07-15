
rem  This make.bat must be in the same directory as the Visual Studio project files, 
rem  or else the error messages will not have the correct directory name preceeding 
rem  the file name, and clicking on the error messages will not work. 

@echo off
prompt $g$s
rem Process the grammar and generate the parser code. 
      
..\..\bin\lrstar calc.grm ^
..\..\skl\parser.cpp.skl calc_parser.cpp ^
..\..\skl\parser.h.skl   calc_parser.h ^
/d	/s /g /lr
 
..\..\bin\dfastar calc.lgr ^
..\..\skl\lexer.cpp.skl  calc_lexer.cpp ^
..\..\skl\lexer.h.skl    calc_lexer.h ^
/line	
 
rem gcc *.cpp -lstdc++ -ocalc_parser -w
pause
