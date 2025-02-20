
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "./css_kit.hpp"

namespace TransSystem
{
    CssKit::CssKit()
    {
        selector = "";
        selector_symbol = "";
        property_value = {};
    }
    void CssKit::setSelector(string s)
    {
        selector = s;
    }
    void CssKit::setSelector(string s1, string s2)
    {
        selector = s1;
        selector_symbol = s2;
    }
    void CssKit::pushPropertyValue(string property, string value)
    {
        property_value[property] = value;
    }
    void CssKit::pushPropertyValue(string property, int value)
    {
        property_value[property] = to_string(value);
    }
    void CssKit::pushPropertyValue(string property, int value, string unit)
    {
        property_value[property] = to_string(value) + unit;
    }
    void CssKit::pushPropertyValue(string property, string value, string unit)
    {
        property_value[property] = value + unit;
    }
    void CssKit::pushPropertyValueRGB(string property, int r, int g, int b)
    {
        string val = "rgb(" + to_string(r) + "," + to_string(g) + "," + to_string(b) + ")";
        property_value[property] = val;
    }
    void CssKit::pushPropertyValueRGBA(string property, int r, int g, int b, int a)
    {
        string val = "rgb(" + to_string(r) + "," + to_string(g) + "," + to_string(b) + "," + to_string(a) + ")";
        property_value[property] = val;
    }

    string CssKit::generate()
    {
        string text = "";
        vstring propertys = getMapKeyString(property_value);

        text += selector_symbol;
        text += selector;
        text += "{";

        for (int i = 0; i < propertys.size(); i++)
        {
            string property = propertys[i];
            string value = property_value[property];

            string at = " " + property + " : " + value + "; ";
            text += at;
        }
        text += "}";
        return text;
    }
    string CssKit::generate(vCssKit css_kit_tree)
    {
        string text = "";

        for (int i = 0; i < css_kit_tree.size(); i++)
        {
            string t = css_kit_tree[i].generate();
            text += t + " ";
        }

        return text;
    }
};
