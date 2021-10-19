#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <security/pam_appl.h>

// bool auth_user(const char *user, const char *password)
// {
// 	FILE *f = fopen("users", "r");
// 	char content[1024];
// 	int pos = 0;
// 	bool authenticated = false;

// 	int c;
// 	/* Reading the file until EOF and filling content */
// 	while ((c = fgetc(f)) != EOF) {
// 		content[pos++] = c;
// 	}

// 	char *userfield = strtok(content, ":");
// 	char *passfield = strtok(NULL, "\n");

// 	while (1) {
// 		if (strcmp(user, userfield) == 0 &&
// 		    strcmp(password, passfield) == 0) {
// 			authenticated = true;
// 			break;
// 		}
// 		userfield = strtok(NULL, ":");
// 		if (userfield == NULL)
// 			break;
// 		passfield = strtok(NULL, "\n");
// 		if (passfield == NULL)
// 			break;
// 	}
// 	return authenticated;
// }

int pam_sm_authenticate(pam_handle_t *handle, int flags, int argc, const char **argv) {
    int pam_code;

    const char *username = NULL;
    const char *password = NULL;

    pam_code = pam_get_user(handle, &username, "USERNAME: ");
    if (pam_code != PAM_SUCCESS) {
        fprintf(stderr, "Can't get username");
        return PAM_PERM_DENIED;
    }

    // pam_code = pam_get_authtok(handle, PAM_AUTHTOK, &password, "PASSWORDL ");
    // if (pam_code != PAM_SUCCESS) {
    //     fprintf(stderr, "Can't get password");
    //     return PAM_PERM_DENIED;
    // }

    // if (flags & PAM_DISALLOW_NULL_AUTHTOK) {
    //     if (password == NULL || strcmp(password, "") == 0) {
    //         fprintf(stderr, "Null authenticatin token is not allowed!");
    //         return PAM_PERM_DENIED;
    //     }
    // }

    // if (auth_user(username, password)) {
        printf("Welcome, user");
        return PAM_SUCCESS;
    // } else {
    //     fprintf(stderr, "Wrong username or password");
    //     return PAM_PERM_DENIED;
    // }
}