+++
title = "变量"
weight = 2
ordersectionsby = "title"
+++

变量代表 WGE 中数据的来源。不同变量对应不同的源数据，大多数变量来源于请求或响应的某一部分。

## 一、变量的数据类型

在 WGE 中，变量的数据类型分为两种：

- **字符串**
- **整形**

绝大多数变量的值为字符串类型，例如 `REQUEST_METHOD`、`REQUEST_URI` 等。

以下情况会产生整形数据：

1. 用户自定义的 `TX` 变量被设置为数值
2. 使用 `&变量名` 取得集合变量的数量

当操作符期望整形数据，但实际传入的是字符串时，操作符将不会匹配（除非还有取反标志）。因此，在编写规则时应注意变量类型与操作符的匹配关系。

---

## 二、标量变量与集合变量

从存储结构上看，变量可分为两类：

### 1. 标量变量（单值变量）

只存储一个数据。

例如：

- `REQUEST_METHOD` —— 一个请求只存在一个请求方法
- `REMOTE_ADDR` —— 一个请求只对应一个客户端地址

---

### 2. 集合变量（多值变量）

存储多个数据项。

例如：

- `REQUEST_HEADERS` —— 一个请求通常包含多个请求头
- `ARGS` —— 可能包含多个参数

集合变量支持额外的访问方式，详见下文。

---

## 三、变量的阶段限制

某些变量只能在特定阶段访问。

例如：

- 响应相关变量在请求阶段始终为空
- 请求相关变量在响应阶段可能无意义

在编写规则时，应确保变量在对应阶段存在有效数据。

---

## 四、集合变量的使用方式

### 1. 获取变量数量

使用 `&变量名` 表示获取集合变量的元素数量（对于标量变量也可以使用，来确定该变量是否有值）。

```apache
# 请求头数量超过 5 个则拦截
SecRule &REQUEST_HEADERS "@gt 5" "id:1001,phase:2,t:none,deny"

# 是否存在referer头
SecRule &REQUEST_HEADERS:referer "@eq 1" "id:1002,phase:2,t:none,deny"
```

### 2. 指定子变量

使用 `变量名:子变量名` 指定集合中的某个元素。

子变量名支持多种形式：

- 普通字符串
- 使用 `/.../` 包围的正则表达式
- 使用 `@...@` 指定文件
- 宏变量

示例：
```apache
# 普通字符串作为子变量名
SecRule REQUEST_HEADERS:host "@rx admin" "id:1001,phase:2,t:none,deny"

# 正则作为子变量名
SecRule REQUEST_HEADERS:/^host$/ "@rx admin" "id:1002,phase:2,t:none,deny"

# 文件名作为子变量名
SecRule REQUEST_HEADERS:@headers.data@ "@rx admin" "id:1003,phase:2,t:none,deny"

# 宏作为子变量名
SecRule REQUEST_HEADERS:%{tx.name} "@rx admin" "id:1003,phase:2,t:none,deny"
```

**注意**：
- `@...@`指定的文件中支持字符串与正则匹配，行为与操作符 pmFromFile 相同。更多信息可见 [pmFromFile](../operators/pmfromfile.md)。
- 普通字符串作为子变量名对于大部分变量都是支持的（即使其时标量变量），但是后集中则是仅有集合变量可以支持。

### 3. 排除变量

使用 `!变量名` 表示排除指定变量。

通常与集合变量配合使用。

```apache
# 检查除了host头之外的所有请求头
SecRule REQUEST_HEADERS|!REQUEST_HEADERS:host "@rx admin" "id:1001,phase:2,t:none,deny"
```
排除机制常用于：
- 忽略特定字段
- 对集合变量进行“白名单排除”

