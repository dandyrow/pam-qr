#include <criterion/criterion.h>

#include "../src/expandable_string.h"

string s;

void setup(void) {
    ExpandableString_init_string(&s);
}

TestSuite(expandable_string, .init=setup);

Test(expandable_string, init) {
    cr_expect(s.len == 0, "Length of the string should be 0");
    cr_expect(s.ptr[s.len] == '\0', "Pointer within string should contain the null char");
}

Test(expandable_string, expand) {
    ExpandableString_insert_at_end("Test", 4, &s);
    cr_expect(s.len == 4, "Length of string should now be 4");
    cr_expect(strcmp(s.ptr, "Test") == 0, "Pointer within string should be equal to Test");
}