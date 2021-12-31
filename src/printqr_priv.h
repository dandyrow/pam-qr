#ifndef PRINT_QR_PRIV_H
#define PRINT_QR_PRIV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Include public interface of this program
#include "printqr.h"

/* Define ANSI character codes and sizes for QR code printing */
#define QR_BLACK_CHAR "\033[40m"
#define QR_BLACK_SIZE 5             // Size 5 to prevent copying null character
#define QR_WHITE_CHAR "\033[47m"
#define QR_WHITE_SIZE 5             // Size 5 to prevent copying null character

void generate_qr_code_margin(int fullwidth, int buffer_size, char *buffer, char *qrcode);
void generate_qr_code_body(int buffer_size, char *buffer, char *qrcode, const QRcode *qrdata);

#endif