

// 構文木

#include "./../lr_table/lr_table.hpp"
#include "./syntactic_analysis.hpp"
#include "./tree_control.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>

namespace Syntactic
{
    std::unordered_set<string> ast_keep_list = {"<function_message_passing>"};
    std::unordered_set<string> ast_keep_list_operator = {"<if_while>"};
    void cutParentheses(vSyntacticTree &tree, int parent_node_index, int current_node_index)
    {
        printf(" cutParentheses : %d\n", current_node_index);
        if (parent_node_index >= 0)
        {
            if ((tree[current_node_index].token_label == is_id_NonterminalSymbol && tree[current_node_index].children.size() == 0) ||
                (tree[current_node_index].token_label == is_id_TerminalSymbol && isTokenSkepSyntacticAnalysis(tree[current_node_index].token)) ||
                (tree[current_node_index].parent_token == "<dot_property>" && tree[current_node_index].token == "."))
            {
                cutChild(tree, parent_node_index, current_node_index);
            }
        }
        deleteNegativeValueChildren(tree, current_node_index);
        for (int i = 0; i < tree[current_node_index].children.size(); i++)
        {
            int c_index = tree[current_node_index].children[i];
            cutParentheses(tree, current_node_index, c_index);
        }
        deleteNegativeValueChildren(tree, current_node_index);

        if (parent_node_index >= 0)
        {
            if (tree[current_node_index].token_label == is_id_NonterminalSymbol &&
                ast_keep_list.find(tree[current_node_index].token) == ast_keep_list.end() && tree[current_node_index].children.size() == 1)
            {
                shortParentChild(tree, parent_node_index, current_node_index);
            }
        }
    }

    void pumpOperator(vSyntacticTree &tree, int parent_node_index, int current_node_index)
    {

        for (int i = 0; i < tree[current_node_index].children.size(); i++)
        {
            int c_index = tree[current_node_index].children[i];
            pumpOperator(tree, current_node_index, c_index);
        }
        printf(" pumpOperator : %d\n", current_node_index);

        if (parent_node_index >= 0 && tree[current_node_index].token_label == is_id_NonterminalSymbol &&
            ast_keep_list_operator.find(tree[current_node_index].token) == ast_keep_list_operator.end())
        {
            if (tree[current_node_index].children.size() == 3)
            {
                int flow_node_index = tree[current_node_index].children[1];
                SyntacticTreeNode token_center = tree[flow_node_index];
                if (token_center.token_label == is_id_TerminalSymbol && isExpr(token_center.token))
                {
                    shortParentChildFlow(tree, parent_node_index, current_node_index, flow_node_index);
                    deleteNegativeValueChildren(tree, flow_node_index);
                }
            }
            if (tree[current_node_index].children.size() >= 4)
            {
                int flow_node_index = tree[current_node_index].children[1];
                SyntacticTreeNode token_center = tree[flow_node_index];
                if (token_center.token_label == is_id_TerminalSymbol && isExpr(token_center.token))
                {
                    int left_index = tree[current_node_index].children[0];
                    int right_index = tree[current_node_index].children[2];

                    tree[current_node_index].children[0] = -1;
                    tree[current_node_index].children[2] = -1;

                    tree[flow_node_index].children.push_back(left_index);
                    tree[flow_node_index].children.push_back(right_index);

                    deleteNegativeValueChildren(tree, current_node_index);
                }
            }
        }
        debugSyntacticAnalysisTree(tree, false);
    }

    void syntacticParseTree(vSyntacticTree cst, vSyntacticTree &ast)
    {
        vSyntacticTree construction_tree = cst;

        debugSyntacticAnalysisTree(construction_tree, false);
        printf("AST解析  - A\n");

        cutParentheses(construction_tree, -1, 0);
        debugSyntacticAnalysisTree(construction_tree, false);
        pumpOperator(construction_tree, -1, 0);
        debugSyntacticAnalysisTree(construction_tree, false);

        printf("AST解析  - B\n");

        ast = construction_tree;
        return;
    }
}
