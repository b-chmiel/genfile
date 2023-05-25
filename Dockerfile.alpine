FROM alpine as dist-env
RUN apk add --no-cache gcc make autoconf automake libtool musl-dev
WORKDIR /app
COPY . .
RUN make dist
RUN mv build/genfile* genfile.tar.gz

FROM alpine as build-env
COPY --from=dist-env /app/genfile.tar.gz /app/genfile.tar.gz
RUN apk add --no-cache gcc make musl-dev
RUN apk add --no-cache argp-standalone
WORKDIR /app
RUN tar xf genfile.tar.gz && mv genfile-* genfile
WORKDIR /app/genfile
RUN ./configure --prefix=/app/install --with-argp-standalone
RUN make
RUN make install

FROM alpine
COPY --from=build-env /app/install/bin/genfile /app/genfile
CMD ["/app/genfile"]