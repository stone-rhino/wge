# WGE
<p align="center">
    <img src="https://avatars.githubusercontent.com/u/215498892?s=400&u=8e3433ea2368169c950ca497194776511c8a1837&v=4" width=200 height=200/>
</p>

English | [简体中文](README_ZH.md)

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

## Features
* Fully compatible with the OWASP Core Rule Set (CRS). Supports all variables, operators, and actions to ensure compatibility with existing rule sets. Effectively detects and defends against all attack types supported by CRS, including SQL injection (SQLI), cross-site scripting (XSS), local file inclusion (LFI), and remote command execution (RCE).
* High performance. Uses Antlr4 to implement a high-performance front-end parser, a carefully designed efficient C++ object graph as the back-end interpreter, and integrates the TCMalloc memory pool to significantly improve throughput and reduce memory fragmentation.
* Rich decoding support. Uses Ragel to implement a high-performance DFA codec engine, supporting 22 common decoding formats, including URL decoding, HTML entity decoding, JavaScript decoding, CSS decoding, etc., ensuring that attack payloads cannot bypass detection through encoding transformations.
* Multi-threading support. Lock-free design supports efficient operation in multi-threaded environments, suitable for high-concurrency scenarios.
* SecLang++. Derived from SecLang, SecLang++ introduces various practical language features such as rule fragments, configuration-driven rules, conditional actions, aliases, and reference variables, enhancing the flexibility and maintainability of rule writing.

## Future Plans
* Continuously evolve SecLang++, introducing more modern programming language features while maintaining the simplicity and ease of use of domain-specific languages, improving the expressiveness and maintainability of rule writing.
* Add support for streaming processing, allowing large request/response bodies to be processed without loading the entire content into memory, reducing memory usage and preventing large request body attacks.
* Add a syntax and semantic engine to reduce false positives and improve detection of complex attacks.
* Introduce machine learning-assisted detection to enhance the identification of unknown attacks.
* Add a multi-pattern matching detection engine to further improve rule execution efficiency.
* Use bytecode + JIT technology to maximize rule execution efficiency.
* Expand more connectors to demonstrate seamless integration with more web servers and frameworks.

## Quick Start
### Prerequisites
* CMake 3.28 or higher https://cmake.org/download/
* vcpkg with cmake installed https://github.com/microsoft/vcpkg
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
If the compiler path is not in the default path, we can copy the `CMakeUserPresets.json.example` to `CMakeUserPresets.json` and modify the compiler path in the `CMakeUserPresets.json` file:
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
The `WGE_LOG_ACTIVE_LEVEL` is a compile-time option that controls the log level:  
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

Refer to the [wge_benchmark](benchmarks/wge/main.cc) for usage examples.

## License
Copyright (c) 2024-2026 Stone Rhino and contributors.
The WGE is distributed under MIT. Please see the enclosed [LICENSE](LICENSE) file for full details.

## Documentation
- Documentation Site: [https://stone-rhino.github.io/wge](https://stone-rhino.github.io/wge/) We are working on the documentation for WGE. In the meantime, please refer to the source code and examples in the `WGE-Connectors` project for guidance on how to use the library.
- [WGE-Connectors](https://github.com/stone-rhino/wge-connectors): A collection of connectors for integrating WGE with various web servers and frameworks.

## Contributing
We welcome contributions to WGE! If you have any ideas, suggestions, or bug reports, please open an issue or submit a pull request on GitHub. Before contributing, please read our [CONTRIBUTING](CONTRIBUTING.md) guidelines.

## Contact
[Stone Rhino](https://www.srhino.com/)
