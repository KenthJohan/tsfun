#include "ts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *ts_node_text(TSNode node, const char *source_code)
{
	uint32_t start_byte = ts_node_start_byte(node);
	uint32_t end_byte = ts_node_end_byte(node);
	size_t length = end_byte - start_byte;

	char *text = malloc(length + 1);
	if (!text) {
		perror("Unable to allocate memory");
		exit(EXIT_FAILURE);
	}

	strncpy(text, source_code + start_byte, length);
	text[length] = '\0';

	return text;
}

void ts_print_tree(TSNode node, int depth, char const *source_code)
{
	// Get the type of the node
	const char *type = ts_node_type(node);

	// Print indentation
	for (int i = 0; i < depth; i++) {
		printf("  ");
	}

	char *text = ts_node_text(node, source_code);
	// If text contains a newline, replace it with a space
	for (size_t i = 0; i < strlen(text); i++) {
		if (text[i] == '\n') {
			text[i] = ' ';
		}
	}

	// Print node type and text range
	/*
	printf("%s [%u, %u] - [%u, %u] %s\n",
	type,
	ts_node_start_point(node).row, ts_node_start_point(node).column,
	ts_node_end_point(node).row, ts_node_end_point(node).column, text ? text : "");
	*/
	printf("%s\n", type);

	free(text);

	// Iterate over children
	uint32_t child_count = ts_node_child_count(node);
	for (uint32_t i = 0; i < child_count; i++) {
		ts_print_tree(ts_node_child(node, i), depth + 1, source_code);
	}
}