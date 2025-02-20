
#ifndef __DFA
#define __DFA
#include "./../BNF/bnf.hpp"
#include "./../definition.hpp"
#include "./../item_set/item_set.hpp"
#include <chrono>

#define ROOT_DFA_SYMBOL "<_S>"
#define START_DFA_SYMBOL "<S>"

namespace DFAParse
{
    struct DFANode;
    class ClosureExpansion;

    struct LRItemStruct;
    struct LRItemFormulaStruct;
    struct LRItemFormulaExpansionStruct;
    typedef std::queue<LRItemStruct> quLRItemStruct;
    typedef std::vector<DFANode> vDFANode;
    typedef std::vector<LRItemFormulaStruct> vLRItemFormulaStruct;
    typedef std::vector<LRItemFormulaExpansionStruct> vLRItemFormulaExpansionStruct;
    typedef std::map<string, LRItemFormulaStruct> mapLRItemFormulaStruct;

    bool isDfaEqual(DFANode a_node, DFANode b_node);
    int generateDFARoot(DFANode &root_dfa_node);
    vstring getNextLabelDFA(DFANode current_node);
    int recursionDFA(BNFParse::DeploymentStruct &deployment_syntax, vDFANode &dfa_node_graph, int current_node_index);
    void outputDFA(vDFANode dfa_node_graph);
    vDFANode generateDFA(BNFParse::DeploymentStruct deployment_syntax);

    /*
    DFANodeはLRItemStructの機能に加えて他nodeの事も考えることができる。LRItemStructはコンポジションである
    */
    class ClosureExpansion
    {
    private:
        BNFParse::DeploymentStruct deployment_syntax;
        // vstring already_explored = {};                  // すでに展開した左辺を登録する。無限に再帰展開されないようにするのが目的
        mp_i_i already_explored_formula_expansion = {}; // 一つのアイテム集合内に、同じ展開式が重複して登録されないようにする
        // int dot = -1;

    public:
        ClosureExpansion(BNFParse::DeploymentStruct deployment_syntax);
        void nodeClosureExpansion(LRItemStruct &lr_item);
        void nodeClosureExpansion(LRItemStruct &lr_item, string search_key);

    private:
        BNFParse::vDeploymentTokenStruct getLatterToken(LRItemFormulaExpansionStruct LR_formula_expansion, int dot, int lookAhead_index);
        BNFParse::vDeploymentTokenStruct getLatterFirstSet(LRItemFormulaExpansionStruct LR_formula_expansion, int dot, int lookAhead_index);
        void recursionNodeClosureExpansion(LRItemStruct &lr_item, string search_key, int LR_formula_expansion_vector_index, BNFParse::vDeploymentTokenStruct first_set);
    };
};

struct DFAParse::LRItemStruct // LRItemStructは自分のこと(自身node)の事しか考えることができない
{
    mapLRItemFormulaStruct LR_formula_map;
};

// 構造体 式
struct DFAParse::LRItemFormulaStruct
{

    vLRItemFormulaExpansionStruct LR_formula_expansion_vector;
};

/*構造体 展開した式
lookAhead : 先読み記号
*/
struct DFAParse::LRItemFormulaExpansionStruct : BNFParse::DeploymentFormulaExpansionStruct
{
    BNFParse::vDeploymentTokenStruct look_ahead;
    int dot;
};

struct DFAParse::DFANode
{
    LRItemStruct lr_item;
    mp_s_i children_nodes;            // 子要素のindex
    vint has_formula_expansion_label; // 高速計算用。子ノードが何を持っているかを保有
    string node_label;                // 受理記号 rootである<_S>にはrootと代入される
};
#endif
