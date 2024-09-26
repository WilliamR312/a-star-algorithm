#include "main.h"

void free_map(struct map** map) {
    for (int i = 0; i < (*map)->nb_lines; i++)
        free((*map)->colider_map[i]);
    free((*map)->colider_map);
    free((*map)->lines_size);
    free(*map);
}

static int get_nb_line(char *str) {
    int size = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != '\n' && str[i] != '\0' && str[i + 1] == '\0') {
            size++;
        }
        else if (str[i] == '\n') {
            size++;
        }
    }
    return (size);
}

/* static int get_nb_line_win(char *str) {
    int size = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != '\r' && (str[i + 1] == '\r' || str[i + 1] == '\n')) {
            size++;
        }
        else if (str[i] != '\0' && str[i + 1] == '\0') {
            size++;
        }
    }
    return (size);
} */

static int get_line_size(char *str, int s_i) {
    int size = 0;
    for (; str[s_i] != '\n' && str[s_i] != '\0' && str[s_i] != '\r'; s_i++, size++);
    return (size);
}

static unsigned char *get_colide_line(int line_size, int x, char *str, int y, struct map *map) {
    unsigned char *colide_map = malloc(line_size * sizeof(unsigned char));
    for (int i = 0; i < line_size; i++) {
        if (str[x + i] == '\0' || str[x + i] == '\n')
            colide_map[i] = '\0';
        else if (str[x + i] == WALL)
            colide_map[i] = WALL;
        else if (str[x + i] == START) {
            map->starting_p.x = i;
            map->starting_p.y = y;
            colide_map[i] = START;
        } else if (str[x + i] == END) {
            map->ending_p.x = i;
            map->ending_p.y = y;
            colide_map[i] = END;
        } else
            colide_map[i] = SPACE;
    }
    return (colide_map);
}

static int skip_backlines(char *raw_map, int x) {
    for (; raw_map[x] == '\n'; x++);
    return (x);
}

static void get_colide_map(struct map* map, char *raw_map) {
    map->colider_map = malloc((map->nb_lines + 1) * sizeof(unsigned char *));
    map->lines_size = malloc(map->nb_lines * sizeof(int));
    int line_size = 0;

    for (int y = 0, x = 0; y < map->nb_lines;) {
        line_size = get_line_size(raw_map, x); // TODO: error management when no line
        if (line_size == 0)
            continue;
        x = skip_backlines(raw_map, x);
        if (line_size > 0) {
            map->lines_size[y] = line_size;
            map->colider_map[y] = get_colide_line(line_size + 1, x, raw_map, y, map);
            y++;
            x += line_size + 1;
        }
    }
    map->colider_map[map->nb_lines] = NULL;
}

struct map *get_map_content(char *raw_map) {
    struct map *map = malloc(sizeof(map_t));
    map->nb_lines = get_nb_line(raw_map);
    map->starting_p.x = 0;
    map->starting_p.y = 0;
    map->ending_p.x = 1;
    map->ending_p.y = 1;
    get_colide_map(map, raw_map);
    return (map);
}