/* 
PAM_QR. This header file provides definitions and imports for pam_qr.c
Copyright (C) 2021 Daniel Lowry

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

The license can be found in the root git repo in the LICENSE file.
*/

/* Define which PAM interfaces we provide */
#define PAM_SM_ACCOUNT
#define PAM_SM_AUTH
#define PAM_SM_PASSWORD
#define PAM_SM_SESSION

/* Define default values for QR code gen */
#define QR_VERSION 5
#define QR_MARGIN 2
#define QR_CASE_SENSITIVITY 1       // 1 = case sensitive, 0 = not case sensitive

/* Define ANSI character codes and sizes for QR code printing */
#define QR_BLACK_CHAR "\033[40m"
#define QR_BLACK_SIZE 5             // Size 5 to prevent copying null character
#define QR_WHITE_CHAR "\033[47m"
#define QR_WHITE_SIZE 5             // Size 5 to prevent copying null character

/* Include standard library includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Include PAM headers */
#include <security/pam_modules.h>
#include <security/pam_appl.h>

/* Include QR Code generator library */
#include <qrencode.h>

int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv);
int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv);
int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv);
int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv);
int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv);
int pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, const char **argv);
int display_message(pam_handle_t *pamh, const char *message);
char* generate_qr_code(const QRcode *qrcode);
void generate_qr_code_margin(int fullwidth, int buffer_size, char *buffer, char *qrcode);