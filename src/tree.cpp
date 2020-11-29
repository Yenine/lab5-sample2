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
    case NODE_BLOCK:
      printf("block");
      break;
    case NODE_FUNC:
      printf("func");
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

void TreeNode::genSymbol() {
  static auto *GlobalSymbolTable=new map<string,int>;
  static auto *FuncSymbolTable=new map<string,int>;
  static map<string,int> *SymMap=GlobalSymbolTable;
  static list<map<string,int>*> STlist;
  static int id=0;
  static int funcid=0;
  TreeNode *cur=this;
  if(cur->nodeType==NODE_VAR){
    STlist.push_back(SymMap);
    for(auto temp=STlist.rbegin();temp!=STlist.rend();temp++){
      if ((*temp)->count(cur->var_name) != 0){
        cur->var_id=(**temp)[cur->var_name];
        STlist.pop_back();
        return;
      }
    }
    cur->var_id=-1;
    STlist.pop_back();
    return;
  }
  if(cur->nodeType==NODE_FUNC) {
    for (TreeNode *chi = cur->child; chi != nullptr; chi = chi->sibling) {
      if (chi->nodeType == NODE_VAR) {
        if(FuncSymbolTable->count(chi->var_name)!=0){
          chi->var_id=-2;
        } else {
          chi->var_id = funcid;
          FuncSymbolTable->insert(make_pair(chi->var_name, funcid));
          funcid++;
        }
        cur=chi->sibling;
        break;
      }
    }
  }
  if(cur->stype==STMT_DECL) {
    for (TreeNode *chi = cur->child; chi != nullptr; chi = chi->sibling) {
      if (chi->nodeType == NODE_VAR) {
        if(SymMap->count(chi->var_name)!=0){
          chi->var_id=-2;
        } else {
          chi->var_id = id;
          SymMap->insert(make_pair(chi->var_name, id));
          id++;
        }
      }
    }
    return;
  }
  if(cur->nodeType==NODE_BLOCK) {
    STlist.push_back(SymMap);
    SymMap = new map<string, int>;
    cur=cur->child;
    while(cur!= nullptr){
      cur->genSymbol();
      cur=cur->sibling;
    }
    SymMap = STlist.back();
    STlist.pop_back();
    return;
  }
  cur=cur->child;
  while(cur!= nullptr){
    cur->genSymbol();
    cur=cur->sibling;
  }


}


// You can output more info...
void TreeNode::printSpecialInfo() {
    switch(this->nodeType){
        case NODE_CONST:
          printf(" type:%s\n",this->type->getTypeInfo().c_str());
          break;
        case NODE_VAR:
          printf(" varname:%s varid:%d\n",this->var_name.c_str(),this->var_id);
            break;
//        case NODE_EXPR:
//          printf("expr");
//            break;
        case NODE_STMT:
          switch (this->stype){
            case STMT_SKIP:
              printf("stmt:skip\n");
              break;
            case STMT_DECL:
              printf("stmt:decl\n");
              break;
            case STMT_ASSI:
              printf("stmt:assign\n");
              break;
            default:
              break;
          }
            break;
        case NODE_TYPE:
          printf(" type:%s\n",this->type->getTypeInfo().c_str());
          break;
        case NODE_EXPR:
          switch (this->optype){
          case OP_ADD:
            printf("op:ADD\n");
            break;
          case OP_MINUS:
            printf("op:MINUS\n");
            break;
          case OP_MUL:
            printf("op:MUL\n");
            break;
          case OP_DIV:
            printf("op:DIV\n");
            break;
          case OP_EQ:
            printf("op:EQ\n");
            break;
          case OP_UEQ:
            printf("op:UEQ\n");
            break;
          case OP_GRE:
            printf("op:GRE\n");
            break;
          case OP_LES:
            printf("op:LES\n");
            break;
          case OP_GREEQ:
            printf("op:GREEQ\n");
            break;
          case OP_LESEQ:
            printf("op:LESEQ\n");
            break;
          case OP_AND:
            printf("op:AND\n");
            break;
          case OP_OR:
            printf("op:OR\n");
            break;
          default:
            break;
          }
          break;
        case NODE_IF:
          printf("if");
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
