#pragma once

#include <tree_sitter/api.h>
#include <flecs.h>

#define META_MAX_FIELDS 100


void meta_append(ecs_world_t * world, ecs_entity_t e, TSQueryCapture capture, const char *source_code);