| 关键字 | 是否实现 | 是否为集合变量 |
|--------|------|------|
| [`ARGS`](./args/) | 是 | 是 |
| [`ARGS_COMBINED_SIZE`](./args_combined_size/) | 否 | 否 |
| [`ARGS_GET`](./args_get/) | 是 | 是 |
| [`ARGS_GET_NAMES`](./args_get_names/) | 是 | 是 |
| [`ARGS_NAMES`](./args_names/) | 是 | 是 |
| [`ARGS_POST`](./args_post/) | 是 | 是 |
| [`ARGS_POST_NAMES`](./args_post_names/) | 是 | 是 |
| [`AUTH_TYPE`](./auth_type/) | 否 | 否 |
| [`DURATION`](./duration/) | 否 | 否 |
| [`ENV`](./env/) | 否 | 是 |
| [`FILES`](./files/) | 是 | 是 |
| [`FILES_COMBINED_SIZE`](./files_combined_size/) | 否 | 否 |
| [`FILES_NAMES`](./files_names/) | 是 | 是 |
| [`FILES_SIZES`](./files_sizes/) | 否 | 否 |
| [`FILES_TMPNAMES`](./files_tmpnames/) | 否 | 是 |
| [`FILES_TMP_CONTENT`](./files_tmp_content/) | 否 | 是 |
| [`FULL_REQUEST`](./full_request/) | 否 | 否 |
| [`FULL_REQUEST_LENGTH`](./full_request_length/) | 否 | 否 |
| [`GEO`](./geo/) | 否 | 是 |
| [`GLOBAL`](./global/) | 是 | 是 |
| [`HIGHEST_SEVERITY`](./highest_severity/) | 否 | 否 |
| [`INBOUND_DATA_ERROR`](./inbound_data_error/) | 否 | 否 |
| [`IP`](./ip/) | 是 | 是 |
| [`MATCHED_VAR`](./matched_var/) | 是 | 否 |
| [`MATCHED_VARS`](./matched_vars/) | 是 | 是 |
| [`MATCHED_VARS_NAMES`](./matched_vars_names/) | 是 | 是 |
| [`MATCHED_VAR_NAME`](./matched_var_name/) | 是 | 否 |
| [`MODSEC_BUILD`](./modsec_build/) | 否 | 否 |
| [`MSC_PCRE_LIMITS_EXCEEDED`](./msc_pcre_limits_exceeded/) | 否 | 否 |
| [`MULTIPART_BOUNDARY_QUOTED`](./multipart_boundary_quoted/) | 否 | 否 |
| [`MULTIPART_BOUNDARY_WHITESPACE`](./multipart_boundary_whitespace/) | 否 | 否 |
| [`MULTIPART_CRLF_LF_LINES`](./multipart_crlf_lf_lines/) | 否 | 否 |
| [`MULTIPART_DATA_AFTER`](./multipart_data_after/) | 否 | 否 |
| [`MULTIPART_DATA_BEFORE`](./multipart_data_before/) | 否 | 否 |
| [`MULTIPART_FILENAME`](./multipart_filename/) | 否 | 否 |
| [`MULTIPART_FILE_LIMIT_EXCEEDED`](./multipart_file_limit_exceeded/) | 否 | 否 |
| [`MULTIPART_HEADER_FOLDING`](./multipart_header_folding/) | 否 | 否 |
| [`MULTIPART_INVALID_HEADER_FOLDING`](./multipart_invalid_header_folding/) | 否 | 否 |
| [`MULTIPART_INVALID_PART`](./multipart_invalid_part/) | 否 | 否 |
| [`MULTIPART_INVALID_QUOTING`](./multipart_invalid_quoting/) | 否 | 否 |
| [`MULTIPART_LF_LINE`](./multipart_lf_line/) | 否 | 否 |
| [`MULTIPART_MISSING_SEMICOLON`](./multipart_missing_semicolon/) | 否 | 否 |
| [`MULTIPART_NAME`](./multipart_name/) | 否 | 否 |
| [`MULTIPART_PART_HEADERS`](./multipart_part_headers/) | 是 | 是 |
| [`MULTIPART_STRICT_ERROR`](./multipart_strict_error/) | 是 | 否 |
| [`MULTIPART_UNMATCHED_BOUNDARY`](./multipart_unmatched_boundary/) | 是 | 否 |
| [`OUTBOUND_DATA_ERROR`](./outbound_data_error/) | 否 | 否 |
| [`PATH_INFO`](./path_info/) | 是 | 否 |
| [`QUERY_STRING`](./query_string/) | 是 | 否 |
| [`REMOTE_ADDR`](./remote_addr/) | 是 | 否 |
| [`REMOTE_HOST`](./remote_host/) | 否 | 否 |
| [`REMOTE_PORT`](./remote_port/) | 否 | 否 |
| [`REMOTE_USER`](./remote_user/) | 否 | 否 |
| [`REQBODY_ERROR`](./reqbody_error/) | 是 | 否 |
| [`REQBODY_ERROR_MSG`](./reqbody_error_msg/) | 否 | 否 |
| [`REQBODY_PROCESSOR`](./reqbody_processor/) | 是 | 否 |
| [`REQBODY_PROCESSOR_ERROR`](./reqbody_processor_error/) | 否 | 否 |
| [`REQUEST_BASENAME`](./request_basename/) | 是 | 否 |
| [`REQUEST_BODY`](./request_body/) | 是 | 否 |
| [`REQUEST_BODY_LENGTH`](./request_body_length/) | 否 | 否 |
| [`REQUEST_COOKIES`](./request_cookies/) | 否 | 是 |
| [`REQUEST_COOKIES_NAMES`](./request_cookies_names/) | 否 | 是 |
| [`REQUEST_FILENAME`](./request_filename/) | 是 | 否 |
| [`REQUEST_HEADERS`](./request_headers/) | 是 | 是 |
| [`REQUEST_HEADERS_NAMES`](./request_headers_names/) | 是 | 是 |
| [`REQUEST_LINE`](./request_line/) | 是 | 否 |
| [`REQUEST_METHOD`](./request_method/) | 是 | 否 |
| [`REQUEST_PROTOCOL`](./request_protocol/) | 是 | 否 |
| [`REQUEST_URI`](./request_uri/) | 是 | 否 |
| [`REQUEST_URI_RAW`](./request_uri_raw/) | 是 | 否 |
| [`RESOURCE`](./resource/) | 是 | 是 |
| [`RESPONSE_BODY`](./response_body/) | 是 | 否 |
| [`RESPONSE_CONTENT_LENGTH`](./response_content_length/) | 是 | 否 |
| [`RESPONSE_CONTENT_TYPE`](./response_content_type/) | 是 | 否 |
| [`RESPONSE_HEADERS`](./response_headers/) | 是 | 是 |
| [`RESPONSE_HEADERS_NAMES`](./response_headers_names/) | 是 | 是 |
| [`RESPONSE_PROTOCOL`](./response_protocol/) | 是 | 否 |
| [`RESPONSE_STATUS`](./response_status/) | 是 | 否 |
| [`RULE`](./rule/) | 是 | 否 |
| [`SERVER_ADDR`](./server_addr/) | 否 | 否 |
| [`SERVER_NAME`](./server_name/) | 否 | 否 |
| [`SERVER_PORT`](./server_port/) | 否 | 否 |
| [`SESSION`](./session/) | 是 | 是 |
| [`SESSIONID`](./sessionid/) | 否 | 否 |
| [`STATUS_LINE`](./status_line/) | 否 | 否 |
| [`TIME`](./time/) | 否 | 否 |
| [`TIME_DAY`](./time_day/) | 否 | 否 |
| [`TIME_EPOCH`](./time_epoch/) | 否 | 否 |
| [`TIME_HOUR`](./time_hour/) | 否 | 否 |
| [`TIME_MIN`](./time_min/) | 否 | 否 |
| [`TIME_MON`](./time_mon/) | 否 | 否 |
| [`TIME_SEC`](./time_sec/) | 否 | 否 |
| [`TIME_WDAY`](./time_wday/) | 否 | 否 |
| [`TIME_YEAR`](./time_year/) | 否 | 否 |
| [`TX`](./tx/) | 是 | 是 |
| [`UNIQUE_ID`](./unique_id/) | 是 | 否 |
| [`URLENCODED_ERROR`](./urlencoded_error/) | 否 | 否 |
| [`USER`](./user/) | 是 | 是 |
| [`USERID`](./userid/) | 否 | 否 |
| [`WEBAPPID`](./webappid/) | 否 | 否 |
| [`XML`](./xml/) | 是 | 是 |