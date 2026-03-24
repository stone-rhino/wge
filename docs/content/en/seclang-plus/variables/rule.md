+++
title = "RULE"
weight = 69
+++

**Description:** Current rule information

**Syntax:** `RULE`

The RULE variable provides access to the metadata of the currently executing rule. Specific properties can be accessed via subkeys, such as `RULE.id` (rule ID), `RULE.phase` (the phase where the rule runs), and `RULE.operator_value` (the rule operator's argument). It is mainly used to reference the current rule's own information in logs and dynamic messages.

**Example:**

```apache
# Reference rule information in logs
SecRule ARGS "@detectSQLi" \
    "id:1047,phase:2,deny,msg:'Rule %{RULE.id} triggered: SQL injection detected',\
    severity:CRITICAL,tag:'attack-sqli'"
```
