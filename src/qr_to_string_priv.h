#ifndef PRINT_QR_PRIV_H
#define PRINT_QR_PRIV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include public interface of this program
#include "qr_to_string.h"

/* Define ANSI character codes and sizes for QR code printing */
#define QR_BLACK_CHAR "  "
#define QR_BLACK_SIZE 2             // Size 5 to prevent copying null character
#define QR_WHITE_CHAR "██"
#define WHITE_CHAR_WIDTH 6             // Size 5 to prevent copying null character
#define NUM_CHAR_PER_CELL 2

    void
    generate_qr_code_margin(int fullwidth, int buffer_size, char *buffer, char *qrcode);
void generate_qr_code_body(int buffer_size, char *buffer, char *qrcode, const QRcode *qrdata);

#endif