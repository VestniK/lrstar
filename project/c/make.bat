
rem  This make.bat must be in the same directory as the Visual Studio project files, 
rem  or else the error messages will not have the correct directory name preceeding 
rem  the file name, and clicking on the error messages will not work. 

@echo off
prompt $g$s
rem Process the grammar and generate the parser code. 

..\..\bin\lrstar c.grm ^
..\..\skl\parser.cpp.skl    c_parser.cpp ^
..\..\skl\parser.h.skl      c_parser.h  

..\..\bin\dfastar c ^
..\..\skl\lexer.cpp.skl     c_lexer.cpp ^
..\..\skl\lexer.h.skl       c_lexer.h  

rem gcc *.cpp -lstdc++ -oc_parser -w

					
