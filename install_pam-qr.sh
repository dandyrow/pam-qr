#!/bin/bash
#This script installs pam-qr and restarts the ssh service

gcc -fPIC -fno-stack-protector -c src/pam-qr.c

sudo rm /lib/x86_64-linux-gnu/security/pam-qr.so
sudo ld -x --shared -o /lib/x86_64-linux-gnu/security/pam-qr.so pam-qr.o 
sudo chmod 644 pam-qr.so

rm pam-qr.so

sudo systemctl restart sshd