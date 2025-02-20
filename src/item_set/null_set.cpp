#include "./item_set.hpp"
namespace ItemSet
{

    NullSetClass::NullSetClass(BNFParse::DeploymentStruct deployment_syntax)
    {
        this->deployment_syntax = deployment_syntax;
        this->formula_map_keys = getMapKeyString(this->deployment_syntax.formula_map);
    }

    vstring NullSetClass::findNullsSet()
    {
        int formula_map_size = this->deployment_syntax.formula_map.size();
        for (int i = 0; i < formula_map_size; i++)
        {
            string current_key = this->formula_map_keys[i];
            recursionNullSet(current_key);
        }
        return null_set;
    }

    bool NullSetClass::recursionNullSet(string current_key)
    {

        if (hasKeyMap(this->already_explored, current_key))
        {
            if (hasKeyMap(this->null_set, current_key))
            {
                return true;
            }
            return false;
        }

        if (hasKeyMap(this->null_set, current_key))
        {
            return true;
        }
        already_explored.push_back(current_key);
        BNFParse::vDeploymentFormulaExpansionStruct formula_expansion_vector = this->deployment_syntax.formula_map[current_key].formula_expansion_vector;
        int formula_expansion_vector_size = formula_expansion_vector.size();

        if (formula_expansion_vector_size == 0)
        {
            // NULLである
            null_set.push_back(current_key);
            return true;
        }

        for (int j = 0; j < formula_expansion_vector_size; j++)
        {
            int null_count = 0;
            BNFParse::vDeploymentTokenStruct token_vector = formula_expansion_vector[j].token_vector;

            int token_vector_size = token_vector.size();

            if (token_vector_size == 0)
            {
                null_set.push_back(current_key);
                return true;
            }

            for (int k = 0; k < token_vector_size; k++)
            {
                string token_str = token_vector[k].token_str;
                int label = token_vector[k].label;

                // あわせて左再帰の除去

                if (current_key == token_str)
                {
                    continue;
                }

                if (label == is_id_NonterminalSymbolRight)
                {
                    bool result = recursionNullSet(token_str);
                    if (result)
                    {
                        null_count++;
                    }
                }

                // }
            }
            if (null_count == token_vector_size)
            {
                null_set.push_back(current_key);
                return true;
            }

            // this->null_set[current_key] = null_flag;
        }
        return false;
    }
};
