#FROM gcc:latest AS core_builder
#RUN apt-get update && apt-get install -y cmake lcov
#COPY . /usr/src/ecs
#WORKDIR /usr/src/ecs
## Generate Makefile using CMake
#RUN cmake .
#
#FROM core_builder AS builder
#RUN make ecs
#
#FROM core_builder AS tester
#CMD make test_runner && ./test_runner
#
#FROM debian:bookworm-slim
#WORKDIR /root/
#COPY --from=builder /usr/src/ecs/ecs .
#CMD ["./ecs"]

FROM gcc:latest AS core_builder
RUN apt-get update && apt-get install -y --no-install-recommends \
    cmake \
    lcov \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src/ecs
COPY . .

# Out-of-source build
RUN cmake -B build -S .

# 2. tests (the build fails if tests are not passing)
FROM core_builder AS tester
RUN cmake --build build --target test_runner
WORKDIR /usr/src/ecs/build
RUN ./test_runner

# 3. build library and demo
FROM core_builder AS builder
RUN cmake --build build --target ecs ecs_demo

# 4. Final image (demo)
FROM debian:bookworm-slim
WORKDIR /app
COPY --from=builder /usr/src/ecs/build/ecs_demo .
CMD ["./ecs_demo"]