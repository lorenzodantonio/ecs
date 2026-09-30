FROM gcc:latest AS core_builder
RUN apt-get update && apt-get install -y --no-install-recommends \
    cmake \
    lcov \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src/ecs
COPY . .

# 1. Out-of-source build
RUN cmake -B build -S .
# 2. target build (lib, demo, test)
RUN cmake --build build
# 3. Run CTest (docker build fails if the test fails)
RUN ["ctest", "--test-dir", "build", "--output-on-failure"]