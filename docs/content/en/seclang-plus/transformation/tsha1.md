+++
title = "t:sha1"
weight = 35
+++

**Description:** Calculates ``SHA1`` hash of a string.

**Syntax:** `t:sha1`

**Implemented:** Yes

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,t:sha1"
```
