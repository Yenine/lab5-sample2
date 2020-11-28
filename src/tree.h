#ifndef TREE_H
#define TREE_H

#include "pch.h"
#include "type.h"

enum NodeType
{
    NODE_CONST, 
    NODE_VAR,
    NODE_EXPR,
    NODE_TYPE,

    NODE_STMT,
    NODE_PROG,
    NODE_FUNC,
    NODE_BLOCK,
};

enum OperatorType
{
  OP_EQ,  // ==
  OP_ADD,  // +
  OP_MINUS,  // -
  OP_MUL,  // *
  OP_DIV,  // /
  OP_GRE,  // >
  OP_LES,  // <
  OP_GREEQ,  // >=
  OP_LESEQ,  // <=
  OP_UEQ,  // !=
  OP_AND,  // &&
  OP_OR,  // ||

};

enum StmtType {
    STMT_SKIP,
    STMT_DECL,
    STMT_ASSI,
}
;

struct TreeNode {
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
public:
    static string nodeType2String (NodeType type);
    static string opType2String (OperatorType type);
    static string sType2String (StmtType type);

public:
    TreeNode(int lineno, NodeType type);
};

#endif