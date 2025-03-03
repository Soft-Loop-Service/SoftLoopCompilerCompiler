#ifndef ANALYSIS_INTERFACE_HPP
#define ANALYSIS_INTERFACE_HPP

#include "./lexical/lexical_analysis.hpp"
#include "./syntactic/syntactic_analysis.hpp"
#include "./definition.hpp"
#include "./lr_table/lr_table.hpp"
#include "./BNF/bnf.hpp"
#include "./input_table.hpp"

// source_code_file_name
// table_file_name
// bytecode
Syntactic::SyntacticTree analysisInterface(string source_code_file_name, string table_file_name);
#endif