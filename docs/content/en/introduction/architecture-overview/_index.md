+++
title = "Architecture Overview"
weight = 2
+++

- [Component Diagram](#component-diagram)
  - [Engine](#engine)
  - [SecLang++ Parser](#seclang-parser)
  - [Rule Object](#rule-object)
  - [Transaction](#transaction)
- [Rule Execution Flow](#rule-execution-flow)
  - [1. HTTP Data Parsing](#1-http-data-parsing)
  - [2. Phase Scheduling](#2-phase-scheduling)
  - [3. Decoding](#3-decoding)
  - [4. Operator Execution](#4-operator-execution)
  - [5. Action Execution](#5-action-execution)
  - [6. Output Results](#6-output-results)
  - [7. Rule Scheduling](#7-rule-scheduling)
- [Threading Model](#threading-model)
- [Summary](#summary)

## Component Diagram

![](./component-diagram.png)

### Engine

**Engine** is the core of WGE, typically existing as a singleton. Its responsibilities include:
- **Rule Loading** - Load rule sets from files or configuration directives
- **Rule Parsing** - Parse SecLang++ rules using the Parser
- **Transaction Creation** - Create transaction objects for each HTTP request
- **Rule Organization** - Organize and manage rules by Phase
- **Configuration Management** - Manage engine configuration and audit log settings
- **Persistent Storage** - Manage persistent storage interfaces, enabling multiple transactions to share data

### SecLang++ Parser

**Parser** is implemented using ANTLR4, responsible for parsing SecLang++ rule text into an Abstract Syntax Tree (AST) and building a C++ object graph for the engine to execute.
- **Lexical Analysis** - Break rule text into tokens
- **Syntax Analysis** - Generate AST based on SecLang++ grammar rules
- **Build Rule Objects** - Convert AST into executable Rule objects

### Rule Object

**Rule** objects contain all information for a single SecLang++ rule, including variables, operators, and actions, for the transaction to evaluate (execute) when processing requests. Rule objects are designed to be stateless and can be shared across multiple transactions.
- **Variables** - Variables used in the rule, serving as input when the rule is evaluated. Variable objects are stateless, containing only data extraction logic, and perform actual data extraction through the context (such as request data) passed in via the transaction. Accessible variable types include request variables (such as ARGS), response variables (such as RESPONSE_BODY), transaction variables (such as TX), and special variables (such as PTREE and persistent storage data).
- **Operators** - Matching logic used in the rule to evaluate variable values. Operator objects are also stateless, containing only matching logic, and perform actual matching calculations through the passed-in variable values. Supported operator types include numeric comparison (such as @eq), string matching (such as @streq), regular expressions (such as @rx), multi-pattern matching (such as @pm), and more.
- **Actions** - Operations executed based on operator evaluation results, divided into **pre-match actions**, **match success actions**, **match failure actions**, and **unconditional actions**. Action objects are similarly stateless, containing only action logic, and execute actual operations through the passed-in transaction context. Supported action types include setting transaction data (such as setvar), dynamically modifying rules (such as ctl), and more.
  - **Pre-match Actions** - Actions executed before evaluating variable values with operators, mainly used for preprocessing variable values. For example, Ragel transcoding functions (html_entity_decode, js_decode, etc.). These transcoding functions use high-performance DFA decoding functions implemented with Ragel to decode and transform variable values, preventing attack payloads from bypassing detection through encoding.
  - **Match Success Actions** - Actions executed when the rule matches successfully, such as setting variables (setvar).
  - **Match Failure Actions** - Actions executed when the rule fails to match, such as setting variables (!setvar).
  - **Unconditional Actions** - Actions executed regardless of whether the rule matches, such as setting variables (*setvar).

### Transaction

`Transaction` represents the processing flow for a single HTTP request/response. Its responsibilities include:
- **HTTP Data Parsing** - Parse connection information, request line, response line, XML/JSON data, etc.
- **Rule Scheduling** - Evaluate rules one by one, and skip unnecessary rules based on skip directives
- **State Management** - Maintain transaction state and transaction variable (TX variable) storage
- **Phase Processing** - Process requests/responses in 5 phases in order:
  - Phase 1: REQUEST_HEADERS (Request header processing)
  - Phase 2: REQUEST_BODY (Request body processing)
  - Phase 3: RESPONSE_HEADERS (Response header processing)
  - Phase 4: RESPONSE_BODY (Response body processing)
  - Phase 5: LOGGING (Log recording)

## Rule Execution Flow

For each HTTP request, the WGE engine creates an independent Transaction instance. This instance is responsible for processing the entire lifecycle of the request, including parsing HTTP data, scheduling rule evaluation by phase, and maintaining transaction state. When processing a request, the transaction executes rules following these steps:

![](./rules-evaluation-flow.png)

### 1. HTTP Data Parsing

The transaction first parses HTTP data, extracting connection information, request line, request headers, request body, response line, response headers, response body, and other content for subsequent rule evaluation. The corresponding interfaces include:

- `Transaction::processConnection()`
```c++
/**
 * Process the connection info.
 * @param downstream_ip the downstream ip.
 * @param downstream_port the downstream port.
 * @param upstream_ip the upstream ip.
 * @param upstream_port the upstream port.
 */
void processConnection(std::string_view downstream_ip, short downstream_port,
                        std::string_view upstream_ip, short upstream_port);
```

- `Transaction::processUri()`
```c++
/**
 * Process the uri info.
 * @param request_line the request line. include method, path, query, protocol, version.
 * E.g. GET / HTTP/1.1
 */
void processUri(std::string_view request_line);

/**
 * Process the uri info.
 * @param uri the uri. E.g. /hello/world
 * @param method the method. E.g. GET
 * @param version the version. E.g. 1.1
 */
void processUri(std::string_view uri, std::string_view method, std::string_view version);
```

### 2. Phase Scheduling

The transaction processes requests/responses in 5 phases in order. In each phase, the transaction obtains the list of rules for that phase and evaluates them one by one. The phases include:
- Phase 1: Request headers
- Phase 2: Request body
- Phase 3: Response headers
- Phase 4: Response body
- Phase 5: Logging

The corresponding interfaces include:

- `Transaction::processRequestHeaders()`
```c++
/**
 * Process the request headers.
 * @param request_header_find the header find function.
 * @param request_header_traversal the header traversal function.
 * @param request_header_count the count of the headers.
 * @param log_callback the log callback. if the rule is matched, the log_callback will be called.
 * @param log_user_data the user data pointer for the log callback.
 * @param additional_cond an "AND" logic based on the original logic of the rule, only if both
 * match successfully is the final result true.
 * @param additional_cond_user_data the user data pointer for the additional condition callback.
 * @return true if the request is safe, false otherwise that means need to deny the request.
 */
bool processRequestHeaders(HeaderFind request_header_find,
                            HeaderTraversal request_header_traversal, size_t request_header_count,
                            LogCallback log_callback = nullptr, void* log_user_data = nullptr,
                            AdditionalCondCallback additional_cond = nullptr,
                            void* additional_cond_user_data = nullptr);
```

- `Transaction::processRequestBody()`
```c++
/**
 * Process the request body.
 * @param body the request body.
 * @param log_callback the log callback. if the rule is matched, the log_callback will be called.
 * @param log_user_data the user data pointer for the log callback.
 * @param additional_cond an "AND" logic based on the original logic of the rule, only if both
 * match successfully is the final result true.
 * @param additional_cond_user_data the user data pointer for the additional condition callback.
 * @return true if the request is safe, false otherwise that means need to deny the request.
 */
bool processRequestBody(std::string_view body, LogCallback log_callback = nullptr,
                        void* log_user_data = nullptr,
                        AdditionalCondCallback additional_cond = nullptr,
                        void* additional_cond_user_data = nullptr);
```

- `Transaction::processResponseHeaders()`
```c++
/**
 * Process the response headers.
 * @param status_code the status code of the response. E.g. 200
 * @param protocol the protocol of the response. E.g. HTTP/1.1
 * @param response_header_find the header find function.
 * @param response_header_traversal the header traversal function.
 * @param response_header_count the count of the headers.
 * @param log_callback the log callback. if the rule is matched, the log_callback will be called.
 * @param log_user_data the user data pointer for the log callback.
 * @param additional_cond an "AND" logic based on the original logic of the rule, only if both
 * match successfully is the final result true.
 * @param additional_cond_user_data the user data pointer for the additional condition callback.
 * @return true if the request is safe, false otherwise that means need to deny the request.
 */
bool processResponseHeaders(std::string_view status_code, std::string_view protocol,
                        HeaderFind response_header_find,
                        HeaderTraversal response_header_traversal,
                        size_t response_header_count, LogCallback log_callback = nullptr,
                        void* log_user_data = nullptr,
                        AdditionalCondCallback additional_cond = nullptr,
                        void* additional_cond_user_data = nullptr);
```

- `Transaction::processResponseBody()`
```c++
/**
 * Process the response body.
 * @param body the response body.
 * @param log_callback the log callback. if the rule is matched, the log_callback will be called.
 * @param log_user_data the user data pointer for the log callback.
 * @param additional_cond an "AND" logic based on the original logic of the rule, only if both
 * match successfully is the final result true.
 * @param additional_cond_user_data the user data pointer for the additional condition callback.
 * @return true if the request is safe, false otherwise that means need to deny the request.
 */
bool processResponseBody(std::string_view body, LogCallback log_callback = nullptr,
                        void* log_user_data = nullptr,
                        AdditionalCondCallback additional_cond = nullptr,
                        void* additional_cond_user_data = nullptr);
```

### 3. Decoding

During rule evaluation, if a rule contains pre-match actions (such as Ragel transcoding functions), the transaction first executes these actions to preprocess variable values, then uses operators for evaluation.

This includes HTML entity decoding (htmlEntityDecode), JavaScript decoding (jsDecode), CSS decoding (cssDecode), URL decoding (urlDecode), and 22 other common transcoding formats, ensuring attack payloads cannot bypass detection through encoding transformations.

These decoding functions use a high-performance DFA decoding engine implemented with Ragel, capable of efficiently processing large amounts of data, suitable for high-concurrency scenarios.

### 4. Operator Execution

The transaction passes variable values to the rule's operator object, executes the matching logic, and obtains the matching result (success or failure).

### 5. Action Execution

Based on the operator evaluation result, the transaction executes the corresponding actions (match success actions, match failure actions, or unconditional actions), such as setting transaction variables (setvar), dynamically modifying rules (ctl), etc.

### 6. Output Results

If the rule matches successfully, the transaction calls the user-provided log callback function (the log_callback parameter in interfaces like processRequestHeaders).

### 7. Rule Scheduling

The transaction continues to evaluate the next rule in the current phase until all rules have been evaluated or an action that terminates the transaction early (allow, deny) is encountered. Then it proceeds to the next phase and repeats the above process until all phases are completed.

## Threading Model

WGE adopts a lock-free design:

- **Engine** instances are typically singletons or globally shared
- **Transaction** is created separately for each request and is not shared between threads
- Variables, rules, and other shared data are designed as read-only stateless objects to avoid race conditions
- Supports efficient operation in high-concurrency scenarios

![](./thread-model.png)

## Summary

WGE's architecture design focuses on the following aspects:

1. **Compatibility** - 100% compatible with OWASP CRS rule sets
2. **Performance** - Over 4x performance improvement compared to ModSecurity
3. **Extensibility** - Modular design supports flexible feature extension
4. **Ease of Use** - Clean API and comprehensive integration examples and documentation
5. **Modernization** - Built with C++23 modern features
