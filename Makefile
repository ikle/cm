AR ?= ar
RANLIB ?= ranlib

TARGETS = libcm.a
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

libcm.a: $(OBJECTS)
	$(AR) rc $@ $^
	$(RANLIB) $@

$(TESTS): libcm.a

test: $(TESTS)
	make -C test
