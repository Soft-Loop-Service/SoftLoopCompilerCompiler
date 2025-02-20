#include "./lr_table.hpp"

namespace LRTable
{

    int bnfMain(char *bnf_file_name, LRTableMultilayer &LR_table_multilayer)
    {
        char *bnf_source = (char *)calloc(source_code_size, sizeof(char *));
        printf("bnf_file_name %s\n", bnf_file_name);
        loadText(bnf_source, bnf_file_name, source_code_size);

        struct BNFParse::BNFToken bnf_token;
        struct BNFParse::BNFSymbol bnf_symbol;

        bnf_token.token_string_array = (char **)calloc(BNFParse::bnf_token_arr_len, sizeof(char **));
        bnf_token.token_len = parseBnf(bnf_source, bnf_token);
        bnf_token.token_label_array = (int *)calloc(bnf_token.token_len, sizeof(int));
        bnf_token.nonterminal_symbol_len = 0;
        bnf_token.terminal_symbol_len = 0;

        free(bnf_source);

        labelingBnf(bnf_token);

        bnf_symbol.symbol_len = bnf_token.nonterminal_symbol_len + bnf_token.terminal_symbol_len;
        bnf_symbol.symbol_table_array = (int *)calloc(bnf_token.token_len, sizeof(int));
        bnf_symbol.symbol_string_array = (char **)calloc(bnf_token.token_len, sizeof(char **));
        printf("bnf_token %d %d %d %d\n", bnf_token.token_len, bnf_symbol.symbol_len, bnf_token.nonterminal_symbol_len, bnf_token.terminal_symbol_len);
        int unique_symbol_len = BNFParse::generateSymbolTable(bnf_token, bnf_symbol);

        printf("char size %ld %ld %ld\n", sizeof(char **), sizeof(char *), sizeof(char));

        struct BNFParse::RetrieveSymbol nonterminal_symbol_left;
        // 左辺非末端記号の配列を取得する
        BNFParse::retrieveSymbolTable(bnf_token, nonterminal_symbol_left, is_id_NonterminalSymbolLeft);
        // 左辺の数を取得する

        struct BNFParse::RetrieveSymbol terminal_symbol;
        // 末端記号の配列を取得する
        BNFParse::retrieveSymbolTable(bnf_token, bnf_symbol, terminal_symbol, is_id_TerminalSymbol);

        struct BNFParse::RetrieveSymbol symbols;
        BNFParse::concatenateArrayRetrieveSymbol(symbols, nonterminal_symbol_left, terminal_symbol);

        BNFParse::DeploymentStruct deployment_syntax = BNFParse::expansionDeployment(bnf_token, bnf_symbol, nonterminal_symbol_left, symbols);

        // ItemSetStruct item_set = generateItemSet(deployment_syntax);
        DFAParse::vDFANode dfa_node_graph = DFAParse::generateDFA(deployment_syntax);
        generateLRtable(dfa_node_graph, bnf_token, terminal_symbol, nonterminal_symbol_left, LR_table_multilayer);

        free((bnf_token.token_label_array));
        free((bnf_token.token_string_array));
        free((bnf_symbol.symbol_string_array));
        free((bnf_symbol.symbol_table_array));
    }

    int generateLRtable(DFAParse::vDFANode dfa_node_graph, BNFParse::BNFToken &bnf_token_p, BNFParse::RetrieveSymbol terminal_symbol, BNFParse::RetrieveSymbol nonterminal_symbol,
                        LRTableMultilayer &LR_table_multilayer)
    {
        LR_table_multilayer = LRTableMultilayer{
            LRTableMakeGoto<LRTableGotoCell>(dfa_node_graph, bnf_token_p, nonterminal_symbol),
            LRTableMakeShift<LRTableShiftCell>(dfa_node_graph, bnf_token_p, terminal_symbol),
            LRTableMakeReduce<LRTableReduceCell>(dfa_node_graph, bnf_token_p, terminal_symbol),
            LRTableMakeAccept<LRTableAcceptCell>(dfa_node_graph, bnf_token_p, terminal_symbol)};

        LR_table_multilayer.LR_table_shift.addSymbol(DOLLAR);
        LR_table_multilayer.LR_table_reduce.addSymbol(DOLLAR);
        LR_table_multilayer.LR_table_accept.addSymbol(DOLLAR);

        LR_table_multilayer.LR_table_goto.makeTable();
        LR_table_multilayer.LR_table_goto.debug();

        LR_table_multilayer.LR_table_shift.makeTable();
        LR_table_multilayer.LR_table_shift.debug();

        LR_table_multilayer.LR_table_reduce.makeTable();
        LR_table_multilayer.LR_table_reduce.debug();

        LR_table_multilayer.LR_table_accept.makeTable();
        LR_table_multilayer.LR_table_accept.debug();
    }

};