
#include "./item_set.hpp"
namespace ItemSet
{
    FirstSetClass::FirstSetClass(BNFParse::DeploymentStruct deployment_syntax, vstring null_set)
    {
        this->deployment_syntax = deployment_syntax;
        this->formula_map_keys = getMapKeyString(this->deployment_syntax.formula_map);
        this->null_set = null_set;
    }
    BNFParse::vDeploymentTokenStruct FirstSetClass::findFirstSetVector(BNFParse::vDeploymentTokenStruct request_token_vector)
    {
        BNFParse::vDeploymentTokenStruct first_set_vecotr = {};

        struct BNFParse::DeploymentTokenStruct root_symbol = {DOLLAR, is_id_Dollar};

        // 一度すべての集合を求める
        first_set = {};
        first_set[DOLLAR].push_back(root_symbol);
        int formula_map_size = this->deployment_syntax.formula_map.size();
        for (int i = 0; i < formula_map_size; i++)
        {
            string current_key = this->formula_map_keys[i];
            recursionFirstsSet(current_key); // 左辺によるfollows集合を求める
        }

        int request_token_vector_size = request_token_vector.size();
        for (int i = 0; i < request_token_vector_size; i++)
        {
            string current_key = request_token_vector[i].token_str;
            int current_label = request_token_vector[i].label;

            if (current_label == is_id_TerminalSymbol)
            {
                first_set_vecotr.push_back(request_token_vector[i]);
                break;
            }

            BNFParse::vDeploymentTokenStruct current_first_set = first_set[current_key];

            // 重複がないよう挿入
            for (int j = 0; j < current_first_set.size(); j++)
            {
                if (!hasDtoken(first_set_vecotr, current_first_set[j]))
                {
                    first_set_vecotr.push_back(current_first_set[j]);
                }
            }
        }

        return first_set_vecotr;
    }

    BNFParse::mp_s_Dtoken FirstSetClass::findFirstSet()
    {
        first_set = {};
        struct BNFParse::DeploymentTokenStruct root_symbol = {DOLLAR, is_id_Dollar};
        first_set[DOLLAR].push_back(root_symbol);
        int formula_map_size = this->deployment_syntax.formula_map.size();
        for (int i = 0; i < formula_map_size; i++)
        {
            string current_key = this->formula_map_keys[i];
            recursionFirstsSet(current_key);
        }

        //
        return first_set;
    }

    void FirstSetClass::recursionFirstsSet(string current_key)
    {
        if (hasKeyMap(this->already_explored, current_key))
        {
            return;
        }

        if (hasKeyMap(getMapKeyString(this->first_set), current_key))
        {
            return;
        }
        already_explored.push_back(current_key);
        BNFParse::vDeploymentFormulaExpansionStruct formula_expansion_vector = this->deployment_syntax.formula_map[current_key].formula_expansion_vector;
        int formula_expansion_vector_size = formula_expansion_vector.size();
        for (int j = 0; j < formula_expansion_vector_size; j++)
        {
            BNFParse::vDeploymentTokenStruct token_vector = formula_expansion_vector[j].token_vector;

            int token_vector_size = token_vector.size();

            if (token_vector_size == 0)
            {
                // NULLである
                continue;
            }
            else
            {
                int k = 0;
                int recursion_flag;
                do
                {
                    recursion_flag = false;
                    string token_str = token_vector[k].token_str;
                    int label = token_vector[k].label;

                    if (label == is_id_NonterminalSymbolRight)
                    {

                        // 左再帰の除去
                        if (current_key == token_str)
                        {
                            recursion_flag = true;
                        }
                        else
                        {
                            recursionFirstsSet(token_str);

                            for (int fc = 0; fc < first_set[token_str].size(); fc++)
                            {
                                BNFParse::DeploymentTokenStruct fc_element = first_set[token_str][fc];
                                if (!hasDtoken(first_set[current_key], fc_element))
                                {
                                    first_set[current_key].push_back(fc_element);
                                }
                            }
                        }
                    }
                    else if (label == is_id_TerminalSymbol)
                    {

                        first_set[current_key].push_back(token_vector[k]);
                    }
                    k++;

                } while (k < token_vector_size && (hasKeyMap(this->null_set, token_vector[k - 1].token_str) || recursion_flag));
            }
        }
    }
};
