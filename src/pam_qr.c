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
#include <syslog.h>

/* PAM entry point for session creation */
int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return(PAM_IGNORE);
}

/* PAM entry point for session cleanup */
int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return(PAM_IGNORE);
}

/* PAM entry point for accounting */
int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return(PAM_IGNORE);
}

/* PAM entry point for authentication verification */
int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    int retval;
    const char *qrcode;
    const char *user = NULL;
    QRcode *encoded_qr;
    string auth_str;
    struct json_object *parsed_auth_str;

    openlog("pam_qr: ", LOG_ODELAY, LOG_AUTHPRIV);

    retval = pam_get_user(pamh, &user, NULL);
    if (retval != PAM_SUCCESS || user == NULL) {
        syslog(LOG_WARNING, "Couldn't get username");
        return (PAM_IGNORE);
    }

    encoded_qr = QRcode_encodeString((char *)"Hello world!", 0, QR_ECLEVEL_L, QR_MODE_8, QR_CASE_SENSITIVITY);

    if (encoded_qr == NULL) {
        if (errno == ENOMEM) {
            //TODO: Output error to sys
        } else {
            //TODO: Output error to sys
        }
        syslog(LOG_ALERT, "Failed to gen QR code, exiting...");
        exit(errno);
    }

    qrcode = QRString_generate_qr_code_string(encoded_qr);
    
    display_message_to_user2(pamh, qrcode);
    // display_message_to_user2(pamh, "\219 Please scan QR then press enter when ready █");

    ExpandableString_init_string(&auth_str);
    request_auth_string_from_api(&auth_str);
    parsed_auth_str = json_tokener_parse(auth_str.ptr);
    retval = check_authentication(pamh, user, parsed_auth_str);

    free(auth_str.ptr);
    return(retval);
}

/* 
    PAM entry point for setting user credentials (that is, to actually
    establish the authenticated user's credential to the service provider)
*/
int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return(PAM_SUCCESS);
}

/* PAM entry point for authentication token (password) changes */
int pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return(PAM_IGNORE);
}

/* Display a message to the user using PAM conv function */
int display_message_to_user(pam_handle_t *pamh, const char *message) {
    int retval;
    struct pam_conv *pam_conv;
    struct pam_message msg[1];
    const struct pam_message *pmsg[1];
    struct pam_response *resp = NULL;

    retval = pam_get_item(pamh, PAM_CONV, (const void **) &pam_conv);
    if (retval != PAM_SUCCESS) {
        return retval;
    }

    pmsg[0] = &msg[0];
    msg[0].msg = message;
    msg[0].msg_style = PAM_TEXT_INFO;
    retval = pam_conv->conv(1, pmsg, &resp, pam_conv->appdata_ptr);

    if (resp) {
        if (resp->resp) {
            free(resp->resp);
        }
        free(resp);
    }

    return retval;
}

int display_message_to_user2(pam_handle_t *pamh, const char *message) {
    int retval;
    struct pam_conv *pam_conv;
    struct pam_message msg[1];
    const struct pam_message *pmsg[1];
    struct pam_response *resp = NULL;

    retval = pam_get_item(pamh, PAM_CONV, (const void **) &pam_conv);
    if (retval != PAM_SUCCESS) {
        return retval;
    }

    pmsg[0] = &msg[0];
    msg[0].msg = message;
    msg[0].msg_style = PAM_PROMPT_ECHO_ON;
    retval = pam_conv->conv(1, pmsg, &resp, pam_conv->appdata_ptr);

    if (resp) {
        if (resp->resp) {
            free(resp->resp);
        }
        free(resp);
    }

    return retval;
}

int request_auth_string_from_api(string *auth_str) {
    CURL *curl;
    CURLcode result;

    curl = curl_easy_init();
    if (!curl) {
        syslog(LOG_WARNING, "Curl init failure");
        // TODO: Handle curl error
        return EXIT_FAILURE;
    }

    curl_easy_setopt(curl, CURLOPT_URL, "http://10.9.67.150:8080/auth");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, store_auth_str_in_var);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, auth_str);
    //TODO: Set timeout for request

    result = curl_easy_perform(curl);
    if (result != CURLE_OK) {
        syslog(LOG_WARNING, "Curl perform failure");
        //TODO: Handle curl error
        return EXIT_FAILURE;
    }

    curl_easy_cleanup(curl);
    return EXIT_SUCCESS;
}

size_t store_auth_str_in_var(char *buffer, size_t itemsize, size_t nitems, string *auth_str) {
    ExpandableString_insert_at_end(buffer, itemsize * nitems, auth_str);
    return itemsize * nitems;
}

int check_authentication(pam_handle_t *pamh, const char *user, struct json_object *parsed_auth_str)
{
    int retval;
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

    if (strncmp(json_object_get_string(authenticated), "true", 4) == 0) {
        // if (strcmp(json_object_get_string(username), user) == 0) {
        return PAM_SUCCESS;
        // }
    }
    syslog(LOG_WARNING, "auth failure");
    return PAM_AUTH_ERR;
}