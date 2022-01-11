CC=gcc
CFLAGS=-g -Wall -Wextra

pam_qr: src/pam_qr.c src/qr_to_string.c
	$(CC) $(CFLAGS) -fPIC -fstack-protector -shared -o pam_qr.so src/pam_qr.c src/qr_to_string.c -lcurl -lqrencode -ljson-c

test_qr_to_string: src/test/test_qr_to_string.c src/qr_to_string.c
	$(CC) $(CFLAGS) -o printqr src/test/test_qr_to_string.c src/qr_to_string.c -lqrencode

test_api: src/test/test_api.c src/expandable_string.c
	$(CC) $(CFLAGS) -o test_api src/test/test_api.c src/expandable_string.c -lcurl -ljson-c

clean:
	rm pam_qr.so
	rm test_api
	rm printqr