%{
    #include "common.h"
    #define YYSTYPE TreeNode *  
    TreeNode* root;
    extern int lineno;
    int yylex();
    int yyerror( char const * );
    int id=0;
    auto *GlobalSymbolTable=new map<string,int>;
    map<string,int> *SymMap=GlobalSymbolTable;
    list<map<string,int>*> STlist;
    map<int,map<string,int>*> STmap;

%}
%token T_CHAR T_INT T_STRING T_BOOL 

%token LOP_ASSIGN 

%token SEMICOLON COMMA
%token WHILE IF ELSE FOR RET
%token LP RP LC RC
%token IDENTIFIER INTEGER CHAR BOOL STRING

%left LOP_EQ

%%

program
: statements {root = new TreeNode(0, NODE_PROG); root->addChild($1);};

statements
:  statement {$$=$1;}
|  statements statement {$$=$1; $$->addSibling($2);}
|  statements fundef {$$=$1; $$->addSibling($2);}
;
fundef
: T IDENTIFIER LP RP block{
		TreeNode* node = new TreeNode($1->lineno, NODE_FUNC);
                node->addSibling($1);
                node->addSibling($2);
                node->addSibling($5);
                $$=node;
}
;
block
: LC statements RC{
		$$=$2;
		SymMap=new map<string,int>;
		STmap.insert(make_pair($2->lineno,SymMap));
		}
;
statement
: SEMICOLON  {$$ = new TreeNode(lineno, NODE_STMT); $$->stype = STMT_SKIP;}
| declaration SEMICOLON {$$ = $1;}
| assignment SEMICOLON {$$ = $1;}
| WHILE block{$$ = $1;$$->addSibling($2);}
;
//| IF LP expr RP statement{$$ = new TreeNode(lineno, NODE_STMT); $$->stype = STMT_SKIP;}
//| IF LP expr RP LC statements RC

//declaration
//: T IDENTIFIER LOP_ASSIGN expr{  // declare and init
//    TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
//    node->stype = STMT_DECL;
//    node->addChild($1);
//    node->addChild($2);
//    node->addChild($4);
//    $$ = node;
//}
//| T IDENTIFIER {
//    TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
//    node->stype = STMT_DECL;
//    node->addChild($1);
//    node->addChild($2);
//    $$ = node;
//}
assignment
: IDENTIFIER LOP_ASSIGN expr{
	TreeNode* node = new TreeNode($1->lineno, NODE_STMT);

    	node->stype = STMT_ASSI;
    	node->addChild($1);
    	node->addChild($3);
    	$$ = node;
}
;
declaration
: T idlist{  // declare and init
    TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
    node->stype = STMT_DECL;
    node->addChild($1);
    node->addChild($2);
    $$ = node;
}
;
idlist
: idlist COMMA IDENTIFIER{
                           $$=$1;
                           $$->addSibling($3);
                           if(SymMap->count($3->var_name)==0){
                            	SymMap->insert(make_pair($3->var_name,id));
                                 printf("ID %s %d\n",$3->var_name.c_str(),id);
                                 id++;
                            }
                            else{
                            	printf("ID %s %d\n",$3->var_name.c_str(),(*SymMap)[$3->var_name]);
                            }
 }
| idlist COMMA IDENTIFIER LOP_ASSIGN expr{
			$$=$1;
                        $$->addSibling($3);
                        if(SymMap->count($3->var_name)==0){
                                                    	SymMap->insert(make_pair($3->var_name,id));
                                                         printf("ID %s %d\n",$3->var_name.c_str(),id);

                                                         id++;
                                                    }
                                                    else{
                                                    	printf("ID %s %d\n",$3->var_name.c_str(),(*SymMap)[$3->var_name]);
                                                    }
                        $$->addSibling($5);
}
| IDENTIFIER{
                $$ = $1;
                if(SymMap->count($1->var_name)==0){
                                            	SymMap->insert(make_pair($1->var_name,id));
                                                 printf("ID %s %d\n",$1->var_name.c_str(),id);
                                                 printf("count:%d",SymMap->count($1->var_name));
                                                 id++;
                                            }
                                            else{
                                            	printf("ID %s %d\n",$1->var_name.c_str(),(*SymMap)[$1->var_name]);
                                            }
}
| IDENTIFIER LOP_ASSIGN expr{
		$$ = $1;
		printf("hello");
		if(SymMap->count($3->var_name)==0){
                                            	SymMap->insert(make_pair($1->var_name,id));
                                                 printf("ID %s %d\n",$1->var_name.c_str(),id);
                                                 id++;
                                            }
                                            else{
                                            	printf("ID %s %d\n",$1->var_name.c_str(),(*SymMap)[$1->var_name]);
                                            }
		$$->addSibling($3);
}
;
expr
: IDENTIFIER {
    $$ = $1;
}
| INTEGER {
    $$ = $1;
}
| CHAR {
    $$ = $1;
}
| STRING {
    $$ = $1;
}
;

T: T_INT {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_INT;}
| T_CHAR {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_CHAR;}
| T_BOOL {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_BOOL;}
;

%%

int yyerror(char const* message)
{
  cout << message << " at line " << lineno << endl;
  return -1;
}