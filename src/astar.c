#include "main.h" 

// Get distance between 2 points
int get_heuristic_manhattan(coords_t coords_a, coords_t coords_b)
{
    return (abs(coords_b.x - coords_a.x) + abs(coords_b.y - coords_a.y));
}

// Octile distance
double get_diagonal_distance(coords_t current, coords_t target)
{
    int dx = abs(current.x - target.x);
    int dy = abs(current.y - target.y);
    double d = 5; // orthogonal movement cost
    double diag_dist = 1.414213; //sqrt(2) //1.4142135 //
    return (d * (dx + dy) + (diag_dist - 2 * d) * fmin(dx, dy));
}

static bool is_same_coords(coords_t current, coords_t target)
{
    if (current.x == target.x && current.y == target.y)
        return true;
    return false;
}

static bool is_space(unsigned char **colider_map, coords_t coords)
{
    if (colider_map[coords.y][coords.x] != WALL)
        return true;
    return false;
}

static void get_neighbor(map_t *map, node_t **open_head, binary_heap_t *open_binary_heap, node_t *current, coords_t coords, bool is_diagonal)
{
    node_t *open = return_node_in_list_from_coords(*open_head, coords);
    int open_index = get_heap_element_index(open_binary_heap, coords);

    double g_cost = is_diagonal ? current->heuristic.g_cost + 1.414213 : current->heuristic.g_cost + 1;
    int test = 0;

    if (!open) {
        open = create_node(coords, current, map, NULL);
        *open_head = set_node_prev(*open_head, open);
        //print_binary_heap(open_binary_heap, "get neighbor");
        open_index = insert_heap_element(open_binary_heap, open);

        //print_node(open, "INSERT");

        test = is_correct(open_binary_heap);
        if (test != 0) {
            print_node(open_binary_heap->nodes[test], "lui!");
            printf("NOT GOOD INSERT!!!!()!!!! %d\n", test);
            exit(1);
        }
    }

    if (open) {
        // OPTIMIZER: better lines but takes more turns (penalize directional changes)
        /* if (current->path && (((current->coords.x - current->path->coords.x) != (open->coords.x - current->coords.x)) || ((current->coords.y - current->path->coords.y) != (open->coords.y - current->coords.y))))
            g_cost += 1; */
        if (g_cost < open->heuristic.g_cost) {
            open->path = current;
            open->heuristic.g_cost = g_cost;
            open->heuristic.f_cost = open->heuristic.g_cost + open->heuristic.h_cost;

            // TODO: find in heap
            replace_heap_element(open_binary_heap, open);
            test = is_correct(open_binary_heap);
            if (test != 0) {
                printf("NOT GOOD REPLACE!!!!()!!!! %d\n", test);
                exit(1);
            }
        }
    }
}

// Get the nearest nodes
static void get_near_nodes(map_t *map, node_t *current, node_t **open_head, node_t **close_head, hash_table_t *close_hash_table, binary_heap_t *open_binary_heap)
{
    if (!current)
        return;

    int x = current->coords.x;
    int y = current->coords.y;

    coords_t coord_up_left = {x - 1, y - 1};
    coords_t coord_up = {x, y - 1};
    coords_t coord_up_right = {x + 1, y - 1};

    coords_t coord_down_left = {x - 1, y + 1};
    coords_t coord_down = {x, y + 1};
    coords_t coord_down_right = {x + 1, y + 1};

    coords_t coord_right = {x + 1, y};
    coords_t coord_left = {x - 1, y};

    //printf("START get_near_nodes\n");

    // UP
    if (coord_up.y >= 0 && coord_up.x < map->lines_size[coord_up.y] && is_space(map->colider_map, coord_up) &&
    is_on_hash_table(close_hash_table, coord_up) == false) {
        get_neighbor(map, open_head, open_binary_heap, current, coord_up, false);
    }
    // DOWN
    if (coord_down.y < map->nb_lines && coord_down.x < map->lines_size[coord_down.y] && is_space(map->colider_map, coord_down) &&
    is_on_hash_table(close_hash_table, coord_down) == false) {
        get_neighbor(map, open_head, open_binary_heap, current, coord_down, false);
    }
    // RIGHT
    if (coord_right.x < map->lines_size[y] && is_space(map->colider_map, coord_right) &&
    is_on_hash_table(close_hash_table, coord_right) == false) {
        get_neighbor(map, open_head, open_binary_heap, current, coord_right, false);
    }
    // LEFT
    if (x - 1 >= 0 && is_space(map->colider_map, coord_left) &&
    is_on_hash_table(close_hash_table, coord_left) == false) {
        get_neighbor(map, open_head, open_binary_heap, current, coord_left, false);
    }
    // UP LEFT
    if (coord_up_left.y >= 0 && coord_up_left.x >= 0 && coord_up_left.x < map->lines_size[coord_up_left.y] && is_space(map->colider_map, coord_up_left) &&
    is_on_hash_table(close_hash_table, coord_up_left) == false) {
        get_neighbor(map, open_head, open_binary_heap, current, coord_up_left, true);
    }
    // UP RIGHT
    if (coord_up_right.y >= 0 && coord_up_right.x < map->lines_size[coord_up_right.y] && is_space(map->colider_map, coord_up_right) &&
    is_on_hash_table(close_hash_table, coord_up_right) == false) {
        get_neighbor(map, open_head, open_binary_heap, current, coord_up_right, true);
    }
    // DOWN LEFT
    if (coord_down_left.y < map->nb_lines && coord_down_left.x < map->lines_size[coord_down_left.y] && coord_down_left.x >= 0 && is_space(map->colider_map, coord_down_left) &&
    is_on_hash_table(close_hash_table, coord_down_left) == false) {
        get_neighbor(map, open_head, open_binary_heap, current, coord_down_left, true);
    }
    // DOWN RIGHT
    if (coord_down_right.y < map->nb_lines && coord_down_right.x < map->lines_size[coord_down_right.y] && is_space(map->colider_map, coord_down_right) &&
    is_on_hash_table(close_hash_table, coord_down_right) == false) {
        get_neighbor(map, open_head, open_binary_heap, current, coord_down_right, true);
    }

    //print_binary_heap(open_binary_heap, "All neighbor");

}

