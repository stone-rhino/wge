+++
title = "Quick Start"
weight = 1
+++

This section explains how to build WGE from source code and integrate it into existing projects. We assume you are an experienced C++ developer with a properly configured C++ development environment. If you are a beginner, don't worry - we provide detailed instructions in the [Building](building.md) chapter that cover development environment configuration and dependency installation. So it's not as complex as you might think. Follow our steps and you'll be able to successfully build WGE, integrate it into your project, and even start contributing code to WGE!

### Dependencies

* CMake 3.28 or higher https://cmake.org/download/
* vcpkg (installed via cmake) https://github.com/microsoft/vcpkg
* C++23 compatible compiler (GCC 13.1 or higher) https://gcc.gnu.org/
* Ragel 6.10
```shell
apt install ragel
```
* JDK 21 or higher
```shell
apt install openjdk-21-jdk-headless
```
* ANTLR4 4.13.2 or higher
```shell
cd /usr/local/lib
curl -O https://www.antlr.org/download/antlr-4.13.2-complete.jar
```
* Configure environment variables. Add the following to `/etc/profile`:
```shell
export CLASSPATH=".:/usr/local/lib/antlr-4.13.2-complete.jar:$CLASSPATH"
alias antlr4='java -Xmx500M -cp "/usr/local/lib/antlr-4.13.2-complete.jar:$CLASSPATH" org.antlr.v4.Tool'
alias grun='java -Xmx500M -cp "/usr/local/lib/antlr-4.13.2-complete.jar:$CLASSPATH" org.antlr.v4.gui.TestRig'
```

### Building

* Update submodules
```shell
git submodule update --init
```
* Configure cmake
```shell
cmake --preset=release-with-debug-info --fresh
```
If the compiler path is not in the default location, you can copy `CMakeUserPresets.json.example` to `CMakeUserPresets.json` and modify the compiler paths in that file. For example:
```json
{
    "name": "my-release-with-debug-info",
    "inherits": "release-with-debug-info",
    "environment": {
    "CC": "/usr/local/bin/gcc",
    "CXX": "/usr/local/bin/g++",
    "LD_LIBRARY_PATH": "/usr/local/lib64"
    }
}
```
Then run the cmake command:
```shell
cmake --preset=my-release-with-debug-info --fresh
```
If you want to enable debug logging to observe WGE's processing flow, you can set `WGE_LOG_ACTIVE_LEVEL` to 1.
```shell
cmake --preset=release-with-debug-info --fresh -DWGE_LOG_ACTIVE_LEVEL=1
```
`WGE_LOG_ACTIVE_LEVEL` is a compile-time option. This macro controls the log level:

1: Trace
2: Debug
3: Info
4: Warn
5: Error
6: Critical
7: Off

* Build with cmake
```shell
cmake --build build/release-with-debug-info
```

### Running Unit Tests
```shell
./build/release-with-debug-info/test/test
```

### Running Benchmark Tests
```shell
./build/release-with-debug-info/benchmarks/wge/wge_benchmark
```

### Integrating into Existing Projects

* Install WGE
```shell
cmake --install build/release-with-debug-info
```
After installation, the WGE library and header files will be available in the system's include and library paths. You can also specify a custom installation path using the `--prefix` option. For example, to install WGE to `/specified/path`:
```shell
cmake --install build/release-with-debug-info --prefix /specified/path
```

* Include WGE headers in your project
```cpp
#include <wge/engine.h>
```

* Link WGE in your project
```cmake
# If WGE is installed in system paths
target_link_libraries(your_target_name PRIVATE wge)
# If WGE is installed in a custom path
target_link_libraries(your_target_name PRIVATE /specified/path/lib/libwge.a)
```

* Using WGE in your project

1. Create the WGE engine instance in the main thread
```cpp
Wge::Engine engine(spdlog::level::off);
```

2. Load rule files in the main thread
```cpp
std::expected<bool, std::string> result = engine.loadFromFile(rule_file);
if (!result.has_value()) {
  // Handle the error
  std::cout << "Load rules error: " << result.error() << std::endl;
}
```

3. Initialize the engine in the main thread
```cpp
engine.init();
```

4. Create a transaction for each request in worker threads
```cpp
// Each request has its own transaction
Wge::TransactionPtr t = engine.makeTransaction();
```

5. Process requests in worker threads
```cpp
// Steps for processing each transaction
// 1. Process connection
t->processConnection(/*params*/);
// 2. Process URI
t->processUri(/*params*/);
// 3. Process request headers
t->processRequestHeaders(/*params*/);
// 4. Process request body
t->processRequestBody(/*params*/);
// 5. Process response headers
t->processResponseHeaders(/*params*/);
// 6. Process response body
t->processResponseBody(/*params*/);
```
