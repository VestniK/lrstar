
@echo off
prompt $g$s
rem Process the grammars and generate the lexer and parser code. 
      
..\..\bin\lrstar lr1.grm ^
..\..\skl\parser.cpp.skl lr1_parser.cpp	^
..\..\skl\parser.h.skl   lr1_parser.h	^
/d 

..\..\bin\dfastar lr1.lgr ^
..\..\skl\lexer.cpp.skl  lr1_lexer.cpp	^
..\..\skl\lexer.h.skl    lr1_lexer.h	

rem gcc *.cpp -lstdc++ -olr1_parser -w
:end	
pause
