#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tree_sitter/api.h>
#include <flecs.h>
#include "file.h"
#include "ts.h"
#include "meta.h"

// Include the C language grammar.
extern const TSLanguage *tree_sitter_c();



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

typedef struct {
    float x, y;
} Position;

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file.c>\n", argv[0]);
		return EXIT_FAILURE;
	}
	ecs_world_t * world = ecs_init();
	ECS_IMPORT(world, FlecsUnits);
	ECS_IMPORT(world, FlecsDoc);
	ecs_set(world, EcsWorld, EcsRest, {.port = 0});
	printf("Remote: %s\n", "https://www.flecs.dev/explorer/?remote=true");


    ECS_COMPONENT(world, Position);

    // Add reflection data to component
    ecs_struct(world, {
        .entity = ecs_id(Position), // Make sure to use existing id
        .members = {
            { .name = "x", .type = ecs_id(ecs_f32_t) }, // builtin float type
            { .name = "y", .type = ecs_id(ecs_f32_t) }
        }
    });


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
		ecs_entity_t e = ecs_new(world);
		ecs_entity_t o = ecs_set_scope(world, e);
		for (uint32_t i = 0; i < match.capture_count; i++) {
			meta_append(world, e, match.captures[i], source_code);
		}
		ecs_set_scope(world, o);
	}

	// Cleanup
	free(source_code);
	ts_query_cursor_delete(cursor);
	ts_query_delete(query);
	ts_tree_delete(tree);
	ts_parser_delete(parser);

	while (1) {
		ecs_progress(world, 0.0f);
		ecs_sleepf(0.1f);
	}


	return EXIT_SUCCESS;
}