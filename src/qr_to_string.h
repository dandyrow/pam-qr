#ifndef PRINT_QR_H
#define PRINT_QR_H

#include <qrencode.h>

/* Define default values for QR code gen */
#define QR_VERSION 5
#define QR_MARGIN 2
#define QR_CASE_SENSITIVITY 1       // 1 = case sensitive, 0 = not case sensitive

char* QRString_generate_qr_code_string(const QRcode *qrdata);

#endif