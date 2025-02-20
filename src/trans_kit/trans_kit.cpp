
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "./trans_kit.hpp"

namespace TransSystem
{

    TransKit::TransKit()
    {
    }

    int TransKit::pushHtmlTree(HtmlKit html_kit)
    {
        html_kit_tree.push_back(html_kit);
        int index = html_kit_tree.size() - 1;
        return index;
    }
    int TransKit::pushCssTree(CssKit css_kit)
    {
        css_kit_tree.push_back(css_kit);
        int index = css_kit_tree.size() - 1;
        return index;
    }
    int TransKit::pushScriptTree(ScriptKit script_kit)
    {
        script_kit_tree.push_back(script_kit);
        int index = script_kit_tree.size() - 1;
        return index;
    }
    vHtmlKit TransKit::getHtmlTree()
    {
        return html_kit_tree;
    }
    vCssKit TransKit::getCssTree()
    {
        return css_kit_tree;
    }
    vScriptKit TransKit::getScriptTree()
    {
        return script_kit_tree;
    }

};
