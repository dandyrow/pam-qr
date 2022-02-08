#include "qr_to_string_priv.h"

/* Modified code from libqrencode/qrenc.c licensed under LGPLv2.1 source found
here: https://github.com/fukuchi/libqrencode */
char* QRString_generate_qr_code_string(const QRcode *qrdata) {
    int row_width, row_buffer_size, qrcode_size;
    char *row_buffer;
    char *qrcode;

    row_width = QR_MARGIN_WIDTH + qrdata->width + QR_MARGIN_WIDTH;     // Margin times 2 as there are 2 margins (one either side per row)
    row_buffer_size = row_width * (WHITE_CHAR_WIDTH * NUM_CHAR_PER_CELL) + 1;		// Plus 1 for newline char
    row_buffer = (char *) malloc((size_t) row_buffer_size);
    if (row_buffer == NULL) {
        //TODO: Handle unable to allocate memory error
    }

    qrcode_size = row_buffer_size * (QR_MARGIN_WIDTH * 2) + row_buffer_size * qrdata->width;    // row_buffer_size = row_buffer size for 1 row. Width of QR = height
    qrcode = (char *) malloc((size_t) qrcode_size);
    if (qrcode == NULL) {
        //TODO: Handle unable to allocate memory error
    }
    memset(qrcode, 0, (size_t) qrcode_size);

    /* Top margin */
    generate_qr_code_margin(qrdata->width, row_buffer_size, row_buffer, qrcode);
    generate_qr_code_body(row_buffer_size, row_buffer, qrcode, qrdata);
    /* Bottom margin */
    generate_qr_code_margin(qrdata->width, row_buffer_size, row_buffer, qrcode);
	
    free(row_buffer);
    return qrcode;
}

void generate_qr_code_margin(int row_width, int row_buffer_size, char *row_buffer, char *qrcode) {
    // int y;
    for (int i = 0; i < row_width + 2; i++) {
        strncat(row_buffer, QR_WHITE_CHAR, (size_t)(WHITE_CHAR_WIDTH));
    }
    strncat(row_buffer, "\n", (size_t)1);
    // printf("Margin: %s", row_buffer);
    strncat(qrcode, row_buffer, (size_t)row_buffer_size);
}

void generate_qr_code_body(int row_buffer_size, char *row_buffer, char *qrcode, const QRcode *qrcode_data) {
    int i = 0;

    memset(row_buffer, '\0', row_buffer_size);
    for (int y = 0; y < qrcode_data->width; y++)
    {
        strncat(row_buffer, QR_WHITE_CHAR, (size_t)WHITE_CHAR_WIDTH);
        for (int x = 0; x < qrcode_data->width; x++) {
           if ((qrcode_data->data[i] & 1) == 0) {
               strncat(row_buffer, QR_WHITE_CHAR, (size_t)WHITE_CHAR_WIDTH);
           } else {
               strncat(row_buffer, QR_BLACK_CHAR, (size_t)QR_BLACK_SIZE);
           }
           i++;
        }
        strncat(row_buffer, QR_WHITE_CHAR, (size_t)WHITE_CHAR_WIDTH);
        strncat(row_buffer, "\n", 1);
        // printf("Row %i: %s", y, row_buffer);
        // exit(0);
        strncat(qrcode, row_buffer, (size_t)row_buffer_size);
        memset(row_buffer, '\0', row_buffer_size);
    }
    // int x, y, last;
    // unsigned char *row;

    // for (y = 0; y < qrcode_data->width; y++) {
    //     row = qrcode_data->data + y * qrcode_data->width;

    //     memset(row_buffer, 0, (size_t) row_buffer_size);
    //     strncpy(row_buffer, QR_WHITE_CHAR, (size_t) WHITE_CHAR_WIDTH);
    //     for(x = 0; x < QR_MARGIN_WIDTH; x++ ){
	// 		strncat(row_buffer, "  ", 2);
	// 	}
    //     last = 0;

    //     for (x = 0; x < qrcode_data->width; x++) {
    //         if(*(row+x)&0x1) {
	// 			if( last != 1 ){
	// 				strncat(row_buffer, QR_BLACK_CHAR, (size_t) QR_BLACK_SIZE);
	// 				last = 1;
	// 			}
	// 		} else if( last != 0 ){
	// 			strncat(row_buffer, QR_WHITE_CHAR, (size_t) WHITE_CHAR_WIDTH);
	// 			last = 0;
	// 		}
	// 		strncat(row_buffer, "  ", 2);
    //     }

    //     if (last != 0) {
    //         strncat(row_buffer, QR_WHITE_CHAR, (size_t) WHITE_CHAR_WIDTH);
    //     }
    //     for (x = 0; x < QR_MARGIN_WIDTH; x++) {
    //         strncat(row_buffer, "  ", 2);
    //     }
    //     strncat(row_buffer, "\033[0m\n", 5);
    //     strncat(qrcode, row_buffer, row_buffer_size);
    // }
}
