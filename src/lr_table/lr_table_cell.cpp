#include "./lr_table.hpp"

namespace LRTable
{

    LRTableLoad::LRTableLoad(string table_string)
    {
        string temp = "";
        for (int i = 0; i < table_string.size(); i++)
        {
            if (table_string[i] == ' ')
            {
                result.push_back(temp);
                temp = "";
                continue;
            }
            temp += table_string[i];
        }
        result.push_back(temp);
    }

    string LRTableLoad::road_token()
    {
        string text = result[point];
        point++;
        return text;
    }

    void LRTableGotoCell::setCell(int next_state)
    {
        this->next_state = next_state;
        this->is_valid = true;
    };
    string LRTableGotoCell::getCellString()
    {
        return to_string(next_state);
    }

    void LRTableGotoCell::parseStringCell(LRTableLoad *loader)
    {
        next_state = stoi(loader->road_token());

        is_valid = next_state == -1 ? false : true;
    }

    int LRTableGotoCell::getCell()
    {
        return this->next_state;
    }
    int LRTableGotoCell::getValid()
    {
        return this->is_valid;
    }
    void LRTableGotoCell::debugCell()
    {
        if (next_state == -1)
        {
            printf("%10s", "");
            return;
        }
        printf("%10d", next_state);
    }

    void LRTableShiftCell::setCell(int next_state)
    {
        this->next_state = next_state;
        this->is_valid = true;
    };
    string LRTableShiftCell::getCellString()
    {
        return to_string(next_state);
    }
    void LRTableShiftCell::parseStringCell(LRTableLoad *loader)
    {
        string next = loader->road_token();
        next_state = stoi(next);
        is_valid = next == "-1" ? false : true;
    }

    bool LRTableShiftCell::getValid()
    {
        return this->is_valid;
    }

    int LRTableShiftCell::getCell()
    {
        return this->next_state;
    }
    void LRTableShiftCell::debugCell()
    {
        if (next_state == -1)
        {
            printf("%10s", "");
            return;
        }
        printf("%10d", next_state);
    }

    void LRTableReduceCell::setCell(string token_left, BNFParse::vDeploymentTokenStruct token_vector)
    {
        this->reduce_formula = ReduceFormula{token_left, token_vector};
        this->is_valid = true;
    };

    void LRTableReduceCell::parseStringCell(LRTableLoad *loader)
    {
        string text_left = loader->road_token();
        is_valid = text_left == "-1" ? false : true;
        // printf("parseStringCell text_left %s\n",text_left.c_str());

        if (!is_valid)
        {
            return;
        }

        int token_vector_size = stoi(loader->road_token());

        BNFParse::vDeploymentTokenStruct token_vector;

        for (int i = 0; i < token_vector_size; i++)
        {

            int label = stoi(loader->road_token());

            string token_str = loader->road_token();

            token_vector.push_back(BNFParse::DeploymentTokenStruct{token_str, label});
        }

        this->reduce_formula = ReduceFormula{text_left, token_vector};
    }

    string LRTableReduceCell::getCellString()
    {
        if (!is_valid)
        {
            return "-1";
        }

        // 前から順に token_left ,

        string text = "";
        text += reduce_formula.token_left;
        text += " ";
        text += to_string(reduce_formula.token_vector.size());
        for (int i = 0; i < reduce_formula.token_vector.size(); i++)
        {
            text += " ";
            text += to_string(reduce_formula.token_vector[i].label);
            text += " ";
            text += reduce_formula.token_vector[i].token_str;
        }

        return text;
    }
    ReduceFormula LRTableReduceCell::getCell()
    {
        return this->reduce_formula;
    }
    bool LRTableReduceCell::getValid()
    {
        return this->is_valid;
    }
    void LRTableReduceCell::debugCell()
    {
        if (!is_valid)
        {
            printf("%10s", "");
            return;
        }
        printf("%10s", reduce_formula.token_left.c_str());
    }

    string LRTableAcceptCell::getCellString()
    {
        return to_string(is_valid);
    }
    void LRTableAcceptCell::parseStringCell(LRTableLoad *loader)
    {
        is_valid = loader->road_token() == "1" ? true : false;
    }

    void LRTableAcceptCell::setCell(bool is_valid)
    {
        this->is_valid = is_valid;
    };
    bool LRTableAcceptCell::getValid()
    {
        return this->is_valid;
    }
    void LRTableAcceptCell::debugCell()
    {
        if (!is_valid)
        {
            printf("%10s", "");
            return;
        }
        printf("%10s", "acc");
    }
};
