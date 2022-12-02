SHELL = /bin/sh

BUILD_DIR_RELEASE = build
TARGET_RELEASE = $(BUILD_DIR_RELEASE)/src/gen_file

all: $(TARGET_RELEASE)

$(TARGET_RELEASE): $(BUILD_DIR_RELEASE) src/main.c
	$(MAKE) -C $(BUILD_DIR_RELEASE)

configure:
	autoreconf --install

dist: $(BUILD_DIR_RELEASE)
	$(MAKE) -C $(BUILD_DIR_RELEASE) dist

$(BUILD_DIR_RELEASE): configure
	-mkdir -v $(BUILD_DIR_RELEASE)
	cd $(BUILD_DIR_RELEASE) && ../configure CFLAGS='-O2'

clean:
	-rm -rfv $(BUILD_DIR_DEBUG)
	-rm -rfv $(BUILD_DIR_RELEASE)
	-rm -rfv autom4te.cache
	-rm -fv missing install-sh depcomp configure config.h.in aclocal.m4 compile
	-find . -name "Makefile.in" -type f -delete