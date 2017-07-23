AR ?= ar
RANLIB ?= ranlib

LIBNAME = libcm
LIBVER  = 0
LIBREV  = 0.1

AFILE  = $(LIBNAME).a
SONAME = $(LIBNAME).so.$(LIBVER)
SOFILE = $(SONAME).$(LIBREV)

TARGETS = $(AFILE) $(SOFILE)
CFLAGS = -Wall -O6 -I"$(CURDIR)"/include

SOURCES = *.c
OBJECTS = $(patsubst %.c,%.o, $(wildcard $(SOURCES)))
TESTS = $(patsubst %.c,%, $(wildcard test/*.c))

all: $(TARGETS)

.PHONY: clean install test

clean:
	rm -f *.o $(OBJECTS) $(TESTS) $(TARGETS)

PREFIX ?= /usr/local

install: $(TARGETS)
	install -D -d $(DESTDIR)/$(PREFIX)/bin
	install -s -m 0755 $^ $(DESTDIR)/$(PREFIX)/bin

$(AFILE): $(OBJECTS)
	$(AR) rc $@ $^
	$(RANLIB) $@

$(OBJECTS): CFLAGS += -fPIC

$(SOFILE): LDFLAGS += -shared
$(SOFILE): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(TESTS): $(AFILE)

test: $(TESTS)
	make -C test
