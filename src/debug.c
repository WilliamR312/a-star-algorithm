#include "main.h"

void print_node(node_t *node, char *title)
{
    if (!node)
        return;
    printf("Node %s (x: %d, y: %d) - f: %f - h: %f - g: %f - %p\n",
    title, node->coords.x, node->coords.y, node->heuristic.f_cost,
    node->heuristic.h_cost, node->heuristic.g_cost, (void *) node);
}

int print_len_node_linked_list(node_t *head, char *title)
{
    int c = 0;
    node_t *current = head;
    while (current) {
        c++;
        current = current->next;
    }
    printf("len %s = %d\n", title, c);
    return c;
}

void print_chained_list(struct node *head, char *title)
{
    printf("<- CHAINED LIST (%s) ->\n", title);
    struct node *current = head;
    while (current) {
        print_node(current, "");
        current = current->next;
    }
    printf("-----\n");
}

static void print_colide_map(unsigned char **c_map, int *s_map, int nb_lines, coords_t str_p, coords_t end_p) 
{
    printf("<- COLIDE MAP ->\n");
    for (int y = 0; y < nb_lines; y++) {
        for (int x = 0; x < s_map[y]; x++) {
            if (str_p.x == x && str_p.y == y)
                printf("%c", START);
            else if (end_p.x == x && end_p.y == y && c_map[y][x] == END)
                printf("%c", END);
            else if (c_map[y][x] == WALL)
                printf("%c", WALL);
            else if (c_map[y][x] == PATH)
                printf("%c", PATH);
            else
                printf("%c", SPACE);
        }
        printf("\n");
    }
    printf("-----\n");
}

/*
 * Print each lines with their size
 */
static void print_map_info(int nb_lines, int *s_line)
{
    printf("<- MAP INFO ->\n nb lines : %d\n", nb_lines);
    for (int y = 0; y < nb_lines; y++) {
        printf("%d - %d ,", y, s_line[y]);
    }
    printf("\n-----\n");
}

void print_map_struct(struct map *map)
{
    //print_map_info(map->nb_lines, map->lines_size);
    print_colide_map(map->colider_map, map->lines_size, map->nb_lines, map->starting_p, map->ending_p);
    //printf("Starting Point : %d, %d\n", map->starting_p.x, map->starting_p.y);
    printf("Ending Point : %d, %d\n", map->ending_p.x, map->ending_p.y);
}