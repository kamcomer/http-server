#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "progargs.h"
#include "server.h"

#define DEFAULT_PORT 8080

int main(int argc, char *argv[])
{
    const ArgumentDefinition defs[] = {
        {"port", true},
        {"root", true}};

    const ArgumentDefinitions prog_args = {defs, 2};

    ParsedArguments *parsed_args = handle_arguments(argc, argv, prog_args);
    if (parsed_args == NULL)
    {
        fprintf(stderr, "Error: Failed to parse arguments\n");
        return EXIT_FAILURE;
    }

    char *port_str = get_argument_value(parsed_args, "port");
    const int port = port_str ? atoi(port_str) : DEFAULT_PORT;

    char *root = get_argument_value(parsed_args, "root");
    if (root != NULL && chdir(root) != 0)
    {
        perror("Failed to change directory");
        free_parsed_args(parsed_args);
        return EXIT_FAILURE;
    }

    ServerContext ctx;
    if (server_init(&ctx, port) < 0)
    {
        free_parsed_args(parsed_args);
        return EXIT_FAILURE;
    }

    int result = server_run(&ctx);
    
    free_parsed_args(parsed_args);
    return result == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
