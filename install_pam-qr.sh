#!/bin/bash
#This script installs pam-qr and restarts the ssh service

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
gcc -Wall -Wextra -fPIC -fstack-protector -shared -o pam_qr.so src/pam_qr.c -lqrencode

# sudo rm /lib/x86_64-linux-gnu/security/pam_qr.so
# sudo ld -x --shared -o /lib/x86_64-linux-gnu/security/pam_qr.so pam_qr.so 
# sudo chmod 644 /lib/x86_64-linux-gnu/security/pam_qr.so

# rm pam_qr.so

# gcc -Wall -Wextra -fPIC -shared -o pam_qr.so src/qrcodegen.o src/pam_qr.c -L src #-lqrcodegen

lxc file push pam_qr.so pam-test/root/
# lxc file push src/libqrcodegen.so pam-test/root/

lxc exec pam-test -- chown root pam_qr.so
lxc exec pam-test -- chgrp root pam_qr.so
lxc exec pam-test -- chmod 644 pam_qr.so

# lxc exec pam-test -- chown root libqrcodegen.so
# lxc exec pam-test -- chgrp root libqrcodegen.so
# lxc exec pam-test -- chmod 644 libqrcodegen.so
# lxc exec pam-test -- ld --shared -o /usr/lib/libqrcodegen.so libqrcodegen.so

lxc exec pam-test -- cp pam_qr.so /lib/x86_64-linux-gnu/security/
lxc exec pam-test -- systemctl restart sshd