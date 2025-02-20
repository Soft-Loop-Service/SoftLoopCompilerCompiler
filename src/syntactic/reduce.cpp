
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "./syntactic_analysis.hpp"

namespace Syntactic
{

    void output_vReduceFormula(string name, LRTable::vReduceFormula v)
    {
        printf("%s 結果 : ", name.c_str());
        int vsize = v.size();
        for (int i = 0; i < vsize; i++)
        {
            // printf("%s ", v[vsize - i - 1].token_left.c_str());
            printf("%s ", v[i].token_left.c_str());
        }
        printf("\n");
    }

    // 入力を一つ消費して、指定された値をpushする
    void syntacticAnalysisProcessShift(LRTable::LRTableMultilayer LR_table_multilayer, string token_tyoke, sint &stack_analysis)
    {
        printf("Shift : ");
        int top = stack_analysis.top();

        printf("top : %d ", top);

        int next_state = LR_table_multilayer.LR_table_shift.LR_table_column_map[token_tyoke][top].getCell();
        stack_analysis.push(next_state);
        printf("\n");
    }

    void syntacticAnalysisProcessReduce(LRTable::LRTableMultilayer LR_table_multilayer, string token_tyoke, sint &stack_analysis, LRTable::vReduceFormula &syntactic_analysis_formula)
    {
        printf("Reduce : ");

        int top = stack_analysis.top();
        LRTable::ReduceFormula state = LR_table_multilayer.LR_table_reduce.LR_table_column_map[token_tyoke][top].getCell();

        syntactic_analysis_formula.push_back(state);

        printf("token_left : %s %d ", state.token_left.c_str(), state.token_vector.size());

        for (int i = 0; i < state.token_vector.size(); i++)
        {
            stack_analysis.pop();
        }

        int top2 = stack_analysis.top();
        printf("top2 : %d ", top2);
        printf("top2 sta : %s %d\n", state.token_left.c_str(), top2);

        int next_state = LR_table_multilayer.LR_table_goto.LR_table_column_map[state.token_left][top2].getCell();

        printf("top2 state : %d\n", next_state);

        stack_analysis.push(next_state);

        printf("top2 end : %d\n", top2);
        printf("\n");
    }

    void syntacticAnalysisProcessAccept()
    {
        printf("syntacticAnalysisProcessAccept\n");
    }
    // syntactic_analysis_formula

    /// @param LR_table_multilayer
    /// @param token_string_vector
    /// @param syntactic_analysis_formula // 構文解析表出力。出力ストリーム
    void syntacticAnalysisProcess(LRTable::LRTableMultilayer LR_table_multilayer, LexicalAnalysis::vLexicalToken token_string_vector, LRTable::vReduceFormula &syntactic_analysis_formula)
    {
        sint stack_analysis; // 構文解析表スタック

        stack_analysis.push(0);

        int i = 0;
        while (i < token_string_vector.size())
        {

            string current_token = token_string_vector[i].token;
            string current_token_type = token_string_vector[i].token_type;

            int top = stack_analysis.top();
            printf("CurrentToken %s %s %d\n", current_token.c_str(), current_token_type.c_str(), top);

            string inquiry_token_accept = LR_table_multilayer.LR_table_accept.LR_table_column_map.count(current_token) != 0 ? current_token : current_token_type;
            string inquiry_token_shift = LR_table_multilayer.LR_table_shift.LR_table_column_map.count(current_token) != 0 ? current_token : current_token_type;
            string inquiry_token_reduce = LR_table_multilayer.LR_table_reduce.LR_table_column_map.count(current_token) != 0 ? current_token : current_token_type;

            if (LR_table_multilayer.LR_table_accept.LR_table_column_map[inquiry_token_accept][top].getValid())
            {
                printf("acc \n");
                output_stack("構文解析 終了", stack_analysis);
                output_vReduceFormula("構文解析 終了", syntactic_analysis_formula);
                return;
            }

            if (LR_table_multilayer.LR_table_shift.LR_table_column_map[inquiry_token_shift][top].getValid())
            {
                printf("shift \n");
                syntacticAnalysisProcessShift(LR_table_multilayer, inquiry_token_shift, stack_analysis);
                output_stack("構文解析", stack_analysis);
                output_vReduceFormula("構文解析", syntactic_analysis_formula);

                printf("\n");
                i++;
                continue;
            }

            if (LR_table_multilayer.LR_table_reduce.LR_table_column_map[inquiry_token_reduce][top].getValid())
            {
                printf("reduce \n");

                syntacticAnalysisProcessReduce(LR_table_multilayer, inquiry_token_reduce, stack_analysis, syntactic_analysis_formula);
                output_stack("構文解析", stack_analysis);
                output_vReduceFormula("構文解析", syntactic_analysis_formula);

                printf("\n");
                continue;
            }
            printf("ERROR\n");
            exit(1);
            return;
        }

        return;
    }
    void syntacticAnalysis(LRTable::LRTableMultilayer LR_table_multilayer, LexicalAnalysis::vLexicalToken token_string_vector, LRTable::vReduceFormula &syntactic_analysis_formula)
    {
        syntacticAnalysisProcess(LR_table_multilayer, token_string_vector, syntactic_analysis_formula);
        return;
    }
};
