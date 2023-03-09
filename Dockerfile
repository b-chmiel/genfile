FROM alpine as build-env
RUN apk add --no-cache build-base git autoconf automake
RUN apk add --no-cache argp-standalone
WORKDIR /app
COPY . .
RUN make

FROM alpine
COPY --from=build-env /app/build/src/gen_file /app/build/src/gen_file
WORKDIR /app
CMD ["/app/build/src/gen_file", "--version"]