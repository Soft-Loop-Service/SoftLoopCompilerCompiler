
#ifndef __LRTABLED
#define __LRTABLED
#include "./../DFA/dfa.hpp"
#include "./../BNF/bnf.hpp"
#include "./../definition.hpp"
#include "./../symbol.hpp"

namespace LRTable
{
    struct LRTableMultilayer;

    int bnfMain(char *bnf_file_name, LRTableMultilayer &LR_table_multilayer);
    int generateLRtable(DFAParse::vDFANode dfa_node_graph, BNFParse::BNFToken &bnf_token_p, BNFParse::RetrieveSymbol terminal_symbol, BNFParse::RetrieveSymbol nonterminal_symbol,
                        LRTableMultilayer &LR_table_multilayer);

    template <typename T>
    class LRTableFoundation
    {
    protected:
        DFAParse::vDFANode dfa_node_graph;
        int column_length;

    public:
        map<string, vector<T>> LR_table_column_map;
        string outputTable();
        void inputTabke();
        void makeTable();
        void debug();
        void addSymbol(string cstr);
        LRTableFoundation();
        LRTableFoundation(string table_string);
        LRTableFoundation(DFAParse::vDFANode dfa_node_graph, BNFParse::BNFToken &bnf_token_p, BNFParse::RetrieveSymbol symbol);
    };

    template <typename U>
    class LRTableMakeGoto : public LRTableFoundation<U>
    {

    public:
        using LRTableFoundation<U>::LRTableFoundation;
        // LRTableMakeGoto() {}
        void makeTable();
    };

    template <typename U>
    class LRTableMakeShift : public LRTableFoundation<U>
    {

    public:
        using LRTableFoundation<U>::LRTableFoundation;
        // LRTableMakeShift(){};

        void makeTable();
    };

    template <typename U>
    class LRTableMakeReduce : public LRTableFoundation<U>
    {

    public:
        using LRTableFoundation<U>::LRTableFoundation;
        void makeTable();

        // LRTableMakeReduce(){};
    };

    template <typename U>
    class LRTableMakeAccept : public LRTableFoundation<U>
    {

    public:
        using LRTableFoundation<U>::LRTableFoundation;
        // LRTableMakeAccept();

        void makeTable();
    };

    struct ReduceFormula // 構文解析用
    {
        string token_left;                             // 左辺
        BNFParse::vDeploymentTokenStruct token_vector; // 展開したBNFの右辺
    };
    typedef vector<ReduceFormula> vReduceFormula;

    class LRTableLoad
    {
    private:
        vector<string> result;
        int point = 0;

    public:
        LRTableLoad(string table_string);
        string road_token();
    };

    class LRTableCell
    {
    protected:
        char operation = LR_table_operation_none;

    public:
        // virtual void setCell(int next_state);
        // virtual void debugCell();
    };

    class LRTableGotoCell : public LRTableCell
    {
    private:
        char operation = LR_table_operation_goto;
        bool is_valid = false;
        int next_state = -1;

    public:
        void setCell(int next_state);
        string getCellString();
        void parseStringCell(LRTableLoad *loader);
        int getCell();
        int getValid();
        void debugCell();
    };

    class LRTableShiftCell : public LRTableCell
    {
    private:
        char operation = LR_table_operation_shift;
        bool is_valid = false;

    public:
        int next_state = -1;
        void setCell(int next_state);
        string getCellString();
        void parseStringCell(LRTableLoad *loader);
        bool getValid();
        int getCell();
        void debugCell();
    };

    class LRTableReduceCell : public LRTableCell
    {
    private:
        char operation = LR_table_operation_reduce;

        struct ReduceFormula reduce_formula;
        bool is_valid = false;

    public:
        void setCell(string token_left, BNFParse::vDeploymentTokenStruct token_vector);
        void parseStringCell(LRTableLoad *loader);
        string getCellString();
        ReduceFormula getCell();
        bool getValid();
        void debugCell();
    };

    class LRTableAcceptCell : public LRTableCell
    {
    private:
        char operation = LR_table_operation_accept;
        bool is_valid = false;

    public:
        string getCellString();
        void parseStringCell(LRTableLoad *loader);
        void setCell(bool is_valid);
        bool getValid();
        void debugCell();
    };
    struct LRTableMultilayer
    {
        LRTableMakeGoto<LRTableGotoCell> LR_table_goto;
        LRTableMakeShift<LRTableShiftCell> LR_table_shift;
        LRTableMakeReduce<LRTableReduceCell> LR_table_reduce;
        LRTableMakeAccept<LRTableAcceptCell> LR_table_accept;
    };

};

#endif