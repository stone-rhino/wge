+++
title = "集成到 Envoy"
weight = 2
+++

## 概述

WGE Envoy Connector 用于将 WGE 安全引擎以动态模块（Dynamic Module）的形式集成到 Envoy 中，从而在 HTTP 请求处理过程中实现流量检测与拦截能力。

Envoy 通过动态模块 ABI 机制加载外部模块，因此 WGE 模块必须与目标 Envoy 版本的 ABI 完全匹配。

---

## 构建前提

Envoy 动态模块依赖 ABI 头文件：

- `abi.h`
- `abi_version.h`

必须确保模块编译时使用的 ABI 与运行 Envoy 的版本一致，否则可能导致：

- 模块加载失败
- 运行时崩溃
- 未定义行为

---

## 获取 Envoy ABI

### 1. 获取 Envoy 版本

```bash
/path/to/envoy --version
```

### 2. 拉取对应 ABI
使用仓库提供的脚本获取 ABI：
```bash
chmod +x tools/fetch_envoy_abi.sh

# 方式一：通过 envoy 二进制自动识别版本
./tools/fetch_envoy_abi.sh /path/to/envoy /path/to/output

# 方式二：直接指定版本
./tools/fetch_envoy_abi.sh 1.36.0 /path/to/output
```
获取到的 ABI 文件需放置到：
```bash
envoy/envoy-abi/
```

## 构建 WGE Envoy 模块
在确保 ABI 正确后执行构建：
```bash
./build.sh
```
构建产物通常位于：
```bash
build/envoy/lib/
```
例如：
- libwge_dma_envoy.so

## 运行 Envoy 并加载模块
### 1. 配置动态模块
示例配置（HTTP filter）：
```yaml
- name: envoy.filters.http.dynamic_modules
  typed_config:
    "@type": type.googleapis.com/envoy.extensions.filters.http.dynamic_modules.v3.DynamicModuleFilter
    dynamic_module_config:
      name: "wge_dma_envoy"
      do_not_close: true
    filter_name: "wge_dma_envoy"
    filter_config:
      "@type": type.googleapis.com/google.protobuf.StringValue
      value: "/path/to/example.conf"
```

### 2. 设置模块加载路径
Envoy 通过动态模块搜索路径加载 .so 文件：
```bash
export ENVOY_DYNAMIC_MODULES_SEARCH_PATH=/path/to/build/envoy/lib/
```

### 3. 启动 Envoy
```bash
/path/to/envoy --config-path /path/to/wge_loader.yaml
```

## 工作机制说明

WGE 作为 Envoy 动态模块运行时：

- 在 Envoy 进程内部加载
- 通过 dynamic module ABI 与 Envoy 交互
- 由 HTTP filter 链路触发执行

模块生命周期由 Envoy 控制。