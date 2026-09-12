FROM gcc:latest AS core_builder
RUN apt-get update && apt-get install -y --no-install-recommends \
    cmake \
    lcov \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src/ecs
COPY . .

# Out-of-source build
# 1. Configurazione out-of-source
RUN cmake -B build -S .
# 2. Compilazione effettiva di tutti i target (lib, demo, test)
RUN cmake --build build
# 3. Esecuzione suite CTest (il build Docker fallisce se un test fallisce)

CMD ["ctest", "--test-dir", "build", "--output-on-failure"]