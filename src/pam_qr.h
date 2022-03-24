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

#ifndef PAM_QR_H
#define PAM_QR_H

/* Include standard library includes */
#include <syslog.h>
#include <stdio.h>
#include <string.h>

/* Include PAM headers */
#include <security/pam_ext.h>
#include <security/pam_modules.h>
#include <security/pam_modutil.h>
#include <security/pam_appl.h>
 
#include <curl/curl.h>

#include <json-c/json.h>

#include "strapp.h"
#include "gen_qr_str.h"

/* Define which PAM interfaces we provide */
#define PAM_SM_ACCOUNT
#define PAM_SM_AUTH
#define PAM_SM_PASSWORD
#define PAM_SM_SESSION

#define QR_STR_SIZE 128

int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv);
int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv);
int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv);
int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv);
int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv);
int pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, const char **argv);
char *request_auth_string_from_api(const char *authUrl, const char *computerId, const char *user);
size_t store_auth_str_in_var(char *buffer, size_t itemsize, size_t nitems, char **str);
int check_authentication(const char *user, struct json_object *parsed_auth_str);

#endif