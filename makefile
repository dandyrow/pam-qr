CC=gcc
CFLAGS=-g -Wall -Wextra

pam_qr: src/pam_qr.c
	$(CC) $(CFLAGS) -fPIC -fstack-protector -shared -o pam_qr.so src/pam_qr.c -lcurl -ljson-c -lqrencode

test_api: src/test/test_api.c
	$(CC) $(CFLAGS) -o test_api src/test/test_api.c -lcurl -ljson

clean:
	rm pam_qr.so