/*
PAM_QR. This program prompts the user with a QR code for them to scan
using the accompanying app to allow them to authenticate.
Copyright (C) 2021 Daniel Lowry

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

The license can be found in the root git repo in the LICENSE file.
*/

#include "pam_qr.h"


/* PAM entry point for session creation */
int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    return (PAM_IGNORE);
}

/* PAM entry point for session cleanup */
int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    return (PAM_IGNORE);
}

/* PAM entry point for accounting */
int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    return (PAM_IGNORE);
}

/* PAM entry point for authentication verification */
int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    if (argc != 2) {
        pam_prompt(pamh, PAM_ERROR_MSG, NULL, "Configuration Error: Missing arguments, please contact the system administrator.");
        return PAM_AUTHINFO_UNAVAIL;
    }
    int retval;
    const char *user = NULL;
    char *encoded_qr_str;
    struct json_object *parsed_auth_str;

    openlog("pam_qr: ", LOG_ODELAY, LOG_AUTHPRIV);

    retval = pam_get_user(pamh, &user, NULL);
    if (retval != PAM_SUCCESS || user == NULL)
    {
        syslog(LOG_WARNING, "Couldn't get username");
        return retval;
    }

    int otp = t2_get_key((const unsigned char *)argv[1], 0);
    char qr_str[QR_STR_SIZE];

    snprintf(qr_str, QR_STR_SIZE, "{ computerId: %s, username: %s, otp: %i }", argv[0], user, otp);
    encoded_qr_str = gen_qr_str(qr_str);

    if (encoded_qr_str == NULL)
    {
        syslog(LOG_ALERT, "Failed to gen QR code");
        return PAM_AUTHINFO_UNAVAIL;
    }

    pam_prompt(pamh, PAM_PROMPT_ECHO_ON, NULL, "%sPlease scan the QR with the QR Authenticate App. If the full QR code isn't shown resize the terminal window\n", encoded_qr_str);

    char *auth_str = request_auth_string_from_api(argv[2], argv[0], user, otp);
    parsed_auth_str = json_tokener_parse(auth_str);
    retval = check_authentication(user, parsed_auth_str);

    return (retval);
}

/*
    PAM entry point for setting user credentials (that is, to actually
    establish the authenticated user's credential to the service provider)
*/
int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    return (PAM_SUCCESS);
}

/* PAM entry point for authentication token (password) changes */
int pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    return (PAM_IGNORE);
}

char* request_auth_string_from_api(const char *authUrl, const char *computerId, const char *user, const int otp)
{
    char *str;
    CURL *curl;
    CURLcode result;

    curl = curl_easy_init();
    if (!curl)
    {
        syslog(LOG_WARNING, "Curl init failure");
        // TODO: Handle curl error
        return NULL;
    }

    char url_str[128];
    snprintf(url_str, 128, "http://%s/auth?computerId=%s&username=%s&otp=%i", authUrl, computerId, user, otp);

    curl_easy_setopt(curl, CURLOPT_URL, url_str);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, store_auth_str_in_var);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &str);
    // TODO: Set timeout for request

    result = curl_easy_perform(curl);
    if (result != CURLE_OK)
    {
        syslog(LOG_WARNING, "Curl perform failure");
        // TODO: Handle curl error
        return NULL;
    }

    curl_easy_cleanup(curl);
    syslog(LOG_WARNING, str);
    return str;
}

size_t store_auth_str_in_var(char *buffer, size_t itemsize, size_t nitems, char **str)
{
    *str = strapp(*str, buffer);
    return itemsize * nitems;
}

int check_authentication(const char *user, struct json_object *parsed_auth_str)
{
    int retauth;
    int retuser;
    struct json_object *authenticated;
    struct json_object *username;

    syslog(LOG_WARNING, "Checking auth");

    retauth = json_object_object_get_ex(parsed_auth_str, "authenticated", &authenticated);
    retuser = json_object_object_get_ex(parsed_auth_str, "username", &username);
    if (retauth != 1 || retuser != 1)
    {
        syslog(LOG_WARNING, "failed to get authenticated or username from JSON");
        return PAM_AUTH_ERR;
    }

    if (strncmp(json_object_get_string(authenticated), "true", 4) == 0)
    {
        if (strcmp(json_object_get_string(username), user) == 0) {
            return PAM_SUCCESS;
        }
    }
    syslog(LOG_WARNING, "auth failure");
    return PAM_AUTH_ERR;
}