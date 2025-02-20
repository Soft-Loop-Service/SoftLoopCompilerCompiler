
#include "./bnf.hpp"

namespace BNFParse
{

    void output_bnf_tablef(BNFToken &bnf_token_p, BNFSymbol &bnf_symbol_p)
    {

        printf("output_bnf_tablef : token_ken %d symbol_len %d\n", bnf_token_p.token_len, bnf_symbol_p.symbol_len);

        for (int i = 0; i < bnf_token_p.token_len; i++)
        {
            // printf("%s\n", &token[variable[i]]);
            printf("obt %3d: %25s %3d \n", i, bnf_token_p.token_string_array[i], bnf_symbol_p.symbol_table_array[i]);
        }

        for (int i = 0; i < bnf_symbol_p.symbol_len; i++)
        {

            printf("obts : %d %s\n", i, bnf_symbol_p.symbol_string_array[i]);
        }

        // for (int i = 0; i < symbol_len; i++){
        //     printf("obt - 2 : ")
        // }
    }

};
