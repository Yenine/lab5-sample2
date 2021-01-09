%option nounput
%{
#include "common.h"
#include "main.tab.h"  // yacc header
int lineno=1;

%}
BLOCKCOMMENT \/\*([^\*^\/]*|[\*^\/*]*|[^\**\/]*)*\*\/
LINECOMMENT \/\/[^\n]*
EOL	(\r\n|\r|\n)
WHILTESPACE [[:blank:]]

INTEGER [1-9]+[0-9]*|[0]
HEX_INTEGER 0x[0-9a-f]+
OCT_INTEGER 0[0-7]+

CHAR \'[\\]?.?\'
STRING \".+\"

IDENTIFIER [[:alpha:]_][[:alpha:][:digit:]_]*

ADDOP [+]|[-]
MULOP [*]|[/]
RELOP <|>|<=|>=|==|!=
LOGOP \&\&|\|\|

%%

{BLOCKCOMMENT}  /* do nothing */
{LINECOMMENT}  /* do nothing */



"int" return T_INT;
"bool" return T_BOOL;
"char" return T_CHAR;

"=" return LOP_ASSIGN;
"+=" return LOP_ADD_ASSIGN;
"-=" return LOP_MINUS_ASSIGN;
"*=" return LOP_MUL_ASSIGN;
"/=" return LOP_DIV_ASSIGN;

"!" return O_NOT;
"+" return O_ADD;
"-" return O_MINUS;
"*" return O_MUL;
"/" return O_DIV;
"%" return O_MOD;
"<" return O_LES;
">" return O_GRE;
"<=" return O_LESEQ;
">=" return O_GREEQ;
"==" return O_EQ;
"!=" return O_UEQ;
"&&" return O_AND;
"||" return O_OR;
"&" return O_ADDRESS;

"++" return O_DOUBLE_ADD;
"--" return O_DOUBLE_MINUS;


";" return  SEMICOLON;
"," return COMMA;
"(" return  LP;
")" return RP;
"{" return  LC;
"}" return RC;

"while" return WHILE;
"if" return IF;
"else" return ELSE;
"for" return FOR;
"return" return RET;
"break" return BRK;
"const" return CONST;

{INTEGER} {
    TreeNode* node = new TreeNode(lineno, NODE_CONST);
    node->type = TYPE_INT;
    node->int_val = atoi(yytext);
    yylval = node;
    return INTEGER;
}
{HEX_INTEGER} {
    TreeNode* node = new TreeNode(lineno, NODE_CONST);
    node->type = TYPE_INT;
    node->int_val = strtol(yytext,nullptr,16);
    yylval = node;
    return INTEGER;
}
{OCT_INTEGER} {
    TreeNode* node = new TreeNode(lineno, NODE_CONST);
    node->type = TYPE_INT;
    node->int_val = strtol(yytext,nullptr,8);
    yylval = node;
    return INTEGER;
}

{CHAR} {
    TreeNode* node = new TreeNode(lineno, NODE_CONST);
    node->type = TYPE_CHAR;
    if(yytext[1]==92){
        if(yytext[2]==92){
            node->int_val = yytext[1];
        }else if(yytext[2]==110){
            node->int_val = 10;
        }else if(yytext[2]==116){
            node->int_val = 11;
        }
    }else{
        node->int_val = yytext[1];
    }
    yylval = node;
    return CHAR;
}

{STRING} {
    TreeNode* node = new TreeNode(lineno, NODE_CONST);
    node->type = TYPE_STRING;
    node->str_val = string(yytext);

    yylval = node;
    return STRING;
}


{IDENTIFIER} {
    TreeNode* node = new TreeNode(lineno, NODE_VAR);

    node->var_name = string(yytext);
    yylval = node;
    return IDENTIFIER;
}
{WHILTESPACE} /* do nothing */

{EOL} lineno++;

. {
    cerr << "[line "<< lineno <<" ] unknown character:" << yytext << endl;
}
%%