+++
title = "Variables"
weight = 2
+++

| Keyword | Description |
|---------|-------------|
| [`ARGS`](./args/) | All request parameters (GET and POST) |
| [`ARGS_GET`](./args_get/) | GET request parameters |
| [`ARGS_POST`](./args_post/) | POST request parameters |
| [`ARGS_NAMES`](./args_names/) | All parameter names |
| [`ARGS_GET_NAMES`](./args_get_names/) | GET parameter names |
| [`ARGS_POST_NAMES`](./args_post_names/) | POST parameter names |
| [`ARGS_COMBINED_SIZE`](./args_combined_size/) | Combined size of all arguments |
| [`REQUEST_HEADERS`](./request_headers/) | Request headers |
| [`REQUEST_HEADERS_NAMES`](./request_headers_names/) | Request header names |
| [`REQUEST_COOKIES`](./request_cookies/) | Request cookies |
| [`REQUEST_COOKIES_NAMES`](./request_cookies_names/) | Request cookie names |
| [`REQUEST_BODY`](./request_body/) | Request body content |
| [`REQUEST_BODY_LENGTH`](./request_body_length/) | Request body length |
| [`REQUEST_METHOD`](./request_method/) | HTTP request method |
| [`REQUEST_URI`](./request_uri/) | Request URI |
| [`REQUEST_URI_RAW`](./request_uri_raw/) | Raw request URI |
| [`REQUEST_FILENAME`](./request_filename/) | Request filename |
| [`REQUEST_BASENAME`](./request_basename/) | Request basename |
| [`REQUEST_PROTOCOL`](./request_protocol/) | Request protocol |
| [`REQUEST_LINE`](./request_line/) | Full request line |
| [`QUERY_STRING`](./query_string/) | Query string |
| [`FULL_REQUEST`](./full_request/) | Full request |
| [`FULL_REQUEST_LENGTH`](./full_request_length/) | Full request length |
| [`RESPONSE_BODY`](./response_body/) | Response body content |
| [`RESPONSE_HEADERS`](./response_headers/) | Response headers |
| [`RESPONSE_HEADERS_NAMES`](./response_headers_names/) | Response header names |
| [`RESPONSE_STATUS`](./response_status/) | Response status code |
| [`RESPONSE_PROTOCOL`](./response_protocol/) | Response protocol |
| [`RESPONSE_CONTENT_LENGTH`](./response_content_length/) | Response content length |
| [`RESPONSE_CONTENT_TYPE`](./response_content_type/) | Response content type |
| [`REMOTE_ADDR`](./remote_addr/) | Client IP address |
| [`REMOTE_HOST`](./remote_host/) | Client hostname |
| [`REMOTE_PORT`](./remote_port/) | Client port |
| [`REMOTE_USER`](./remote_user/) | Authenticated username |
| [`SERVER_ADDR`](./server_addr/) | Server IP address |
| [`SERVER_NAME`](./server_name/) | Server name |
| [`SERVER_PORT`](./server_port/) | Server port |
| [`AUTH_TYPE`](./auth_type/) | Authentication type |
| [`PATH_INFO`](./path_info/) | Path info |
| [`MULTIPART_FILENAME`](./multipart_filename/) | Multipart filename |
| [`MULTIPART_NAME`](./multipart_name/) | Multipart name |
| [`MULTIPART_PART_HEADERS`](./multipart_part_headers/) | Multipart part headers |
| [`MULTIPART_CRLF_LF_LINES`](./multipart_crlf_lf_lines/) | Multipart CRLF/LF lines |
| [`MULTIPART_STRICT_ERROR`](./multipart_strict_error/) | Multipart strict error |
| [`MULTIPART_UNMATCHED_BOUNDARY`](./multipart_unmatched_boundary/) | Multipart unmatched boundary |
| [`MULTIPART_BOUNDARY_QUOTED`](./multipart_boundary_quoted/) | Multipart boundary quoted |
| [`MULTIPART_BOUNDARY_WHITESPACE`](./multipart_boundary_whitespace/) | Multipart boundary whitespace |
| [`MULTIPART_DATA_BEFORE`](./multipart_data_before/) | Multipart data before |
| [`MULTIPART_DATA_AFTER`](./multipart_data_after/) | Multipart data after |
| [`MULTIPART_HEADER_FOLDING`](./multipart_header_folding/) | Multipart header folding |
| [`MULTIPART_LF_LINE`](./multipart_lf_line/) | Multipart LF line |
| [`MULTIPART_MISSING_SEMICOLON`](./multipart_missing_semicolon/) | Multipart missing semicolon |
| [`MULTIPART_INVALID_QUOTING`](./multipart_invalid_quoting/) | Multipart invalid quoting |
| [`MULTIPART_INVALID_PART`](./multipart_invalid_part/) | Multipart invalid part |
| [`MULTIPART_INVALID_HEADER_FOLDING`](./multipart_invalid_header_folding/) | Multipart invalid header folding |
| [`MULTIPART_FILE_LIMIT_EXCEEDED`](./multipart_file_limit_exceeded/) | Multipart file limit exceeded |
| [`REQBODY_ERROR`](./reqbody_error/) | Request body error |
| [`REQBODY_ERROR_MSG`](./reqbody_error_msg/) | Request body error message |
| [`REQBODY_PROCESSOR`](./reqbody_processor/) | Request body processor |
| [`REQBODY_PROCESSOR_ERROR`](./reqbody_processor_error/) | Request body processor error |
| [`INBOUND_DATA_ERROR`](./inbound_data_error/) | Inbound data error |
| [`OUTBOUND_DATA_ERROR`](./outbound_data_error/) | Outbound data error |
| [`URLENCODED_ERROR`](./urlencoded_error/) | URL encoding error |
| [`STATUS_LINE`](./status_line/) | Response status line |
| [`HIGHEST_SEVERITY`](./highest_severity/) | Highest severity |
| [`MSC_PCRE_LIMITS_EXCEEDED`](./msc_pcre_limits_exceeded/) | PCRE limits exceeded |
| [`ENV`](./env/) | Environment variables |
| [`GEO`](./geo/) | Geolocation data |
| [`RULE`](./rule/) | Rule metadata |
| [`SESSIONID`](./sessionid/) | Session ID |
| [`USERID`](./userid/) | User ID |
| [`WEBAPPID`](./webappid/) | Web application ID |
| [`XML`](./xml/) | XML data |
| [`MODSEC_BUILD`](./modsec_build/) | ModSecurity build info |
| [`FILES`](./files/) | Uploaded files |
| [`FILES_NAMES`](./files_names/) | Uploaded file names |
| [`FILES_SIZES`](./files_sizes/) | Uploaded file sizes |
| [`FILES_TMPNAMES`](./files_tmpnames/) | Uploaded file temp names |
| [`FILES_TMP_CONTENT`](./files_tmp_content/) | Uploaded file temp content |
| [`FILES_COMBINED_SIZE`](./files_combined_size/) | Uploaded files combined size |
| [`TX`](./tx/) | Transaction variables |
| [`GLOBAL`](./global/) | Global collection |
| [`IP`](./ip/) | IP address collection |
| [`SESSION`](./session/) | Session collection |
| [`USER`](./user/) | User collection |
| [`RESOURCE`](./resource/) | Resource collection |
| [`MATCHED_VAR`](./matched_var/) | Last matched variable |
| [`MATCHED_VAR_NAME`](./matched_var_name/) | Last matched variable name |
| [`MATCHED_VARS`](./matched_vars/) | All matched variables |
| [`MATCHED_VARS_NAMES`](./matched_vars_names/) | All matched variable names |
| [`UNIQUE_ID`](./unique_id/) | Unique transaction ID |
| [`DURATION`](./duration/) | Transaction duration |
| [`TIME`](./time/) | Current time |
| [`TIME_EPOCH`](./time_epoch/) | Unix timestamp |
| [`TIME_YEAR`](./time_year/) | Current year |
| [`TIME_MON`](./time_mon/) | Current month |
| [`TIME_DAY`](./time_day/) | Current day |
| [`TIME_HOUR`](./time_hour/) | Current hour |
| [`TIME_MIN`](./time_min/) | Current minute |
| [`TIME_SEC`](./time_sec/) | Current second |
| [`TIME_WDAY`](./time_wday/) | Current weekday |
