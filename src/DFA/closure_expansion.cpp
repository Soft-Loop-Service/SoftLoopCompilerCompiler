#include "./dfa.hpp"

/*
dot と tokenの関係は以下の通り
  token0 token1 token2 token3 ・・・
dot0   dot1  dot2   dot3・・・
*/

namespace DFAParse
{

    ClosureExpansion::ClosureExpansion(BNFParse::DeploymentStruct deployment_syntax)
    {
        this->deployment_syntax = deployment_syntax;
        // this->dot = dot;
    }

    void ClosureExpansion::cashFirstSet(size_t cash_key, BNFParse::vDeploymentTokenStruct &cash_first_set)
    {
        this->cash_first_set[cash_key] = cash_first_set;
    }

    bool ClosureExpansion::hasCashFirstSet(size_t cash_key)
    {
        return this->cash_first_set.find(cash_key) != this->cash_first_set.end();
    }

    BNFParse::vDeploymentTokenStruct ClosureExpansion::getCashFirstSet(size_t &cash_key)
    {
        return this->cash_first_set[cash_key];
    }

    size_t ClosureExpansion::getCashKey(vstring null_set, BNFParse::vDeploymentTokenStruct cash_first_set)
    {

        // null_set をsortする
        std::sort(null_set.begin(), null_set.end());

        // cash_first_set をsortする
        // std::sort(cash_first_set.begin(), cash_first_set.end(), [](const BNFParse::DeploymentTokenStruct &a, const BNFParse::DeploymentTokenStruct &b)
        //           { return a.token_str < b.token_str; });

        size_t hash = 0;
        for (const auto &state : cash_first_set)
        {
            hash ^= std::hash<string>()(state.token_str) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            hash ^= std::hash<int>()(state.label) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }

        // f_m.formula_map

        // f_mを出力する

        // vstring formula_map_keys = getMapKeyString(f_m.formula_map);
        // for (int i = 0; i < formula_map_keys.size(); i++)
        // {
        //     string key = formula_map_keys[i];
        //     BNFParse::vDeploymentTokenStruct token_vector = f_m.formula_map[key].formula_expansion_vector;
        //     for (const auto &state : token_vector)
        //     {
        //         hash ^= std::hash<string>()(state.token_str) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        //         hash ^= std::hash<int>()(state.label) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        //     }
        // }

        for (const auto &state : null_set)
        {
            hash ^= std::hash<string>()(state) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }

        return hash;
    }

    void ClosureExpansion::nodeClosureExpansion(LRItemStruct &lr_item)
    {
        // already_explored = {};
        already_explored_formula_expansion = {};

        vstring LR_formula_map_keys = getMapKeyString(lr_item.LR_formula_map);

        for (int i = 0; i < LR_formula_map_keys.size(); i++)
        {
            string key = LR_formula_map_keys[i];

            for (int j = 0; j < lr_item.LR_formula_map[key].LR_formula_expansion_vector.size(); j++)
            {
                int dot = lr_item.LR_formula_map[key].LR_formula_expansion_vector[j].dot;

                int la_size = lr_item.LR_formula_map[key].LR_formula_expansion_vector[j].look_ahead.size();
                for (int k = 0; k < la_size; k++)
                {
                    BNFParse::vDeploymentTokenStruct first_set = getLatterFirstSet(lr_item.LR_formula_map[key].LR_formula_expansion_vector[j], dot, k);
                    recursionNodeClosureExpansion(lr_item, key, j, first_set);
                }
            }
        }
    }

    void ClosureExpansion::nodeClosureExpansion(LRItemStruct &lr_item, string search_key)
    {
        // already_explored = {};
        already_explored_formula_expansion = {};
        for (int j = 0; j < lr_item.LR_formula_map[search_key].LR_formula_expansion_vector.size(); j++)
        {
            int dot = lr_item.LR_formula_map[search_key].LR_formula_expansion_vector[j].dot;
            int la_size = lr_item.LR_formula_map[search_key].LR_formula_expansion_vector[j].look_ahead.size();
            for (int k = 0; k < la_size; k++)
            {
                BNFParse::vDeploymentTokenStruct first_set = getLatterFirstSet(lr_item.LR_formula_map[search_key].LR_formula_expansion_vector[j], dot, k);
                recursionNodeClosureExpansion(lr_item, search_key, j, first_set);
            }
        }
    }

