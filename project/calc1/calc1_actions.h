
/* Source code with tabs = 3 */

#ifndef cActions_H
#define cActions_H

		class calc1_token_action : public calc1_parser
		{
			public:	
         static int   error       (int&);
         static int   lookup      (int&);
		};

		class calc1_parse_action : public calc1_parser
		{
			public:	
			static int Start (int); 
		};
		
		class calc1_node_action : public calc1_parser
		{
			public:		
			static int emit (int);
		};
		
#endif

