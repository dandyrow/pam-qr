CC=gcc
CFLAGS=-g -Wall -Wextra -fPIC -fstack-protector
LDLIBS=-lqrencode -lcurl -ljson-c

# Directories
SRCDIR=src
OBJDIR=obj
LIBDIR=lib
TESTDIR=test
TESTBINDIR=$(TESTDIR)/bin
BINDIR=bin

# mkdir rules to make sure they exist
$(OBJDIR):
	mkdir $@

$(LIBDIR):
	mkdir $@

$(TESTBINDIR):
	mkdir $@

# List of src and obj files
SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

# Compile x.c to x.o without linking
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@ 

pam_qr: CFLAGS+= -shared
pam_qr: $(OBJS) $(LIBDIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(LIBDIR)/$@.so $(LDLIBS)

qr_to_string: CFLAGS=-g -Wall -Wextra
qr_to_string:
	$(CC) $(CFLAGS) $(SRCDIR)/qr_to_string.c $(SRCDIR)/test/test_qr_to_string.c -o test/bin/$@.test -lqrencode

expandable_string.test: $(TESTDIR)/expandable_string.test.c $(OBJDIR)/expandable_string.o $(TESTBINDIR)
	$(CC) $(CFLAGS) $< $(OBJDIR)/expandable_string.o -o $(TESTBINDIR)/$@ -lcriterion

clean:
	rm -ir $(OBJDIR)/*.o $(LIBDIR)/*.so
