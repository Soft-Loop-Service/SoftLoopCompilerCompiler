
// 解析木

#include "./../lr_table/lr_table.hpp"
#include "./syntactic_analysis.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

namespace Syntactic
{

    // 親要素を設定する
    void recursionSyntacticAnalysisTreeParentDFS(vSyntacticTree &syntactic_analysis_tree, int current_index)
    {
        for (int i = 0; i < syntactic_analysis_tree[current_index].children.size(); i++)
        {
            int c_index = syntactic_analysis_tree[current_index].children[i];
            syntactic_analysis_tree[c_index].setParent(current_index);
            recursionSyntacticAnalysisTreeParentDFS(syntactic_analysis_tree, c_index);
        }
    }

    /// @brief
    /// @param syntactic_analysis_tree
    /// @param syntactic_analysis_formula

    /// @param search_first_index 後ろから探索しているという条件で、探索済みの場所の先端の1つ手前
    /// @param depth 深さ
    /// 導出木に変換するため深さ優先探索を行う。その地点で木構造に分類する。つまり細かく木構造に分割していくイメージ。
    void recursionSyntacticAnalysisTreeDFS(vSyntacticTree &syntactic_analysis_tree, LRTable::vReduceFormula &syntactic_analysis_formula, LexicalAnalysis::vLexicalToken token_string_vector, int &search_first_index, int parent_node_index, int depth)
    {
        LRTable::ReduceFormula current_reduce_formula = syntactic_analysis_formula[search_first_index];

        SyntacticTreeNode new_node_nonterminal(current_reduce_formula.token_left,
                                               is_id_NonterminalSymbol,
                                               {});
        syntactic_analysis_tree.push_back(new_node_nonterminal);

        int new_current_index = syntactic_analysis_tree.size() - 1;

        int size = current_reduce_formula.token_vector.size();
        if (parent_node_index >= 0)
        {
            auto it_0 = syntactic_analysis_tree[parent_node_index].children.begin();
            syntactic_analysis_tree[parent_node_index].children.insert(it_0, new_current_index);
            syntactic_analysis_tree[new_current_index].parent_token = syntactic_analysis_tree[parent_node_index].token;
        }

        // 親ノードがnumberであるならば、末端記号ノードも数字である。

        for (int i = size - 1; i >= 0; i--)
        {
            BNFParse::DeploymentTokenStruct bnf = current_reduce_formula.token_vector[i];

            if (bnf.label == is_id_TerminalSymbol)
            {

                SyntacticTreeNode new_node_terminal(bnf.token_str, is_id_TerminalSymbol, {}, current_reduce_formula.token_left);
                syntactic_analysis_tree.push_back(new_node_terminal);

                auto it_p = syntactic_analysis_tree[new_current_index].children.begin();
                syntactic_analysis_tree[new_current_index].children.insert(it_p, syntactic_analysis_tree.size() - 1);
                continue;
            }

            search_first_index--;

            recursionSyntacticAnalysisTreeDFS(syntactic_analysis_tree, syntactic_analysis_formula, token_string_vector, search_first_index, new_current_index, (depth + 1));
        }
    }
    void debugSyntacticAnalysisTree(vSyntacticTree &syntactic_analysis_tree)
    {
        debugSyntacticAnalysisTree(syntactic_analysis_tree, true);
    }
    void debugSyntacticAnalysisTree(vSyntacticTree &syntactic_analysis_tree, bool all)
    {
        printf("debugSyntacticAnalysisTree * * * * * * * * * * * * * * * * \n");
        printf("%5s | ", "node");
        printf("%5s | ", "index");
        printf("%20s | ", "token");
        printf("%15s | ", "token label ");
        printf("%20s | ", "parent token ");
        printf("%7s | ", "parent ");
        printf("%10s | ", "children ");
        printf("\n");

        int count = 0;
        for (int i = 0; i < syntactic_analysis_tree.size(); i++)
        {
            SyntacticTreeNode node = syntactic_analysis_tree[i];

            if (!all && node.token_label == is_id_Unnecessary)
            {
                continue;
            }

            printf("%5d | ", i);
            printf("%5d | ", count);
            printf("%20s | ", node.token.c_str());
            printf("%15d | ", node.token_label);
            printf("%20s | ", node.parent_token.c_str());
            printf("%7d | ", node.parent_node_index);

            printf("{");
            for (int j = 0; j < node.children.size(); j++)
            {
                printf(" %3d ", node.children[j]);
            }

            printf("}\n");
            count++;
        }
    }

    void syntacticAnalysisTreeSubstitution(LRTable::vReduceFormula syntactic_analysis_formula, vSyntacticTree &syntactic_analysis_tree, LexicalAnalysis::vLexicalToken token_string_vector)
    {
        int count = 0;
        int size = syntactic_analysis_tree.size();

        int last_line = -1;
        int last_column = -1;
        int last_token_order = -1;

        for (int i = 0; i < size; i++)
        {
            int li = size - i - 1;
            if (syntactic_analysis_tree[li].token_label == is_id_TerminalSymbol)
            {
                syntactic_analysis_tree[li].token = token_string_vector[count].token;

                syntactic_analysis_tree[li].setTrace(token_string_vector[count].source_code_line, token_string_vector[count].source_code_column, token_string_vector[count].token_order);

                last_line = syntactic_analysis_tree[li].source_code_line;
                last_column = syntactic_analysis_tree[li].source_code_column;
                last_token_order = syntactic_analysis_tree[li].token_order;

                count++;
            }
            else
            {
                syntactic_analysis_tree[li].setTrace(last_line, last_column, last_token_order);
            }
        }
    }
    void syntacticAnalysisTree(LRTable::vReduceFormula syntactic_analysis_formula, vSyntacticTree &syntactic_analysis_tree, LexicalAnalysis::vLexicalToken token_string_vector)
    {

        // syntactic_analysis_formulaは構文解析の結果 後ろから見ていくことで木構造を構築する

        int size = syntactic_analysis_formula.size();
        int last = size - 1;

        recursionSyntacticAnalysisTreeDFS(syntactic_analysis_tree, syntactic_analysis_formula, token_string_vector, last, -1, 0);
        syntacticAnalysisTreeSubstitution(syntactic_analysis_formula, syntactic_analysis_tree, token_string_vector);
        recursionSyntacticAnalysisTreeParentDFS(syntactic_analysis_tree, 0);

        debugSyntacticAnalysisTree(syntactic_analysis_tree);
    }
};
