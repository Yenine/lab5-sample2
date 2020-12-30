#include "tree.h"

void movlNode(TreeNode* e,ostream &out,const string& reg="%eax"){
  out << "\tmovl $";
  if (e->nodeType == NODE_VAR)
    out<<"_" << e->var_name<<e->var_id;
  else if (e->nodeType == NODE_CONST)
    out<< e->int_val;
  else out << "t" << e->temp_var;
  out << ", " <<reg<<endl;
}
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
//        cerr << "Bad assign type at line: " << this->lineno << endl;
//        exit(1);
      }
    }
  }
  return this->type;
}
string TreeNode::new_label()
{
  char tmp[20];
  sprintf(tmp, "@%d", TreeNode::label_seq);
  TreeNode::label_seq++;
  return tmp;
}
void TreeNode::stmt_get_label()
{
  switch (this->stype){
//  case COMP_STMT:
//  {
//    Node *last;
//    Node *p;
//    for (p = t->children[0]; p->kind == DECL_NODE; p = p->sibling) ;
//
//    p->label.begin_label = t->label.begin_label;
//    for (; p; p = p->sibling)
//    {
//      last = p;
//      recursive_get_label(p);
//    }
//
//    t->label.next_label = last->label.next_label;
//    if (t->sibling)
//      t->sibling->label.begin_label = t->label.next_label;
//  }
//    break;

  case STMT_WHILE:
  {
    TreeNode *e = this->child;
    TreeNode *s = this->child->sibling;

    if (this->label.begin_label.empty())
      this->label.begin_label = new_label();
    s->label.next_label = this->label.begin_label;

    s->label.begin_label = e->label.true_label = new_label();

    if (this->label.next_label == "")
      this->label.next_label = new_label();
    e->label.false_label = this->label.next_label;
    if (this->sibling)
      this->sibling->label.begin_label = this->label.next_label;

    e->recursive_get_label();
    s->recursive_get_label();
  }
  case STMT_IF:
  {
    TreeNode *e = this->child;
    TreeNode *s = this->child->sibling;

    if (this->label.begin_label.empty())
      this->label.begin_label = new_label();
    s->label.next_label = this->label.begin_label;

    s->label.begin_label = e->label.true_label = new_label();

    if (this->label.next_label == "")
      this->label.next_label = new_label();
    e->label.false_label = this->label.next_label;
    if (this->sibling)
      this->sibling->label.begin_label = this->label.next_label;

    e->recursive_get_label();
    s->recursive_get_label();
  }
//  case STMT_FOR:
//    TreeNode *e = this->child;
//    TreeNode *s = this->child->sibling;
//
//    if (this->label.begin_label.empty())
//      this->label.begin_label = new_label();
//    s->label.next_label = this->label.begin_label;
//
//    s->label.begin_label = e->label.true_label = new_label();
//
//    if (this->label.next_label == "")
//      this->label.next_label = new_label();
//    e->label.false_label = this->label.next_label;
//    if (this->sibling)
//      this->sibling->label.begin_label = this->label.next_label;
//
//    e->recursive_get_label();
//    s->recursive_get_label();
//    break;

  default:
    if (this->sibling)
      this->sibling->label.begin_label = this->label.next_label;
    /* ... */
  }
}

void TreeNode::expr_get_label()
{
  if (this->type->type != VALUE_BOOL)
    return;

  TreeNode *e1 = this->child;
  TreeNode *e2 = this->child->sibling;
  switch (this->optype)
  {
  case OP_OR:
    e1->label.true_label = e2->label.true_label = this->label.true_label;
    e1->label.false_label = new_label();
    e2->label.false_label=this->label.false_label;
    break;
  case OP_AND:
    e1->label.true_label = new_label();
    e2->label.true_label = this->label.true_label;
    e1->label.false_label = e2->label.false_label = this->label.false_label;
    break;
  case OP_NOT:
    e1->label.true_label=this->label.false_label;
    e1->label.false_label=this->label.true_label;
    break;
  default:
    break;
    /* ... */
  }
  if (e1)
    e1->recursive_get_label();
  if (e2)
    e2->recursive_get_label();
}

