+++
title = "Variables"
weight = 2
ordersectionsby = "title"
+++

Variables represent the sources of data in WGE. Different variables correspond to different source data, and most variables come from some part of the request or response.

## 1. Variable Data Types

In WGE, variables have two data types:

- **String**
- **Integer**

Most variables are string values, such as `REQUEST_METHOD` and `REQUEST_URI`.

Integer data is produced in the following cases:

1. A user-defined `TX` variable is set to a numeric value.
2. `&variable_name` is used to obtain the number of elements in a collection variable.

When an operator expects an integer but receives a string, it will not match, unless the negation flag is also present. When writing rules, make sure the variable type matches the operator.

---

## 2. Scalar Variables and Collection Variables

From the perspective of storage structure, variables can be divided into two categories.

### 1. Scalar Variables

A scalar variable stores only one value.

For example:

- `REQUEST_METHOD` — a request has only one request method.
- `REMOTE_ADDR` — a request maps to only one client address.

---

### 2. Collection Variables

A collection variable stores multiple data items.

For example:

- `REQUEST_HEADERS` — a request usually contains multiple headers.
- `ARGS` — a request may contain multiple parameters.

Collection variables support additional access patterns, described below.

---

## 3. Phase Restrictions

Some variables can be accessed only in specific phases.

For example:

- Response-related variables are always empty during request phases.
- Request-related variables may be meaningless during response phases.

When writing rules, make sure the variable contains valid data in the phase where it is used.

---

## 4. Working with Collection Variables

### 1. Getting the Number of Elements

Use `&variable_name` to get the number of elements in a collection variable. It can also be used on scalar variables to determine whether that variable has a value.

```apache
# Block requests with more than 5 headers
SecRule &REQUEST_HEADERS "@gt 5" "id:1001,phase:2,t:none,deny"

# Check whether the Referer header exists
SecRule &REQUEST_HEADERS:referer "@eq 1" "id:1002,phase:2,t:none,deny"
```

### 2. Specifying a Subvariable

Use `variable_name:subvariable_name` to reference a specific element in a collection.

Subvariable names support several forms:

- Plain strings
- Regular expressions wrapped in `/.../`
- Files specified with `@...@`
- Macro variables

Example:

```apache
# Plain string subvariable name
SecRule REQUEST_HEADERS:host "@rx admin" "id:1001,phase:2,t:none,deny"

# Regex subvariable name
SecRule REQUEST_HEADERS:/^host$/ "@rx admin" "id:1002,phase:2,t:none,deny"

# File-based subvariable name
SecRule REQUEST_HEADERS:@headers.data@ "@rx admin" "id:1003,phase:2,t:none,deny"

# Macro-based subvariable name
SecRule REQUEST_HEADERS:%{tx.name} "@rx admin" "id:1003,phase:2,t:none,deny"
```

**Note:**

- Files specified with `@...@` support both string and regex matching. The behavior is the same as the `pmFromFile` operator. See [pmFromFile](../operators/pmfromfile.md) for more information.
- Plain string subvariable names are supported for most variables, even scalar variables, but the other forms are supported only by collection variables.

### 3. Excluding Variables

Use `!variable_name` to exclude a specific variable.

This is usually used together with collection variables.

```apache
# Inspect all request headers except Host
SecRule REQUEST_HEADERS|!REQUEST_HEADERS:host "@rx admin" "id:1001,phase:2,t:none,deny"
```

The exclusion mechanism is commonly used to:

- Ignore specific fields
- Apply "allowlist exclusion" to collection variables

