#ifndef GEN_QR_STR
#define GEN_QR_STR

#include <qrencode.h>

char *gen_qr_margin(const QRcode *qrdata);
char *gen_qr_body(const QRcode *qrdata);
char *gen_qr_str(const char *str);

#endif