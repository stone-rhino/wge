+++
title = "@rx"
weight = 1
+++

**Description:** Regular expression matching using RE2

**Syntax:** `@rx pattern`

**Input Data Type:** `string`

`@rx` is the most commonly used operator in SecLang. By default, WGE uses RE2 for regex matching. If RE2 compilation fails, WGE attempts to use PCRE2 as fallback.

If no operator keyword is explicitly provided in a rule, regex matching is used by default.

**Example:**

```apache
# Detect SQL injection keywords
SecRule ARGS "@rx (?i:select|union|insert|update|delete|drop)" \
    "id:1001,phase:2,deny,msg:'SQL Injection detected'"

# Detect XSS attack pattern
SecRule ARGS "@rx ]*>.*?" \
    "id:1002,phase:2,deny,msg:'XSS Attack detected'"

# Use regex when operator keyword is omitted
SecRule ARGS "admin" "id:1003,phase:2,deny,msg:'test'"
```
