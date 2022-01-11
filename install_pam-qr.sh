#!/bin/bash

#This script installs pam-qr and restarts the ssh service
# Copyright (C) 2021 Daniel Lowry

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

# The license can be found in the root git repo in the LICENSE file.

# if ! command -v gcc &> /dev/null
# then
#     echo "Installing GCC..."
#     apt install gcc
# fi

# if ! command -v ld &> /dev/null
# then
#     echo "Installing bintuils..."
#     apt install bintuils
# fi

# if ! command -v libpam0g-dev &> /dev/null
# then
#     echo "Installing libpam0g-dev..."
#     apt install libpam0g-dev
# fi

# gcc -fPIC -fno-stack-protector -c src/pam-qr.c
# gcc -Wall -Wextra -g -c src/pam-qr.c
# gcc -Wall -Wextra -fPIC -fstack-protector -shared -o pam_qr.so src/pam_qr.c -lqrencode

# sudo rm /lib/x86_64-linux-gnu/security/pam_qr.so
# sudo ld -x --shared -o /lib/x86_64-linux-gnu/security/pam_qr.so pam_qr.so 
# sudo chmod 644 /lib/x86_64-linux-gnu/security/pam_qr.so

# rm pam_qr.so

# gcc -Wall -Wextra -fPIC -shared -o pam_qr.so src/qrcodegen.o src/pam_qr.c -L src #-lqrcodegen
# make pam_qr

lxc file push -r src/ current-ape/root/
lxc file push makefile current-ape/root/
# lxc file push src/libqrcodegen.so pam-test/root/

lxc exec current-ape -- make pam_qr
lxc exec current-ape -- chown root pam_qr.so
lxc exec current-ape -- chgrp root pam_qr.so
lxc exec current-ape -- chmod 644 pam_qr.so

# lxc exec pam-test -- chown root libqrcodegen.so
# lxc exec pam-test -- chgrp root libqrcodegen.so
# lxc exec pam-test -- chmod 644 libqrcodegen.so
# lxc exec pam-test -- ld --shared -o /usr/lib/libqrcodegen.so libqrcodegen.so

lxc exec current-ape -- cp pam_qr.so /lib/x86_64-linux-gnu/security/
lxc exec current-ape -- systemctl restart sshd
