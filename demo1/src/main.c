#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tree_sitter/api.h>
#include <flecs.h>
#include "file.h"
#include "ts.h"
#include "treef.h"




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

	treef_context_t ctx;
	ctx.world = world;
	ctx.source_code = file_read_allocated(argv[1]);
	ctx.source_code_length = strlen(ctx.source_code);

	treef_init(&ctx);
	treef_append(&ctx);
	treef_fini(&ctx);



	while (1) {
		ecs_progress(world, 0.0f);
		ecs_sleepf(0.1f);
	}


	return EXIT_SUCCESS;
}