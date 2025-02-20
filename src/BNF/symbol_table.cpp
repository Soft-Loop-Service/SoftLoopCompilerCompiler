#include "./bnf.hpp"

namespace BNFParse
{
    /*
    SymbolTable
    非末端記号と末端記号を整理したもの
    */

    /*
    insertSymbolTable
    token_stringとsymbol_stringの紐付け。1次元配列(=string)のポインタをもらう

    symbol_stringに存在しなければ、末尾に挿入する。戻り値として挿入した場所を伝える
    symbol_stringにすでに存在すれば、存在する場所を戻り値として伝える。

    新規挿入する。
    */
    int insertSymbolTable(char *current_token_string, char **symbol_string_array, int symbol_len)
    {
        // 格納されていないところを探す

        int index = -1;

        for (int i = 0; i < symbol_len; i++)
        {
            char *current_symbol_string_array = symbol_string_array[i];

            // 探索した場所がNULLだったら返却する
            if (current_symbol_string_array[0] == '\0')
            {
                index = i;
                break;
            }

            // すでに挿入済みであるか調べる
            if (strncmp(current_token_string, current_symbol_string_array, bnf_token_len) == 0)
            {
                return i;
            }
        }

        if (index < 0)
        {
            return -1;
        }

        // symbol_string_array[index] =
        strncpy(symbol_string_array[index], current_token_string, bnf_token_len);

        return index;
    }

    /*
    generateSymbolTable
    非終端記号と終端記号の関係をまとめる。ユニークな非終端記号と終端記号の合計値を返す
    */
    int generateSymbolTable(BNFToken &bnf_token_p, BNFSymbol &bnf_symbol_p)
    {

        for (int i = 0; i < bnf_token_p.token_len; i++)
        {
            bnf_symbol_p.symbol_string_array[i] = (char *)calloc(bnf_token_len, 1);
            bnf_symbol_p.symbol_string_array[i][0] = '\0';
        }

        int count = 0;
        for (int si = 0; si < bnf_token_p.token_len; si++)
        {
            int ctl = bnf_token_p.token_label_array[si];
            switch (ctl)
            {
            case is_id_NonterminalSymbolLeft:  // 左辺定義 非末端記号
            case is_id_NonterminalSymbolRight: // 右辺定義 非末端記号
            case is_id_TerminalSymbol:         // 末端記号(TerminalSymbol)

            {
                int n = insertSymbolTable(bnf_token_p.token_string_array[si], bnf_symbol_p.symbol_string_array, bnf_symbol_p.symbol_len);
                if (n > 0)
                {
                    bnf_symbol_p.symbol_table_array[si] = n;
                    count++;
                }
            }

            break;

            case is_id_ParenthesisLeft:
            case is_id_BracketLeft:
            case is_id_CurlyBracketLeft:
            {
                break;
            }

            default:
                bnf_symbol_p.symbol_table_array[si] = -1;
                break;
            }
        }

        bnf_symbol_p.unique_symbol_len = count;
        return count;
    }
};
