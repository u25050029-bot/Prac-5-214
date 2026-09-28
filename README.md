# CampusGuard

## Prerequisites

* Docker Engine 20.10 or newer (Docker Desktop on Windows or macOS)

Verify it is available:

```
docker --version
```

## 1. Build the Docker image

Run this from the project root, the directory containing the `Dockerfile`:

```
docker build -t campusguard .
```

This installs the toolchain, copies the source into `/campusguard` inside the image and compiles the project. If compilation fails, the image build fails, so a successful `docker build` is itself proof that the project compiles cleanly.

To force a completely fresh build with no cached layers:

```
docker build --no-cache -t campusguard .
```

With Docker Compose instead:

```
docker compose build
```

## 2. Run the program

```
docker run --rm -it campusguard
```

To run it through the Makefile target instead:

```
docker run --rm -it campusguard make run
```

With Docker Compose:

```
docker compose run --rm campusguard
```

## 3. Debug with GDB

```
docker run --rm -it \
  --cap-add=SYS_PTRACE \
  --security-opt seccomp=unconfined \
  campusguard gdb ./campusguard
```

With Docker Compose, which already grants the ptrace capability:

```
docker compose run --rm campusguard gdb ./campusguard
```

The binary is compiled with `-g -O0`, so full symbol and line information is available and no statements are optimised away.

A typical GDB session:

```
(gdb) break main
(gdb) run
(gdb) next
(gdb) step
(gdb) print variableName
(gdb) backtrace
(gdb) info locals
(gdb) continue
(gdb) quit
```

To break on a specific source location:

```
(gdb) break EmergencyCoordinator.cpp:42
```

## 4. Check for memory errors with Valgrind

```
docker run --rm -it campusguard \
  valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./campusguard
```

Single line version:

```
docker run --rm -it campusguard valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./campusguard
```

To write the report to a file inside a mounted directory on the host:

```
docker run --rm -it -v "$PWD":/campusguard campusguard \
  bash -c "make CXXFLAGS='-std=c++11 -Wall -Wextra -pedantic -g -O0' && valgrind --leak-check=full --show-leak-kinds=all --log-file=valgrind.log ./campusguard"
```

The report then appears as `valgrind.log` in the project directory on the host.

## 5. Interactive shell inside the container

For exploring the container or running several commands in one session:

```
docker run --rm -it --cap-add=SYS_PTRACE --security-opt seccomp=unconfined campusguard bash
```

Inside the shell:

```
make
./campusguard
gdb ./campusguard
valgrind --leak-check=full ./campusguard
```
