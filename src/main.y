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

%token LOP_ASSIGN LOP_ADD_ASSIGN LOP_MINUS_ASSIGN  LOP_MUL_ASSIGN  LOP_DIV_ASSIGN

%token O_ADD O_MINUS O_MUL O_DIV O_EQ O_UEQ O_GRE O_LES O_GREEQ O_LESEQ O_AND O_OR O_NOT O_MOD O_DOUBLE_ADD O_DOUBLE_MINUS O_ADDRESS


%token SEMICOLON COMMA
%token WHILE IF ELSE FOR RET BRK
%token LP RP LC RC
%token IDENTIFIER INTEGER CHAR BOOL STRING


//%token O_ADD
//%token O_MUL
//%left O_ADD
%left LOP_EQ
%left O_OR
%left O_AND
%left O_EQ O_UEQ O_GRE O_LES O_GREEQ O_LESEQ
%left O_ADD O_MINUS
%left O_MOD
%left O_MUL O_DIV
%right O_NOT O_ADDRESS
%right PRE_ADD PRE_MINUS UMINUS
%left AFT_ADD AFT_MINUS
//%left O_ADD O_MINUS O_MUL O_DIV O_EQ O_UEQ O_GRE O_LES O_GREEQ O_LESEQ O_AND O_OR

%%

program
: statements {
root = new TreeNode(0, NODE_PROG); root->addChild($1);};

statements
:  statement {$$=$1;}
|  statements statement {$$=$1; $$->addSibling($2);}
//|  statements fundes {
//$$=$1;
//$$->addSibling($2);
//}
//|  statements if {
//$$=$1;
//$$->addSibling($2);
//}
//| statements if else{
//$$=$1;
//$$->addSibling($2);
//$$->addSibling($3);
//}
//|  statements IF LP expr RP
;
for
: FOR LP declaration SEMICOLON expr SEMICOLON expr RP block{
		TreeNode* node = new TreeNode($1->lineno+1, NODE_STMT);
		node->stype = STMT_FOR;
                node->addChild($9);
                $7->sibling=$9->child;
                $9->child=$3;
                $9->addChild($5);
                $9->addChild($7);
                $$=node;
}
| FOR LP assignment SEMICOLON expr SEMICOLON expr RP block{
 		TreeNode* node = new TreeNode($1->lineno+1, NODE_STMT);
 		node->stype = STMT_FOR;
                node->addChild($9);
                $7->sibling=$9->child;
                $9->child=$3;
                $9->addChild($5);
                $9->addChild($7);
                 $$=node;
 }
;
if
: if else{
$$=$1;
$1->addSibling($2);
}
|IF LP expr RP block{
		TreeNode* node = new TreeNode($1->lineno+1, NODE_STMT);
		node->stype = STMT_IF;
                node->addChild($3);
                node->addChild($5);
                $$=node;
}
| IF LP expr RP statement{
                         		TreeNode* node = new TreeNode($1->lineno+1, NODE_STMT);
                         		node->stype = STMT_IF;
                                         node->addChild($3);
                                         node->addChild($5);
                                         $$=node;
                         }

;
else
: ELSE block{
	TreeNode* node = new TreeNode($1->lineno+1, NODE_STMT);
        node->stype = STMT_ELSE;
        node->addChild($2);
        $$=node;
}
| ELSE statement{
	TreeNode* node = new TreeNode($1->lineno+1, NODE_STMT);
        node->stype = STMT_ELSE;
        node->addChild($2);
        $$=node;
}
;

while
: WHILE LP expr RP block{
		TreeNode* node = new TreeNode($1->lineno+1, NODE_STMT);
		node->stype = STMT_WHILE;
                node->addChild($3);
                node->addChild($5);
                $$=node;
}
fundes
: T IDENTIFIER LP funcparams RP block{
		TreeNode* node = new TreeNode($1->lineno, NODE_FUNC);
                node->addChild($1);
                node->addChild($2);
                node->addChild($4);
                node->addChild($6);
                $$=node;
}
| T IDENTIFIER LP RP block{
 		TreeNode* node = new TreeNode($1->lineno, NODE_FUNC);
                 node->addChild($1);
                 node->addChild($2);
                 node->addChild($5);
                 $$=node;
 }
;
funcparams
:funcparams COMMA declsingle {
$$=$1;
$$->addSibling($3);
}
| declsingle{
$$=$1;
}
;
declsingle
:T IDENTIFIER {
    TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
    node->stype = STMT_DECL;
    node->addChild($1);
    node->addChild($2);
    $$ = node;
 }
