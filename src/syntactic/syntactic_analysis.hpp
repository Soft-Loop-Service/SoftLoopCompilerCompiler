
#ifndef __SYNTDT
#define __SYNTDT
#include "./../definition.hpp"
#include "./../lr_table/lr_table.hpp"
#include "./../symbol.hpp"
#include "./../lexical/lexical_analysis.hpp"

namespace Syntactic
{
    class SyntacticTreeNode // 構文解析用
    {

    public:
        string token;
        int token_label;
        vint children;
        string parent_token;

        int source_code_line;
        int source_code_column;
        int token_order;

        int parent_node_index;

        SyntacticTreeNode(string token, int token_label, vint children)
        {
            this->token = token;
            this->token_label = token_label;
            this->children = children;
            this->parent_token = "";

            this->source_code_line = -1;
            this->source_code_column = -1;
            this->token_order = -1;

            this->parent_node_index = -1;
        }
        SyntacticTreeNode(string token, int token_label, vint children, string parent_token)
        {
            this->token = token;
            this->token_label = token_label;
            this->children = children;
            this->parent_token = parent_token;

            this->source_code_line = -1;
            this->source_code_column = -1;
            this->token_order = -1;

            this->parent_node_index = -1;
        }

        void setParent(int parent_node_index)
        {
            this->parent_node_index = parent_node_index;
        }

        void setTrace(int source_code_line,
                      int source_code_column,
                      int token_order)
        {
            this->source_code_line = source_code_line;
            this->source_code_column = source_code_column;
            this->token_order = token_order;
        }
    };
    typedef vector<SyntacticTreeNode> vSyntacticTree;

    void output_vReduceFormula(string name, LRTable::vReduceFormula v);

    void syntacticAnalysis(LRTable::LRTableMultilayer LR_table_multilayer, LexicalAnalysis::vLexicalToken token_string_vector, LRTable::vReduceFormula &syntactic_analysis_formula);

    bool isTokenSkepSyntacticAnalysis(string token_str);

    void syntacticParseTree(vSyntacticTree cst, vSyntacticTree &ast);

    void debugSyntacticAnalysisTree(vSyntacticTree &syntactic_analysis_tree, bool all);
    void debugSyntacticAnalysisTree(vSyntacticTree &syntactic_analysis_tree);

    void syntacticAnalysisTree(LRTable::vReduceFormula syntactic_analysis_formula, vSyntacticTree &syntactic_analysis_tree, LexicalAnalysis::vLexicalToken token_string_vector);
}
#endif