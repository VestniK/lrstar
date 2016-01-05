
/* Original source code with tabs = 3 */

#ifndef cActions_H
#define cActions_H

		class lr1_token_action : public lr1_parser
		{
			public:	
         static int   error       (short& terminal_number);
         static int   lookup      (short& terminal_number);
		};

		class lr1_parse_action : public lr1_parser
		{
			public:	
			static short defterm     (short prod_number); 
		};

		class lr1_node_action : public lr1_parser
		{
			public:		
			static short goal_      (int node_number);
		};

#endif
