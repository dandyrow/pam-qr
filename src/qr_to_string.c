#include "qr_to_string_priv.h"

/* Modified code from libqrencode/qrenc.c licensed under LGPLv2.1 source found
here: https://github.com/fukuchi/libqrencode */
char* QRString_generate_qr_code_string(const QRcode *qrdata) {
    int fullwidth, buffer_size, qrcode_size;
    char *buffer;
    char *qrcode;

    fullwidth = qrdata->width + QR_MARGIN * 2;     // Margin times 2 as there are 2 margins (one either side per row)
    buffer_size = (fullwidth * QR_WHITE_SIZE) * 2;		// Times 2 to account for printing 2 characters each time to conserve 1:1 aspect ratio
    buffer = (char *) malloc((size_t) buffer_size);
    if (buffer == NULL) {
        //TODO: Handle unable to allocate memory error
    }

    qrcode_size = buffer_size * (QR_MARGIN * 2) + buffer_size * qrdata->width;    // buffer_size = buffer size for 1 row. Width of QR = height
    qrcode = (char *) malloc((size_t) qrcode_size);
    if (qrcode == NULL) {
        //TODO: Handle unable to allocate memory error
    }
    memset(qrcode, 0, (size_t) qrcode_size);

    /* Top margin */
    generate_qr_code_margin(fullwidth, buffer_size, buffer, qrcode);
    generate_qr_code_body(buffer_size, buffer, qrcode, qrdata);
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

void generate_qr_code_body(int buffer_size, char *buffer, char *qrcode, const QRcode *qrcode_data) {
    int x, y, last;
    unsigned char *row;

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
}
