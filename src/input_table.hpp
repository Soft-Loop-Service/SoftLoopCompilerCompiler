
#ifndef __INPUT_TABLE
#define __INPUT_TABLE

#include "definition.hpp"
#include "./lr_table/lr_table.hpp"

inline void inputTable(string table_file, LRTable::LRTableMultilayer &LR_table_multilayer)
{
    std::ifstream ifs(table_file);
    printf("inputTable\n");

    string table_accept_string;
    string table_goto_string;
    string table_reduce_string;
    string table_shift_string;
    if (ifs.fail())
    {
        std::cout << "Failed to open file." << std::endl;
        return ;
    }
    getline(ifs, table_accept_string);

    printf("%s\n", table_accept_string.c_str());

    getline(ifs, table_goto_string);
    printf("%s\n", table_goto_string.c_str());

    getline(ifs, table_reduce_string);
    printf("%s\n", table_reduce_string.c_str());

    getline(ifs, table_shift_string);
    printf("%s\n", table_shift_string.c_str());

    LR_table_multilayer = LRTable::LRTableMultilayer{
        LRTable::LRTableMakeGoto<LRTable::LRTableGotoCell>(table_goto_string),
        LRTable::LRTableMakeShift<LRTable::LRTableShiftCell>(table_shift_string),
        LRTable::LRTableMakeReduce<LRTable::LRTableReduceCell>(table_reduce_string),
        LRTable::LRTableMakeAccept<LRTable::LRTableAcceptCell>(table_accept_string)};

    LR_table_multilayer.LR_table_goto.debug();
    LR_table_multilayer.LR_table_shift.debug();
    LR_table_multilayer.LR_table_reduce.debug();
    LR_table_multilayer.LR_table_accept.debug();

    printf("inputTable end\n");
}

#endif