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

    retval = pam_get_user(pamh, &user, NULL);
    if (retval != PAM_SUCCESS || user == NULL) {
        return(PAM_IGNORE);
    }

    encoded_qr = QRcode_encodeString((char *) "Hello world!", QR_VERSION, QR_ECLEVEL_L, QR_MODE_8, QR_CASE_SENSITIVITY);

    if (encoded_qr == NULL) {
        if (errno == ENOMEM) {
            //TODO: Output error to sys
        } else {
            //TODO: Output error to sys
        }
        exit(errno);
    }

    qrcode = PrintQR_generate_qr_code_string(encoded_qr);
    
    display_message_to_user(pamh, qrcode);
    display_message_to_user2(pamh, "Please scan QR then press enter when ready");
    
    request_auth_string_from_api(&auth_str);
    //TODO: Parse JSON
    //TODO: Use parsed json to verfy if user is authenticated
    
    return(PAM_SUCCESS);
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
        //TODO: Handle curl error
    }

    ExpandableString_init_string(auth_str);

    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/auth");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, );
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, auth_str);
    //TODO: Set timeout for request

    result = curl_easy_perform(curl);
    if (result != CURLE_OK) {
        //TODO: Handle curl error
    }

    curl_easy_cleanup(curl);
    return EXIT_SUCCESS;
}