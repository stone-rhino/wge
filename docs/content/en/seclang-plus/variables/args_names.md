+++
title = "ARGS_NAMES"
weight = 4
+++

**Description:** All parameter names

**Syntax:** `ARGS_NAMES`

ARGS_NAMES contains the names (keys) of all request parameters, including both GET and POST parameters. This variable is used to detect whether parameter names themselves contain attack payloads, which is a common bypass technique where attackers may inject malicious code in parameter names.

**Example:**

```apache
# Detect SQL injection keywords in parameter names
SecRule ARGS_NAMES "@pm select union insert delete drop" \
    "id:1005,phase:2,deny,msg:'Parameter name contains SQL keyword'"

# Limit parameter names to alphanumeric and underscores only
SecRule ARGS_NAMES "!@rx ^[a-zA-Z0-9_\[\]\.]+$" \
    "id:1006,phase:2,deny,msg:'Parameter name contains illegal characters'"
```