// Optimization: can get it before, during the get_near_node
static node_t *get_nearest_node(binary_heap_t *heap, node_t *node)
{
    /* node_t *current = node;
    node_t *nearest = node;

    while (current) {
        if (current->heuristic.f_cost < nearest->heuristic.f_cost || (current->heuristic.f_cost == nearest->heuristic.f_cost && current->heuristic.h_cost < nearest->heuristic.h_cost)) // si fcost plus petit ou si identique: h cost plus petit
            nearest = current;
        current = current->next;
    } */

    //extract_heap_root_element(heap);
    if (heap->last_index != 0)
        return extract_heap_root_element(heap);
    return NULL;

    //return nearest;
}

static int add_path_on_map(node_t *head, unsigned char **colider_map)
{
    node_t *current = head;
    int path_size = 0;

    while (current) {
        if (current != head && current->path) {
            colider_map[current->coords.y][current->coords.x] = PATH;
            path_size++;
        }
        current = current->path;
    }
    return path_size;
}

int astar(struct map *map)
{
    node_t *close_head = NULL;
    node_t *open_head = create_node(map->starting_p, NULL, map, NULL);
    node_t *current_node = NULL;
    hash_table_t *close_hash_table = generate_hash_table(CLOSE_HASH_TABLE_SIZE);
    binary_heap_t *open_binary_heap = generate_binary_heap(OPEN_BINARY_HEAP);

    insert_heap_element(open_binary_heap, open_head);

    /* node_t origin = {.heuristic = {0, 1, 252.38}, .coords = {0, 0}};
    node_t node2 = {.heuristic = {0, 1, 11}, .coords = {0, 1}};
    node_t node3 = {.heuristic = {0, 1, 12}, .coords = {0, 2}};
    node_t node4 = {.heuristic = {0, 1, 8}, .coords = {0, 3}};
    node_t node5 = {.heuristic = {0, 1, 10}, .coords = {0, 4}};
    node_t node6 = {.heuristic = {0, 1, 7}, .coords = {0, 5}};
    node_t node7 = {.heuristic = {0, 1, 8}, .coords = {0, 6}}; */

/*
    insert_heap_element(open_binary_heap, &origin);
    print_binary_heap(open_binary_heap, "");
    insert_heap_element(open_binary_heap, &node2);
    print_binary_heap(open_binary_heap, "");
    insert_heap_element(open_binary_heap, &node3);
    print_binary_heap(open_binary_heap, "");
    insert_heap_element(open_binary_heap, &node4);
    print_binary_heap(open_binary_heap, "");
    insert_heap_element(open_binary_heap, &node5);
    print_binary_heap(open_binary_heap, "");
    insert_heap_element(open_binary_heap, &node6);
    print_binary_heap(open_binary_heap, "");
    insert_heap_element(open_binary_heap, &node7); */
    
    //print_binary_heap(open_binary_heap);
    //extract_heap_root_element(open_binary_heap);
    //print_binary_heap(open_binary_heap);
    //extract_heap_root_element(open_binary_heap);
    //print_binary_heap(open_binary_heap);
    //extract_heap_root_element(open_binary_heap);
    //print_binary_heap(open_binary_heap);

/*     replace_heap_element(open_binary_heap, (coords_t) {0, 8}, 1);
    replace_heap_element(open_binary_heap, (coords_t) {0, 6}, 2); */
    //print_binary_heap(open_binary_heap, "START");


    int return_value = 0;
    int c = 0;
    bool found = false;

    while (found == false) {
        //printf("c: %d\n", c);
        current_node = get_nearest_node(open_binary_heap, open_head);
        add_to_hash_table(close_hash_table, current_node);
        //print_node(current_node, "current node");
        //print_hash_table(close_hash_table);
        get_near_nodes(map, current_node, &open_head, &close_head, close_hash_table, open_binary_heap);
        //print_binary_heap(open_binary_heap, "NEAR");
        /* if (open_head == current_node && open_head)
            open_head = open_head->next;
        close_head = set_node_prev(close_head, current_node); // TOTO: unlink current_node */
        // extract_heap_root_element(open_binary_heap);
        if (current_node == NULL || (current_node && is_same_coords(current_node->coords, map->ending_p))) {
            found = true;
            break;
        }
        //puts("+");
        c++;
    }

    if (!current_node) {
        printf("No path found. (%d turn.s)\n", c);
        return_value = 1;
    }
    else {
        printf("Path found. (%d turn.s)\n", c);
        int path_size = add_path_on_map(current_node, map->colider_map);
        printf("Path size %d\n", path_size);
    }

    //print_len_node_linked_list(close_head, "close");
    //print_len_node_linked_list(open_head, "open");
    //print_hash_table(close_hash_table);

    //insert_heap_element(open_binary_heap, &origin);

    //print_binary_heap(open_binary_heap, "A* END");
    //print_hash_table(close_hash_table);
    free_hash_table(&close_hash_table);
    free_binary_heap(&open_binary_heap);
    free_node_list(close_head);
    free_node_list(open_head);

    return return_value;
}