void TreeNode::recursive_get_label(){
  if (this->nodeType == NODE_STMT) {
    this->stmt_get_label();
  }else if (this->nodeType == NODE_EXPR){
//    this->expr_get_label();
  }else{
    TreeNode *cur=this->child;
    while (cur){
      cur->recursive_get_label();
      cur=cur->sibling;
    }
  }
}

void TreeNode::get_label()
{
  this->label.begin_label = "_start";
  this->recursive_get_label();

}

void TreeNode::gen_header(ostream &out)
{
  out << "# your asm code header here" << endl;
  /*your code here*/
}

void TreeNode::gen_decl(ostream &out){
  if(this->stype==STMT_DECL){
    TreeNode *p = this->child;
    if(p->type->type == VALUE_INT){
      p=p->sibling;
      while(p){
        out << "_" << p->var_name<<p->var_id<< ":" << endl;
        out << "\t.zero\t4" << endl;
        out << "\t.align\t4" << endl;
        p=p->sibling;
      }
    }
  }
  TreeNode *cur=this->child;
  while (cur){
    cur->gen_decl(out);
    cur=cur->sibling;
  }
}
int TreeNode::get_temp_var(){
  static int temp_var_seq=0;

  if (this->nodeType!=NODE_EXPR) {
    TreeNode *cur=this->child;
    while (cur){
      cur->get_temp_var();
      cur=cur->sibling;
    }
    return temp_var_seq;
  }
  if (this->optype ==OP_NOT || this->optype>OP_DIV)
    return temp_var_seq;

  TreeNode *arg1 = this->child;
  TreeNode *arg2 = this->child->sibling;

  if (arg1->nodeType==NODE_EXPR) {
    arg1->get_temp_var();
    if (arg2 && arg2->nodeType==NODE_EXPR) {
      arg2->get_temp_var();
      temp_var_seq--;
    }
    temp_var_seq--;
  } else{
    if (arg2 && arg2->nodeType==NODE_EXPR) {
      arg2->get_temp_var();
      temp_var_seq--;
    }
  }

  this->temp_var = temp_var_seq;
  temp_var_seq++;
  return temp_var_seq;
}
void TreeNode::gen_temp_var(ostream &out){
  int temp_var_seq=this->get_temp_var();
  for(int i=1;i<temp_var_seq;i++){
    out<<"t"<<i<<":"<<endl;
    out<<"\tDWORD  0"<<endl;
  }
}
void TreeNode::stmt_gen_code(ostream &out) {
  switch (this->stype) {
  case STMT_WHILE:
    if (this->label.begin_label != "")
      out << this->label.begin_label << ":" << endl;
    this->child->recursive_gen_code(out);
    out << this->child->label.true_label<<":"<<endl;
    this->child->sibling->recursive_gen_code(out);
    out << "\tjmp " << this->label.begin_label << endl;
    break;
  case STMT_IF:
    if (this->label.begin_label != "")
      out << this->label.begin_label << ":" << endl;
    this->child->recursive_gen_code(out);
    out << this->child->label.true_label<<":"<<endl;
    this->child->sibling->recursive_gen_code(out);
    break;
  case STMT_ASSI:
    if (this->label.begin_label != "")
      out << this->label.begin_label << ":" << endl;
    this->child->sibling->recursive_gen_code(out);
    out << "\tmovl %eax, $_"<<this->child->var_name<<this->child->var_id<<endl;
    break;
  }
}
//  if (t->kind_kind == COMP_STMT)
//  {
//    for (int i = 0; t->children[i]; i++)
//    {
//      recursive_gen_code(out, t->children[i]);
//      for (Node *p = t->children[i]->sibling; p; p = p->sibling)
//        recursive_gen_code(out, p);
//    }

//  else if (t->kind_kind == WHILE_STMT)
//  {
//    if (t->label.begin_label != "")
//      out << t->label.begin_label << ":" << endl;
//    recursive_gen_code(out, t->children[0]);
//    recursive_gen_code(out, t->children[1]);
//    out << "\tjmp " << t->label.begin_label << endl;
//  }
//  else if (t->kind_kind == PRINT_STMT)
//  {
//    /* ... */
//  }
  /* ... */



