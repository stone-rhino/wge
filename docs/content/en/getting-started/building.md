+++
title = "Building"
weight = 2
+++

This chapter provides detailed instructions on how to build WGE from source code and integrate it into existing projects. We assume you are a beginner and need step-by-step guidance to complete the build process. Therefore, we will start from development environment configuration, cover the installation of required dependencies, and guide you through successfully building WGE and integrating it into your project.

If you are already an experienced C++ developer with a properly configured C++ development environment, you can still refer to this chapter, as it contains best practices for C++ development environment configuration recommended by the WGE development team. These practices are not only applicable to WGE but also to other C++ project development. So regardless of your experience level, this chapter will help you successfully complete the WGE build and integration.

## Development Environment

To build WGE, you need a suitable C++ development environment. Our recommended development environment configuration is:

- **Operating System**: Windows 10/11 (64-bit) + WSL2 (Ubuntu 20.04)
- **Compiler**: GCC 13.2.0
- **Code Editor**: Visual Studio Code + related extensions (C++, CMake, Ragel, etc.)
- **Build Tool**: CMake 3.28
- **Package Manager**: vcpkg
- **Other Dependencies**: Ragel 6.10, JDK 21, ANTLR4 4.13.2

If you are an experienced developer, you may already have your own preferred configuration. However, for beginners or users who want to get started quickly, we recommend using the above configuration. This helps community users stay aligned with the WGE development team, facilitating communication and collaboration. Besides, we don't want environment configuration to become a barrier to using WGE. We hope you can focus on understanding WGE's principles and implementing features, rather than spending too much time on environment setup.

## Installation Steps

The following sections will guide you through setting up the recommended development environment step by step:

- Installing WSL2 and Ubuntu 20.04
- Installing GCC 13.2.0
- Installing Visual Studio Code and related extensions
- Installing CMake 3.28
- Installing vcpkg
- Installing Ragel 6.10
- Installing JDK 21
- Installing ANTLR4 4.13.2

For detailed build instructions, please refer to the [Quick Start](./quick-start.md) guide.
