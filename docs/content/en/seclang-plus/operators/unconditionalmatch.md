+++
title = "@unconditionalMatch"
weight = 31
+++

**Description:** Unconditional match (always returns true)

**Syntax:** `@unconditionalMatch`

**Input Data Type:** `int|string`

`@unconditionalMatch` always returns match. It is related to `@noMatch`: when negated (for example `!@unconditionalMatch`), behavior is equivalent to `@noMatch`.

**Example:**

```apache
# Use in rule chain
SecRule ARGS "@rx user" \
    "id:1118,phase:1,chain,pass"
    SecRule &MATCHED_VARS "@unconditionalMatch" \
        "setvar:tx.is_post_request+=1"
```
