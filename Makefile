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
	-mkdir -pv $(BUILD_DIR_RELEASE)
	cd $(BUILD_DIR_RELEASE) && ../configure CFLAGS='-Ofast -Wall -Wextra -pedantic'
	$(MAKE) -C $(BUILD_DIR_RELEASE) dist
	$(MAKE) -C $(BUILD_DIR_RELEASE)

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


IMAGE_NAME=genfile-dev
CONTAINER_NAME=genfile-dev-container

define run_in_docker
	docker build -t $(IMAGE_NAME) -f Dockerfile.debian .
	- docker run -v $(shell pwd):/app --rm --name $(CONTAINER_NAME) -d -it $(IMAGE_NAME) bash
	docker exec $(CONTAINER_NAME) $(1)
	docker kill $(CONTAINER_NAME)
	sudo chown $(USER):$(USER) -R .
endef

deb: dist
	mkdir -pv $(BUILD_DIR_RELEASE)/genfile/usr/bin
	cp -R DEBIAN $(BUILD_DIR_RELEASE)/genfile/
	cp $(BUILD_DIR_RELEASE)/src/genfile $(BUILD_DIR_RELEASE)/genfile/usr/bin
	dpkg-deb --build $(BUILD_DIR_RELEASE)/genfile

deb-docker:
	$(call run_in_docker, make deb)

clean:
	-rm -rfv $(BUILD_DIR_DEBUG)
	-rm -rfv $(BUILD_DIR_RELEASE)
	-rm -rfv autom4te.cache
	-rm -rfv m4
	-rm -fv missing install-sh depcomp configure config.h.in aclocal.m4 compile configure~ ltmain.sh config.sub config.guess
	-find . -name "Makefile.in" -type f -delete
