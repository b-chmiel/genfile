SHELL = /bin/sh

BUILD_DIR_RELEASE = build_release
BUILD_DIR_DEBUG = build
TARGET = $(BUILD_DIR_DEBUG)/src/genfile
ARGP_PATH=$(shell pwd)/external/argp-standalone
ARGP_LIB=$(ARGP_PATH)/libargp.a

all: $(TARGET)

$(TARGET): FORCE $(BUILD_DIR_DEBUG)
	$(MAKE) -C $(BUILD_DIR_DEBUG)

FORCE: ;

configure:
	autoreconf --install

dist: configure
	-mkdir -v $(BUILD_DIR_RELEASE)
	cd $(BUILD_DIR_RELEASE) && ../configure CFLAGS='-Ofast -Wall -Wextra -pedantic'
	$(MAKE) -C $(BUILD_DIR_RELEASE) dist

$(BUILD_DIR_DEBUG): configure $(ARGP_LIB)
	-mkdir -v $(BUILD_DIR_DEBUG)
	cd $(BUILD_DIR_DEBUG) && ../configure \
		LDFLAGS='-L $(ARGP_PATH)' \
		CFLAGS='-O0 -g -Wall -Wextra -pedantic -I $(ARGP_PATH)' \
		--with-argp-standalone=yes

$(ARGP_LIB):
	cd $(ARGP_PATH) && \
		autoreconf --install && \
		./configure && \
		$(MAKE)


env: env/touchfile

env/touchfile: requirements.txt
	test -d env || python -m venv env
	. env/bin/activate && pip install -r requirements.txt
	touch env/touchfile

test: env $(TARGET)
	. env/bin/activate && pytest test/test.py

clean:
	-rm -rfv $(BUILD_DIR_DEBUG)
	-rm -rfv $(BUILD_DIR_RELEASE)
	-rm -rfv autom4te.cache
	-rm -rfv m4
	-rm -fv missing install-sh depcomp configure config.h.in aclocal.m4 compile configure~ ltmain.sh config.sub config.guess
	-find . -name "Makefile.in" -type f -delete
