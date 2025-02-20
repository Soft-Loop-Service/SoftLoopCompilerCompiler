
#ifndef __Debug
#define __Debug
#include "definition.hpp"

inline int helloWorld(string text)
{
    if (text == "hello world")
    {
        printf("hello world");
    }
    else
    {
        printf("z z z z");
    }
}

inline void output_stack(string name, sint v)
{
    printf("%s top : ", name.c_str());
    int vsize = v.size();
    for (int i = 0; i < vsize; i++)
    {
        printf("%d ", v.top());
        v.pop();
    }
    printf("\n");
}

inline void output_vector(string name, vstring v)
{
    printf("%s 配列長 : %ld\n", name.c_str(), v.size());
    for (int i = 0; i < v.size(); i++)
    {
        printf("%s : %s\n", name.c_str(), v[i].c_str());
    }
}

inline void output_token_string(char **token_string, int line)
{
    for (int i = 0; i < line; i++)
    {
        printf("output_token_string :%d %s\n", i, *&token_string[i]);
    }
}
inline void output_token_progression(char **token_string, int *token_progression, int line)
{
    for (int i = 0; i < line; i++)
    {
        // printf("%s\n", &token[variable[i]]);
        printf("output_token_progression a : %d\n", token_progression[i]);
    }
    for (int i = 0; i < line; i++)
    {
        // printf("%s\n", &token[variable[i]]);
        printf("output_token_progression b : %s\n", *&token_string[token_progression[i]]);
    }
}

inline int output_labeling_bnf(char **token_string, int *token_label, int token_len)
{
    for (int i = 0; i < token_len; i++)
    {
        // printf("%s\n", &token[variable[i]]);
        printf("olb %d: %25s %d\n", token_len, token_string[i], token_label[i]);
    }
}

#endif