      
@echo off
 
prompt $g$s
rem Process the grammar and generate the lexer and parser code. 
      
..\..\bin\lrstar typedef.grm ^
..\..\skl\parser.cpp.skl typedef_parser.cpp ^
..\..\skl\parser.h.skl   typedef_parser.h ^
/d 

..\..\bin\dfastar typedef.lgr ^
..\..\skl\lexer.cpp.skl  typedef_lexer.cpp ^
..\..\skl\lexer.h.skl    typedef_lexer.h 

rem gcc *.cpp -lstdc++ -otypedef_parser -w
pause

