

#include "./../lr_table/lr_table.hpp"
#include "./syntactic_analysis.hpp"
#include "./tree_control.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

namespace Syntactic
{
    // 不要な文字であるか判定する
    bool isTokenSkepSyntacticAnalysis(string token_str)
    {
        const char *token = token_str.c_str();
        return (strchr("(){}[];,\'\"", token[0]) != 0);
    }

    /// @brief 自身の存在を消すメソッド
    /// @param tree グラフ(木)
    /// @param parent_node_index 親要素
    /// @param parent_node_child_index 親要素の子要素index 変更するノード
    /// @param move_node_index 移動するノード
    /// @return
    void deleteChild(vSyntacticTree &tree, int current_node_index)
    {
        tree[current_node_index].parent_node_index = -1;
        tree[current_node_index].token_label = is_id_Unnecessary;
    }

    /// @brief 自分自身を削除し、自身の指定した子供を現在の位置に移動、親と接続する。子要素は共有しない
    /// @param tree
    /// @param current_node_index
    /// @param child_flow_index
    void climbChild(vSyntacticTree &tree, int current_node_index, int child_flow_index)
    {
        // 親要素の子要素リストを変更する
        int parent_index = tree[current_node_index].parent_node_index;
        int ps = tree[parent_index].children.size();
        for (int i = 0; i < ps; i++)
        {
            if (tree[parent_index].children[i] == current_node_index)
            {
                tree[parent_index].children[i] = child_flow_index;
                break;
            }
        }

        deleteChild(tree, current_node_index);
    }

    /// @brief 自分自身を削除し、自身の指定した子供を現在の位置に移動、親と接続する。子要素の共有を行う
    /// @param tree
    /// @param current_node_index
    /// @param child_flow_index
    void moveChild(vSyntacticTree &tree, int current_node_index, int child_flow_index)
    {
        vint current_node_children = tree[current_node_index].children;
        climbChild(tree, current_node_index, child_flow_index);

        for (int i = 0; i < current_node_children.size(); i++)
        {
            if (current_node_children[i] == child_flow_index)
            {
                current_node_children[i] = -1;
            }
        }

        // tree[child_flow_index].children.insert(tree[child_flow_index].children.end(), current_node_children.begin(), current_node_children.end()); // 連結
        joinArray(tree[child_flow_index].children, current_node_children);
    }

    /// @brief 自分自身を削除し、親要素にも削除させる
    /// @param tree
    /// @param parent_node_index 親要素
    /// @param delete_child_node 現在要素（消したい要素）
    /// @return
    bool cutChild(vSyntacticTree &tree, int parent_node_index, int delete_child_node)
    {
        for (int i = 0; i < tree[parent_node_index].children.size(); i++)
        {
            if (tree[parent_node_index].children[i] == delete_child_node)
            {
                tree[parent_node_index].children[i] = -1;

                deleteChild(tree, delete_child_node);

                return true;
            }
        }
        debugSyntacticAnalysisTree(tree);

        return false;
    }

    /// @brief 自分自身を削除し、親要素と孫要素を連結する。つまり現在の要素の子要素群を親要素の子要素群とする
    /// @param tree
    /// @param parent_node_index
    /// @param delete_child_node
    /// @return
    bool shortParentChild(vSyntacticTree &tree, int parent_node_index, int delete_child_node)
    {
        // tree[parent_node_index].children = tree[delete_child_node].children;

        vint new_children = {};
        for (int i = 0; i < tree[parent_node_index].children.size(); i++)
        {
            if (tree[parent_node_index].children[i] == delete_child_node)
            {
                joinArray(new_children, tree[delete_child_node].children);
            }
            else
            {
                new_children.push_back(tree[parent_node_index].children[i]);
            }
        }
        tree[parent_node_index].children = new_children;
        deleteChild(tree, delete_child_node);
    }

    /// @brief 現在位置の要素を削除し(中間)、子要素(下位)を現在位置(中間)に移動させる。親要素(上位)の子要素(中間)を変更する
    /// @param tree
    /// @param parent_node_index =親要素
    /// @param delete_current_node 消したい要素（現在位置）
    /// @param flow_child_node 現在位置に移動する子要素
    /// @return
    bool shortParentChildFlow(vSyntacticTree &tree, int parent_node_index, int delete_current_node, int flow_child_node)
    {
        for (int i = 0; i < tree[parent_node_index].children.size(); i++)
        {
            if (tree[parent_node_index].children[i] == delete_current_node)
            {
                tree[parent_node_index].children[i] = flow_child_node;
            }
        }

        vint delete_children = tree[delete_current_node].children;
        for (int i = 0; i < delete_children.size(); i++)
        {
            if (delete_children[i] == flow_child_node)
            {
                delete_children[i] = -1;
            }
        }
        tree[flow_child_node].children = delete_children;
        deleteChild(tree, delete_current_node);
    }

    /// @brief 削除するといわれ-1を付与されたchildren配列内の要素を削除する。
    /// @param tree
    /// @param current_node_index 検査対象（その子要素群を検査する）
    /// @return
    bool deleteNegativeValueChildren(vSyntacticTree &tree, int current_node_index)
    {
        int c_index = tree[current_node_index].children.size();
        for (int i = c_index - 1; i >= 0; i--)
        {
            if (tree[current_node_index].children[i] < 0)
            {
                tree[current_node_index].children.erase(tree[current_node_index].children.begin() + i);
            }
        }
    }

}