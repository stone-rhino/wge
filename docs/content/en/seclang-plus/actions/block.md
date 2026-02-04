+++
title = "block"
weight = 17
+++

**Description:** Use default disruptive action

**Syntax:** `block | block:value`

The block action executes the default disruptive action configured by SecDefaultAction. This allows rules to be written without hardcoding a specific action, making it easier to switch between different blocking behaviors (like deny or redirect) across the entire configuration.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,block,status:403"
```

**Parameter Type:** `none`

**Case Sensitive:** Yes
