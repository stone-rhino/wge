+++
title = "logdata"
weight = 22
+++

**Description:** Log additional data

**Syntax:** `logdata:'data'`

The logdata action records additional information to the log when a rule matches. This is useful for including context such as the matched value, user information, or other relevant data that helps with incident investigation. The data can include variable expansion using the %{VARNAME} syntax.

**Example:**

```apache
SecRule ARGS "@rx attack" "id:1001,deny,logdata:'User: %{REMOTE_ADDR}, Matched: %{MATCHED_VAR}'"
```

**Parameter Type:** `string`

**Case Sensitive:** Yes
