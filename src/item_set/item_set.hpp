

#ifndef __ITEMSET
#define __ITEMSET

#include "./../definition.hpp"
#include "./../debug.hpp"
#include "./../symbol.hpp"
#include "./../BNF/bnf.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>

namespace ItemSet
{

    class FirstSetClass
    {
    private:
        BNFParse::DeploymentStruct deployment_syntax;
        vstring null_set;
        BNFParse::mp_s_Dtoken first_set;
        vstring formula_map_keys;
        vstring already_explored;

    public:
        FirstSetClass(BNFParse::DeploymentStruct deployment_syntax, vstring null_set);
        BNFParse::vDeploymentTokenStruct findFirstSetVector(BNFParse::vDeploymentTokenStruct request_token_vector);
        BNFParse::mp_s_Dtoken findFirstSet();
        void recursionFirstsSet(string current_key);
    };

    class FollowSetClass
    {
    private:
        BNFParse::DeploymentStruct deployment_syntax;
        BNFParse::mp_s_Dtoken first_set;
        vstring null_set;
        BNFParse::mp_s_Dtoken follow_set;
        vstring formula_map_keys;
        vstring already_explored;

    public:
        FollowSetClass(BNFParse::DeploymentStruct deployment_syntax, vstring null_set, BNFParse::mp_s_Dtoken first_set);
        BNFParse::mp_s_Dtoken findFolllowSet();
        void recursionFollowSet(string search_key);

    private: /// @brief
        /// @return nullの可能性があるか否か
        bool findNext(string search_key, string current_key, string next_key);
        // Nullの可能性があるとき
        bool findNextNull(string search_key, string current_key);
    };

    /*
    bnf_right   : 3次元配列 上から順に 式：展開した式：式を構成するtoken
    children    : 4次元配列 上から順に アイテム集合群：式：展開した式：式を構成するtoken
    dot
    */

    // 左辺非末端記号とNUll集合であるかどうか
    class NullSetClass
    {
    public:
        NullSetClass(BNFParse::DeploymentStruct deployment_syntax);
        vstring findNullsSet();

    private:
        vstring null_set;
        BNFParse::DeploymentStruct deployment_syntax;
        vstring formula_map_keys;
        vstring already_explored;
        bool recursionNullSet(string current_key);
    };
}


#endif