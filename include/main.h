#ifndef MAIN_H_
#define MAIN_H_

#define _GNU_SOURCE
#include <stdio.h> // input output
#include <stdlib.h> // memory handle + min()
#include <stdbool.h> // bool
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h> // sqrt
#include <string.h> // memset

#define WALL '#'
#define SPACE ' '
#define START 'P'
#define END 'O'
#define PATH '*'

#define CLOSE_HASH_TABLE_SIZE 80009 //50009 //TODO: PRIME NUMBER OPTIMIZATION FOR HASH
#define OPEN_BINARY_HEAP 600

// Coordinates
typedef struct coordinates {
    int x;
    int y;
} coords_t;

typedef struct heuristic {
    double h_cost; // Distance from end node
    double g_cost; // Distance from start node
    double f_cost; // Total cost
} heuristic_t;

// Node element
typedef struct node {
    coords_t coords;
    heuristic_t heuristic;
    struct node *next; // next node in list
    struct node *prev; // prev node in list
    struct node *path; // previous path
} node_t;

// Map configuration
typedef struct map {
    unsigned char **colider_map;
    int *lines_size; // Array with each line size
    int nb_lines;
    coords_t starting_p;
    coords_t ending_p;
} map_t;

/* typedef struct bucket {
    node_t *node;
    bool occuped; // is not empty
} bucket_t; */

typedef struct hash_table {
    node_t **nodes;
    int total;
    int occuped;
    int no_col;
    int coll_case;
    int coll_avoided;
    int coll_break;
} hash_table_t;

/* typedef struct heap_item {
    node_t *node;
    coords_t coords;
    double h_cost; // Distance from end node
    double g_cost; // Distance from start node
    double f_cost; // Total cost
} heap_item_t; */

typedef struct binary_heap {
    node_t **nodes;
    int capacity_index;
    int last_index;
} binary_heap_t;

char *get_file_content(char *file_name);
struct map *get_map_content(char *raw_map);
int astar(struct map *map);
int copy_in_file(unsigned char **file_content);

void free_map(struct map** map);

int get_heuristic_manhattan(coords_t coords_a, coords_t coords_b);
double get_diagonal_distance(coords_t current, coords_t target);

node_t *create_node(coords_t coords, node_t *parent, map_t *map, node_t *next);
node_t *set_node_prev(node_t *node, node_t *prev_node);

bool is_coord_on_chained_list(node_t *head, coords_t coords);
node_t *return_node_in_list_from_coords(node_t *head, coords_t coords);

void free_node_list(node_t *head);


// HEAP
binary_heap_t *generate_binary_heap(int size);
int insert_heap_element(binary_heap_t *bin_heap, node_t *node);
node_t *extract_heap_root_element(binary_heap_t *bin_heap);
int replace_heap_element(binary_heap_t *heap, node_t *node);
void free_binary_heap(binary_heap_t **heap);
void print_binary_heap(binary_heap_t *bin_heap, char *title);
int get_heap_element_index(binary_heap_t *heap, coords_t coords);
int is_correct(binary_heap_t *heap);


// DEBUG
void print_map_struct(struct map *map);
void print_chained_list(struct node *head, char *title);
void print_node(node_t *node, char *title);
int print_len_node_linked_list(node_t *head, char *title);

// HASH
int hash(coords_t coords, int capacity);
int add_to_hash_table(hash_table_t *hash_table, node_t *node);
hash_table_t *generate_hash_table(int size);
bool is_on_hash_table(hash_table_t *hash_table, coords_t coords);
void print_hash_table(hash_table_t *hash_table);
void free_hash_table(hash_table_t **hash_table);


#endif /* MAIN_H_*/