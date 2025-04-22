# SrSecurity
## What is SrSecurity?
SrSecurity is a high-performance web application firewall (WAF) library based on C++. It is designed to be compatible with the OWASP Core Rule Set (CRS) and can be used as a drop-in replacement for ModSecurity. The library is built using C++23 and is designed to be fast, efficient, and easy to use.
## Performance Comparison
CPU: Intel(R) Core(TM) i5-10400 CPU @ 2.90GHz   2.90 GHz  
RAM: 32GB  
OS: Ubuntu 20.04.6 LTS (5.15.153.1-microsoft-standard-WSL2)  
Worker Thread: 8  
Input: [White requests](benchmarks/test_data/white.data) and [Black requests](benchmarks/test_data/black.data)
| Test Case         | Enable Memory Pool(TCMalloc) |ModSecurity | SrSecurity |
|-------------------|------------------------------|------------|------------|
| CRS v4.3.0        |         No                   | 4010 QPS   | 17560 QPS  |
| CRS v4.3.0        |         Yes                  | 4927 QPS   | 18864 QPS  |  


The benchmark results show that SrSecurity is significantly faster than ModSecurity, with a performance improvement of over 4 times. This is due to the use of modern C++ features and excellent architecture design and implementation. The library is designed to be easy to use and integrate into existing applications, making it a great choice for developers looking to add WAF functionality to their projects.

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
cmake --preset=release-with-debug-info
```
* Build with cmake
```shell
cmake --build build/release-with-debug-info
```

### Run Unit Tests

### Run Benchmark

## License
Copyright (c) 2024-2025 Stone Rhino and contributors.
The SrSecurity is distributed under MIT. Please see the enclosed [LICENSE](LICENSE) file for full details.

## Documentation

## Contribute

## Contact
[Stone Rhino](https://www.srhino.com/)
