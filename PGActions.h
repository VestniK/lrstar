
		#pragma once
      #include "PGParser.h"

      class PGTokenAction : public PGParser 
      {
         public:
      // Variables ...
			static char		errormsg[256];
         static int		head_column;
         static int		arrow_column;
         static int		arrow_length;

      // Functions ...
         static int   	error   (int);
         static int   	eof     (int);
         static int   	lookup  (int);
         static int   	lookupi (int);
         static int   	checkforspace (int);
         static int   	action_operator (int);
         static int   	firstsym (int);
         static int   	arrow    (int);
      };

      class PGParseAction : public PGParser 
      {
         public:
      // Variables ...
			static int   	STI;
  			static char  	errormsg[256];
			static int   	production_length;

      // Functions ...	
			static int   	CheckOptions ();
         static int   	arrow_						(int p);
         static int   	code_start_					(int p);
         static int   	init_code_start_			(int p);
         static int   	end_code_start_			(int p);
         static int   	blank_node_name_			(int p);
         static int   	pa_blank_node_name_		(int p);
         static int   	blanknodeaction_			(int p);
         static int   	options_      				(int p);
         static int   	lexer_        				(int p);
         static int   	constant_     				(int p);
         static int   	value_        				(int p);
         static int   	errordecl_    				(int p);
         static int   	terminal_     				(int p);
         static int   	terminalres_  				(int p);
         static int   	leftassoc_    				(int p);
         static int   	rightassoc_   				(int p);
         static int   	oper_         				(int p);
         static int   	goalsymbol_   				(int p);
         static int   	headsymbol_   				(int p);
         static int   	eof_          				(int p);
         static int   	prod_         				(int p);
         static int   	prec_         				(int p);
         static int   	tailposition_ 				(int p);
         static int   	semantic_     				(int p);
         static int   	tail_         				(int p);
         static int   	taileof_      				(int p);
         static int   	tailkw_       				(int p);
         static int   	sep_          				(int p);
         static int   	rule_actions_  			(int p);
         static int   	token_action_spec_  		(int p);
         static int   	parse_action_spec_  		(int p);
         static int   	parse_action_name_  		(int p);
         static int   	blank_parse_action_	   (int p);
         static int   	node_spec_     			(int p);
         static int   	pa_node_spec_   			(int p);
         static int   	node_name_     			(int p);
         static int   	pa_node_name_   			(int p);
         static int   	node_action_  				(int p);
         static int   	node_action_name_			(int p);
         static int   	blank_node_action_name_	(int p);
         static int   	blank_node_action_     	(int p);
         static int   	arg_first_     			(int p);
         static int   	arg_integer_       		(int p);
         static int   	arg_alpha_     			(int p);
         static int   	arg_terminal_      		(int p);
         static int   	arg_string_       		(int p);
         static int   	arg_null_       			(int p);
         static int   	arg_expr_      			(int p);
         static int   	arg_expr_integer_   		(int p);
         static int   	arg_expr_alpha_ 			(int p);

         static int   	make_node_ 					(int p);
         static int   	make_node_wa_ 				(int p);
         static int   	parse_action_make_node_ 	(int p);
         static int   	parse_action_make_node_wa_	(int p);
         static int   	args_ 							(int p);
         static int   	token_action_name_ 			(int p);

       };

      class PGNodeAction : public PGParser 
      {
         public:
      // Variables ...
			static char**  Defcon_term;
			static char**  head_name;
			static int*    head_sym;
			static int*    head_line; 
			static int*    term_line; 
			static int*    prod_len;
			static int*    prod_line;
			static int*    nullable;
			static int     n_nulls;
			static int     n_prods;
			static int     n_tails;
			static int     amt_space;
			static int*    Tail;
	      static int*    F_tail;
			static char**  term_name;

  			static char  	errormsg[256];
         static int   	linelength;
         static int   	prevtail;
         static int   	nodesti;
         static int   	arrow; 
         static char* 	oper;
         static int   	assoc;
         static int   	constant;
         static int   	n_values;
         static int   	generated;
         static int   	precedence;
         static int   	terminal_mode;
         static int   	separator;
			static int     max_prec;
	      static char**  term_start;
		   static char**  term_end;
			static int*    term_type;
			static char**  Int_start;
         static Node* 	lastnonterm;
         static Node* 	lastprod;
         static Node* 	lasttails;
         static Node* 	lasttail;

      // Functions ...	
			static int   	start_(Node*);
         static int   	grammar_(Node*);
         static int   	options_(Node*);
         static int   	constdecls_(Node*);
         static int   	terminaldecls_(Node*);
         static int   	declarations_(Node*);
         static int   	constdecl_(Node*);
         static int   	error_(Node*);
         static int   	token_decl_(Node*);
         static int   	constant_(Node*);
         static int   	constnull_(Node*);
         static int   	value_(Node*);
         static int   	terminal_(Node*);
         static int   	terminalres_(Node*);
         static int   	reserved_(Node*);
         static int   	operprec_(Node*);
         static int   	leftassoc_(Node*);
         static int   	rightassoc_(Node*);
         static int   	rules_(Node*);
         static int   	goaldef_(Node*);
         static int   	headdef_(Node*);
         static int   	prod_(Node*);
         static int   	tails_(Node*);
         static int   	tail_(Node*);
         static int   	taileof_(Node*);
         static int   	tailkw_(Node*);
         static int   	tailexpr_(Node*);
         static int   	group_(Node*);
         static int   	optgroup_(Node*);
         static int   	reverse_(Node*);
         static int   	genrepeat(Node*);
         static int   	repeat_(Node*);
         static int   	repeatrev_(Node*);
         static int   	repeatsep_(Node*);
         static int   	repeatseprev_(Node*);
         static int   	sepexpr_(Node*);
         static int   	sepexprrev_(Node*);
         static int   	sep_(Node*);
         static int   	rule_actions_(Node*);
         static int   	token_action_(Node*);
         static int   	parse_action_(Node*);
         static int   	parse_action_make_node_(Node*);
         static int   	parse_action_make_node_wa_(Node*);
         static int   	make_node_(Node*);
         static int   	make_node_wa_(Node*);
         static int   	token_action_spec_(Node*);
         static int   	node_spec_(Node*);
         static int   	pa_node_spec_(Node*);
         static int   	parse_action_spec_(Node*);
         static int   	blank_parse_action_(Node*);
         static int   	node_action_(Node*);
         static int   	blank_node_action_name_(Node*);
         static int   	stackindex_(Node*);
         static int   	typeexpr_(Node*);
         static int   	type_(Node*);
         static int   	args_(Node*);
         static int   	arg_integer_(Node*);
         static int   	arg_alpha_(Node*);
         static int   	arg_terminal_(Node*);
         static int   	arg_null_(Node*);
         static int   	arg_string_(Node*);
         static int   	arg_expr_(Node*);
         static int   	arg_expr_integer_(Node*);
         static int   	arg_expr_alpha_(Node*);
         static int   	oper_(Node*);
         static int   	opstart_(Node*);
         static int   	opended_(Node*);
         static int   	leftparen_(Node*);
         static int   	rightparen_(Node*);
         static int   	comma_(Node*);
         static int   	semicolon_(Node*);
         static int   	end_(Node*);

			static void  	add_quotes ();
         static void  	get_last_nonterminal ();
         static void  	number_the_terminals ();
         static void  	list_rest_of_terminals ();
         static void  	make_keyword_nonterminal ();
         static void  	alloc_struc ();
         static void  	fill_struc  ();
         static void  	print_struc ();

         static int   	group_length (Node* np, int& x_prods, int& n_tails);
         static void  	get_group_symbol (Node* np, int len, char, char, char*&, char*&);
         static char* 	group_symbol (char* p, Node* np);
         static void  	group_prod (Node* np, int arrow);
         static int   	prt_sym (char* before, Node* np, char* after, int length);
         static void  	make_head (int);
         static void  	make_prod (int);
         static void  	make_tail (int);
         static void  	SORTNODES (Node* np);

      };

		class PGActions : public PGTokenAction, public PGParseAction, public PGNodeAction
		{
			public:
		};

