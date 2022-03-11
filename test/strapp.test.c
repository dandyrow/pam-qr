#include <criterion/criterion.h>
#include "../src/strapp.h"

Test(strapp, append_to_string) {
    char *s1 = "hello ";
    char *s2 = "world!";
    char *str = strapp(s1, s2);

    cr_expect(strcmp(str, "hello world!") == 0, "str should be the combination of s1 & s2");
}