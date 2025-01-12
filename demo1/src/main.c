#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tree_sitter/api.h>
#include <flecs.h>
#include "file.h"
#include "ts.h"

// Include the C language grammar.
extern const TSLanguage *tree_sitter_c();

// Helper function to read a file into memory




// Function to generate a stringify function for an enum
void generate_stringify_function(const char *enum_name, const char **values, size_t count)
{
	printf("\nconst char *%s_to_string(int value) {\n", enum_name);
	printf("    switch (value) {\n");
	for (size_t i = 0; i < count; i++) {
		printf("        case %s: return \"%s\";\n", values[i], values[i]);
	}
	printf("        default: return \"Unknown\";\n");
	printf("    }\n");
	printf("}\n");
}



int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file.c>\n", argv[0]);
		return EXIT_FAILURE;
	}

	// Initialize the parser
	const TSLanguage *language = tree_sitter_c();
	TSParser *parser = ts_parser_new();
	ts_parser_set_language(parser, language);

	// Read the input file
	char *source_code = file_read_allocated(argv[1]);

	// Parse the source code
	TSTree *tree = ts_parser_parse_string(parser, NULL, source_code, strlen(source_code));
	TSNode root_node = ts_tree_root_node(tree);

	printf("Syntax Tree:\n");
	ts_print_tree(root_node, 0, source_code);

	const char *query_string2 =
	"(enum_specifier name: (type_identifier) @enum.name "
	"body: (enumerator_list (enumerator) @enum.value))";

	const char *query_string1 = file_read_allocated("src/query1.scm");
	printf("Query: %s\n", query_string1);

	uint32_t error_offset;
	TSQueryError error_type;
	TSQuery *query = ts_query_new(language, query_string1, strlen(query_string1), &error_offset, &error_type);
	if (!query) {
		fprintf(stderr, "Failed to create query at offset %u: error %d\n", error_offset, error_type);
		fprintf(stderr, "Query string: %s\n", query_string1 + error_offset);
		ts_tree_delete(tree);
		ts_parser_delete(parser);
		return 1;
	}

	TSQueryCursor *cursor = ts_query_cursor_new();
	ts_query_cursor_exec(cursor, query, root_node);

	// Process matches
	TSQueryMatch match;
	while (ts_query_cursor_next_match(cursor, &match)) {
		const char *enum_name = NULL;
		const char **values = malloc(sizeof(char *) * 100); // Arbitrary limit
		size_t value_count = 0;

		for (uint32_t i = 0; i < match.capture_count; i++) {
			TSQueryCapture capture = match.captures[i];
			uint32_t length = 0;
			const char *capture_name = ts_query_capture_name_for_id(query, capture.index, &length);
			printf("%s %s\n", capture_name, ts_node_text(capture.node, source_code));
			if (strcmp(capture_name, "type") == 0) {
				//enum_name = ts_node_text(capture.node, source_code);
			} else if (strcmp(capture_name, "field") == 0) {
				//values[value_count++] = ts_node_text(capture.node, source_code);
			}
			printf(" ");
		}

		if (enum_name && value_count > 0) {
			generate_stringify_function(enum_name, values, value_count);
		}
		// Free allocated memory for captured values
		for (size_t i = 0; i < value_count; i++) {
			free((void *)values[i]);
		}
		free(values);
	}

	// Cleanup
	free(source_code);
	ts_query_cursor_delete(cursor);
	ts_query_delete(query);
	ts_tree_delete(tree);
	ts_parser_delete(parser);

	return EXIT_SUCCESS;
}