#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "../qr_to_string.h"

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