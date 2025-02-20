#include "./lexical/lexical_analysis.hpp"
#include "./syntactic/syntactic_analysis.hpp"
#include "./definition.hpp"
#include "./lr_table/lr_table.hpp"
#include "./BNF/bnf.hpp"
#include "./input_table.hpp"

// source_code_file_name
// table_file_name
// bytecode
int main(int argc, char *argv[])
{
    char *source_code = new char[source_code_size * sizeof(char)];
    string table_file_name = argv[2];
    // string workspace_directory = argv[3];

    loadText(source_code, argv[1], source_code_size);
    string source_code_file_name = argv[1];
    vstring token_class_type = {};
    LexicalAnalysis::vLexicalToken token_string_vector = LexicalAnalysis::lexSyntax(source_code, token_class_type);

    LRTable::LRTableMultilayer LR_table_multilayer;
    inputTable(table_file_name, LR_table_multilayer);

    printf("syntacticAnalysis\n");

    LRTable::vReduceFormula syntactic_analysis_formula = {};
    Syntactic::syntacticAnalysis(LR_table_multilayer, token_string_vector, syntactic_analysis_formula);

    printf("syntacticPurseTree\n");
    Syntactic::vSyntacticTree syntactic_analysis_tree = {};

    Syntactic::syntacticAnalysisTree(syntactic_analysis_formula, syntactic_analysis_tree, token_string_vector);
    Syntactic::vSyntacticTree cst = syntactic_analysis_tree;
    Syntactic::vSyntacticTree ast = {};
    Syntactic::syntacticParseTree(cst, ast);

    // astが抽象構文木

    printf("analysis - end\n");
}
