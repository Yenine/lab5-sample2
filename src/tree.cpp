#include "tree.h"
void getNode(TreeNode* e,ostream &out){
  if (e->nodeType == NODE_VAR)
    out<<"_" << e->var_name<<e->var_id;
  else if (e->nodeType == NODE_CONST) {
    out << "$" << e->int_val;
  }
  else out << "t" << e->temp_var;
}
void movlNode(TreeNode* e,ostream &out,const string& reg="%eax"){
  if (e->nodeType == NODE_VAR)
    out<<"\tmovl _" << e->var_name<<e->var_id;
  else if (e->nodeType == NODE_CONST) {
    if(e->type->type==VALUE_INT) {
      out <<"\tmovl $" << e->int_val;
    } else if(e->type->type==VALUE_CHAR) {
      out <<"\tmovb $" << e->int_val;
    }
  }else {
    out << "\tmovl t" << e->temp_var;
  }
  out << ", " <<reg<<endl;
}
void getMov(TreeNode* e,ostream &out){
  if (e->nodeType == NODE_CONST) {
    if(e->type->type==VALUE_INT) {
      out <<"\tmovl $" ;
    } else if(e->type->type==VALUE_CHAR) {
      out <<"\tmovb $";
    }
  } else
    out<<"movl ";
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

void TreeNode::printNodeInfo(ostream &out) {
  out<<"lno"<<this->lineno<<"  @"<<this->nodeID<<"\t";
  switch(this->nodeType) {
    case NODE_CONST:
      out<<"const";
      break;
    case NODE_VAR:
      out<<"var";
      break;
    case NODE_EXPR:
      out<<"expr";
      break;
    case NODE_STMT:
      out<<"stmt";
      break;
    case NODE_TYPE:
      out<<"type";
      break;
    case NODE_PROG:
      out<<"prog";
      break;
    case NODE_BLOCK:
      out<<"block";
      break;
    case NODE_FUNC:
      out<<"func";
      break;
    default:
      break;
  }
  this->printChildrenId(out);
  this->printSpecialInfo(out);
}

void TreeNode::printChildrenId(ostream &out) {
  if(this->child!= nullptr) {
    out<<" children:[";
    for (TreeNode *cur = this->child; cur != nullptr; cur = cur->sibling) {
      out<<" @"<< cur->nodeID;
    }
    out<<" ]";
  }
}

void TreeNode::printAST(ostream &out) {
  this->printNodeInfo(out);
  TreeNode *cur=this;
  cur=cur->child;
  if(cur!= nullptr){
    cur->printAST(out);
    while (cur->sibling!= nullptr){
      cur=cur->sibling;
      cur->printAST(out);
    }
  }
}
//void TreeNode::genSymbol() {
//  static auto *GlobalSymbolTable=new map<string,symbol_entry>;
//  static auto *FuncSymbolTable=new map<string,int>;
//  static map<string,symbol_entry> *SymMap=GlobalSymbolTable;
//  static list<map<string,symbol_entry>*> STlist;
//  static int id=0;
//  static int funcid=0;
//  TreeNode *cur=this;
//  if(cur->nodeType==NODE_VAR){
//    STlist.push_back(SymMap);
//    for(auto temp=STlist.rbegin();temp!=STlist.rend();temp++){
//      if ((*temp)->count(cur->var_name) != 0){
//        cur->var_id=(**temp)[cur->var_name].token;
//        cur->type=(**temp)[cur->var_name].type;
//        STlist.pop_back();
//        return;
//      }
//    }
//    cur->var_id=-1;
//    STlist.pop_back();
//    return;
//  }
//  if(cur->nodeType==NODE_FUNC) {
//    for (TreeNode *chi = cur->child; chi != nullptr; chi = chi->sibling) {
//      if (chi->nodeType == NODE_VAR) {
//        if(FuncSymbolTable->count(chi->var_name)!=0){
//          chi->var_id=-2;
//        } else {
//          chi->var_id = funcid;
//          FuncSymbolTable->insert(make_pair(chi->var_name, funcid));
//          funcid++;
//        }
//        cur=chi->sibling;
//        break;
//      }
//    }
//  }
//  if(cur->stype==STMT_DECL ||cur->stype==STMT_DECL_CONST) {
//    this->type=cur->child->type;
//    for (TreeNode *chi = cur->child; chi != nullptr; chi = chi->sibling) {
//      if (chi->nodeType == NODE_VAR) {
//        if(SymMap->count(chi->var_name)!=0){
//          chi->var_id=-2;
//        } else {
//          symbol_entry symbol;
//          symbol.name=chi->var_name;
//          symbol.type=chi->type=this->type;
//          symbol.token=chi->var_id = id;
//          SymMap->insert(make_pair(chi->var_name, symbol));
//          id++;
//        }
//      } else if(chi->nodeType==NODE_STMT && chi->stype==STMT_ASSI){
//        if(SymMap->count(chi->child->var_name)!=0){
//          chi->var_id=-2;
//        } else {
//          symbol_entry symbol;
//          symbol.name=chi->child->var_name;
//          symbol.type=chi->child->type=this->type;
//          symbol.token=chi->child->var_id = id;
//          SymMap->insert(make_pair(chi->child->var_name, symbol));
//          id++;
//        }
//      }
//    }
//    return;
//  }
//  if(cur->nodeType==NODE_BLOCK) {
//    STlist.push_back(SymMap);
//    SymMap = new map<string, symbol_entry>;
//    cur=cur->child;
//    while(cur!= nullptr){
//      cur->genSymbol();
//      cur=cur->sibling;
//    }
//    SymMap = STlist.back();
//    STlist.pop_back();
//    return;
//  }
//  cur=cur->child;
//  while(cur!= nullptr){
//    cur->genSymbol();
//    cur=cur->sibling;
//  }
//}
void Tree::genSymbol(TreeNode *t) {
  static auto *GlobalSymbolTable=new map<string,symbol_entry>;
  static auto *FuncSymbolTable=new map<string,int>;
  static map<string,symbol_entry> *SymMap=GlobalSymbolTable;
  static list<map<string,symbol_entry>*> STlist;
  static int id=0;
  static int funcid=0;
  TreeNode *cur=t;
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
    if(cur->var_name!="scanf"&&cur->var_name!="printf") {
      cur->var_id = -1;
      cout << cur->var_name<<" not defined at line " << cur->lineno << endl;
      exit(1);
    } else{
      cur->var_id = 0;
    }
//    cur->var_id = -1;
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
  if(cur->stype==STMT_DECL||cur->stype==STMT_DECL_CONST) {
    t->type=cur->child->type;
    for (TreeNode *chi = cur->child; chi != nullptr; chi = chi->sibling) {
      if (chi->nodeType == NODE_VAR) {
        if(SymMap->count(chi->var_name)!=0){
          chi->var_id=-2;
          cout << chi->var_name<<" redefined at line " << cur->lineno << endl;
          exit(1);
        } else {
          symbol_entry symbol;
          symbol.name=chi->var_name;
          symbol.type=chi->type=t->type;
          symbol.token=chi->var_id = id;
          SymMap->insert(make_pair(chi->var_name, symbol));
          id++;
        }
      } else if(chi->nodeType==NODE_STMT && chi->stype==STMT_ASSI){
        if(SymMap->count(chi->child->var_name)!=0){
          chi->var_id=-2;
        } else {
          symbol_entry symbol;
          symbol.name=chi->child->var_name;
          symbol.type=chi->child->type=t->type;
          symbol.token=chi->child->var_id = id;
          SymMap->insert(make_pair(chi->child->var_name, symbol));
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
      genSymbol(cur);
      cur=cur->sibling;
    }
    SymMap = STlist.back();
    STlist.pop_back();
    return;
  }
  cur=cur->child;
  while(cur!= nullptr){
    this->genSymbol(cur);
    cur=cur->sibling;
  }


}


// You can output more info...
void TreeNode::printSpecialInfo(ostream &out) {
    switch(this->nodeType){
        case NODE_CONST:
          out<<" type:"<<this->type->getTypeInfo().c_str()<<endl;
          break;
        case NODE_VAR:
          out<<" varname:"<<this->var_name.c_str()<<" varid:"<<this->var_id<<endl;
            break;
//        case NODE_EXPR:
//          printf("expr");
//            break;
        case NODE_STMT:
          switch (this->stype){
            case STMT_SKIP:
              out<<"stmt:skip\n";
              break;
            case STMT_DECL:
              out<<"stmt:decl\n";
              break;
            case STMT_DECL_CONST:
              out<<"stmt:decl const\n";
              break;
            case STMT_ASSI:
              out<<"stmt:assign\n";
              break;
          case STMT_ADD_ASSI:
            out<<"stmt:add assign\n";
            break;
          case STMT_MINUS_ASSI:
            out<<"stmt:minus assign\n";
            break;
          case STMT_MUL_ASSI:
            out<<"stmt:mul assign\n";
            break;
          case STMT_DIV_ASSI:
            out<<"stmt:div assign\n";
            break;
          case STMT_IF:
            out<<"stmt:if\n";
            break;
          case STMT_ELSE:
            out<<"stmt:else\n";
            break;
          case STMT_FOR:
            out<<"stmt:for\n";
            break;
          case STMT_WHILE:
            out<<"stmt:while\n";
            break;
          case STMT_CALL:
            out<<"stmt:call\n";
            break;
          case STMT_RET:
            out<<"stmt:return\n";
            break;
          case STMT_BRK:
            out<<"stmt:break\n";
            break;
            default:
              break;
          }
            break;
        case NODE_TYPE:
          out<<" type:"<<this->type->getTypeInfo().c_str()<<endl;
          break;
        case NODE_EXPR:
          switch (this->optype){
          case OP_ADD:
            out<<"op:ADD\n";
            break;
          case OP_MINUS:
            out<<"op:MINUS\n";
            break;
          case OP_MUL:
            out<<"op:MUL\n";
            break;
          case OP_DIV:
            out<<"op:DIV\n";
            break;
          case OP_EQ:
            out<<"op:EQ\n";
            break;
          case OP_UEQ:
            out<<"op:UEQ\n";
            break;
          case OP_GRE:
            out<<"op:GRE\n";
            break;
          case OP_LES:
            out<<"op:LES\n";
            break;
          case OP_GREEQ:
            out<<"op:GREEQ\n";
            break;
          case OP_LESEQ:
            out<<"op:LESEQ\n";
            break;
          case OP_AND:
            out<<"op:AND\n";
            break;
          case OP_OR:
            out<<"op:OR\n";
            break;
          case OP_NOT:
            out<<"op:NOT\n";
            break;
          case OP_AFT_ADD:
            out<<"op:AFT_ADD\n";
            break;
          case OP_AFT_MINUS:
            out<<"op:AFT_MINUS\n";
            break;
          case OP_PRE_ADD:
            out<<"op:PRE_ADD\n";
            break;
          case OP_PRE_MINUS:
            out<<"op:PRE_MINUS\n";
            break;
          case OP_MOD:
            out<<"op:MOD\n";
            break;
          case OP_ADDRESS:
            out<<"op:address\n";
            break;
          case OP_UMINUS:
            out<<"op:uminus\n";
            break;
          default:
            break;
          }
          break;

//        case NODE_PROG:
//          printf("prog");
//            break;

        default:
          out<<"\n";
          break;
    }
}
void TreeNode::typeCheck() {
//  this->printNodeInfo();
  TreeNode *cur=this;
  cur=cur->child;
  while (cur!= nullptr){
      cur->typeCheck();
      cur=cur->sibling;
  }
  if(this->nodeType==NODE_EXPR){
    if(this->optype<=OP_OR){
      this->type=TYPE_BOOL;
      if(this->optype==OP_NOT ||this->optype==OP_AND ||this->optype==OP_OR) {
        cur = this->child;
        while (cur != nullptr) {
          if (cur->type->type != VALUE_BOOL) {
            cout << "Bad boolean type at line: " << cur->lineno << endl;
            exit(1);
          }
          cur = cur->sibling;
        }
      }
    }
    if(this->optype==OP_ADD||this->optype==OP_MINUS){
      cur=this->child;
//      cout<<cur->var_name;
      while (cur!=nullptr){
        if(cur->type) {
          if (cur->type->type != VALUE_INT && cur->type->type != VALUE_FLOAT) {
            cout << "Bad int or float type at line: " << cur->lineno << endl;
            exit(1);
          }
        }
        cur = cur->sibling;
      }
      this->type=TYPE_INT;
    }
  }
  if(this->nodeType==NODE_STMT){
    if(this->stype==STMT_WHILE || this->stype==STMT_IF) {
      if(!this->child->type||this->child->type->type!=VALUE_BOOL){
        cout<<"Bad boolean type at line: " << this->lineno << endl;
        exit(1);
      }
    }
//    else if(this->stype==STMT_FOR) {
//      if (this->child->child->sibling->type->type != VALUE_BOOL) {
//        cerr << "Bad boolean type at line: " << this->lineno << endl;
//        exit(1);
//      }
//    }
    else if(this->stype==STMT_DECL){
      type=this->child->type;
      cur=this->child->sibling;
      while (cur!= nullptr){
//        if(cur->)
//        printf("%d",cur->type->type);
//        if(cur->type->type!=type->type){
//          cerr << "Bad decl type at line: " << cur->lineno << endl;
//          exit(1);
//        }
        cur=cur->sibling;
      }
    }
    else if(this->stype==STMT_ASSI ||this->stype==STMT_ADD_ASSI||this->stype==STMT_MINUS_ASSI){
//      if(this->child->type->type!=this->child->sibling->type->type){
//        cout << "Bad assign type at line: " << this->lineno << endl;
//        exit(1);
//      }
    }
  }
}
//string TreeNode::new_label()
//{
//  char tmp[20];
//  sprintf(tmp, "@%d", TreeNode::label_seq);
//  TreeNode::label_seq++;
//  return tmp;
//}
string Tree::new_label()
{
  char tmp[20];
  sprintf(tmp, ".L%d", Tree::label_seq);
  Tree::label_seq++;
  return tmp;
}
void Tree::stmt_get_label(TreeNode *t)
{
  switch (t->stype){
  case STMT_WHILE:
  {
    TreeNode *e = t->child;
    TreeNode *s = t->child->sibling;

    if (t->label.begin_label.empty())
      t->label.begin_label = new_label();
    s->label.next_label = t->label.begin_label;

    s->label.begin_label = e->label.true_label = new_label();

    if (t->label.next_label == "")
      t->label.next_label = new_label();
    e->label.false_label = t->label.next_label;
    if (t->sibling)
      t->sibling->label.begin_label = t->label.next_label;

    this->recursive_get_label(e);
    this->recursive_get_label(s);
    break;
  }
  case STMT_IF:
  {
    TreeNode *e = t->child;
    TreeNode *s = t->child->sibling;

//    if (this->label.begin_label.empty())
//      this->label.begin_label = new_label();
//    s->label.next_label = this->label.begin_label;

    s->label.begin_label = e->label.true_label = new_label();

    if (t->label.next_label == "")
      t->label.next_label = new_label();
    if (t->sibling&&t->sibling->stype==STMT_ELSE) {
      t->sibling->label.begin_label=e->label.false_label = new_label();
    } else{
      e->label.false_label = t->label.next_label;
    }

    s->label.next_label=t->label.next_label;

    this->recursive_get_label(e);
    this->recursive_get_label(s);
    if (t->sibling) {
      if(t->sibling->stype==STMT_ELSE){
        if(t->sibling->sibling)
          t->sibling->sibling->label.begin_label = t->label.next_label;
      }
      else
        t->sibling->label.begin_label = t->label.next_label;
    }
    break;
  }
  case STMT_ELSE:{
    TreeNode *e = t->child;
    TreeNode *s = t->child->sibling;
    this->recursive_get_label(e);
    if(s) {
      this->recursive_get_label(s);
    }
    break;
  }
  case STMT_FOR: {
    TreeNode *i = t->child;
    TreeNode *e = i->sibling;
    this->recursive_get_label(i);
    this->recursive_get_label(e);
    break;

  }

  default:
    if (t->sibling)
      t->sibling->label.begin_label = t->label.next_label;
    /* ... */
  }
}


void Tree::expr_get_label(TreeNode *t)
{
  if (t->optype != OP_OR && t->optype != OP_AND&&t->optype != OP_NOT)
    return;
  TreeNode *e1 = t->child;
  TreeNode *e2 = t->child->sibling;
  switch (t->optype)
  {
  case OP_OR:
    e1->label.true_label = e2->label.true_label = t->label.true_label;
    e1->label.false_label = new_label();
    e2->label.false_label=t->label.false_label;
    break;
  case OP_AND:
    e1->label.true_label = new_label();
    e2->label.true_label = t->label.true_label;
    e1->label.false_label = e2->label.false_label = t->label.false_label;
    break;
  case OP_NOT:
    e1->label.true_label=t->label.false_label;
    e1->label.false_label=t->label.true_label;
    break;
  default:
    break;
    /* ... */
  }
  if (e1)
    recursive_get_label(e1);
  if (e2)
    recursive_get_label(e2);
}

void Tree::recursive_get_label(TreeNode *t){
  if (t->nodeType == NODE_STMT) {
    this->stmt_get_label(t);
  }else if (t->nodeType == NODE_EXPR){
    this->expr_get_label(t);
//    this->expr_get_label();
  }else{
    TreeNode *cur=t->child;
    while (cur){
      recursive_get_label(cur);
      cur=cur->sibling;
    }
  }
}

void Tree::get_label(){
  this->label_seq=0;
  this->recursive_get_label(root);

}
void Tree::genText(ostream &out) {
  for(auto iter = this->string_map.begin(); iter != string_map.end(); iter++){
    out<<".LC"<<iter->second<<":\n\t.string "<<iter->first<<endl;
  }
}

void Tree::gen_header(ostream &out)
{
  out << "# your asm code header here" << endl;
  /*your code here*/
  out<<"\t.file\t"<<"\"input.c\""<<endl;
//  out<<"\t.file\t"<<this->filename<<endl;
  out<<"\t.text"<<endl;
}
void TreeNode::gen_decl(ostream &out){
  if(this->stype==STMT_DECL ){
    TreeNode *p = this->child;
    while(p){
      if(p->nodeType==NODE_STMT&&p->stype==STMT_ASSI){
        out<<"\t.global _"<< p->child->var_name<< p->child->var_id<<endl;
        out<<"\t.data\n\t.align 4\n\t.type _"<<p->child->var_name<< p->child->var_id<<" @object\n";
        out<<"_"<<p->child->var_name<< p->child->var_id<<":\n";
        if(p->child->sibling->type->type==VALUE_INT) {
          out << "\t.long "<<p->child->sibling->int_val<<endl;
        }

//        out<<"\t.comm\t"<< "_" << p->child->var_name<< p->child->var_id<<",4,4\n";
      } else if(p->nodeType==NODE_VAR){
        out<<"\t.comm\t"<< "_" << p->var_name<< p->var_id<<",4,4\n";
      }
      p=p->sibling;
    }
  } else if(this->stype==STMT_DECL_CONST){
    TreeNode *p = this->child;
    while(p){
      if(p->nodeType==NODE_STMT&&p->stype==STMT_ASSI){
        out<<"\t.global _"<< p->child->var_name<< p->child->var_id<<endl;
        out<<"\t.section\t.rodata\n\t.align 4\n\t.type _"<<p->child->var_name<< p->child->var_id<<" @object\n";
        out<<"_"<<p->child->var_name<< p->child->var_id<<":\n";
        if(p->child->sibling->type->type==VALUE_INT) {
          out << "\t.long "<<p->child->sibling->int_val<<endl;
        }
      }
      p=p->sibling;
    }
  }
  TreeNode *cur=this->child;
  while (cur){
    cur->gen_decl(out);
    cur=cur->sibling;
  }
}
void Tree::get_temp_var(TreeNode *t){
  if (t->nodeType!=NODE_EXPR) {
    TreeNode *cur=t->child;
    while (cur){
      this->get_temp_var(cur);
      cur=cur->sibling;
    }
    return;
  }

  TreeNode *arg1 = t->child;
  TreeNode *arg2 = t->child->sibling;

  if (arg1->nodeType==NODE_EXPR) {
    this->get_temp_var(arg1);
    if (arg2 && arg2->nodeType==NODE_EXPR) {
      this->get_temp_var(arg2);
      if (arg2->optype >OP_MOD && arg2->optype <OP_UMINUS) {
        temp_var_seq--;
      }
    }
    if (arg1->optype >OP_MOD && arg1->optype <OP_UMINUS) {
      temp_var_seq--;
    }
  } else{
    if (arg2 && arg2->nodeType==NODE_EXPR) {
      this->get_temp_var(arg2);
      if (arg2->optype >OP_MOD && arg2->optype <OP_UMINUS) {
        temp_var_seq--;
      }
    }
  }
  if ((t->optype <OP_MOD || t->optype >OP_UMINUS)) {
//    temp_var_seq++;
    return;
  }
  t->temp_var = temp_var_seq;
//  cout<<"@"<<t->nodeID<<" "<<t->temp_var<<endl;
  temp_var_seq++;

  if(temp_var_seq>temp_var_nums){
    temp_var_nums=temp_var_seq;
  }
}
void Tree::gen_temp_var(ostream &out){
  get_temp_var(this->root);
  for(int i=0;i<temp_var_nums;i++){
    out<<"\t.comm\t"<< "t" << i<<",4,4\n";
//    out<<"t"<<i<<":"<<endl;
//    out<<"\tDWORD  0"<<endl;
  }
}
void Tree::func_gen_code(ostream &out,TreeNode *t) {
  out<<"\t.globl\t"<<t->child->sibling->var_name<<endl;
  out<<"\t.type\t"<<t->child->sibling->var_name<<", @function"<<endl;
  out<<t->child->sibling->var_name<<":"<<endl;
}
void Tree::stmt_gen_code(ostream &out,TreeNode *t) {
  TreeNode *cur=t->child;
  TreeNode *e,*s;
  int paramNum=0;
  switch (t->stype) {
  case STMT_CALL:
    if (t->label.begin_label != "")
      out << t->label.begin_label << ":" << endl;
    e=t->child->sibling;
    s=e->sibling;
    if(s&&e){
      if(s->sibling){
        out<<"\tsubl $"<<4<<", %esp"<<endl;
        if(s->sibling->nodeType==NODE_VAR)
          out<<"\tpushl _"<<s->sibling->var_name<<s->sibling->var_id<<endl;
        else if(s->sibling->nodeType==NODE_EXPR&&s->sibling->optype==OP_ADDRESS){
          out<<"\tpushl $_"<<s->sibling->child->var_name<<s->sibling->child->var_id<<endl;
        }
      } else {
        out << "\tsubl $" << 8 << ", %esp" << endl;
      }
      if(s->nodeType==NODE_VAR)
        out<<"\tpushl _"<<s->var_name<<s->var_id<<endl;
      else if(s->nodeType==NODE_EXPR&&s->optype==OP_ADDRESS){
        out<<"\tpushl $_"<<s->child->var_name<<s->child->var_id<<endl;
      }
      out<<"\tpushl $.LC"<<this->string_map[e->str_val]<<endl;
    }else{
      out<<"\tsubl $"<<12<<", %esp"<<endl;
      out<<"\tpushl $.LC"<<this->string_map[e->str_val]<<endl;
    }

    out<<"\tcall "<<t->child->var_name<<endl;
    out<<"\taddl $"<<16<<", %esp"<<endl;
    break;
  case STMT_RET:
    if (t->label.begin_label != "")
      out << t->label.begin_label << ":" << endl;
    movlNode(t->child,out);
    out<<"\tret"<<endl;
    break;

  case STMT_WHILE:
    if (t->label.begin_label != "")
      out << t->label.begin_label << ":" << endl;
    this->recursive_gen_code(out,t->child);
    out << t->child->label.true_label<<":"<<endl;
    this->recursive_gen_code(out,t->child->sibling);
    out << "\tjmp " << t->label.begin_label << endl;
    if(!t->sibling)
      out << t->label.next_label << ":" << endl;
    break;
  case STMT_FOR:
    if (t->label.begin_label != "")
      out << t->label.begin_label << ":" << endl;
    this->recursive_gen_code(out,t->child);
    this->recursive_gen_code(out,t->child->sibling);
    break;
  case STMT_IF:
    if (t->label.begin_label != "")
      out << t->label.begin_label << ":" << endl;
    this->recursive_gen_code(out,t->child);
    out << t->child->label.true_label<<":"<<endl;
    this->recursive_gen_code(out,t->child->sibling);
    if(!t->sibling)
      out << t->label.next_label << ":" << endl;
    else if(t->sibling->nodeType==NODE_STMT&&t->sibling->stype==STMT_ELSE){
      out<<"\tjmp "<<t->label.next_label<<endl;
      t->sibling->label.next_label=t->label.next_label;
    }
    break;
  case STMT_ELSE:
    if (t->label.begin_label != "")
      out << t->label.begin_label << ":" << endl;
    this->recursive_gen_code(out,t->child);
    if(t->child->sibling){
      this->recursive_gen_code(out,t->child->sibling);
    }
    if(!t->sibling)
      out << t->label.next_label << ":" << endl;
    break;
//  case STMT_FOR:
//    if (t->label.begin_label != "")
//      out << t->label.begin_label << ":" << endl;
//    this->recursive_gen_code(out,t->child);
//    out << t->child->label.true_label<<":"<<endl;
//    this->recursive_gen_code(out,t->child->sibling);
//    out << "\tjmp " << t->label.begin_label << endl;
//    break;
  case STMT_ASSI:
    if (t->label.begin_label != "")
      out << t->label.begin_label << ":" << endl;


    if(t->child->sibling->nodeType==NODE_EXPR || (t->child->sibling->nodeType==NODE_STMT&&t->child->sibling->stype==STMT_ASSI)) {
      this->recursive_gen_code(out,t->child->sibling);
    }else if(t->child->sibling->nodeType==NODE_VAR){
      getMov(t->child->sibling,out);
      out<<t->child->sibling->var_name<<t->child->sibling->var_id<<", %eax"<<endl;
    }else{
      movlNode(t->child->sibling,out);
    }
    if(t->optype==OP_ADD) {
      movlNode(t->child,out);
      out << "\taddl ";
      getNode(t->child->sibling,out);
      out<<",%eax\n";
    } else if(t->optype==OP_MINUS) {
      movlNode(t->child,out);
      out << "\tsubl ";
      getNode(t->child->sibling,out);
      out<<",%eax\n";
    }
    out<<"\tmovl %eax,_";
    out<<t->child->var_name<<t->child->var_id<<endl;

//    movlNode(t->child,out);
//
//    if(t->optype==OP_ADD) {
//      out << "\taddl ";
//    } else if(t->optype==OP_MINUS) {
//      out << "\tsubl ";
//    }
//    getNode(t->child->sibling,out);
//    out<<""
//    out << "\tmovl %eax, _"<<t->child->var_name<<t->child->var_id<<endl;
    break;
  default:
    break;
  }
}



void Tree::expr_gen_code(ostream &out,TreeNode *t)
{
  TreeNode *e1 = t->child;
  TreeNode *e2 = t->child->sibling;
  if(e1->nodeType==NODE_EXPR){
    expr_gen_code(out,e1);
  }
  switch (t->optype)
  {
    case OP_UMINUS:
      if(e2) {
        if (e2->nodeType == NODE_EXPR) {
          expr_gen_code(out, e2);
        }
      }
      movlNode(e1,out);
      out<<"\tnegl %eax\n";
      out << "\tmovl %eax, t" << t->temp_var << endl;
      break;
    case OP_ADD:case OP_MINUS:case OP_MUL:
      if(e2) {
        if (e2->nodeType == NODE_EXPR) {
          expr_gen_code(out, e2);
        }
      }
      movlNode(e1,out);
    if(t->optype==OP_ADD) {
      out << "\taddl ";
    } else if(t->optype==OP_MINUS) {
      out << "\tsubl ";
    } else if(t->optype==OP_MUL) {
      out << "\timull ";
    }
    getNode(e2,out);
    out << ", %eax" << endl;
    out << "\tmovl %eax, t" << t->temp_var << endl;
    break;
  case OP_DIV:case OP_MOD:
    if(e2) {
      if (e2->nodeType == NODE_EXPR) {
        expr_gen_code(out, e2);
      }
    }
    movlNode(e1,out);
    movlNode(e2,out,"%ecx");
    out << "\tcltd"<<endl;
    out << "\tidivl %ecx"<<endl;
    if(t->optype==OP_MOD) {
      out << "\tmovl %edx, %eax"<<endl;
    }
    out << "\tmovl %eax, t" << t->temp_var << endl;
    break;
  case OP_LES:case OP_GRE:case OP_LESEQ:case OP_GREEQ:case OP_EQ:case OP_UEQ: {
    if(e2) {
      if (e2->nodeType == NODE_EXPR) {
        expr_gen_code(out, e2);
      }
    }
    //    if(e1->nodeType!=NODE_CONST)
    if (t->label.begin_label != "") {
      out << t->label.begin_label << ":" << endl;
    }
    movlNode(e1, out, "%eax");
    //    if(e2->nodeType!=NODE_CONST)
    movlNode(e2, out, "%edx");
    out << "\tcmpl %edx,%eax\n";

    if (t->optype == OP_GREEQ) {
      out << "\tjl ";
    } else if (t->optype == OP_GRE) {
      out << "\tjle ";
    } else if (t->optype == OP_LES) {
      out << "\tjge ";
    } else if (t->optype == OP_LESEQ) {
      out << "\tjg ";
    } else if (t->optype == OP_EQ) {
      out << "\tjne ";
    } else if (t->optype == OP_UEQ) {
      out << "\tje ";
    }
    out << t->label.false_label << endl;

    if(t->sibling){
      out<<"\tjmp "<<t->label.true_label<<endl;
    }
    break;
  }
  case OP_OR:
    out<<e1->label.false_label<<":\n";
    if(e2) {
      if (e2->nodeType == NODE_EXPR) {
        expr_gen_code(out, e2);
      }
    }
    break;
  case OP_AND:
    out<<e1->label.true_label<<":\n";
    if(e2) {
      if (e2->nodeType == NODE_EXPR) {
        expr_gen_code(out, e2);
      }
    }
    break;
  default:
    break;
  }
}
//
void Tree::recursive_gen_code(ostream &out,TreeNode *t){
  if (t->nodeType == NODE_STMT){
    this->stmt_gen_code(out,t);
  } else if (t->nodeType == NODE_EXPR){
    this->expr_gen_code(out,t);
  }else if(t->nodeType==NODE_FUNC){
    this->func_gen_code(out,t);
    TreeNode *cur = t->child;
    while (cur) {
      recursive_gen_code(out,cur);
      cur = cur->sibling;
    }
  }else if(t->nodeType == NODE_BLOCK ||t->nodeType == NODE_PROG){
    TreeNode *cur = t->child;
    while (cur) {
      recursive_gen_code(out,cur);
      cur = cur->sibling;
    }
  }
}

void Tree::gen_code(ostream &out)
{
  this->gen_header(out);
//  Node *p = root->children[0];
//  if (p->kind == DECL_NODE)
  this->root->gen_decl(out);
  this->gen_temp_var(out);
  this->genText(out);
  out << endl << endl << "# your asm code here" << endl;
  out << "\t.text" << endl;
//  out << "\t.globl _start" << endl;
  this->recursive_gen_code(out,this->root);
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
