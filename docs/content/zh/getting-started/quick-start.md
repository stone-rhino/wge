+++
title = "快速开始"
weight = 1
+++

这里介绍如何从源码构建 WGE，并将其集成到现有项目中。当然，这里假设您是一名经验丰富的 C++ 开发者，并且已经配置好了适合 C++ 开发的环境。如果您是一位初学者，也请不要担心，我们在[构建](building/_index.md)一章中详细介绍了如何构建 WGE，包括开发环境的配置和依赖项的安装。所以，没有您想象中的那么复杂，跟着我们的步骤走，您一定能顺利完成构建，并将 WGE 集成到您的项目中，甚至开始为 WGE 贡献代码！
### 依赖环境
* CMake 3.28 或更高版本 https://cmake.org/download/
* vcpkg (使用 cmake 安装) https://github.com/microsoft/vcpkg
* 支持 C++23 的编译器 (GCC 13.1 或更高版本) https://gcc.gnu.org/
* Ragel 6.10
```shell
apt install ragel
```
* JDK 21 或更高版本
```shell
apt install openjdk-21-jdk-headless
```
* ANTLR4 4.13.2 或更高版本
```shell
cd /usr/local/lib
curl -O https://www.antlr.org/download/antlr-4.13.2-complete.jar
```
* 配置环境变量。将以下内容添加到 `/etc/profile`：
```shell
export CLASSPATH=".:/usr/local/lib/antlr-4.13.2-complete.jar:$CLASSPATH"
alias antlr4='java -Xmx500M -cp "/usr/local/lib/antlr-4.13.2-complete.jar:$CLASSPATH" org.antlr.v4.Tool'
alias grun='java -Xmx500M -cp "/usr/local/lib/antlr-4.13.2-complete.jar:$CLASSPATH" org.antlr.v4.gui.TestRig'
```
### 构建
* 更新子模块
```shell
git submodule update --init
```
* 配置 cmake
```shell
cmake --preset=release-with-debug-info --fresh
```
如果编译器路径不在默认路径中，我们可以将 `CMakeUserPresets.json.example` 复制为 `CMakeUserPresets.json`，并在该文件中修改编译器路径，例如：
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
然后我们可以运行 cmake 命令：
```shell
cmake --preset=my-release-with-debug-info --fresh
```
如果我们想启用调试日志以便观察 WGE 的处理过程，可以将 `WGE_LOG_ACTIVE_LEVEL` 设置为 1。
```shell
cmake --preset=release-with-debug-info --fresh -DWGE_LOG_ACTIVE_LEVEL=1
```
`WGE_LOG_ACTIVE_LEVEL` 是一个编译期的选项，该宏用于控制日志级别：

1: Trace  
2: Debug  
3: Info  
4: Warn  
5: Error  
6: Critical  
7: Off 
* 使用 cmake 构建
```shell
cmake --build build/release-with-debug-info
```
### 运行单元测试
```shell
./build/release-with-debug-info/test/test
```
### 运行基准性能测试
```shell
./build/release-with-debug-info/benchmarks/wge/wge_benchmark
```
### 集成到现有项目
* 安装 WGE
```shell
cmake --install build/release-with-debug-info
```
安装完成后，WGE 库和头文件将可在系统的包含路径和库路径中使用。我们也可以通过指定 `--prefix` 选项将 WGE 安装到其他路径。例如，要将 WGE 安装到 `/specified/path`，我们可以运行：
```shell
cmake --install build/release-with-debug-info --prefix /specified/path
```
* 在现有项目中包含 WGE 的头文件
```cpp
#include <wge/engine.h>
```
* 在现有项目中链接 WGE
```cmake
# 如果 WGE 安装在系统路径中
target_link_libraries(your_target_name PRIVATE wge)
# 如果 WGE 安装在指定路径中
target_link_libraries(your_target_name PRIVATE /specified/path/lib/libwge.a)
```
* 在现有项目中使用 WGE
1. 在主线程中构造 WGE 引擎实例
```cpp
Wge::Engine engine(spdlog::level::off);
```
2. 在主线程中加载规则文件
```cpp
std::expected<bool, std::string> result = engine.loadFromFile(rule_file);
if (!result.has_value()) {
  // Handle the error
  std::cout << "Load rules error: " << result.error() << std::endl;
}
```
3. 在主线程中初始化引擎
```cpp
engine.init();
``` 
4. 在工作线程中为每个请求创建事务
```cpp
// 每个请求都有自己的事务
Wge::TransactionPtr t = engine.makeTransaction();
```
5. 在工作线程中处理请求
```cpp
// 处理每个事务的步骤如下
// 1. 处理连接
t->processConnection(/*params*/);
// 2. 处理 URI
t->processUri(/*params*/);
// 3. 处理请求头
t->processRequestHeaders(/*params*/);
// 4. 处理请求体
t->processRequestBody(/*params*/);
// 5. 处理响应头
t->processResponseHeaders(/*params*/);
// 6. 处理响应体
t->processResponseBody(/*params*/);
```