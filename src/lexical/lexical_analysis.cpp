#include "./lexical_analysis.hpp"

#include "./../definition.hpp"

namespace LexicalAnalysis
{

    string getToken(char *source_text, int token_search_len)
    {
        // ここから登録
        char *new_token = (char *)calloc(code_token_len, sizeof(char));
        strncpy(new_token, source_text, token_search_len);
        string token_string = string(new_token);
        free(new_token);

        return token_string;
    }
    // スキャナ
    vLexicalToken lexSyntax(char *source_code, vstring &token_class_type)
    {
        // source_codeとtokenのポインタを得る
        vLexicalToken lexical_token = {};

        int source_code_line = 1;
        int source_code_column = 1;
        int indent_depth = 0;

        int i_s = 0; // 現在地点
        int token_count = 0;
        for (;;)

        { // 終了コードまで
            // return;

            string token_type = "\t";
            int token_search_len = 0;

            if (source_code[i_s] == 0) // ファイル終端
            {
                break;
            }

            if (source_code[i_s] == '\n' || source_code[i_s] == '\r')
            { // 改行
                source_code_column = 1;
                source_code_line++;
                indent_depth = 0;

                i_s++;
                continue;
            }
            if (source_code[i_s] == ' ' || source_code[i_s] == '\t')
            { // スペース、タブ.
                indent_depth++;
                i_s++;
                continue;
            }
            if (strchr("(){}[]{};,", source_code[i_s]) != 0)
            { // 1文字記号.
                source_code_column++;
                token_search_len = 1;
            }
            else if (isNumber(source_code[i_s]))
            {
                source_code_column++;
                token_type = "NUM";
                while (isNumber(source_code[i_s + token_search_len]) || (source_code[i_s + token_search_len] == '.' && token_search_len > 0))
                {
                    token_search_len++;
                }
            }
            else if (isAlphabetOrNumber(source_code[i_s]))
            {
                source_code_column++;
                while (isAlphabetOrNumber(source_code[i_s + token_search_len]) || (token_search_len > 0 && &source_code[i_s + token_search_len] == "_"))
                {
                    token_search_len++;
                }
                token_type = "TEXT";

                for (int i = 0; i < VALUE_TYPE.size(); i++)
                {
                    if (VALUE_TYPE[i].length() == token_search_len && strncmp(VALUE_TYPE[i].c_str(), &source_code[i_s], token_search_len) == 0)
                    {
                        token_type = "TYPE";
                    }
                }
                for (int i = 0; i < DEFINITION_TYPE.size(); i++)
                {
                    if (DEFINITION_TYPE[i].length() == token_search_len && strncmp(DEFINITION_TYPE[i].c_str(), &source_code[i_s], token_search_len) == 0)
                    {
                        token_type = "DEFINITION";
                    }
                }
                for (int i = 0; i < ACCESS_TYPE.size(); i++)
                {
                    if (ACCESS_TYPE[i].length() == token_search_len && strncmp(ACCESS_TYPE[i].c_str(), &source_code[i_s], token_search_len) == 0)
                    {
                        token_type = "ACCESS";
                    }
                }
            }
            else if (strchr("=+-*/!%&~|<>?:.#_", source_code[i_s]) != 0)
            { // 現在文字が普通の記号.
                source_code_column++;

                while (strchr("=+-*/!%&~|<>?:.#_", source_code[i_s + token_search_len]) != 0 && source_code[i_s + token_search_len] != 0)
                {
                    if (token_search_len > 0)
                    {
                        // ><の場合は分割
                        if (strncmp("><", &source_code[i_s + token_search_len - 1], 2) == 0)
                        {
                            break;
                        }
                    }
                    token_search_len++;
                }
            }

            else if (strchr("\'\"\\", source_code[i_s]) != 0)
            { // 特殊1文字記号
                source_code_column++;
                token_search_len = 1;
            }
            else
            {
                printf("syntax error : %.10s\n", &source_code[i_s]);
                exit(1);
            }
            string current_token = getToken(&source_code[i_s], token_search_len);

            if (token_type == "\t")
            {
                token_type = current_token;
            }

            struct LexicalToken ts = {current_token, token_type, source_code_line, source_code_column, indent_depth, token_count};

            lexical_token.push_back(ts);
            i_s += token_search_len;
            token_count++;
        }

        struct LexicalToken tsd = {DOLLAR, DOLLAR};
        lexical_token.push_back(tsd);

        for (int i = lexical_token.size() - 1; i > 0; i--)
        {
            LexicalToken tb = lexical_token[i - 1];
            LexicalToken tc = lexical_token[i];

            if (tb.token == "<" && tc.token == "/")
            {
                lexical_token[i - 1].token = "</";
                lexical_token.erase(lexical_token.begin() + i);
            }
            else if (tb.token == "/" && tc.token == ">")
            {
                lexical_token[i - 1].token = "/>";
                lexical_token.erase(lexical_token.begin() + i);
            }
            else if (tb.token_type == "DEFINITION" && tc.token_type == "TEXT")
            {
                lexical_token[i].token_type = "TYPE";
                token_class_type.push_back(tc.token);
            }
        }

        for (int i = 0; i < lexical_token.size(); i++)
        {
            if (lexical_token[i].token_type == "TEXT")
            {
                for (int j = 0; j < token_class_type.size(); j++)
                {
                    if (lexical_token[i].token == token_class_type[j])
                    {
                        lexical_token[i].token_type = "TYPE";
                    }
                }
            }
        }

        return lexical_token;

        // output_token_string(token_string, token_string_arr_size);
        // output_token_progression(token_string, token_progression, token_progression_endline);
    }
}
