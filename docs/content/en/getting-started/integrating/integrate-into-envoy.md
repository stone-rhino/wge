+++
title = "Integrate into Envoy"
weight = 2
+++

## Overview

The WGE Envoy Connector integrates the WGE security engine into Envoy as a dynamic module, enabling traffic inspection and interception during HTTP request processing.

Envoy loads external modules via its dynamic module ABI mechanism, so the WGE module must exactly match the ABI of the target Envoy version.

---

## Prerequisites

The Envoy dynamic module depends on the following ABI header files:

- `abi.h`
- `abi_version.h`

You must ensure that the ABI used when compiling the module matches the version of the running Envoy binary. A mismatch may result in:

- Module load failure
- Runtime crash
- Undefined behavior

---

## Obtaining the Envoy ABI

### 1. Check the Envoy Version

```bash
/path/to/envoy --version
```

### 2. Fetch the Corresponding ABI

Use the script provided in the repository to fetch the ABI:

```bash
chmod +x tools/fetch_envoy_abi.sh

# Option 1: auto-detect version from the Envoy binary
./tools/fetch_envoy_abi.sh /path/to/envoy /path/to/output

# Option 2: specify the version directly
./tools/fetch_envoy_abi.sh 1.36.0 /path/to/output
```

Place the retrieved ABI files into:

```bash
envoy/envoy-abi/
```

## Building the WGE Envoy Module

Once the correct ABI is in place, run the build:

```bash
./build.sh
```

Build artifacts are typically located at:

```bash
build/envoy/lib/
```

For example:
- `libwge_dma_envoy.so`

## Running Envoy with the Module

### 1. Configure the Dynamic Module

Example configuration (HTTP filter):

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

### 2. Set the Module Search Path

Envoy locates `.so` files via the dynamic module search path:

```bash
export ENVOY_DYNAMIC_MODULES_SEARCH_PATH=/path/to/build/envoy/lib/
```

### 3. Start Envoy

```bash
/path/to/envoy --config-path /path/to/wge_loader.yaml
```

## How It Works

When WGE runs as an Envoy dynamic module:

- It is loaded inside the Envoy process
- It interacts with Envoy through the dynamic module ABI
- It is triggered by the HTTP filter chain

The module lifecycle is managed by Envoy.
