
#ifndef __TTK
#define __TTK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "./../definition.hpp"
#include "./../lexical/lexical_analysis.hpp"
#include "./../syntactic/syntactic_analysis.hpp"

#include "./html_kit/html_kit.hpp"
#include "./css_kit/css_kit.hpp"
#include "./script_kit/script_kit.hpp"

namespace TransSystem
{

    class TransKit
    {
    private:
        vHtmlKit html_kit_tree = {};
        vCssKit css_kit_tree = {};
        vScriptKit script_kit_tree = {};

    public:
        TransKit();

        int pushHtmlTree(HtmlKit);
        int pushCssTree(CssKit);
        int pushScriptTree(ScriptKit);
        vHtmlKit getHtmlTree();
        vCssKit getCssTree();
        vScriptKit getScriptTree();
    };

};

#endif