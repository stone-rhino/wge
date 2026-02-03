+++
title = "tag"
weight = 5
+++

**Description:** Rule tag

**Syntax:** `tag:'tagname'`

The tag action assigns a tag to a rule for categorization and filtering purposes. Multiple tags can be assigned to a single rule. Tags are useful for organizing rules by attack type, compliance standard, or any other classification scheme.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,tag:'attack-sqli'"
```

**Parameter Type:** `string`

**Case Sensitive:** Yes
