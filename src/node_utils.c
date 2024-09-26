#include "main.h"

node_t *create_node(coords_t coords, node_t *path, map_t *map, node_t *next)
{
    node_t *node = malloc(sizeof(node_t));
    node->coords.x = coords.x; // x
    node->coords.y = coords.y; // y
    node->heuristic.f_cost = 0;
    node->heuristic.g_cost = 0;
    node->heuristic.h_cost = get_diagonal_distance(coords, map->ending_p);
    node->next = next;
    node->prev = NULL;
    node->path = path;

    if (path) {
        if (path->coords.x != node->coords.x || path->coords.y != node->coords.y) // Diagonal movement
            node->heuristic.g_cost = path->heuristic.g_cost + 1.414213;
        else
            node->heuristic.g_cost = path->heuristic.g_cost + 1; // Orthogonal movement
    }        
    // F cost = H cost + G cost
    node->heuristic.f_cost = node->heuristic.h_cost + node->heuristic.g_cost;
    return (node);
}

bool is_coord_on_chained_list(node_t *head, coords_t coords)
{
    node_t *current = head;

    while (current) {
        if (current->coords.x == coords.x && current->coords.y == coords.y) {
            return true;
        }
        current = current->next;
    }
    return false;
}

node_t *return_node_in_list_from_coords(node_t *head, coords_t coords)
{
    node_t *current = head;

    while (current) {
        if (current->coords.x == coords.x && current->coords.y == coords.y)
            return current;
        current = current->next;
    }
    return NULL;
}

node_t *set_node_prev(node_t *node, node_t *prev_node)
{
    if (!prev_node) {
        node->prev = NULL;
        return node;
    }
    // If node is empty
    if (node == NULL) {
        if (prev_node->next)
            prev_node->next->prev = prev_node->prev;
        if (prev_node->prev)
            prev_node->prev->next = prev_node->next;
        prev_node->next = NULL;
        prev_node->prev = NULL;
        return prev_node;
    }

    // Remove prev_node from its original list 
    if (prev_node->next)
        prev_node->next->prev = prev_node->prev;
    if (prev_node->prev)
        prev_node->prev->next = prev_node->next;

    // Prev Node set
    prev_node->prev = node->prev;
    prev_node->next = node;

    // Node set
    if (node->prev)
        node->prev->next = prev_node;
    node->prev = prev_node;

    return prev_node;
}

node_t *add_to_head_chained_node(struct node *src, struct node *to_add)
{
    if (src == NULL) {
        to_add->next = NULL;
        return to_add;
    }
    to_add->next = src;

    return to_add;
}

void free_node_list(node_t *head)
{
    if (!head)
        return;
    node_t *current = head;
    node_t *next = NULL;
    int c = 0;
    while (current) {
        next = current->next;
        free(current);
        current = next;
        c++;
    }
    printf("%d nodes removed.\n", c);
}