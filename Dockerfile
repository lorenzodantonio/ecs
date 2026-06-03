FROM gcc:latest AS core_builder
RUN apt-get update && apt-get install -y cmake lcov
COPY . /usr/src/storage
WORKDIR /usr/src/storage
# Generate Makefile using CMake
RUN cmake .

FROM core_builder AS builder
RUN make storage

FROM core_builder AS tester
CMD make test_runner && ./test_runner

FROM debian:bookworm-slim
WORKDIR /root/
COPY --from=builder /usr/src/storage/storage .
CMD ["./storage"]
