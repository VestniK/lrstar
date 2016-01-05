
/* Original source code by Paul Mann with tabs = 3 */

#ifndef cActions_H
#define cActions_H

		class ielr_token_action : public ielr_parser
		{
			public:	
         static int   error		(int terminal_number);
         static int   lookup     (int terminal_number);
		};

		class ielr_parse_action : public ielr_parser
		{
			public:	
			static int defterm		(int prod_number); 
		};

		class ielr_node_action : public ielr_parser
		{
			public:		
			static int goal_			(int node_number);
		};

		class ielr_ast_action : public ielr_parser
		{
			static int goal_			(int n);
		};

#endif
