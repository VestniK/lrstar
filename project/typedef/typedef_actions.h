
/* Original source code with tabs = 3 */

#ifndef cActions_H
#define cActions_H

		class typedef_token_action : public typedef_parser
		{
			public:	
         static int   error       (int& terminal_number);
         static int   lookup      (int& terminal_number);
		};

		class typedef_parse_action : public typedef_parser
		{
			public:	
			static int typedefidentifier_ (int prod_number); 
		};

		class typedef_node_action : public typedef_parser
		{
			public:		
			static int goal_      (int node_number);
		};

#endif
