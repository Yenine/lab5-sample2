#include "tree.h"
void TreeNode::addChild(TreeNode* child) {
  if(this->child== nullptr){
    this->child=child;
  } else{
    this->child->addSibling(child);
  }
}

void TreeNode::addSibling(TreeNode* sibling){
  TreeNode *cur=this;
  while(cur->sibling!= nullptr){
    cur=cur->sibling;
  }
  cur->sibling=sibling;
}

TreeNode::TreeNode(int lineno, NodeType type) {
  this->lineno=lineno;
  this->nodeType=type;
}

void TreeNode::genNodeId() {
  static int num=0;
  this->nodeID=num;
  num++;
  TreeNode *cur=this;
  cur=cur->child;
  if(cur!= nullptr){
    cur->genNodeId();
    while (cur->sibling!= nullptr){
      cur=cur->sibling;
      cur->genNodeId();
    }
  }
//  printf("lno%d  %d   ",this->lineno,this->nodeID);
//  cout<<this->type->getTypeInfo();
//  cout<<this->type;
}

void TreeNode::printNodeInfo() {
  printf("lno%d  @%d   ",this->lineno,this->nodeID);
  switch(this->nodeType) {
    case NODE_CONST:
      printf("const");
      break;
    case NODE_VAR:
      printf("var");
      break;
    case NODE_EXPR:
      printf("expr");
      break;
    case NODE_STMT:
      printf("stmt");
      break;
    case NODE_TYPE:
      printf("type");
      break;
    case NODE_PROG:
      printf("prog");
      break;
    default:
      break;
  }
  this->printChildrenId();
  this->printSpecialInfo();
}

void TreeNode::printChildrenId() {
  if(this->child!= nullptr) {
    printf(" children:[");
    for (TreeNode *cur = this->child; cur != nullptr; cur = cur->sibling) {
      printf(" @%d", cur->nodeID);
    }
    printf(" ]");
  }
}

void TreeNode::printAST() {
  this->printNodeInfo();
  TreeNode *cur=this;
  cur=cur->child;
  if(cur!= nullptr){
    cur->printAST();
    while (cur->sibling!= nullptr){
      cur=cur->sibling;
      cur->printAST();
    }
  }
}


// You can output more info...
void TreeNode::printSpecialInfo() {
    switch(this->nodeType){
        case NODE_CONST:
          printf(" type:%s\n",this->type->getTypeInfo().c_str());
          break;
        case NODE_VAR:
          printf(" varname:%s\n",this->var_name.c_str());
            break;
//        case NODE_EXPR:
//          printf("expr");
//            break;
//        case NODE_STMT:
//          printf("stmt");
//            break;
        case NODE_TYPE:
          printf(" type:%s\n",this->type->getTypeInfo().c_str());
          break;
//        case NODE_PROG:
//          printf("prog");
//            break;
        default:
          printf("\n");
          break;
    }
}

string TreeNode::sType2String(StmtType type) {
    return "?";
}


string TreeNode::nodeType2String (NodeType type){
    return "<>";
}
