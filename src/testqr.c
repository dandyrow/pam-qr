#include "pam_qr.h"

int print_message(const char *message);

// int margin = 1;
// QRcode *encode(const unsigned char *intext, int length);
// void writeANSI_margin(int realwidth, int buffer_s,
//                              char* buffer, char *output);
// char* writeANSI(const QRcode *qrcode);


// int main()
// {
// 	char *output;
// 	QRcode *qrcode;    

// 	qrcode = QRcode_encodeString((char *) "Hello world!", QR_VERSION, QR_ECLEVEL_L, QR_MODE_8, QR_CASE_SENSITIVITY);
// 	if(qrcode == NULL) {
// 		exit(EXIT_FAILURE);
// 	}

// 	// int realwidth = (qrcode->width + margin * 2);
// 	// int buffer_s = (realwidth * 5) * 2;
// 	// int buff_tot = buffer_s * 2 + buffer_s * qrcode->width;
// 	// output = (char *)malloc((size_t)buff_tot);
	
// 	fprintf(stderr, "Version: %d\n", qrcode->version);

// 	output = writeANSI(qrcode);
// 	fputs(output, stdout);

//     QRcode_free(qrcode);
//     return 0;
// }

// void writeANSI_margin(int realwidth, int buffer_s,
//                              char* buffer, char *output)
// {
// 	int y;

// 	strncpy(buffer, QR_WHITE_CHAR, (size_t) QR_WHITE_SIZE);
// 	memset(buffer + QR_WHITE_SIZE, ' ', (size_t) realwidth * 2);
// 	strcpy(buffer + QR_WHITE_SIZE + realwidth * 2, "\033[0m\n"); // reset to default colors
// 	for(y = 0; y < margin; y++ ){
// 		// fputs(buffer, stdout);
// 		strncat(output, buffer, buffer_s);
// 	}
// }

// char* writeANSI(const QRcode *qrcode)
// {
// 	unsigned char *row, *p;
// 	int x, y;
// 	int realwidth;
// 	int last;

// 	// const char *white, *black;
// 	char *buffer;
// 	int buffer_s;
// 	char *output;
// 	// int white_s, black_s, buffer_s;

// 	// white = "\033[47m";
// 	// white_s = 5;
// 	// black = "\033[40m";
// 	// black_s = 5;

// 	realwidth = (qrcode->width + QR_MARGIN * 2);
// 	buffer_s = (realwidth * QR_WHITE_SIZE) * 2;
// 	buffer = (char *)malloc((size_t)buffer_s);
// 	if(buffer == NULL) {
// 		fprintf(stderr, "Failed to allocate memory.\n");
// 		exit(EXIT_FAILURE);
// 	}

// 	int buff_tot = buffer_s * (QR_MARGIN * 2) + buffer_s * qrcode->width;
// 	output = (char *) malloc((size_t) buff_tot);
// 	memset(output, 0, (size_t)buff_tot);

// 	/* top margin */
// 	writeANSI_margin(realwidth, buffer_s, buffer, output);

// 	// strncat(output, buffer, buffer_s);

// 	/* data */
// 	// p = qrcode->data;
// 	for(y = 0; y < qrcode->width; y++) {
// 		row = qrcode->data + y*qrcode->width;

// 		memset(buffer, 0, (size_t)buffer_s);
// 		strncpy(buffer, QR_WHITE_CHAR, (size_t) QR_WHITE_SIZE);
// 		for(x = 0; x < margin; x++ ){
// 			strncat(buffer, "  ", 2);
// 		}
// 		last = 0;

// 		for(x = 0; x < qrcode->width; x++) {
// 			if(*(row+x)&0x1) {
// 				if( last != 1 ){
// 					strncat(buffer, QR_BLACK_CHAR, (size_t) QR_BLACK_SIZE);
// 					last = 1;
// 				}
// 			} else if( last != 0 ){
// 				strncat(buffer, QR_WHITE_CHAR, (size_t) QR_WHITE_SIZE);
// 				last = 0;
// 			}
// 			strncat(buffer, "  ", 2);
// 		}

// 		if( last != 0 ){
// 			strncat(buffer, QR_WHITE_CHAR, (size_t) QR_WHITE_SIZE);
// 		}
// 		for(x = 0; x < margin; x++ ){
// 			strncat(buffer, "  ", 2);
// 		}
// 		strncat(buffer, "\033[0m\n", 5);
// 		// fputs(buffer, stdout);
// 		strncat(output, buffer, buffer_s);
// 	}

// 	/* bottom margin */
// 	writeANSI_margin(realwidth, buffer_s, buffer, output);

// 	strncat(output, buffer, buffer_s);

// 	// fputs("\n", stdout);
// 	// fputs(output, stdout);

// 	free(buffer);

// 	return output;
//}
// #include "pam_qr.h"

int main(int argc, char const *argv[])
{
	const char *qrcode;
	QRcode *qrcode_data;

	qrcode_data = QRcode_encodeString((char *) "I wonder how long this string can be hopefully long enough to have a URL with numbers like 1 or 7 in it", QR_VERSION, QR_ECLEVEL_L, QR_MODE_8, QR_CASE_SENSITIVITY);

    if (qrcode_data == NULL) {
        // Handle no qrcode data error
    }

    qrcode = generate_qr_code(qrcode_data);
	return print_message(qrcode);
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

int print_message(const char *message) {
	fputs(message, stdout);
	return 0;
}