| Keyword | Implemented | Collection Variable |
|---------|-------------|---------------------|
| [`ARGS`](./args/) | Yes | Yes |
| [`ARGS_COMBINED_SIZE`](./args_combined_size/) | No | No |
| [`ARGS_GET`](./args_get/) | Yes | Yes |
| [`ARGS_GET_NAMES`](./args_get_names/) | Yes | Yes |
| [`ARGS_NAMES`](./args_names/) | Yes | Yes |
| [`ARGS_POST`](./args_post/) | Yes | Yes |
| [`ARGS_POST_NAMES`](./args_post_names/) | Yes | Yes |
| [`AUTH_TYPE`](./auth_type/) | No | No |
| [`DURATION`](./duration/) | No | No |
| [`ENV`](./env/) | No | Yes |
| [`FILES`](./files/) | Yes | Yes |
| [`FILES_COMBINED_SIZE`](./files_combined_size/) | No | No |
| [`FILES_NAMES`](./files_names/) | Yes | Yes |
| [`FILES_SIZES`](./files_sizes/) | No | No |
| [`FILES_TMPNAMES`](./files_tmpnames/) | No | Yes |
| [`FILES_TMP_CONTENT`](./files_tmp_content/) | No | Yes |
| [`FULL_REQUEST`](./full_request/) | No | No |
| [`FULL_REQUEST_LENGTH`](./full_request_length/) | No | No |
| [`GEO`](./geo/) | No | Yes |
| [`GLOBAL`](./global/) | Yes | Yes |
| [`HIGHEST_SEVERITY`](./highest_severity/) | No | No |
| [`INBOUND_DATA_ERROR`](./inbound_data_error/) | No | No |
| [`IP`](./ip/) | Yes | Yes |
| [`MATCHED_VAR`](./matched_var/) | Yes | No |
| [`MATCHED_VARS`](./matched_vars/) | Yes | Yes |
| [`MATCHED_VARS_NAMES`](./matched_vars_names/) | Yes | Yes |
| [`MATCHED_VAR_NAME`](./matched_var_name/) | Yes | No |
| [`MODSEC_BUILD`](./modsec_build/) | No | No |
| [`MSC_PCRE_LIMITS_EXCEEDED`](./msc_pcre_limits_exceeded/) | No | No |
| [`MULTIPART_BOUNDARY_QUOTED`](./multipart_boundary_quoted/) | No | No |
| [`MULTIPART_BOUNDARY_WHITESPACE`](./multipart_boundary_whitespace/) | No | No |
| [`MULTIPART_CRLF_LF_LINES`](./multipart_crlf_lf_lines/) | No | No |
| [`MULTIPART_DATA_AFTER`](./multipart_data_after/) | No | No |
| [`MULTIPART_DATA_BEFORE`](./multipart_data_before/) | No | No |
| [`MULTIPART_FILENAME`](./multipart_filename/) | No | No |
| [`MULTIPART_FILE_LIMIT_EXCEEDED`](./multipart_file_limit_exceeded/) | No | No |
| [`MULTIPART_HEADER_FOLDING`](./multipart_header_folding/) | No | No |
| [`MULTIPART_INVALID_HEADER_FOLDING`](./multipart_invalid_header_folding/) | No | No |
| [`MULTIPART_INVALID_PART`](./multipart_invalid_part/) | No | No |
| [`MULTIPART_INVALID_QUOTING`](./multipart_invalid_quoting/) | No | No |
| [`MULTIPART_LF_LINE`](./multipart_lf_line/) | No | No |
| [`MULTIPART_MISSING_SEMICOLON`](./multipart_missing_semicolon/) | No | No |
| [`MULTIPART_NAME`](./multipart_name/) | No | No |
| [`MULTIPART_PART_HEADERS`](./multipart_part_headers/) | Yes | Yes |
| [`MULTIPART_STRICT_ERROR`](./multipart_strict_error/) | Yes | No |
| [`MULTIPART_UNMATCHED_BOUNDARY`](./multipart_unmatched_boundary/) | Yes | No |
| [`OUTBOUND_DATA_ERROR`](./outbound_data_error/) | No | No |
| [`PATH_INFO`](./path_info/) | Yes | No |
| [`QUERY_STRING`](./query_string/) | Yes | No |
| [`REMOTE_ADDR`](./remote_addr/) | Yes | No |
| [`REMOTE_HOST`](./remote_host/) | No | No |
| [`REMOTE_PORT`](./remote_port/) | No | No |
| [`REMOTE_USER`](./remote_user/) | No | No |
| [`REQBODY_ERROR`](./reqbody_error/) | Yes | No |
| [`REQBODY_ERROR_MSG`](./reqbody_error_msg/) | No | No |
| [`REQBODY_PROCESSOR`](./reqbody_processor/) | Yes | No |
| [`REQBODY_PROCESSOR_ERROR`](./reqbody_processor_error/) | No | No |
| [`REQUEST_BASENAME`](./request_basename/) | Yes | No |
| [`REQUEST_BODY`](./request_body/) | Yes | No |
| [`REQUEST_BODY_LENGTH`](./request_body_length/) | No | No |
| [`REQUEST_COOKIES`](./request_cookies/) | No | Yes |
| [`REQUEST_COOKIES_NAMES`](./request_cookies_names/) | No | Yes |
| [`REQUEST_FILENAME`](./request_filename/) | Yes | No |
| [`REQUEST_HEADERS`](./request_headers/) | Yes | Yes |
| [`REQUEST_HEADERS_NAMES`](./request_headers_names/) | Yes | Yes |
| [`REQUEST_LINE`](./request_line/) | Yes | No |
| [`REQUEST_METHOD`](./request_method/) | Yes | No |
| [`REQUEST_PROTOCOL`](./request_protocol/) | Yes | No |
| [`REQUEST_URI`](./request_uri/) | Yes | No |
| [`REQUEST_URI_RAW`](./request_uri_raw/) | Yes | No |
| [`RESOURCE`](./resource/) | Yes | Yes |
| [`RESPONSE_BODY`](./response_body/) | Yes | No |
| [`RESPONSE_CONTENT_LENGTH`](./response_content_length/) | Yes | No |
| [`RESPONSE_CONTENT_TYPE`](./response_content_type/) | Yes | No |
| [`RESPONSE_HEADERS`](./response_headers/) | Yes | Yes |
| [`RESPONSE_HEADERS_NAMES`](./response_headers_names/) | Yes | Yes |
| [`RESPONSE_PROTOCOL`](./response_protocol/) | Yes | No |
| [`RESPONSE_STATUS`](./response_status/) | Yes | No |
| [`RULE`](./rule/) | Yes | No |
| [`SERVER_ADDR`](./server_addr/) | No | No |
| [`SERVER_NAME`](./server_name/) | No | No |
| [`SERVER_PORT`](./server_port/) | No | No |
| [`SESSION`](./session/) | Yes | Yes |
| [`SESSIONID`](./sessionid/) | No | No |
| [`STATUS_LINE`](./status_line/) | No | No |
| [`TIME`](./time/) | No | No |
| [`TIME_DAY`](./time_day/) | No | No |
| [`TIME_EPOCH`](./time_epoch/) | No | No |
| [`TIME_HOUR`](./time_hour/) | No | No |
| [`TIME_MIN`](./time_min/) | No | No |
| [`TIME_MON`](./time_mon/) | No | No |
| [`TIME_SEC`](./time_sec/) | No | No |
| [`TIME_WDAY`](./time_wday/) | No | No |
| [`TIME_YEAR`](./time_year/) | No | No |
| [`TX`](./tx/) | Yes | Yes |
| [`UNIQUE_ID`](./unique_id/) | Yes | No |
| [`URLENCODED_ERROR`](./urlencoded_error/) | No | No |
| [`USER`](./user/) | Yes | Yes |
| [`USERID`](./userid/) | No | No |
| [`WEBAPPID`](./webappid/) | No | No |
| [`XML`](./xml/) | Yes | Yes |
