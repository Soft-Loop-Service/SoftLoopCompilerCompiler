#include "./bnf.hpp"

namespace BNFParse
{

    // 記号の読み方
    // https://www602.math.ryukoku.ac.jp/Prog1/charnames.html

    void concatenateArrayRetrieveSymbol(RetrieveSymbol &newdata, RetrieveSymbol &data1, RetrieveSymbol &data2)
    {

        newdata.array = (int *)calloc(data1.len + data2.len, sizeof(int));

        newdata.len = data1.len + data2.len;
        memcpy(newdata.array, data1.array, data1.len * sizeof(int));
        memcpy(newdata.array + data1.len, data2.array, data2.len * sizeof(int));
    }

    char *get_bnf_arr(BNFToken &bnf_token_p, int number)
    {
        return bnf_token_p.token_string_array[number];
    }
};
