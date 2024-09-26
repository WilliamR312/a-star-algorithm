#include "main.h"

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("A map is required.");
        return (0);
    }
    // Get file content
    char *file_content = get_file_content(argv[1]);
    if (!file_content)
        return (0);
    // Format the map content
    struct map *map = get_map_content(file_content);
    free(file_content);
    astar(map);
    //print_map_struct(map);
    copy_in_file(map->colider_map);
    free_map(&map);
    return (1);
}