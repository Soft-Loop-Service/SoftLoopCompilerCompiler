
#ifndef __TSK
#define __TSK

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

    class ScriptKit;
    typedef vector<ScriptKit> vScriptKit;

    class ScriptKit
    {
    private:
    public:
        ScriptKit();
        string generate();
        string generate(vScriptKit);
    };
};

#endif