    // dotの後ろにある末端記号・非末端記号と、先読み記号を合体させる
    BNFParse::vDeploymentTokenStruct ClosureExpansion::getLatterToken(LRItemFormulaExpansionStruct LR_formula_expansion, int dot, int look_ahead_index)
    {
        BNFParse::vDeploymentTokenStruct latter_token = {};
        for (int i = dot + 1; i < LR_formula_expansion.token_vector.size(); i++)
        {
            latter_token.push_back(LR_formula_expansion.token_vector[i]);
        }

        latter_token.push_back(LR_formula_expansion.look_ahead[look_ahead_index]);

        return latter_token;
    }

    // 指定したdot以降の右辺tokenと、先読み記号をもとにfirst集合を作成する
    BNFParse::vDeploymentTokenStruct ClosureExpansion::getLatterFirstSet(LRItemFormulaExpansionStruct LR_formula_expansion, int dot, int look_ahead_index)
    {

        ItemSet::NullSetClass cnull_set_class = ItemSet::NullSetClass(deployment_syntax);
        vstring null_set = cnull_set_class.findNullsSet();
        ItemSet::FirstSetClass cfirst_set_class = ItemSet::FirstSetClass(deployment_syntax, null_set);
        BNFParse::vDeploymentTokenStruct latter_token = getLatterToken(LR_formula_expansion, dot, look_ahead_index);
        // auto p3 = std::chrono::high_resolution_clock::now();

        // latter_tokenでfirst_setのメモ化する hasKeyMapは使えない

        size_t cash_key = getCashKey(null_set, latter_token);
        if (hasCashFirstSet(cash_key))
        {
            // cash_keyを print
            // printf("cash_key : %zu\n", cash_key);

            // BNFParse::vDeploymentTokenStruct first_set_test = cfirst_set_class.findFirstSetVector(latter_token);

            // for (int i = 0; i < first_set_test.size(); i++)
            // {
            //     printf("first_set_test : %s\n", first_set_test[i].token_str.c_str());
            // }

            BNFParse::vDeploymentTokenStruct v = getCashFirstSet(cash_key);

            // first_set_testとvが違うかったらprint

            // first_set_testとvはsort

            // std::sort(first_set_test.begin(), first_set_test.end(), [](const BNFParse::DeploymentTokenStruct &a, const BNFParse::DeploymentTokenStruct &b)
            //           { return a.token_str < b.token_str; });

            // std::sort(v.begin(), v.end(), [](const BNFParse::DeploymentTokenStruct &a, const BNFParse::DeploymentTokenStruct &b)
            //           { return a.token_str < b.token_str; });

            // for (int i = 0; i < first_set_test.size(); i++)
            // {
            //     if (first_set_test[i].token_str != v[i].token_str)
            //     {
            //         printf("f : %s\n", first_set_test[i].token_str.c_str());
            //         printf("v : %s\n", v[i].token_str.c_str());
            //     }
            // }

            // for (int i = 0; i < v.size(); i++)
            // {
            //     printf("v : %s\n", v[i].token_str.c_str());
            // }
            return v;

            // return getCashFirstSet(cash_key);
        }

        BNFParse::vDeploymentTokenStruct first_set = cfirst_set_class.findFirstSetVector(latter_token); // ここ重たい(1ms)
        this->cashFirstSet(cash_key, first_set);

        // auto p4 = std::chrono::high_resolution_clock::now();

        // std::chrono::duration<double, std::milli> elapsed4 = p4 - p3;
        // std::cout << "経過時間: " << elapsed4.count() << " " << std::endl;
        return first_set;
    }

