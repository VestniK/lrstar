
@echo off
prompt $g$s
rem Process the grammars and generate the lexer and parser code. 
      
..\..\bin\lrstar ielr.grm ^
..\..\skl\parser.cpp.skl ielr_parser.cpp ^
..\..\skl\parser.h.skl   ielr_parser.h ^
/d /nd=2	
						   												
..\..\bin\dfastar ielr.lgr ^
..\..\skl\lexer.cpp.skl  ielr_lexer.cpp ^
..\..\skl\lexer.h.skl    ielr_lexer.h  
						   												
rem gcc *.cpp -lstdc++ -oielr_parser -w
pause
