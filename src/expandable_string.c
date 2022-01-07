#include "expandable_string.h"

int ExpandableString_init_string(string *s) {
    s->len = 0;
    s->ptr = malloc(s->len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() on string failed\n");
        return EXIT_FAILURE;
    }
    s->ptr[0] = '\0';
    return EXIT_SUCCESS;
}

int ExpandableString_insert_at_end(char *string_to_insert, size_t size_of_string_inserting, string *s) {
    size_t new_len = s->len + size_of_string_inserting;
    s->ptr = realloc(s->ptr, new_len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() on string failed\n");
        return EXIT_FAILURE;
    }
    memcpy(s->ptr + s->len, string_to_insert, size_of_string_inserting);
    s->ptr[new_len] = '\0';
    s->len = new_len;
    return EXIT_SUCCESS;
}