#include "./bnf.hpp"

namespace BNFParse
{
    // typedef std::vector<DeploymentGroupStruct> vDeploymentGroupStruct;

    // typedef std::set<DeploymentTokenStruct> setDeploymentTokenStruct;

    bool hasDtoken(vector<DeploymentTokenStruct> keys, DeploymentTokenStruct key)
    {
        bool flag = false;
        for (size_t i = 0; i < keys.size(); i++)
        {
            if (keys[i].token_str == key.token_str && keys[i].label == key.label)
            {
                return true;
            }
            /* code */
        }
        return false;
    }

    DeploymentStruct expansionDeployment(BNFToken &bnf_token_p, BNFSymbol &bnf_symbol_p, RetrieveSymbol &nonterminal_symbol_left, RetrieveSymbol &symbols)
    {

        int **LR0Automaton;
        bool arr_bool = getTwoDimensionalArray(LR0Automaton, 100, bnf_symbol_p.unique_symbol_len);
        if (!arr_bool)
        {
            printf("配列確保に失敗");
        }

        struct DeploymentStruct deployment_syntax;
        int formula_expansion_count = 0;

        // 3次元配列 上から順に すべての式：式単体：式を構成するtoken
        for (int i = 0; i < nonterminal_symbol_left.len; i++)
        {
            struct DeploymentFormulaStruct formula;
            v2string bnf_right_tokens;
            mp_s_i bnf_right_map;
            generateBNFRight(bnf_token_p, bnf_symbol_p, nonterminal_symbol_left, i, bnf_right_tokens, bnf_right_map);

            string key = string(bnf_token_p.token_string_array[nonterminal_symbol_left.array[i]]);

            if (hasKeyMap(getMapKeyString(deployment_syntax.formula_map), key))
            {
                formula = deployment_syntax.formula_map[key];
            }

            for (int j = 0; j < bnf_right_tokens.size(); j++)
            {
                struct DeploymentFormulaExpansionStruct formula_expansion;

                for (int k = 0; k < bnf_right_tokens[j].size(); k++)
                {

                    struct DeploymentTokenStruct token;
                    token.token_str = bnf_right_tokens[j][k];

                    printf("token_str %s\n", token.token_str.c_str());

                    token.label = bnf_right_map[token.token_str];

                    formula_expansion.token_vector.push_back(token);
                }
                // if (bnf_right_tokens[j].size() > 0)
                // {
                formula_expansion.formula_expansion_label = formula_expansion_count;
                formula.formula_expansion_vector.push_back(formula_expansion);
                formula_expansion_count++;
                // }
            }
            // deployment_syntax.formula_vector.push_back(formula);

            deployment_syntax.formula_map[key] = formula;
        }

        return deployment_syntax;
    }
};
