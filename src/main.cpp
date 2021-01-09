#include "common.h"
#include <fstream>
//extern TreeNode *root;
extern Tree *tree;
extern FILE *yyin;
extern int yyparse();

using namespace std;
int main(int argc, char *argv[])
{
    char *outfilename=new char[20];
    if (argc == 2){
        FILE *fin = fopen(argv[1], "r");
        if (fin != nullptr)
        {
            yyin = fin;
        }
        else
        {
            cerr << "failed to open file: " << argv[1] << endl;
        }
    }
    yyin=stdin;

//  if (argc == 3){
//    FILE *fin = fopen(argv[1], "r");
//    if (fin != nullptr)
//    {
//      yyin = fin;
//    }
//    else
//    {
//      cerr << "failed to open file: " << argv[1] << endl;
//    }
//    memcpy(outfilename,argv[2],strlen(argv[2]));
//  }
    yyparse();

    ofstream out("result.txt");
//    memcpy(outfilename,argv[1],strlen(argv[1]));
//    memcpy(tree->filename,argv[1],strlen(argv[1]));

    TreeNode *root=tree->root;
    if(root != NULL) {
        root->genNodeId();
        tree->genSymbol(tree->root);
        root->typeCheck();
        root->printAST(out);
        tree->get_label();
        tree->gen_code(cout);
    }
//    return 0;
}