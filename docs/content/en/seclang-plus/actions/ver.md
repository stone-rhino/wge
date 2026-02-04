+++
title = "ver"
weight = 6
+++

**Description:** Rule version

**Syntax:** `ver:'version'`

The ver action specifies the version of a rule or rule set. This metadata is useful for tracking rule updates and managing different versions of rule sets across environments.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,ver:'1.0.0'"
```

**Parameter Type:** `string`

**Case Sensitive:** Yes
