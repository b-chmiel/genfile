FROM alpine as dist-env
RUN apk add --no-cache gcc make autoconf automake libtool musl-dev
WORKDIR /app
COPY . .
RUN make dist
RUN mv build/gen_file* gen_file.tar.gz

FROM alpine as build-env
COPY --from=dist-env /app/gen_file.tar.gz /app/gen_file.tar.gz
RUN apk add --no-cache gcc make musl-dev
RUN apk add --no-cache argp-standalone
WORKDIR /app
RUN tar xf gen_file.tar.gz && mv gen_file-* gen_file
WORKDIR /app/gen_file
RUN ./configure --prefix=/app/install --with-argp-standalone
RUN make
RUN make install

FROM alpine
COPY --from=build-env /app/install/bin/gen_file /app/gen_file
CMD ["/app/gen_file"]