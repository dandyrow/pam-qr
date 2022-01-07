CC=gcc
CFLAGS=-g -Wall -Wextra

pam_qr: src/pam_qr.c src/printqr.c
	$(CC) $(CFLAGS) -fPIC -fstack-protector -shared -o pam_qr.so src/pam_qr.c src/printqr.c -lcurl -lqrencode

printqr: src/printqr.c
	$(CC) $(CFLAGS) -o printqr src/printqr.c -lqrencode

test_qr_to_string: src/test/test_qr_to_string.c src/qr_to_string.c
	$(CC) $(CFLAGS) -o printqr src/test/test_qr_to_string.c src/qr_to_string.c -lqrencode

test_api: src/test/test_api.c
	$(CC) $(CFLAGS) -o test_api src/test/test_api.c -lcurl -ljson-c

clean:
	rm pam_qr.so
	rm test_api
	rm printqr