    // search_key : 展開したいkey(左辺key)
    // 展開した式ごとに関数を実行する
    void ClosureExpansion::recursionNodeClosureExpansion(LRItemStruct &lr_item, string search_key, int LR_formula_expansion_vector_index, BNFParse::vDeploymentTokenStruct first_set)
    {

        LRItemFormulaStruct lr_item_formula = lr_item.LR_formula_map[search_key];
        vLRItemFormulaExpansionStruct LR_formula_expansion_vector = lr_item_formula.LR_formula_expansion_vector;

        LRItemFormulaExpansionStruct LR_formula_expansion = LR_formula_expansion_vector[LR_formula_expansion_vector_index];
        BNFParse::vDeploymentTokenStruct token_vector = LR_formula_expansion.token_vector;

        if (LR_formula_expansion.dot >= token_vector.size())
        {
            return;
        }

        BNFParse::DeploymentTokenStruct token = token_vector[LR_formula_expansion.dot];
        // 展開しなければならないkey(右辺)
        // これをもとに、左辺を呼び出して展開する

        if (token.label == is_id_TerminalSymbol)
        {
            return;
        }

        BNFParse::vDeploymentFormulaExpansionStruct dfexp = deployment_syntax.formula_map[token.token_str].formula_expansion_vector;

        for (int k = 0; k < dfexp.size(); k++)
        {

            if (!hasKeyMap(getMapKeyString(already_explored_formula_expansion), dfexp[k].formula_expansion_label))
            {
                struct LRItemFormulaExpansionStruct new_lrf_exp;
                new_lrf_exp.token_vector = dfexp[k].token_vector;
                new_lrf_exp.formula_expansion_label = dfexp[k].formula_expansion_label;
                new_lrf_exp.look_ahead = first_set;
                new_lrf_exp.dot = 0;

                lr_item.LR_formula_map[token.token_str].LR_formula_expansion_vector.push_back(new_lrf_exp);
                already_explored_formula_expansion[new_lrf_exp.formula_expansion_label] = lr_item.LR_formula_map[token.token_str].LR_formula_expansion_vector.size() - 1;

                int n_index = lr_item.LR_formula_map[token.token_str].LR_formula_expansion_vector.size() - 1;
                int dot = lr_item.LR_formula_map[token.token_str].LR_formula_expansion_vector[n_index].dot;
                int la_size = lr_item.LR_formula_map[token.token_str].LR_formula_expansion_vector[n_index].look_ahead.size();
                for (int t = 0; t < la_size; t++)
                {
                    BNFParse::vDeploymentTokenStruct new_first_set = getLatterFirstSet(lr_item.LR_formula_map[token.token_str].LR_formula_expansion_vector[n_index], dot, t);
                    recursionNodeClosureExpansion(lr_item, token.token_str, n_index, new_first_set);
                }
            }
            else
            { // 先読み記号だけ追加する
                int formula_expansion_index = already_explored_formula_expansion[dfexp[k].formula_expansion_label];
                BNFParse::vDeploymentTokenStruct *current_look_ahead_p = &(lr_item.LR_formula_map[token.token_str].LR_formula_expansion_vector[formula_expansion_index].look_ahead);

                for (int n = 0; n < first_set.size(); n++)
                {
                    bool flag = true; // 既に登録されているか
                    for (int s = 0; s < current_look_ahead_p->size(); s++)
                    {
                        if (current_look_ahead_p->at(s).token_str == first_set[n].token_str)
                        {
                            flag = false;
                            break;
                        }
                    }
                    if (flag)
                    {
                        current_look_ahead_p->push_back(first_set[n]);

                        int dot = lr_item.LR_formula_map[token.token_str].LR_formula_expansion_vector[formula_expansion_index].dot;
                        BNFParse::vDeploymentTokenStruct new_first_set = getLatterFirstSet(lr_item.LR_formula_map[token.token_str].LR_formula_expansion_vector[formula_expansion_index], dot, current_look_ahead_p->size() - 1);
                        recursionNodeClosureExpansion(lr_item, token.token_str, formula_expansion_index, new_first_set);
                    }
                }
            }
        }
    }
};
