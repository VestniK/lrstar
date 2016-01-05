
/* Source code by Paul Mann with tabs = 3 */

#ifndef cActions_H
#define cActions_H

		class calc_token_action : public calc_parser
		{
			public:	
			static int goal_       (int&); 
			static int stack       (int&); 
         static int error       (int&);
         static int lookup      (int&);
		};

		class calc_parse_action : public calc_parser
		{
			public:	
			static int pgmname_    (int); 
			static int stack       (int); 
         static int error       (int);
         static int lookup      (int);
		};

		class calc_node_action : public calc_parser
		{
			public:		
         static int goal_     (int);
         static int program_  (int);
         static int store_    (int);
         static int if_       (int);
         static int target_   (int);
         static int add_      (int);
         static int sub_      (int);
         static int mul_      (int);
         static int div_      (int);
         static int ident_    (int);
         static int int_      (int);
         static int eq_       (int);
         static int ne_       (int);
         static int then_     (int);
         static int else_     (int);
         static int then2_    (int);
         static int else2_    (int);
         static int emit      (int);
		};

#endif

