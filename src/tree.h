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
  OP_AFT_MINUS,
  OP_AFT_ADD,
  OP_PRE_MINUS,
  OP_PRE_ADD,
  OP_UMINUS,
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
    
    void printNodeInfo();
    void printChildrenId();

    void printAST(); // 先输出自己 + 孩子们的id；再依次让每个孩子输出AST。
    void printSpecialInfo();

    void genSymbol();
    void genNodeId();

    Type* typeCheck();
    string new_label();
    void get_label();
    void recursive_get_label();
    void stmt_get_label();
    void expr_get_label();

  int get_temp_var();
  void gen_header(ostream &out);
  void gen_decl(ostream &out);
  void gen_temp_var(ostream &out);
  void recursive_gen_code(ostream &out);
  void stmt_gen_code(ostream &out);
  void expr_gen_code(ostream &out);

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
  void gen_code(ostream &out);
};
class tree{
private:
  TreeNode *root;
  int node_seq = 0;
  int temp_var_seq = 0;
  int label_seq = 0;
public:

};


#endif