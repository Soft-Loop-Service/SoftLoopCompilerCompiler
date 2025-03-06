
#include "./syntactic_analysis.hpp"

namespace Syntactic
{
    int SyntacticTree::getChildrenNodeIndex(int request_node_index, int request_children_index)
    {
        if (request_node_index < 0 || request_node_index >= tree.size())
        {
            // 例外スロー
            printf("error : %d is out of range (tree)\n", request_node_index);
            exit(1);
        }
        SyntacticTreeNode current_node = tree[request_node_index];

        if (request_children_index < 0 || request_children_index >= current_node.children.size())
        {
            // 例外スロー
            printf("error : node:%d max:%d,  %d is out of range (children)  \n", request_node_index, current_node.children.size(), request_children_index);
            exit(1);
        }
        int i = current_node.children[request_children_index];
        return i;
    }

    SyntacticTreeNode SyntacticTree::getChildrenNode(int request_node_index, int request_children_index)
    {
        if (request_node_index < 0 || request_node_index >= tree.size())
        {
            // 例外スロー
            printf("error : %d is out of range (tree)\n", request_node_index);
            exit(1);
        }
        SyntacticTreeNode current_node = tree[request_node_index];

        if (request_children_index < 0 || request_children_index >= current_node.children.size())
        {
            // 例外スロー
            printf("error : %d is out of range (children)\n", request_children_index);
            exit(1);
        }

        SyntacticTreeNode c = tree[current_node.children[request_children_index]];
        return c;
    }

    SyntacticTreeNode SyntacticTree::getNode(int request_node_index)
    {
        if (request_node_index < 0 || request_node_index >= tree.size())
        {
            // 例外スロー
            printf("error : %d is out of range\n", request_node_index);
            exit(1);
        }

        SyntacticTreeNode current_node = tree[request_node_index];
        return current_node;
    }

}
