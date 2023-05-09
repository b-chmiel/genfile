SHELL = /bin/sh

BUILD_DIR_RELEASE = build
TARGET_RELEASE = $(BUILD_DIR_RELEASE)/src/gen_file
CC=../external/musl/install/bin/musl-gcc

all: $(TARGET_RELEASE)

$(TARGET_RELEASE): FORCE $(BUILD_DIR_RELEASE)
	$(MAKE) -C $(BUILD_DIR_RELEASE)

FORCE: ;

configure:
	autoreconf --install

dist: $(BUILD_DIR_RELEASE)
	$(MAKE) -C $(BUILD_DIR_RELEASE) dist

$(BUILD_DIR_RELEASE): configure
	-mkdir -v $(BUILD_DIR_RELEASE)
	cd $(BUILD_DIR_RELEASE) && ../configure CFLAGS='-Ofast -g -Wall -Wextra -pedantic'

env: env/touchfile

env/touchfile: requirements.txt
	test -d env || python -m venv env
	. env/bin/activate && pip install -r requirements.txt
	touch env/touchfile

test: env $(TARGET_RELEASE)
	. env/bin/activate && pytest test/test.py

clean:
	-rm -rfv $(BUILD_DIR_RELEASE)
	-rm -rfv autom4te.cache
	-rm -rfv m4
	-rm -fv missing install-sh depcomp configure config.h.in aclocal.m4 compile configure~ ltmain.sh config.sub config.guess
	-find . -name "Makefile.in" -type f -delete
