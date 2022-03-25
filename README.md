# pam-qr

This is a PAM module that presents the user with a QR code to scan to authenticate rather than entering a password. This is part of my honours project for my 4th year of Computer Science at the University of Strathclyde Glasgow.

Root user is not allowed to be logged into using pam-qr yet as it is in alpha and possibly insecure.

## Dependecies (Ubuntu Names)

### Build:

- libpam0g-dev
- libcurl4-openssl-dev
- libjson-c-dev

### Runtime:

- libpam0g
- libcurl4
- libjson-c5

## Build Instructions

1. Install build dependencies (Ubuntu package names listed above).
2. Download source code from releases or clone repo for latest dev version.
3. If downloaded from releases, extract the compressed file.
4. cd into `pam-qr` folder.
5. run `make pam_qr` to build.
6. the compiled binary will be in the `lib` folder.

## Installation Instructions

### Install the PAM module

1. Copy the compiled binary from the lib folder to your distro's PAM module folder. In Ubuntu this is `/lib/x86_64-linux-gnu/security/`.
2. Change the owner and group owner of the file to root with `sudo chown root pam_qr.so` & `sudo chgrp root pam_qr.so`.
3. Change the permissions to read & write for the owner and read for the group and everyone else with `sudo chmod 644 pam_qr.so`.

## Usage with SSH

1. Open sshd_config (`/etc/ssh/sshd_config` in Ubuntu).
2. Set `PasswordAuthentication no`.
3. Set `ChallengeResponseAuthentication yes`.
4. Set `UsePAM yes`.
5. Save and exit sshd_config.
6. Open sshd PAM config (`/etc/pam.d/sshd` in Ubuntu).
7. Add pam-qr to the PAM chain by adding the line following line above the standard Un*x authentication include. `auth sufficient pam_qr.so <computer id> <auth-server URL>` subbing in the computer ID which identifies it to the auth server and the url (including http:\\\\ or https:\\\\) of the auth server.
8. Save and exit the config file.
9. Restart sshd with `sudo systemctl restart sshd` for systems using systemd.
10. Add user accounts for those whom will have authorisation to logon to the computer.