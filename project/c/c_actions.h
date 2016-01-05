
/* Original source code with tabs = 3 */

#ifndef cActions_H
#define cActions_H

		class c_token_action : public c_parser
		{
			public:	
         static int error  (int& terminal_number);
         static int lookup (int& terminal_number);
		};

		class c_parse_action : public c_parser
		{
			public:	
			static int goal_					(int prod_number); 
			static int typedefidentifier_  (int prod_number); 
		};

		class c_node_action : public c_parser
		{
			public:		
			static int goal_      (int node_number);
		};

#endif
