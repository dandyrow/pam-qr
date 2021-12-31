#include "printqr_priv.h"

int main(int argc, char const *argv[])
{
    const char *pname = argv[0];
    QRcode *encoded_qr;

    if (argc > 2) {
        fprintf(stderr, "%s: Too many arguments supplied. If trying to input a string please place it between quotes like so: \"<string>\"\n", pname);
        exit(EXIT_FAILURE);
    } else if (argc == 1) {
        fprintf(stderr, "%s: Missing expected argument. Please enter the string you wish to print as a qrcode\n", pname);
        exit(EXIT_FAILURE);
    }

    encoded_qr = QRcode_encodeString((char *) argv[1], QR_VERSION, QR_ECLEVEL_L, QR_MODE_8, QR_CASE_SENSITIVITY);

    if (encoded_qr == NULL) {
        if (errno == ENOMEM) {
            fprintf(stderr, "%s: Unable to allocate memory\n", pname);
        } else {
            fprintf(stderr, "%s: Invalid input\n", pname);
        }
        exit(errno);
    }

    printf(PrintQR_generate_qr_code_string(encoded_qr));

    return EXIT_SUCCESS;
}

/* Modified code from libqrencode/qrenc.c licensed under LGPLv2.1 source found
here: https://github.com/fukuchi/libqrencode */
char* PrintQR_generate_qr_code_string(const QRcode *qrdata) {
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
