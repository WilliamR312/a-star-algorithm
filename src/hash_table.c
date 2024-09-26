#include "main.h"

int szudziks_pair_fun_hash(coords_t coords, int capacity) // Szudzik’s Pairing Function (Optimized Cantor): 0
{
    if (coords.x >= coords.y) {
        return ((coords.x * coords.x + coords.x + coords.y) % capacity);
    }
    return ((coords.y * coords.y + coords.x) % capacity);
}

int bit_prime_hash(coords_t coords, int capacity) // Efficient Hash Function Using Bitwise XOR and Prime Multiplication
{
    unsigned int p1 = 73856093;
    unsigned int p2 = 19349663;
    unsigned int res = ((coords.x * p1) ^ (coords.y * p2)) % capacity;
    return res;
}

int cantor_pair_hash(coords_t coords, int capacity) // Cantor Pairing Function : 214
{
    int sum = coords.x + coords.y;
    sum = (sum * (sum + 1)) / 2 + coords.y;
    return sum % capacity; 
}

int bitwise_shift_xor_hash(coords_t coords, int capacity) // Bitwise Shift and XOR
{
    return ((coords.x ^ (coords.y << 16)) % capacity); // Simple Bitwise Shift and XOR CUSTOM: 562
    // return (((coords.x << 16) ^ coords.x) % capacity); // Bitwise Shift and XOR ORIGINAL: BIG TIME
}

int hash_second(coords_t coords, int capacity)
{
    int res = cantor_pair_hash(coords, capacity); // 2
    return res + 1;
}

int hash_first(coords_t coords, int capacity)
{
    return bitwise_shift_xor_hash(coords, capacity); // 1
    // return cantor_pair_hash(coords, capacity); // 2
    // return szudziks_pair_fun_hash(coords, capacity); // 3
    // return bit_prime_hash(coords, capacity); // 4
}

hash_table_t *generate_hash_table(int size)
{
    hash_table_t *hash_table = malloc(sizeof(hash_table_t));
    hash_table->total = size; // TODO vérifier les pb de taille
    hash_table->coll_avoided = 0;
    hash_table->coll_break = 0;
    hash_table->coll_case = 0;
    hash_table->occuped = 0;
    hash_table->no_col = 0;
    hash_table->nodes = calloc(size, sizeof(node_t *));
    return hash_table;
}

// Double Hash
bool is_on_hash_table_old(hash_table_t *hash_table, coords_t coords)
{
    int first_hash = hash_first(coords, hash_table->total);
    node_t *node = hash_table->nodes[first_hash];

    if (node == NULL)
        return false;
    else if (node->coords.x == coords.x && node->coords.y == coords.y)
        return true;
    
    int second_hash = hash_second(coords, hash_table->total);
    int try_hash = 0;

    for (int i = 1; i < hash_table->total; i++) {
        unsigned long long a = i * second_hash;
        unsigned long long res = ((a + first_hash) % hash_table->total);
        try_hash = (int) res;
        node = hash_table->nodes[try_hash];
        if (node && node->coords.x == coords.x && node->coords.y == coords.y)
            return true;
    }
    return false;
}

bool is_on_hash_table(hash_table_t *hash_table, coords_t coords)
{
    int hash_i = hash_first(coords, hash_table->total);
    node_t *node = hash_table->nodes[hash_i];

    if (node == NULL)
        return false;
    else if (node->coords.x == coords.x && node->coords.y == coords.y)
        return true;

    for (int i = hash_i + 1; i != hash_i; i++) {
        if (i >= hash_table->total)
            i = 0;
        if (i == hash_i) // to fix when hash_val = 0
            return false;
        node = hash_table->nodes[i];
        if (node && node->coords.x == coords.x && node->coords.y == coords.y)
            return true;
    }
    return false;
}

void print_hash_table(hash_table_t *hash_table)
{
    printf("Hash Table : %d / %d - nocol: %d + col: %d (avoided: %d + break: %d)\n", hash_table->occuped, hash_table->total, hash_table->no_col, hash_table->coll_case, hash_table->coll_avoided, hash_table->coll_break);
}

int add_to_hash_table(hash_table_t *hash_table, node_t *node) // node already unliked of his list
{
    if (hash_table->occuped + 1 == hash_table->total) {
        // rehasment
        // rehash & copy
        printf("TOO SMALL HASH TABLE\n");
        print_hash_table(hash_table);
        exit(1);
    }

    int first_hash = hash_first(node->coords, hash_table->total);

    node_t *node_cell = hash_table->nodes[first_hash];
    if (node_cell == NULL) {
        hash_table->nodes[first_hash] = node;
        hash_table->no_col++;
    } else {
        /* int second_hash = hash_second(coords, hash_table->total);
        int try_hash = 0;
        bool found = false;
        for (int i = 1; found == false && i < hash_table->total; i++) {
            try_hash = (first_hash + i * second_hash) % hash_table->total;
            bucket = &hash_table->buckets[try_hash];
            if (bucket->occuped == false) {
                found = true;
                //if (i > 10)
                //    printf("%d\n", i);
            }
        }
        if (found == true) {
            bucket->occuped = true;
            bucket->coords = coords;
            hash_table->coll_avoided++;
        } else {
            printf("CANNOT ASSIGN TO HASH TABLE WITH DOUBLE HASH\n");
            hash_table->coll_break++;
        }
        hash_table->coll_case++; */
        bool found = false;
        for (int i = first_hash + 1; i != first_hash && !found; i++) {
            if (i >= hash_table->total)
                i = 0;
            if (i == first_hash) // to fix when hash_val = 0
                break;
            node_cell = hash_table->nodes[i];
            if (node_cell == NULL) {
                hash_table->nodes[i] = node;
                found = true;
                hash_table->coll_avoided++;
            }
        }
        if (found == false) {
            hash_table->coll_break++;
        }
        hash_table->coll_case++;

    }
    hash_table->occuped++;
    return 0;
}

void free_hash_table(hash_table_t **hash_table)
{
    free((*hash_table)->nodes);
    free(*hash_table);
}