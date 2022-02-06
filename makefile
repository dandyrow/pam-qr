CC=gcc
CFLAGS=-g -Wall -Wextra
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

pam_qr: CFLAGS+=-fPIC -fstack-protector -shared
pam_qr: $(OBJS) $(LIBDIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(LIBDIR)/$@.so $(LDLIBS)

expandable_string.test: $(TESTDIR)/expandable_string.test.c $(OBJDIR)/expandable_string.o $(TESTBINDIR)
	$(CC) $(CFLAGS) $< $(OBJDIR)/expandable_string.o -o $(TESTBINDIR)/$@ -lcriterion

clean:
	rm -ir $(OBJDIR)/*.o $(LIBDIR)/*.so
