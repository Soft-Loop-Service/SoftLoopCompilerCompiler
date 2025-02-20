#include "./lexical/lexical_analysis.hpp"
#include "./syntactic/syntactic_analysis.hpp"
#include "definition.hpp"
#include "./lr_table/lr_table.hpp"
#include "./lr_table/lr_table_definition.hpp"

#include "./BNF/bnf.hpp"

// 第1引数 : 入力BNF名
// 第2引数 : 出力tableファイル名

int main(int argc, char *argv[])
{
    LRTable::LRTableMultilayer LR_table_multilayer;
    LRTable::bnfMain(argv[1], LR_table_multilayer);

    string table_accept_string = LR_table_multilayer.LR_table_accept.outputTable();
    string table_goto_string = LR_table_multilayer.LR_table_goto.outputTable();
    string table_reduce_string = LR_table_multilayer.LR_table_reduce.outputTable();
    string table_shift_string = LR_table_multilayer.LR_table_shift.outputTable();

    std::ofstream writing_file;
    std::string filename = argv[2];
    writing_file.open(filename, std::ios::out);

    writing_file << table_accept_string << endl;
    writing_file << table_goto_string << endl;
    writing_file << table_reduce_string << endl;
    writing_file << table_shift_string << endl;

    writing_file.close();
}