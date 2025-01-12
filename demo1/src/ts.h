#pragma once

#include <tree_sitter/api.h>

char *ts_node_text(TSNode node, const char *source_code);

void ts_print_tree(TSNode node, int depth, char const *source_code);