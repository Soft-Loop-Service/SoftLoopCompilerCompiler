
#ifndef __TCK
#define __TCK

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
    class CssKit;
    typedef vector<CssKit> vCssKit;
    class CssKit
    {
    private:
        string selector;
        string selector_symbol;
        mapstr property_value;

    public:
        CssKit();

        void setSelector(string);
        void setSelector(string, string);

        void pushPropertyValue(string, string);
        void pushPropertyValue(string, int);
        void pushPropertyValue(string, int, string);
        void pushPropertyValue(string, string, string);
        void pushPropertyValueRGB(string, int, int, int);
        void pushPropertyValueRGBA(string, int, int, int, int);
        string generate();
        string generate(vCssKit);
    };
};

#endif