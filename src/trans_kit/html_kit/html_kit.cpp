
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "./html_kit.hpp"

namespace TransSystem
{
    HtmlKit::HtmlKit()
    {
        children = {};
        attributes = {};
        tag = "";
        element = "";
    }

    void HtmlKit::setTag(string s)
    {
        tag = s;
    }
    void HtmlKit::setElement(string s)
    {
        element = s;
    }
    void HtmlKit::linkChild(int child_index)
    {
        children.push_back(child_index);
    }
    void HtmlKit::pushAttribute(string type, string name)
    {
        attributes[type] = name;
    }
    string HtmlKit::generate(vHtmlKit &html_kit_tree)
    {
        string start = startTagGenerate();
        string child = "";
        for (int i = 0; i < children.size(); i++)
        {
            int ci = children[i];
            string c = html_kit_tree[ci].generate(html_kit_tree);
            child += " " + c;
        }

        string element = elementTagGenerate();
        string end = endTagGenerate();

        string return_v = start + " " + child + " " + element + " " + end;
        return return_v;
    }
    string HtmlKit::startTagGenerate()
    {
        return "<" + tag + " " + attributeTagGenerate() + " " + ">";
    }
    string HtmlKit::attributeTagGenerate()
    {
        string attribute = "";
        vstring keys = getMapKeyString(attributes);

        for (int i = 0; i < keys.size(); i++)
        {
            string key = keys[i];
            string val = attributes[key];

            string at = " " + key + " = " + val + " ";
            attribute += at;
        }

        return attribute;
    }
    string HtmlKit::elementTagGenerate()
    {
        return element;
    }
    string HtmlKit::endTagGenerate()
    {
        return "</" + tag + ">";
    }
};
