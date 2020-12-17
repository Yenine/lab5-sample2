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
  static auto *GlobalSymbolTable=new map<string,symbol_entry>;
  static auto *FuncSymbolTable=new map<string,int>;
  static map<string,symbol_entry> *SymMap=GlobalSymbolTable;
  static list<map<string,symbol_entry>*> STlist;
  static int id=0;
  static int funcid=0;
  TreeNode *cur=this;
  if(cur->nodeType==NODE_VAR){
    STlist.push_back(SymMap);
    for(auto temp=STlist.rbegin();temp!=STlist.rend();temp++){
      if ((*temp)->count(cur->var_name) != 0){
        cur->var_id=(**temp)[cur->var_name].token;
        cur->type=(**temp)[cur->var_name].type;
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
    type=cur->child->type;
    for (TreeNode *chi = cur->child; chi != nullptr; chi = chi->sibling) {
      if (chi->nodeType == NODE_VAR) {
        if(SymMap->count(chi->var_name)!=0){
          chi->var_id=-2;
        } else {
          symbol_entry symbol;
          symbol.name=chi->var_name;
          symbol.type=chi->type=type;
          symbol.token=chi->var_id = id;
          SymMap->insert(make_pair(chi->var_name, symbol));
          id++;
        }
      }
    }
    return;
  }
  if(cur->nodeType==NODE_BLOCK) {
    STlist.push_back(SymMap);
    SymMap = new map<string, symbol_entry>;
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
          case STMT_ADD_ASSI:
            printf("stmt:add assign\n");
            break;
          case STMT_MINUS_ASSI:
            printf("stmt:minus assign\n");
            break;
          case STMT_MUL_ASSI:
            printf("stmt:mul assign\n");
            break;
          case STMT_DIV_ASSI:
            printf("stmt:div assign\n");
            break;
          case STMT_IF:
            printf("stmt:if\n");
            break;
          case STMT_FOR:
            printf("stmt:for\n");
            break;
          case STMT_WHILE:
            printf("stmt:while\n");
            break;
          case STMT_CALL:
            printf("stmt:call\n");
            break;
          case STMT_RET:
            printf("stmt:return\n");
            break;
          case STMT_BRK:
            printf("stmt:break\n");
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
          case OP_NOT:
            printf("op:NOT\n");
            break;
          case OP_AFT_ADD:
            printf("op:AFT_ADD\n");
            break;
          case OP_AFT_MINUS:
            printf("op:AFT_MINUS\n");
            break;
          case OP_PRE_ADD:
            printf("op:PRE_ADD\n");
            break;
          case OP_PRE_MINUS:
            printf("op:PRE_MINUS\n");
            break;
          case OP_MOD:
            printf("op:MOD\n");
            break;
          case OP_ADDRESS:
            printf("op:address\n");
            break;
          case OP_UMINUS:
            printf("op:uminus\n");
            break;
          default:
            break;
          }
          break;

//        case NODE_PROG:
//          printf("prog");
//            break;

        default:
          printf("\n");
          break;
    }
}
Type* TreeNode::typeCheck() {
//  this->printNodeInfo();
  TreeNode *cur=this;
  cur=cur->child;
  while (cur!= nullptr){
      cur->typeCheck();
      cur=cur->sibling;
  }
  if(this->nodeType==NODE_EXPR){
    if(this->optype<=8){
      this->type=TYPE_BOOL;
      if(this->optype==OP_NOT ||this->optype==OP_AND ||this->optype==OP_OR) {
        cur = this->child;
        while (cur != nullptr) {
          if (cur->type->type != VALUE_BOOL) {
            cerr << "Bad boolean type at line: " << cur->lineno << endl;
            exit(1);
          }
          cur = cur->sibling;
        }
      }
    }
//    if(this->optype>=9&&this->optype<=18){
//      cur=this->child;
//      while (cur!=nullptr){
//        if(cur->type->type!=VALUE_INT){
//          cerr << "Bad int or float type at line: " << cur->lineno << endl;
//          exit(1);
//        }
//        cur=cur->sibling;
//      }
//    }
  }
  if(this->nodeType==NODE_STMT){
//    printf("hellkasd");
    if(this->stype==STMT_WHILE || this->stype==STMT_IF) {
      if(this->child->type->type!=VALUE_BOOL){
        cerr<<"Bad boolean type at line: " << this->lineno << endl;
        exit(1);
      }
    }else if(this->stype==STMT_FOR) {
      if (this->child->child->sibling->type->type != VALUE_BOOL) {
        cerr << "Bad boolean type at line: " << this->lineno << endl;
        exit(1);
      }
    } else if(this->stype==STMT_DECL){
      type=this->child->type;
      cur=this->child->sibling;
      while (cur!= nullptr){
        if(cur->type->type!=type->type){
          cerr << "Bad decl type at line: " << cur->lineno << endl;
          exit(1);
        }
        cur=cur->sibling;
      }
    } else if(this->stype==STMT_ASSI ||this->stype==STMT_ADD_ASSI||this->stype==STMT_MINUS_ASSI){
      if(this->child->type->type!=this->child->sibling->type->type){
        cerr << "Bad assign type at line: " << this->lineno << endl;
        exit(1);
      }
    }
  }
  return this->type;
}

string TreeNode::sType2String(StmtType type) {
    return "?";
}


string TreeNode::nodeType2String (NodeType type){
    return "<>";
}
