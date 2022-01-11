#include "../expandable_string.h"

#include <curl/curl.h>
#include <json-c/json.h>
#include <string.h>

size_t print_data(char *buffer, size_t itemsize, size_t nitems, string *s) {
    ExpandableString_insert_at_end(buffer, itemsize * nitems, s);
    return itemsize * nitems;
}

int check_authentication(struct json_object *parsed_auth_str) {
    int retval;
    struct json_object *authenticated;

    retval = json_object_object_get_ex(parsed_auth_str, "authenticated", &authenticated);
    printf("Authenticated: %s\n", json_object_get_string(authenticated));
    if (retval != 1) {
        return 7;
    }

    if (strncmp(json_object_get_string(authenticated), "true", 4) == 0) {
        return 0;
    } else {
        return 7;
    }
}

int main(int argc, char const *argv[]) {
    int retval;
    string s;
    CURL *curl;
    CURLcode res;
    struct json_object *parsed_auth_str;

    curl = curl_easy_init();

    if (!curl) {
        fprintf(stderr, "Curl init failed\n");
        return EXIT_FAILURE;
    }

    ExpandableString_init_string(&s);

    //set options
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/auth");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, print_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    
    //perform out action
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "Download error: %s\n", curl_easy_strerror(res));
    }

    printf(s.ptr);
    printf("\n");
    parsed_auth_str = json_tokener_parse(s.ptr);
    retval = check_authentication(parsed_auth_str);
    // free(s.ptr);

    curl_easy_cleanup(curl);

    if (retval == 7) {
        printf("PAM_AUTH_ERR\n");
    } else {
        printf("PAM_SUCCESS\n");
    }
    return retval;
}