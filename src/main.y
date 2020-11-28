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
%token O_ADD O_MINUS O_MUL O_DIV O_EQ O_UEQ O_GRE O_LES O_GREEQ O_LESEQ O_AND O_OR


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
|  statements fundef {
$$=$1;
$$->addSibling($2);
}
//|  statements IF LP expr RP
;
fundef
: T IDENTIFIER LP RP block{
		printf("begin_func\n");
		TreeNode* node = new TreeNode($1->lineno, NODE_FUNC);
                node->addChild($1);
                node->addChild($2);
                node->addChild($5);
                $$=node;
}
;
block
: LC statements RC {
		TreeNode* node = new TreeNode($1->lineno, NODE_BLOCK);
		node->addChild($2);
		$$=node;
		}
;



statement
: SEMICOLON  {$$ = new TreeNode(lineno, NODE_STMT); $$->stype = STMT_SKIP;}
| declaration SEMICOLON {$$ = $1;}
| assignment SEMICOLON {$$ = $1;}
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
 }
| idlist COMMA IDENTIFIER LOP_ASSIGN expr{
			$$=$1;
                        $$->addSibling($3);
                        $$->addSibling($5);
}
| IDENTIFIER{
                $$ = $1;
}
| IDENTIFIER LOP_ASSIGN expr{
		$$ = $1;
		$$->addSibling($3);
}
;
expr
: expr O_AND unit{
 	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
 	node->optype=OP_AND;
 	node->addChild($1);
        node->addChild($3);
        $$=node;
 }
| expr O_OR unit{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_OR;
	node->addChild($1);
        node->addChild($3);
        $$=node;
}
| expr O_EQ unit{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_EQ;
	node->addChild($1);
        node->addChild($3);
        $$=node;
}
| expr O_UEQ unit{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_UEQ;
	node->addChild($1);
        node->addChild($3);
        $$=node;
}
| expr O_GRE unit{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_GRE;
	node->addChild($1);
        node->addChild($3);
        $$=node;
}
| expr O_LES unit{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_LES;
	node->addChild($1);
        node->addChild($3);
        $$=node;
}
| expr O_GREEQ unit{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_GREEQ;
	node->addChild($1);
        node->addChild($3);
        $$=node;
}
| expr O_LESEQ unit{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_LESEQ;
	node->addChild($1);
        node->addChild($3);
        $$=node;
}
| expr O_ADD unit{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_ADD;
	node->addChild($1);
        node->addChild($3);
        $$=node;
}
| expr O_MINUS unit{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_MINUS;
	node->addChild($1);
        node->addChild($3);
        $$=node;
}
| expr O_MUL unit{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_MUL;
	node->addChild($1);
        node->addChild($3);
        $$=node;
}
| expr O_DIV unit{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_DIV;
	node->addChild($1);
        node->addChild($3);
        $$=node;
}
| unit{
	$$=$1;
}
;
unit
:
IDENTIFIER {
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