#include "./bnf.hpp"

namespace BNFParse
{
    int searchRetrieveSymbolTableDuplication(BNFToken &bnf_token_p, BNFSymbol &bnf_symbol_p, int *array, int len, int current_number)
    {
        for (int i = 0; i < len; i++)
        {
            char *old = get_bnf_arr(bnf_token_p, array[i]);
            char *current = get_bnf_arr(bnf_token_p, current_number);

            if (strncmp(old, current, bnf_token_len) == 0)
            {
                return true;
            }
        }
        return false;
    }

    /// retrieveSymbolTable
    /// SymbolTableから欲しい種別だけ抽出する
    /// @param target_id       抽出対象
    /// @param retrieve_label  抽出したラベル
    /// @return                抽出したラベルデータの長さ　
    int retrieveSymbolTable(BNFToken &bnf_token_p, BNFSymbol &bnf_symbol_p, RetrieveSymbol &retrieve_symbol, int target_id)
    {

        int *array = (int *)calloc(bnf_token_p.token_len, sizeof(int));
        int len = 0;

        for (int i = 0; i < bnf_token_p.token_len; i++)
        {
            int c_sta = bnf_symbol_p.symbol_table_array[i];
            if (c_sta < 0)
            {
                continue;
            }

            int ctl = bnf_token_p.token_label_array[i];
            if (ctl == target_id)
            {
                if (!(searchRetrieveSymbolTableDuplication(bnf_token_p, bnf_symbol_p, array, len, i)))
                {
                    array[len] = i;
                    len++;
                }
            }
        }

        int new_size = resizeArray(array, len);

        retrieve_symbol.array = array;
        retrieve_symbol.len = len;

        return len;
    }

    /// retrieveSymbolTable
    /// SymbolTableから欲しい種別だけ抽出する
    /// @param target_id       抽出対象
    /// @param retrieve_label  抽出したラベル
    /// @return                抽出したラベルデータの長さ　
    int retrieveSymbolTable(BNFToken &bnf_token_p, RetrieveSymbol &retrieve_symbol, int target_id)
    {

        int count = 0;

        for (int i = 0; i < bnf_token_p.token_len; i++)
        {
            int ctl = bnf_token_p.token_label_array[i];

            if (ctl == target_id)
            {
                count++;
            }
        }

        retrieve_symbol.array = (int *)calloc(count, sizeof(int));
        int len = 0;

        for (int i = 0; i < bnf_token_p.token_len; i++)
        {
            int ctl = bnf_token_p.token_label_array[i];

            if (ctl == target_id)
            {
                retrieve_symbol.array[len] = i;
                len++;
            }
        }
        // resizeNull(retrieve_label , len);
        retrieve_symbol.len = len;
        return len;
    }
};
