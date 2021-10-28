/* Define which PAM interfaces we provide */
#define PAM_SM_ACCOUNT
#define PAM_SM_AUTH
#define PAM_SM_PASSWORD
#define PAM_SM_SESSION

#include <unistd.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>

/* Include PAM headers */
#include <security/pam_modules.h>
#include <security/pam_appl.h>

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
    struct pam_conv *conv;
    const struct pam_message *pmsg[1];
    struct pam_message msg[1];  
    struct pam_response *resp = NULL;  
    const char *user = NULL;
    char buffer[1024], checkfile[1024];
    int pgu_ret, gpn_ret, snp_ret, a_ret, retval;

    pgu_ret = pam_get_user(pamh, &user, NULL);
    if (pgu_ret != PAM_SUCCESS || user == NULL) {
        return(PAM_IGNORE);
    }

    retval = pam_get_item(pamh, PAM_CONV, (const void **) &conv);
    if (retval != PAM_SUCCESS) {
        return retval;
    }

    if (!conv || !conv->conv) {
        return PAM_CONV_ERR;
    }

    pmsg[0] = &msg[0];
    msg[0].msg = "Welcome to the system!";
    msg[0].msg_style = PAM_TEXT_INFO;
    retval = conv->conv(1, pmsg, &resp, conv->appdata_ptr);

    if (retval != PAM_SUCCESS) {
        return retval;
    }

    if (resp) {
        if (resp->resp) {
            free(resp-resp);
        }
        free(resp);
    }
    
    return(PAM_SUCCESS);
}

/* 
    PAM entry point for setting user credentials (that is, to actually
    establish the authenticated user's credential to the service provider)
*/
int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return(PAM_IGNORE);
}

/* PAM entry point for authentication token (password) changes */
int pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return(PAM_IGNORE);
}