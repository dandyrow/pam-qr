#!/bin/bash
#This script installs pam-qr and restarts the ssh service

if ! command -v gcc &> /dev/null
then
    echo "Installing GCC..."
    apt install gcc
fi

if ! command -v ld &> /dev/null
then
    echo "Installing bintuils..."
    apt install bintuils
fi

if ! command -v libpam0g-dev &> /dev/null
then
    echo "Installing libpam0g-dev..."
    apt install libpam0g-dev
fi

gcc -fPIC -fno-stack-protector -c src/pam-qr.c

sudo rm /lib/x86_64-linux-gnu/security/pam-qr.so
sudo ld -x --shared -o /lib/x86_64-linux-gnu/security/pam-qr.so pam-qr.o 
sudo chmod 644 /lib/x86_64-linux-gnu/security/pam-qr.so

rm pam-qr.o

sudo systemctl restart sshd