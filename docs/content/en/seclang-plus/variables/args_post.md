+++
title = "ARGS_POST"
weight = 3
+++

**Description:** POST request parameters

**Syntax:** `ARGS_POST | ARGS_POST:Key`

ARGS_POST contains only parameters passed in the request body, that is, POST data. WGE supports parsing request bodies in `application/x-www-form-urlencoded`, `multipart/form-data`, and `application/json` formats. Use this variable to inspect only data submitted in the request body without checking URL parameters. `SecRequestBodyAccess` must be enabled to access this variable.

**Note:**

- This variable must be used in phase 2 or later. Otherwise, its value is empty.
- When `SecParseXmlIntoArgs` is `On` or `OnlyArgs`, it also includes data parsed from `application/xml` request bodies.

**Example:**

```apache
# Check for XSS in POST form parameters
SecRule ARGS_POST "@detectXSS" "id:1003,phase:2,deny,status:403,msg:'XSS detected in POST parameters'"

# Validate login form username format
SecRule ARGS_POST:username "!@rx ^[a-zA-Z0-9_]{3,20}$" \
    "id:1004,phase:2,deny,msg:'Invalid username format'"
```