| T IDENTIFIER LOP_ASSIGN expr{
	TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
    node->stype = STMT_DECL;
    node->addChild($1);
    node->addChild($2);
    node->addChild($4);
    $$ = node;
}
;
funcall
: IDENTIFIER LP params RP {
		TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
		node->stype = STMT_CALL;
                node->addChild($1);
                node->addChild($3);
                $$=node;
}
| IDENTIFIER LP RP{
                  		TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
                  		node->stype = STMT_CALL;
                                  node->addChild($1);
                                  $$=node;
}
;
params
: params COMMA unit{
		$$=$1;
                $$->addSibling($3);
}
| expr{
$$=$1;
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
| expr SEMICOLON{$$ = $1;}
| funcall SEMICOLON{$$ = $1;}
| fundes{$$ = $1;}
| if{$$ = $1;}
| for{$$ = $1;}
| while{$$ = $1;}
| RET unit SEMICOLON{
$$ = new TreeNode(lineno, NODE_STMT); $$->stype = STMT_RET;
$$->addChild($2);
}
| BRK SEMICOLON{
$$ = new TreeNode(lineno, NODE_STMT); $$->stype = STMT_BRK;
}
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
//    	TreeNode* cur = new TreeNode($1->lineno, NODE_EXPR);
    	node->addChild($3);
    	$$ = node;
}
| IDENTIFIER LOP_ADD_ASSIGN expr{
	TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
    	node->stype = STMT_ADD_ASSI;
    	node->addChild($1);
    	node->addChild($3);
    	$$ = node;
}
| IDENTIFIER LOP_MINUS_ASSIGN expr{
	TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
    	node->stype = STMT_MINUS_ASSI;
    	node->addChild($1);
    	node->addChild($3);
    	$$ = node;
}
| IDENTIFIER LOP_MUL_ASSIGN expr{
	TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
    	node->stype = STMT_MUL_ASSI;
    	node->addChild($1);
    	node->addChild($3);
    	$$ = node;
}
| IDENTIFIER LOP_DIV_ASSIGN expr{
	TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
    	node->stype = STMT_DIV_ASSI;
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

expr:
//: assignment
 expr O_AND expr{
 	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
 	node->optype=OP_AND;
 	node->addChild($1);
        node->addChild($3);
        $$=node;
 }
| expr O_OR expr{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_OR;
	node->addChild($1);
        node->addChild($3);
        $$=node;
}
| expr O_EQ expr{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_EQ;
	node->addChild($1);
        node->addChild($3);
        $$=node;
}
| expr O_UEQ expr{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_UEQ;
	node->addChild($1);
        node->addChild($3);
        $$=node;
}
| expr O_GRE expr{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_GRE;
	node->addChild($1);
        node->addChild($3);
        $$=node;
}
| expr O_LES expr{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_LES;
	node->addChild($1);
        node->addChild($3);
        $$=node;
}
| expr O_GREEQ expr{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_GREEQ;
	node->addChild($1);
        node->addChild($3);
        $$=node;
}
| expr O_LESEQ expr{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_LESEQ;
	node->addChild($1);
        node->addChild($3);
        $$=node;
}
| expr O_ADD expr{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_ADD;
	node->addChild($1);
        node->addChild($3);
        $$=node;
}
| expr O_MINUS expr{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_MINUS;
	node->addChild($1);
        node->addChild($3);
        $$=node;
}
| expr O_MUL expr{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_MUL;
	node->addChild($1);
        node->addChild($3);
        $$=node;
}
| expr O_DIV expr{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_DIV;
	node->addChild($1);
        node->addChild($3);
        $$=node;
}
| expr O_MOD expr{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_MOD;
	node->addChild($1);
        node->addChild($3);
        $$=node;
}
| O_MINUS expr %prec UMINUS{
	TreeNode* node = new TreeNode($2->lineno, NODE_EXPR);
	node->optype=OP_UMINUS;
	node->addChild($2);
        $$=node;
}
| O_NOT expr{
	TreeNode* node = new TreeNode($2->lineno, NODE_EXPR);
	node->optype=OP_NOT;
	node->addChild($2);
        $$=node;
}
| expr O_DOUBLE_ADD %prec AFT_ADD{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_AFT_ADD;
	node->addChild($1);
        $$=node;
}
| expr O_DOUBLE_MINUS %prec AFT_MINUS{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_AFT_MINUS;
	node->addChild($1);
        $$=node;
}
| O_DOUBLE_ADD expr %prec PRE_ADD{
	TreeNode* node = new TreeNode($2->lineno, NODE_EXPR);
	node->optype=OP_PRE_ADD;
	node->addChild($2);
        $$=node;
}
| O_DOUBLE_MINUS expr %prec PRE_MINUS{
	TreeNode* node = new TreeNode($2->lineno, NODE_EXPR);
	node->optype=OP_PRE_MINUS;
	node->addChild($2);
        $$=node;
}
| LP expr RP{
	printf("hello");
//	TreeNode* node = new TreeNode(10, NODE_EXPR);
//	node->addChild($2);
	$$=$2;
}
| unit{
	$$=$1;
}

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
| funcall{
$$ = $1;
}
| O_ADDRESS IDENTIFIER{
	TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
	node->optype=OP_ADDRESS;
	node->addChild($2);
        $$=node;
}
;

T: T_INT {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_INT;}
| T_CHAR {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_CHAR;}
| T_BOOL {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_BOOL;}
//| T_VOID {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_VOID;}
;

%%

int yyerror(char const* message)
{
  cout << message << " at line " << lineno << endl;
  return -1;
}