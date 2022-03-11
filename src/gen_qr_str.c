#include "gen_qr_str.h"
#include "strapp.h"

#define QR_VERSION 0
#define QR_CASE_SENSITIVITY 1

#define QR_BLACK "  "
#define QR_WHITE "██"
#define QR_MARGIN_THICKNESS 1


char *gen_qr_margin(const QRcode *qrdata)
{
    int row_width = qrdata->width + 2 * QR_MARGIN_THICKNESS;
    char *str = "";

    for (int i = 0; i < row_width; i++)
    {
        str = strapp(str, QR_WHITE);
    }

    str = strapp(str, "\n");

    return str;
}

char *gen_qr_body(const QRcode *qrdata)
{
    int i = 0;
    char *str = "";

    for (int y = 0; y < qrdata->width; y++)
    {
        str = strapp(str, QR_WHITE);
        for (int x = 0; x < qrdata->width; x++)
        {
            if ((qrdata->data[i] & 1) == 0)
            {
                str = strapp(str, QR_WHITE);
            }
            else
            {
                str = strapp(str, QR_BLACK);
            }
            i++;
        }
        str = strapp(str, QR_WHITE);
        str = strapp(str, "\n");
    }

    return str;
}

char *gen_qr_str(const char *str)
{
    char *s = "\n";
    QRcode *symbol = QRcode_encodeString(str, QR_VERSION, QR_ECLEVEL_L, QR_MODE_8, QR_CASE_SENSITIVITY);

    s = strapp(s, gen_qr_margin(symbol));
    s = strapp(s, gen_qr_body(symbol));
    s = strapp(s, gen_qr_margin(symbol));

    return s;
}