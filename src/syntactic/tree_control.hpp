

#include "./../lr_table/lr_table.hpp"
#include "./syntactic_analysis.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

namespace Syntactic
{
    bool isTokenSkepSyntacticAnalysis(string);

    void deleteChild(vSyntacticTree &, int);
    void climbChild(vSyntacticTree &tree, int current_node_index, int child_flow_index);
    bool cutChild(vSyntacticTree &tree, int parent_node_index, int delete_child_node);
    void moveChild(vSyntacticTree &tree, int current_node_index, int child_flow_index);
    bool shortParentChild(vSyntacticTree &tree, int parent_node_index, int delete_child_node);
    bool shortParentChildFlow(vSyntacticTree &tree, int parent_node_index, int delete_current_node, int flow_child_node);
    bool deleteNegativeValueChildren(vSyntacticTree &tree, int current_node_index);
}
