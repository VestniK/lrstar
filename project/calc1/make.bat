
rem  This make.bat must be in the same directory as the Visual Studio project files, 
rem  or else the error messages will not have the correct directory name preceeding 
rem  the file name, and clicking on the error messages will not work. 

@echo off
prompt $g$s
rem Process the grammars and generate the lexer and parser code. 
      
..\..\bin\lrstar calc1.grm ^
..\..\skl\parser.cpp.skl calc1_parser.cpp ^
..\..\skl\parser.h.skl   calc1_parser.h ^
/d /g /w 

..\..\bin\dfastar calc1.lgr ^
..\..\skl\lexer.cpp.skl  calc1_lexer.cpp ^
..\..\skl\lexer.h.skl    calc1_lexer.h 

rem gcc *.cpp -lstdc++ -ocalc1_parser -w
pause
