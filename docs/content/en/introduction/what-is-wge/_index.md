+++
title = "What Is WGE"
weight = 1
+++

WGE is a high-performance Web Application Firewall (WAF) library built with C++. It has been successfully deployed in commercial products: Stone Rhino - Web Governance Engine (SR-WGE). WGE is designed to be fully compatible with the OWASP Core Rule Set (CRS) and can serve as a drop-in replacement for ModSecurity. The library is built with C++23, with design goals focused on speed, efficiency, and ease of use.

## Performance Comparison

Processor: Intel(R) Core(TM) i5-10400 CPU @ 2.90GHz 2.90 GHz
Memory: 32GB
Operating System: Ubuntu 20.04.6 LTS (5.15.153.1-microsoft-standard-WSL2)
Worker Threads: 8
Test Samples: [White samples](https://github.com/stone-rhino/wge/blob/main/benchmarks/test_data/white.data) and [Black samples](https://github.com/stone-rhino/wge/blob/main/benchmarks/test_data/black.data)

| Rule Set          | Memory Pool (TCMalloc) | ModSecurity |    WGE     |
|-------------------|------------------------|-------------|------------|
| CRS v4.3.0        | Disabled               | 4010 QPS    | 17560 QPS  |
| CRS v4.3.0        | Enabled                | 4927 QPS    | 18864 QPS  |

Benchmark results show that WGE significantly outperforms ModSecurity, with performance improvements exceeding 4x. This is achieved through the application of modern C++ features and excellent architecture design and implementation. WGE is designed to be easy to use and seamlessly integrate into existing applications, making it an ideal choice for developers looking to add WAF capabilities to their projects.

## Feature List

* Fully compatible with OWASP Core Rule Set (CRS). Supports all variables, operators, and actions, ensuring compatibility with existing rule sets. All CRS-supported attack types including SQL Injection (SQLi), Cross-Site Scripting (XSS), Local File Inclusion (LFI), Remote Command Execution (RCE), and more can be effectively detected and defended against.
* High performance. Uses ANTLR4 for high-performance frontend parsing, a carefully designed efficient C++ object graph as the backend interpreter, and integrates TCMalloc memory pool to significantly improve throughput and reduce memory fragmentation.
* Rich transcoding support. High-performance DFA encoding/decoding engine implemented with Ragel, supporting 22 common transcoding formats including URL decoding, HTML entity decoding, JavaScript decoding, CSS decoding, and more, ensuring attack payloads cannot bypass detection through encoding transformations.
* Multi-threading support. Lock-free design enables efficient operation in multi-threaded environments, suitable for high-concurrency scenarios.
* SecLang++. Derived from SecLang, SecLang++ introduces multiple practical language features such as rule fragments, configuration-driven rules, conditional actions, aliases, reference variables, and more. These enhance the flexibility and maintainability of rule writing.

## Future Plans

* Continue evolving SecLang++ by introducing more language features inspired by modern programming concepts, while maintaining the simplicity and ease of use of a domain-specific language, enhancing the expressiveness and maintainability of rule writing.
* Add streaming processing support to handle large request bodies without loading entire requests/responses into memory, reducing memory usage and defending against large request body attacks.
* Introduce a syntax and semantic engine to reduce false positives and improve detection capabilities for complex attacks.
* Introduce machine learning-assisted detection to improve identification of unknown attacks.
* Add multi-pattern matching detection engine to further improve rule execution efficiency.
* Adopt bytecode + JIT technology to maximize rule execution efficiency.
* Expand more connectors to demonstrate seamless integration with more web servers and frameworks.
