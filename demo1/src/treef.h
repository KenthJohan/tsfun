#pragma once

#include <tree_sitter/api.h>
#include <flecs.h>

#define META_MAX_FIELDS 100

typedef struct treef
{
	ecs_world_t * world;
	TSParser *parser;
	const TSLanguage * language;
	const char *source_code;
	uint32_t source_code_length;
	TSTree *tree;
	TSNode root_node;
	char const * query1_string;
	TSQuery *query1;
	char const * query2_string;
	TSQuery *query2;
} treef_context_t;

int treef_init(treef_context_t *ctx);
int treef_fini(treef_context_t *ctx);

void treef_append(treef_context_t * ctx);