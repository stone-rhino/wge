# WGE
## What is WGE?
WGE is a high-performance web application firewall (WAF) library based on C++. It has been successfully applied in the commercial product Stone Rhino - Web Governance Engine (SR-WGE). It is designed to be compatible with the OWASP Core Rule Set (CRS) and can be used as a drop-in replacement for ModSecurity. The library is built using C++23 and is designed to be fast, efficient, and easy to use.

## Performance Comparison
CPU: Intel(R) Core(TM) i5-10400 CPU @ 2.90GHz   2.90 GHz  
RAM: 32GB  
OS: Ubuntu 20.04.6 LTS (5.15.153.1-microsoft-standard-WSL2)  
Worker Thread: 8  
Input: [White requests](benchmarks/test_data/white.data) and [Black requests](benchmarks/test_data/black.data)
| Test Case         | Enable Memory Pool(TCMalloc) |ModSecurity |    WGE     |
|-------------------|------------------------------|------------|------------|
| CRS v4.3.0        |         No                   | 4010 QPS   | 17560 QPS  |
| CRS v4.3.0        |         Yes                  | 4927 QPS   | 18864 QPS  |  


The benchmark results show that WGE is significantly faster than ModSecurity, with a performance improvement of over 4 times. This is due to the use of modern C++ features and excellent architecture design and implementation. The library is designed to be easy to use and integrate into existing applications, making it a great choice for developers looking to add WAF functionality to their projects.

## Quick Start
### Prerequisites
* CMake 3.28 or higher https://cmake.org/download/
* vcpkg with cmake installed https://github.com/microsoft/vcpkg
* C++23 compatible compiler (GCC 13.1 or higher) https://gcc.gnu.org/
* Ragle 6.10
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
* Configure the environment variables. Add the following to `/etc/profile`:
```shell
export CLASSPATH=".:/usr/local/lib/antlr-4.13.2-complete.jar:$CLASSPATH"
alias antlr4='java -Xmx500M -cp "/usr/local/lib/antlr-4.13.2-complete.jar:$CLASSPATH" org.antlr.v4.Tool'
alias grun='java -Xmx500M -cp "/usr/local/lib/antlr-4.13.2-complete.jar:$CLASSPATH" org.antlr.v4.gui.TestRig'
```
### Build
* Update the submodule
```shell
git submodule update --init
```
* Configure the cmake
```shell
cmake --preset=release-with-debug-info --fresh
```
If the gcc path is not in the default path, we can rename the file `CMakeUserPresets.json.example` to `CMakeUserPresets.json` and modify the gcc path:
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
Then we can run the cmake command:
```shell
cmake --preset=my-release-with-debug-info --fresh
```
If we want to enable the debug log that help us to watch the process of WGE, we can set the `WGE_LOG_ACTIVE_LEVEL` to 1.
```shell
cmake --preset=release-with-debug-info --fresh -DWGE_LOG_ACTIVE_LEVEL=1
```
The WGE_LOG_ACTIVE_LEVEL is a compile-time option that controls the log level:  
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
### Run Unit Tests
```shell
./build/release-with-debug-info/test/test
```
### Run Benchmark
```shell
./build/release-with-debug-info/benchmarks/wge/wge_benchmark
```
### Integrate Into Existing Projects
* Install WGE
```shell
cmake --install build/release-with-debug-info
```
After installation, the WGE library and header files will be available in the system include and library paths. We also can install the WGE into another path by specifying the `--prefix` option. For example, to install WGE into `/specified/path`, we can run:
```shell
cmake --install build/release-with-debug-info --prefix /specified/path
```
* Include WGE in existing projects
```cpp
#include <wge/engine.h>
```
* Link WGE in existing projects
```cmake
# If the WGE installed in the system path
target_link_libraries(your_target_name PRIVATE wge)
# If the WGE installed in the specified path
target_link_libraries(your_target_name PRIVATE /specified/path/lib/libwge.a)
```
* Use WGE in existing projects
1. Construct a WGE engine in the main thread
```cpp
Wge::Engine engine(spdlog::level::off);
```
2. Load the rules in the main thread
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
4. Create a transaction when each request comes in the worker thread
```cpp
// Each request has its own transaction
Wge::TransactionPtr t = engine.makeTransaction();
```
5. Process the request in the worker thread
```cpp
// Process each transaction is following the flowing steps
// 1. Process the connection
t->processConnection(/*params*/);
// 2. Process the URI
t->processUri(/*params*/);
// 3. Process the request headers
t->processRequestHeaders(/*params*/);
// 4. Process the request body
t->processRequestBody(/*params*/);
// 5. Process the response headers
t->processResponseHeaders(/*params*/);
// 6. Process the response body
t->processResponseBody(/*params*/);
```

Refer to the [wge_benchmark](benchmarks/wge/main.cpp) for usage examples.

## License
Copyright (c) 2024-2025 Stone Rhino and contributors.
The WGE is distributed under MIT. Please see the enclosed [LICENSE](LICENSE) file for full details.

## Documentation

## Contribute

## Contact
[Stone Rhino](https://www.srhino.com/)
