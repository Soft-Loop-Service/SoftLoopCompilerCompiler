#include "./item_set.hpp"

namespace ItemSet
{

    FollowSetClass::FollowSetClass(BNFParse::DeploymentStruct deployment_syntax, vstring null_set, BNFParse::mp_s_Dtoken first_set)
    {
        this->deployment_syntax = deployment_syntax;
        this->formula_map_keys = getMapKeyString(this->deployment_syntax.formula_map);
        this->null_set = null_set;
        this->first_set = first_set;
    }
    BNFParse::mp_s_Dtoken FollowSetClass::findFolllowSet()
    {
        struct BNFParse::DeploymentTokenStruct root_symbol = {DOLLAR, is_id_Dollar};
        follow_set[DOLLAR].push_back(root_symbol);
        int formula_map_size = this->deployment_syntax.formula_map.size();
        for (int s = 0; s < formula_map_size; s++)
        {
            string search_key = this->formula_map_keys[s];
            recursionFollowSet(search_key);
        }

        return follow_set;
    }

    void FollowSetClass::recursionFollowSet(string search_key)
    {
        if (hasKeyMap(this->already_explored, search_key))
        {
            return;
        }
        already_explored.push_back(search_key);

        int formula_map_size = this->deployment_syntax.formula_map.size();
        for (int i = 0; i < formula_map_size; i++)
        {
            string current_key = this->formula_map_keys[i];
            BNFParse::vDeploymentFormulaExpansionStruct formula_expansion_vector = this->deployment_syntax.formula_map[current_key].formula_expansion_vector;
            int formula_expansion_vector_size = formula_expansion_vector.size();
            for (int j = 0; j < formula_expansion_vector_size; j++)
            {
                BNFParse::vDeploymentTokenStruct token_vector = formula_expansion_vector[j].token_vector;

                int token_vector_size = token_vector.size();
                for (int k = 0; k < token_vector_size; k++)
                {
                    if (search_key == token_vector[k].token_str)
                    {
                        bool flag = true;
                        int n = k;
                        while (flag && n < token_vector_size)
                        {

                            if (n == token_vector_size - 1) // 一番最後であるとき
                            {
                                findNextNull(search_key, current_key);
                                flag = false;
                            }
                            else // 最後以外の時
                            {
                                string next_key = token_vector[n + 1].token_str;
                                int next_label = token_vector[n + 1].label;

                                if (next_label == is_id_TerminalSymbol)
                                {
                                    if (!hasDtoken(this->follow_set[search_key], token_vector[n + 1]))
                                    {
                                        this->follow_set[search_key].push_back(token_vector[n + 1]);
                                    }
                                    flag = false;
                                }

                                flag = findNext(search_key, current_key, next_key);
                                if (flag)
                                {
                                    findNextNull(search_key, current_key);
                                }
                            }

                            n++;
                        }
                    }
                }
            }
        }
    }

    bool FollowSetClass::findNext(string search_key, string current_key, string next_key)
    {

        BNFParse::vDeploymentTokenStruct next_first_set = first_set[next_key];

        for (int i = 0; i < next_first_set.size(); i++)
        {
            if (!hasDtoken(this->follow_set[search_key], next_first_set[i]))
            {
                this->follow_set[search_key].push_back(next_first_set[i]);
            }
        }
        bool isNull = hasKeyMap(this->null_set, next_key);
        return isNull;
    }

    bool FollowSetClass::findNextNull(string search_key, string current_key)
    {
        recursionFollowSet(current_key);

        for (int i = 0; i < follow_set[current_key].size(); i++)
        {
            if (!hasDtoken(this->follow_set[search_key], follow_set[current_key][i]))
            {

                this->follow_set[search_key].push_back(follow_set[current_key][i]);
            }
        }
    }
};
