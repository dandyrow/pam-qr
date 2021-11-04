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
    QRcode *qrcode_data;

    retval = pam_get_user(pamh, &user, NULL);
    if (retval != PAM_SUCCESS || user == NULL) {
        return(PAM_IGNORE);
    }

    qrcode_data = QRcode_encodeString((char *) "Hello world!", QR_VERSION, QR_ECLEVEL_L, QR_MODE_8, QR_CASE_SENSITIVITY);

    if (qrcode_data == NULL) {
        // Handle no qrcode data error
    }

    qrcode = generate_qr_code(qrcode_data);

    display_message(pamh, (const char *) "Welcome!\n");
    display_message(pamh, qrcode);
    
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
int display_message(pam_handle_t *pamh, const char *message) {
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

/* Modified code from libqrencode/qrenc.c licensed under LGPLv2.1 
source found here: https://github.com/fukuchi/libqrencode */
char* generate_qr_code(const QRcode *qrcode_data) {
    int fullwidth, buffer_size, qrcode_size, y, x, last;
    char *buffer;
    char *qrcode;
    unsigned char *row;

    fullwidth = qrcode_data->width + QR_MARGIN * 2;     // Margin times 2 as there are 2 margins (one either side per row)

    buffer_size = (fullwidth * QR_WHITE_SIZE) * 2;		// Times 2 to account for printing 2 characters each time to conserve 1:1 aspect ratio
    buffer = (char *) malloc((size_t) buffer_size);
    if (buffer == NULL) {
        //Handle unable to allocate memory error
    }

    qrcode_size = buffer_size * (QR_MARGIN * 2) + buffer_size * qrcode_data->width;    // buffer_size = buffer size for 1 row. Width of QR = height
    qrcode = (char *) malloc((size_t) qrcode_size);
    if (qrcode == NULL) {
        //Handle unable to allocate memory error
    }
    memset(qrcode, 0, (size_t) qrcode_size);

    /* Top margin */
    generate_qr_code_margin(fullwidth, buffer_size, buffer, qrcode);

    for (y = 0; y < qrcode_data->width; y++) {
        row = qrcode_data->data + y * qrcode_data->width;

        memset(buffer, 0, (size_t) buffer_size);
        strncpy(buffer, QR_WHITE_CHAR, (size_t) QR_WHITE_SIZE);
        for(x = 0; x < QR_MARGIN; x++ ){
			strncat(buffer, "  ", 2);
		}
        last = 0;

        for (x = 0; x < qrcode_data->width; x++) {
            if(*(row+x)&0x1) {
				if( last != 1 ){
					strncat(buffer, QR_BLACK_CHAR, (size_t) QR_BLACK_SIZE);
					last = 1;
				}
			} else if( last != 0 ){
				strncat(buffer, QR_WHITE_CHAR, (size_t) QR_WHITE_SIZE);
				last = 0;
			}
			strncat(buffer, "  ", 2);
        }

        if (last != 0) {
            strncat(buffer, QR_WHITE_CHAR, (size_t) QR_WHITE_SIZE);
        }
        for (x = 0; x < QR_MARGIN; x++) {
            strncat(buffer, "  ", 2);
        }
        strncat(buffer, "\033[0m\n", 5);
        strncat(qrcode, buffer, buffer_size);
    }

    /* Bottom margin */
    generate_qr_code_margin(fullwidth, buffer_size, buffer, qrcode);
	
    free(buffer);
    return qrcode;
}

void generate_qr_code_margin(int fullwidth, int buffer_size, char *buffer, char *qrcode) {
    int y;

    strncpy(buffer, QR_WHITE_CHAR, (size_t) QR_WHITE_SIZE);
    memset(buffer + QR_WHITE_SIZE, ' ', (size_t) fullwidth * 2);    // Fills buffer with white blocks
    strcpy(buffer + QR_WHITE_SIZE + fullwidth * 2, "\033[0m\n");    // Reset colour

    for (y = 0; y < QR_MARGIN; y++) {
		strncat(qrcode, buffer, buffer_size);	// Add margin to qrcode		
    }
}