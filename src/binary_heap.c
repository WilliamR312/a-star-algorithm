#include "main.h" 

void print_binary_heap(binary_heap_t *heap, char *title)
{
    printf("%s - ", title);
    printf("HEAP SIZE: %d / %d\n", heap->last_index, heap->capacity_index + 1);
    for (int i = 0; i < heap->last_index; i++)
        printf("f:%f (%p) -", heap->nodes[i]->heuristic.f_cost, (void *) heap->nodes[i]);
    //printf("f:%f (%p)-", bin_heap->nodes[i].f_cost, (void *) &bin_heap->nodes[i]);
    printf("\n");
}

int is_correct(binary_heap_t *heap)
{
    double first_f = heap->nodes[0]->heuristic.f_cost;
    for (int i = 1; i < heap->last_index; i++)
        if (heap->nodes[i]->heuristic.f_cost < first_f) {
            //printf("tmp:%f - cur: %f\n", heap->nodes[i]->heuristic.f_cost, first_f);
            return i;
        }
    return 0;
}

binary_heap_t *generate_binary_heap(int size)
{
    binary_heap_t *bin_heap = malloc(sizeof(binary_heap_t));

    bin_heap->capacity_index = size - 1;
    bin_heap->last_index = 0;
    bin_heap->nodes = calloc(size - 1, sizeof(node_t *)); //TODO: is size ok ?
    return bin_heap;
}

static bool compare_heuristic(heuristic_t heur_a, heuristic_t heur_b)
{
    if (heur_a.f_cost < heur_b.f_cost || (heur_a.f_cost == heur_b.f_cost && heur_a.h_cost < heur_b.h_cost))
        return true;
    return false;
}

int get_parent(int i)
{
    return (i - 1) / 2;
}

int get_left_child(int i)
{
    return 2 * i + 1;
}

int get_right_child(int i)
{
    return 2 * i + 2;
}

static void swap(node_t **a, node_t **b)
{
    node_t *tmp = *a;
    *a = *b;
    *b = tmp;
}

static int heapify_up(binary_heap_t *bin_heap, int current_index)
{
    int parent_index = 0;
    node_t *current_node = NULL;
    node_t *parent_node = NULL;

    while (current_index > 0) {
        parent_index = get_parent(current_index);
        current_node = bin_heap->nodes[current_index];
        parent_node = bin_heap->nodes[parent_index];

        if (compare_heuristic(current_node->heuristic, parent_node->heuristic)) {
            swap(&bin_heap->nodes[current_index], &bin_heap->nodes[parent_index]);
            current_index = parent_index;
        } else
            return current_index;
    }
    return current_index;
}

static int heapify_down(binary_heap_t *bin_heap, int current_index)
{
    node_t *current_node = NULL;
    node_t *right_parent = NULL;
    node_t *left_parent = NULL;

    double current_value = 0;

    int right_parent_index = 0;
    double right_value = 0;

    int left_parent_index = 0;
    double left_value = 0;

    while (current_index < bin_heap->last_index) {
        current_node = bin_heap->nodes[current_index];
        current_value = current_node->heuristic.f_cost;

        right_parent_index = get_right_child(current_index);
        right_parent = bin_heap->nodes[right_parent_index];

        left_parent_index = get_left_child(current_index);
        left_parent = bin_heap->nodes[left_parent_index];

        if ((right_parent == NULL || right_parent_index >= bin_heap->last_index) &&
        (left_parent == NULL || left_parent_index >= bin_heap->last_index)) // Parent out of array
            return current_index;

        if (right_parent && right_parent_index < bin_heap->last_index) {
            right_value = right_parent->heuristic.f_cost;
        }
        if (left_parent && left_parent_index < bin_heap->last_index) {
            left_value = left_parent->heuristic.f_cost;
        }

        if (right_parent && (!left_parent || right_value < left_value) &&
        compare_heuristic(right_parent->heuristic, current_node->heuristic)) {
            swap(&bin_heap->nodes[current_index], &bin_heap->nodes[right_parent_index]);
            current_index = right_parent_index;
        } else if (left_parent && (!right_parent || left_value <= right_value) &&
        compare_heuristic(left_parent->heuristic, current_node->heuristic)) {
            swap(&bin_heap->nodes[current_index], &bin_heap->nodes[left_parent_index]);
            current_index = left_parent_index;
        } else {
            return current_index;
        }
    }
    return current_index;
}

/* int get_heap_element_index_2(binary_heap_t *heap, coords_t coords, heap_item_t **item)
{
    for (int i = 0; i < heap->last_index; i++) {
        if (heap->items[i].coords.x == coords.x && heap->items[i].coords.y == coords.y) {
            if (*item)
                *item = &heap->items[i];
            return i;
        }
    }
    *item = NULL;
    return -1;
} */

int get_heap_element_index(binary_heap_t *heap, coords_t coords)
{
    node_t *tmp = NULL;

    for (int i = 0; i < heap->last_index; i++) {
        tmp = heap->nodes[i];
        if (tmp->coords.x == coords.x && tmp->coords.y == coords.y)
            return i;
    }
    return -1;
}

int replace_heap_element(binary_heap_t *heap, node_t *node)
{
    int index = 0;
    //node_t *node = NULL;

    index = get_heap_element_index(heap, node->coords);
    if (index == -1 || heap->last_index == 0)
        return -1;
    //node = heap->nodes[index];
    int up_index = heapify_up(heap, index);
    if (up_index == index && index < heap->last_index)
        index = heapify_down(heap, index);
    /* if (f_cost < node->heuristic.f_cost || (f_cost == node->heuristic.f_cost && h_cost < node->heuristic.f_cost)) {
        node->heuristic.g_cost = g_cost;
        node->heuristic.f_cost = f_cost + g_cost;
        index = heapify_up(heap, index);
    }
    else if (f_cost > node->heuristic.f_cost || (f_cost == node->heuristic.f_cost && g_cost > node->heuristic.f_cost)) {
        node->heuristic.g_cost = g_cost;
        node->heuristic.f_cost = f_cost + g_cost;
        index = heapify_down(heap, index);
    }
    else {
        // is equal, so use g_cost or h_cost ?
    } */
    return index;
}

node_t *extract_heap_root_element(binary_heap_t *bin_heap)
{
    if (bin_heap->last_index == 0)
        return NULL;

    node_t *root = bin_heap->nodes[0];
    if (bin_heap->last_index > 1) {
        bin_heap->nodes[0] = bin_heap->nodes[bin_heap->last_index - 1];
        bin_heap->nodes[bin_heap->last_index - 1] = NULL;
    }
    bin_heap->last_index--;
    heapify_down(bin_heap, 0);
    return root;
}

// left child: 2 * i + 1
// right child: 2 * i + 2
// Parent (i - 1) / 2

int insert_heap_element(binary_heap_t *bin_heap, node_t *node)
{
    int index = bin_heap->last_index;
    if (bin_heap->last_index == bin_heap->capacity_index + 1) {
        printf("BINARY HEAP IS FULL\n");
        return -1;
    }
    //print_node(node, "heap node insertion");
    bin_heap->nodes[index] = node;
    bin_heap->last_index++;
    index = heapify_up(bin_heap, index);
    return index;
}

void free_binary_heap(binary_heap_t **heap)
{
    free((*heap)->nodes);
    free(*heap);
}