// 字句解析定義ファイル

#ifndef __LexicalAnalysis
#define __LexicalAnalysis

#include "./../definition.hpp"
#include "./../debug.hpp"
#include "./../symbol.hpp"
namespace LexicalAnalysis
{

    struct LexicalToken
    {
        string token;
        string token_type;

        int source_code_line;
        int source_code_column;

        int indent_depth;
        int token_order;
    };

    typedef vector<LexicalToken> vLexicalToken;

    string getToken(char *source_text, int token_search_len);
    vLexicalToken lexSyntax(char *source_code, vstring &token_class_type);
};
#endif