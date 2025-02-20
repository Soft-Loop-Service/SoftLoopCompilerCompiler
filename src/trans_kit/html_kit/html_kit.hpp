
#ifndef __THK
#define __THK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "./../../definition.hpp"
#include "./../../lexical/lexical_analysis.hpp"
#include "./../../syntactic/syntactic_analysis.hpp"

namespace TransSystem
{
    class HtmlKit;
    typedef vector<HtmlKit> vHtmlKit;
    class HtmlKit
    {
    private:
        string tag;
        string element;
        mapstr attributes;
        vint children;

    public:
        HtmlKit();

        string generate(vHtmlKit &);

        void setTag(string);
        void setElement(string);

        void linkChild(int);
        void pushAttribute(string, string);

    private:
        string startTagGenerate();
        string elementTagGenerate();
        string endTagGenerate();

        string attributeTagGenerate();
    };
};

#endif