void TreeNode::expr_gen_code(ostream &out)
{
  TreeNode *e1 = this->child;
  TreeNode *e2 = this->child->sibling;
  if(e1->nodeType==NODE_EXPR){
    e1->expr_gen_code(out);
  }
  if(e2->nodeType==NODE_EXPR){
    e2->expr_gen_code(out);
  }
  switch (this->optype)
  {
    case OP_ADD:case OP_MINUS:case OP_MUL:
    out << "\tmovl $";
    if (e1->nodeType == NODE_VAR)
      out<<"_" << e1->var_name<<e1->var_id;
    else if (e1->nodeType == NODE_CONST)
      out<< e1->int_val;
    else out << "t" << e1->temp_var;
    out << ", %eax" <<endl;
    if(this->optype==OP_ADD) {
      out << "\taddl $";
    } else if(this->optype==OP_MINUS) {
      out << "\tsubl $";
    } else if(this->optype==OP_MUL) {
      out << "\timull $";
    }
    if (e2->nodeType == NODE_VAR)
      out << "_" << e2->var_name<<e2->var_id;
    else if (e2->nodeType == NODE_CONST)
      out << e2->int_val;
    else out << "t" << e2->temp_var;
    out << ", %eax" << endl;
    out << "\tmovl %eax, $t" << this->temp_var << endl;
    break;
  case OP_DIV:case OP_MOD:
    movlNode(e1,out);
    movlNode(e2,out,"ecx");
    out << "\tcltd"<<endl;
    out << "\tidivl %ecx"<<endl;
    if(this->optype==OP_MOD) {
      out << "\tmovl\t%edx, %eax";
    }
    out << "\tmovl %eax, $t" << this->temp_var << endl;
  case OP_LES:case OP_GRE:case OP_LESEQ:case OP_GREEQ:case OP_EQ:case OP_UEQ:
    out << "\tcmp $";
    if (e1->nodeType == NODE_VAR)
      out<<"_" << e1->var_name<<e1->var_id;
    else if (e1->nodeType == NODE_CONST)
      out<< e1->int_val;
    else out << "t" << e1->temp_var;
    out << ", $";
    if (e2->nodeType == NODE_VAR)
      out << "_" << e2->var_name<<e1->var_id<<endl;
    else if (e2->nodeType == NODE_CONST)
      out << e2->int_val<<endl;
    else out << "t" << e2->temp_var<<endl;
    if(this->optype==OP_GREEQ) {
      out << "\tjl ";
    } else if(this->optype==OP_GRE){
      out << "\tjle ";
    } else if(this->optype==OP_LES){
      out << "\tjge ";
    } else if(this->optype==OP_LESEQ){
      out << "\tjg ";
    } else if(this->optype==OP_EQ){
      out << "\tjne ";
    } else if(this->optype==OP_UEQ){
      out << "\tje ";
    }
    out<< this->label.false_label << endl;
    break;
  case OP_AND:
    out << "\t# your own code of AND operation here" << endl;
    out << "\tjl @1" << endl;
    out << "\t# your asm code of AND operation end" << endl;
    /* ... */
  default:
    break;
  }
}
//
void TreeNode::recursive_gen_code(ostream &out){
  if (this->nodeType == NODE_STMT){
    this->stmt_gen_code(out);
  } else if (this->nodeType == NODE_EXPR){
    this->expr_gen_code(out);
  }else if(this->nodeType == NODE_BLOCK ||this->nodeType == NODE_PROG ||this->nodeType==NODE_FUNC){
    TreeNode *cur = this->child;
    while (cur) {
      cur->recursive_gen_code(out);
      cur = cur->sibling;
    }
  }
}

void TreeNode::gen_code(ostream &out)
{
  gen_header(out);
//  Node *p = root->children[0];
//  if (p->kind == DECL_NODE)
    this->gen_decl(out);
    this->gen_temp_var(out);
  out << endl << endl << "# your asm code here" << endl;
  out << "\t.text" << endl;
  out << "\t.globl _start" << endl;
  this->recursive_gen_code(out);
//  if (root->label.next_label != "")
//    out << root->label.next_label << ":" << endl;
//  out << "\tret" << endl;
}
string TreeNode::sType2String(StmtType type) {
    return "?";
}


string TreeNode::nodeType2String (NodeType type){
    return "<>";
}
