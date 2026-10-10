FROM gcc:14 AS test

RUN apt-get update && apt-get install -y --no-install-recommends cmake \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src/ecs
COPY . .

RUN cmake -B build -S . -DECS_SANITIZE=ON -DECS_WERROR=ON
RUN cmake --build build
RUN ["ctest", "--test-dir", "build", "--output-on-failure"]

FROM test AS coverage

RUN apt-get update && apt-get install -y --no-install-recommends gcovr \
 && rm -rf /var/lib/apt/lists/*
RUN cmake -B build-coverage -S . -DECS_COVERAGE=ON
RUN cmake --build build-coverage --target coverage