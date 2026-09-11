FROM gcc:latest AS core_builder
RUN apt-get update && apt-get install -y cmake lcov
COPY . /usr/src/ecs
WORKDIR /usr/src/ecs
# Generate Makefile using CMake
RUN cmake .

FROM core_builder AS builder
RUN make ecs

FROM core_builder AS tester
CMD make test_runner && ./test_runner

FROM debian:bookworm-slim
WORKDIR /root/
COPY --from=builder /usr/src/ecs/ecs .
CMD ["./ecs"]
