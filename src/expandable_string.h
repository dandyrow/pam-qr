#ifndef EXPANDABLE_STRING_H
#define EXPANDABLE_STRING_H

#include <stdio.h>
#include <stdlib.h>

typedef struct string {
    char *ptr;
    size_t len;
} string;

int ExpandableString_init_string(string *s);
int ExpandableString_insert_at_end(char *string_to_insert, size_t size_of_string_inserting, string *s);

#endif