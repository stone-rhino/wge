+++
title = "rev"
weight = 7
+++

**Description:** Rule revision

**Syntax:** `rev:'revision'`

The rev action specifies the revision number of a rule. This metadata tracks how many times a rule has been modified, helping with rule lifecycle management and auditing.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,rev:'2'"
```

**Parameter Type:** `string`

**Case Sensitive:** Yes
