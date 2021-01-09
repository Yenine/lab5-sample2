#ifndef TREE_H
#define TREE_H

#include "pch.h"
#include "type.h"
#include "symbol.h"
enum NodeType
{
    NODE_CONST, 
    NODE_VAR,
    NODE_EXPR,
    NODE_TYPE,

    NODE_STMT,
    NODE_PROG,
    NODE_BLOCK,
    NODE_FUNC,
//    NODE_IF,
//    NODE_ELSE,
//    NODE_WHILE,
//    NODE_FOR,
};

enum OperatorType
{
  OP_EQ,  // ==
  OP_GRE,  // >
  OP_LES,  // <
  OP_GREEQ,  // >=
  OP_LESEQ,  // <=
  OP_UEQ,  // !=
  OP_NOT, //!
  OP_AND,  // &&
  OP_OR,  // ||
  OP_MOD,  // %
  OP_ADD,  // +
  OP_MINUS,  // -
  OP_MUL,  // *
  OP_DIV,  // /
  OP_UMINUS,
  OP_AFT_MINUS,
  OP_AFT_ADD,
  OP_PRE_MINUS,
  OP_PRE_ADD,
  OP_ADDRESS,
};

enum StmtType {

    STMT_IF,
    STMT_ELSE,
    STMT_FOR,
    STMT_WHILE,
    STMT_CALL,
    STMT_RET,
    STMT_BRK,

    STMT_SKIP,
    STMT_DECL,
    STMT_DECL_CONST,
    STMT_ASSI,
    STMT_ADD_ASSI,
    STMT_MINUS_ASSI,
    STMT_MUL_ASSI,
    STMT_DIV_ASSI,
}
;
struct Label {
    string true_label;
    string false_label;
    string begin_label;
    string next_label;
};

struct TreeNode {
private:
  int label_seq = 0;
public:
    int nodeID;  // 用于作业的序号输出
    int lineno;
    NodeType nodeType;

    TreeNode* child = nullptr;
    TreeNode* sibling = nullptr;

    void addChild(TreeNode*);
    void addSibling(TreeNode*);
    
    void printNodeInfo(ostream &out);
    void printChildrenId(ostream &out);

    void printAST(ostream &out); // 先输出自己 + 孩子们的id；再依次让每个孩子输出AST。
    void printSpecialInfo(ostream &out);

    void genNodeId();

  void typeCheck();

//  void recursive_get_label();
//  int stmt_get_label(int lab_seq);
//  int expr_get_label(int lab_seq);
//  void genSymbol();

  void gen_decl(ostream &out);

public:
    OperatorType optype;  // 如果是表达式
    Type* type;  // 变量、类型、表达式结点，有类型。
    StmtType stype;
    int int_val;
    char ch_val;
    bool b_val;
    string str_val;
    string var_name;
    int var_id;

    Label label;
    int temp_var;
public:
    static string nodeType2String (NodeType type);
    static string opType2String (OperatorType type);
    static string sType2String (StmtType type);

public:
    TreeNode(int lineno, NodeType type);

};
class Tree{
private:
//  int node_seq;
//  int temp_var_seq;
//  int text_seq;
public:
  char filename[20];
  int label_seq;
  int temp_var_seq;
  int temp_var_nums;
  map<string,int> string_map;
  TreeNode *root;
  Tree(TreeNode *root){
    this->root=root;
    this->label_seq=temp_var_nums=temp_var_seq=0;
//    this->string_map=new map<string,int>;
//    printf("%d",this->string_map->size());
  }
  void genSymbol(TreeNode *t);
  void genText(ostream &out);
  string new_label();
  void get_label();
  void recursive_get_label(TreeNode *t);
  void stmt_get_label(TreeNode *t);
  void expr_get_label(TreeNode *t);

  void gen_code(ostream &out);
  void gen_header(ostream &out);

  void get_temp_var(TreeNode *t);
  void gen_temp_var(ostream &out);

  void recursive_gen_code(ostream &out,TreeNode *t);
  void stmt_gen_code(ostream &out,TreeNode *t);
  void expr_gen_code(ostream &out,TreeNode *t);
  void func_gen_code(ostream &out,TreeNode *t);
};


#endif