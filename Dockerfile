FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install -y --no-install-recommends build-essential make gdb valgrind \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /campusguard

COPY . .

RUN make clean && make CXXFLAGS="-std=c++11 -Wall -Wextra -pedantic -g -O0"

CMD ["./campusguard"]
