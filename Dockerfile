FROM alpine:3.19 AS builder

RUN apk add --no-cache cmake clang make libc-dev

WORKDIR /build

COPY . .

RUN mkdir -p build && \
    cd build && \
    cmake .. && \
    make

FROM alpine:3.19

RUN apk add --no-cache ca-certificates

WORKDIR /app

COPY --from=builder /build/build/app/http-server /app/
COPY --from=builder /build/test.html /app/

EXPOSE 8080

CMD ["/app/http-server"]
