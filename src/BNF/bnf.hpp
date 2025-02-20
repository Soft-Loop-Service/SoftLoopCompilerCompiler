

#ifndef __BNF
#define __BNF
#include "./../definition.hpp"
#include "./../symbol.hpp"

namespace BNFParse
{
    const int bnf_token_len = 50; // BNFトークンの最大長
    const int bnf_token_arr_len = 1000;

    struct stest;
    struct BNFToken;
    struct BNFSymbol;
    struct RetrieveSymbol;

    struct DeploymentStruct;
    struct DeploymentFormulaStruct;
    struct DeploymentFormulaExpansionStruct;
    struct DeploymentTokenStruct;
    typedef std::vector<DeploymentStruct> vDeploymentStruct;
    typedef std::vector<DeploymentFormulaStruct> vDeploymentFormulaStruct;
    typedef std::vector<DeploymentFormulaExpansionStruct> vDeploymentFormulaExpansionStruct;
    typedef std::vector<DeploymentTokenStruct> vDeploymentTokenStruct;
    typedef std::map<string, DeploymentFormulaStruct> mapDeploymentFormulaStruct;
    typedef std::map<std::string, vDeploymentTokenStruct> mp_s_Dtoken; // を生成

    void output_bnf_tablef(BNFToken &bnf_token_p, BNFSymbol &bnf_symbol_p);
    void concatenateArrayRetrieveSymbol(RetrieveSymbol &newdata, RetrieveSymbol &data1, RetrieveSymbol &data2);
    char *get_bnf_arr(BNFToken &bnf_token_p, int number);
    bool hasDtoken(vector<DeploymentTokenStruct> keys, DeploymentTokenStruct key);
    DeploymentStruct expansionDeployment(BNFToken &bnf_token_p, BNFSymbol &bnf_symbol_p, RetrieveSymbol &nonterminal_symbol_left, RetrieveSymbol &symbols);
    int labelingBnf(BNFToken &bnf_token_p);
    int parseBnf(char *source_code, BNFToken &bnf_token_p);
    void dequeueBNFRight(qustr &bnf_que, v2string &bnf_right);
    void generateBNFRight(BNFToken &bnf_token_p, BNFSymbol &bnf_symbol_p, RetrieveSymbol &nonterminal_symbol_left, int current_left, v2string &bnf_right_tokens, mp_s_i &bnf_right_map);

    int searchRetrieveSymbolTableDuplication(BNFToken &bnf_token_p, BNFSymbol &bnf_symbol_p, int *array, int len, int current_number);
    int retrieveSymbolTable(BNFToken &bnf_token_p, BNFSymbol &bnf_symbol_p, RetrieveSymbol &retrieve_symbol, int target_id);
    int retrieveSymbolTable(BNFToken &bnf_token_p, RetrieveSymbol &retrieve_symbol, int target_id);
    int insertSymbolTable(char *current_token_string, char **symbol_string_array, int symbol_len);
    int generateSymbolTable(BNFToken &bnf_token_p, BNFSymbol &bnf_symbol_p);
};

struct BNFParse::stest
{
    int test;
};

/*
BNFToken
token_string_array    : tokenの一覧
token_label_array     : tokenの役割
token_len       : tokenの数
nonterminal_symbol_len : 非末端記号の数
terminal_symbol_len    : 末端記号の数
*/
struct BNFParse::BNFToken
{
    char **token_string_array;
    int *token_label_array;
    int token_len;
    int nonterminal_symbol_len;
    int terminal_symbol_len;
};

/*
symbol_table_array        : token_string と symbol_stringの関連付け。 index番号(token_string) : 要素(symbol_string)の格納位置
symbol_string_array       : ユニークなシンボル一覧
unique_symbol_len   : ユニークなシンボルの数
symbol_len          : 述べシンボルの数
*/
struct BNFParse::BNFSymbol
{
    int *symbol_table_array;
    char **symbol_string_array;
    int unique_symbol_len;
    int symbol_len;
};

/*
array   : arrayとsymbol_stringの関連付け。要素番号=symbol_stringの格納場所
*/
struct BNFParse::RetrieveSymbol
{
    int *array;
    int len;
    /* data */
};

struct BNFParse::DeploymentStruct
{
    mapDeploymentFormulaStruct formula_map;
    // vDeploymentStruct children;
};

// 構造体 式
struct BNFParse::DeploymentFormulaStruct
{
    vDeploymentFormulaExpansionStruct formula_expansion_vector;
};

// 構造体 展開した式
struct BNFParse::DeploymentFormulaExpansionStruct
{
    vDeploymentTokenStruct token_vector;
    int formula_expansion_label;
};

// 構造体 token
struct BNFParse::DeploymentTokenStruct
{
    string token_str;
    int label;
};
#endif
