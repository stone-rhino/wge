+++
title = "setenv"
weight = 27
+++

**Description:** Set environment variable

**Syntax:** `setenv:name=value`

The setenv action sets an environment variable that can be accessed by backend applications or used in response headers. This is useful for passing security-related information from the WAF to downstream components.

**Example:**

```apache
SecRule ARGS "@rx attack" "id:1001,pass,setenv:attack_detected=1"
```

**Parameter Type:** `string`

**Case Sensitive:** Yes
