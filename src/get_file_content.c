#include "main.h"
#include <sys/stat.h>

static int get_file_size(char *file_name) {
    int size = 0;
    struct stat st;

    int instat_value = stat(file_name, &st);
    if (instat_value == -1)
        return instat_value;
    size = st.st_size;
    return (size);
}

char *get_file_content(char *file_name) {
    FILE *fptr = NULL;
    int file_size = get_file_size(file_name);
    if (file_size == -1)
        return (NULL);
    char * file_content = malloc(file_size + 1);
    file_content[file_size] = '\0';

    fptr = fopen(file_name, "r");
    if (fptr == NULL)
        return (NULL);
    fread(file_content, file_size, 1, fptr);
    fclose(fptr);
    return (file_content);
}

int copy_in_file(unsigned char **file_content)
{
    FILE *fp = fopen("result", "w");

    for (int y = 0; file_content[y]; y++)
        fprintf(fp, "%s\n", file_content[y]);
    fclose(fp);
    return 0;
}